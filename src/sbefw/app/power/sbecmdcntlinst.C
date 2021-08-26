/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdcntlinst.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/*
 * @file: ppe/sbe/sbefw/sbecmdcntlinst.C
 *
 * @brief This file contains the SBE Control Instruction chipOps
 *
 */

#include "sbecmdcntlinst.H"
#include "sbecmdiplcontrol.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbestates.H"
#include "sberegaccess.H"


#include "fapi2.H"
#include "p10_thread_control.H"
#include "p10_scom_c.H"
#include "p10_scom_eq_3.H"
#include "p10_scom_eq_1.H"
#include "p10_query_corecachemma_access_state.H"
#include "sbeglobals.H"
using namespace fapi2;

//Utility function to mask special attention
extern ReturnCode maskUnmaskSpecialAttn( const Target<TARGET_TYPE_EQ>& i_target, bool isMaskReq);
p10_thread_control_FP_t threadCntlhwp = &p10_thread_control;

static const uint64_t SPWKUP_ASSERT   = 0x8000000000000000ull;
static const uint64_t SPWKUP_DEASSERT = 0x0000000000000000ull;
static const uint64_t GPMMR_SPWKUP_DONE = 0x800000000000000ull;
static const uint32_t SPECIAL_WAKE_UP_POLL_INTERVAL_NS = 1000000;   //1ms
static const uint32_t SPECIAL_WAKE_UP_POLL_INTERVAL_SIMICS = 1000000;
static const uint32_t SPECIAL_WAKEUP_TIMEOUT_COUNT = 100; // 100 * 1ms



/* @brief Map User Thread Command to Hwp ThreadCommands Enum */
ThreadCommands getThreadCommand(const sbeCntlInstMsgHdr_t & i_req)
{
    ThreadCommands cmd = PTC_CMD_START;
    switch(i_req.threadOps)
    {
        case THREAD_START_INS:  cmd = PTC_CMD_START;  break;
        case THREAD_STOP_INS:   cmd = PTC_CMD_STOP;   break;
        case THREAD_STEP_INS:   cmd = PTC_CMD_STEP;   break;
        case THREAD_SRESET_INS: cmd = PTC_CMD_SRESET; break;
    }
    return cmd;
}

/* @brief Map User Mode Command to Hwp Warn Check flag */
inline bool getWarnCheckFlag(const sbeCntlInstMsgHdr_t & i_req)
{
    bool warnCheck = false;

    // EXIT_ON_FIRST_ERROR and IGNORE_HW_ERRORS are mutually exclusive
    if( EXIT_ON_FIRST_ERROR != (i_req.mode & 0x1) )
    {
        warnCheck = true;
    }
    return warnCheck;
}

// TODO : Convert this to Multi-cast write, so that it done via one scom write
static uint32_t specialWakeUpCoreAssert(
        const Target<TARGET_TYPE_CORE>& i_target, ReturnCode &o_fapiRc)
{
    #define SBE_FUNC "specialWakeUpCoreAssert"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    o_fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        uint64_t data = 0;
        uint8_t pollCount = 0;

        data = SPWKUP_ASSERT; // assert
        o_fapiRc = putscom_abs_wrap(&i_target, scomt::c::QME_SPWU_FSP, data);
        if(o_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " special wakeup putscom failed");
            rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
            break;
        }

        //Check if the QME is halted
        //TODO: This check is only for DD1
        fapi2::buffer<uint64_t> l_qmeIar;
        auto l_parentEq  =  i_target.getParent< fapi2::TARGET_TYPE_EQ >();
        o_fapiRc = getscom_abs_wrap(&l_parentEq, scomt::eq::QME_SCOM_XIDBGPRO, &l_qmeIar());
        if(o_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " EQ:0x%.8x ,Failed to read scomt::eq::QME_SCOM_XIDBGPRO to detect if QME is halted",
                      l_parentEq.get());
            rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
            break;
        }
        if( l_qmeIar.getBit( scomt::eq::QME_SCOM_XIDBGPRO_XSR_HS ) )
        {
            SBE_INFO("QME assocaited with the core=0x%.8x is HALTED",i_target.get());
            fapi2::buffer<uint64_t> l_scrData;
            uint32_t l_pmState = 0;
            o_fapiRc = getscom_abs_wrap(&i_target, scomt::c::QME_SCSR, &l_scrData());
            if(o_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC " Core:0x%.8x ,Failed to read QME_SCSR to determine stop state",
                          i_target.get());
                rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
                break;
            }
            l_scrData.extractToRight< 60, 4 >( l_pmState );
            if( ( l_scrData.getBit( scomt::c::QME_SCSR_PM_STATE_ACTIVE )) && ( ( l_pmState == 0 ) || ( l_pmState == 1 ) ) )
            {
                SBE_INFO("Core:0x%.8x  is  in STOP 0/1:0x%.8x",i_target.get(),l_pmState);
                l_scrData.setBit( scomt::c::QME_SCSR_ASSERT_PM_EXIT ); // Enable exit from core STOP state
                l_scrData.clearBit( scomt::c::QME_SCSR_AUTO_SPECIAL_WAKEUP_DISABLE ); // Enable auto-special wakeup
                o_fapiRc = putscom_abs_wrap(&i_target, scomt::c::QME_SCSR, l_scrData);
                if(o_fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR("Core=0x%.8x , Failed to update QME_SCSR",i_target.get());
                    rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
                    break;
                }
            }

        }

        do
        {
            delay(SPECIAL_WAKE_UP_POLL_INTERVAL_NS, SPECIAL_WAKE_UP_POLL_INTERVAL_SIMICS);
            o_fapiRc = getscom_abs_wrap(&i_target, scomt::c::QME_SPWU_FSP, &data);
            if(o_fapiRc != FAPI2_RC_SUCCESS)
            {
                rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
                break;
            }
        }
        while(!(data & GPMMR_SPWKUP_DONE) &&
                (++pollCount < SPECIAL_WAKEUP_TIMEOUT_COUNT));

        if(!(data & GPMMR_SPWKUP_DONE))
        {
            if(rc == SBE_SEC_OPERATION_SUCCESSFUL) {
                SBE_ERROR(SBE_FUNC " special wakeup done timeout");
                rc = SBE_SEC_SPECIAL_WAKEUP_TIMEOUT;
            }
            break;
        }
    } while(false);

    return rc;
    #undef SBE_FUNC
}

// TODO : Convert this to Multi-cast write, so that it done via one scom write
static uint32_t specialWakeUpCoreDeAssert(
        const Target<TARGET_TYPE_CORE>& i_target, ReturnCode &o_fapiRc)
{
    #define SBE_FUNC "specialWakeUpCoreDeAssert"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    o_fapiRc = FAPI2_RC_SUCCESS;
    uint64_t data = 0;

    do
    {
        o_fapiRc = getscom_abs_wrap(&i_target, scomt::c::QME_SPWU_FSP, &data);
        if(o_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " special wakeup getscom failed");
            rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
            break;
        }
        if(data & GPMMR_SPWKUP_DONE)
        {
            uint64_t dataDeassert = SPWKUP_DEASSERT;
            o_fapiRc = putscom_abs_wrap(&i_target, scomt::c::QME_SPWU_FSP, dataDeassert);
            if(o_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC " special wakeup putscom failed");
                rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
                break;
            }
        }

    } while(0);

    return rc;
    #undef SBE_FUNC
}


bool isCoreOrL2CacheScomEnabled(const Target<TARGET_TYPE_CORE>& i_coreTgt,scomStatus_t& i_scomStateData)
{
    switch(i_coreTgt.get().getTargetInstance() % 4)
    {
        case 0: //CORE INSTANCE 0
             return(i_scomStateData.ecl20);
             break;

        case 1: //CORE INSTANCE 1
             return(i_scomStateData.ecl21);
             break;

        case 2: //CORE INSTANCE 2
             return(i_scomStateData.ecl22);
             break;

        case 3: //CORE INSTANCE 3
             return(i_scomStateData.ecl23);
             break;
        default : return false;

    }
}

bool isCoreOrL2CacheScanEnabled(const Target<TARGET_TYPE_CORE>& i_coreTgt, scanStatus_t& i_scanStateData)
{
    switch(i_coreTgt.get().getTargetInstance() % 4)
    {
        case 0: //CORE INSTANCE 0
             return(i_scanStateData.ecl20);
             break;

        case 1: //CORE INSTANCE 1
             return(i_scanStateData.ecl21);
             break;

        case 2: //CORE INSTANCE 2
             return(i_scanStateData.ecl22);
             break;

        case 3: //CORE INSTANCE 3
             return(i_scanStateData.ecl23);
             break;
        default : return false;

    }
}

bool isL3CacheScanEnabled(const Target<TARGET_TYPE_CORE>& i_coreTgt, scanStatus_t& i_scanStateData)
{
    switch(i_coreTgt.get().getTargetInstance() % 4)
    {
        case 0: //CORE INSTANCE 0
             return(i_scanStateData.l30);
             break;

        case 1: //CORE INSTANCE 1
             return(i_scanStateData.l31);
             break;

        case 2: //CORE INSTANCE 2
             return(i_scanStateData.l32);
             break;

        case 3: //CORE INSTANCE 3
             return(i_scanStateData.l33);
             break;
        default : return false;

    }
}

bool isCoreOrMmaScanEnabled(const Target<TARGET_TYPE_CORE>& i_coreTgt, scanStatus_t& i_scanStateData)
{
    switch(i_coreTgt.get().getTargetInstance() % 4)
    {
        case 0: //CORE INSTANCE 0
             return(i_scanStateData.mma0);
             break;

        case 1: //CORE INSTANCE 1
             return(i_scanStateData.mma1);
             break;

        case 2: //CORE INSTANCE 2
             return(i_scanStateData.mma2);
             break;

        case 3: //CORE INSTANCE 3
             return(i_scanStateData.mma3);
             break;
        default : return false;

    }
}

bool isL3CacheScomEnabled(const Target<TARGET_TYPE_CORE>& i_coreTgt,scomStatus_t& i_scomStateData)
{
    switch(i_coreTgt.get().getTargetInstance() % 4)
    {
        case 0: //CORE INSTANCE 0
             return(i_scomStateData.l30);
             break;

        case 1: //CORE INSTANCE 1
             return(i_scomStateData.l31);
             break;

        case 2: //CORE INSTANCE 2
             return(i_scomStateData.l32);
             break;

        case 3: //CORE INSTANCE 3
             return(i_scomStateData.l33);
             break;
        default : return false;
    }
}

bool isCoreCheckStopped(const Target<TARGET_TYPE_CORE>& i_coreTgt,fapi2::buffer<uint64_t>& i_data)
{
    switch(i_coreTgt.get().getTargetInstance() % 4)
    {
        case 0: //CORE INSTANCE 0
             return(i_data.getBit<scomt::eq::LOCAL_XSTOP_IN05>());
             break;

        case 1: //CORE INSTANCE 1
             return(i_data.getBit<scomt::eq::LOCAL_XSTOP_IN06>());
             break;

        case 2: //CORE INSTANCE 2
             return(i_data.getBit<scomt::eq::LOCAL_XSTOP_IN07>());
             break;

        case 3: //CORE INSTANCE 3
             return(i_data.getBit<scomt::eq::LOCAL_XSTOP_IN08>());
             break;
        default : return false;

    }

}

///////////////////////////////////////////////////////////////////////
// @brief stopAllCoreInstructions  Stop all core instructions function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
ReturnCode stopAllCoreInstructions( )
{
#define SBE_FUNC "stopAllCoreInstructions"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();

    do{
        //Check for core local checkstop and update the core functional
        //states
        fapiRc = checkCoreCheckStopAndUpdateFuncState();
        if(fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR("Failed in checkCoreCheckStopAndUpdateFuncState() ,RC=0x%.8x",
                    fapiRc);
            break;
        }
        //Fetch all EQ Targets
        for(auto eqTgt: procTgt.getChildren<fapi2::TARGET_TYPE_EQ>())
        {
            //Mask Core Special Attentions for all cores related to the EQ
            fapiRc = maskUnmaskSpecialAttn(eqTgt, true);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failure in maskSpecialAttn()");
                break;
            }

            //Default Set the bits represeting the core SCOM state as TRUE
            //based on the bits defined in scomStatus_t
            scomStatus_t scomStateData;
            scanStatus_t scanStateData;
            SBE_EXEC_HWP(fapiRc,p10_query_corecachemma_access_state, eqTgt,
                    scomStateData,scanStateData);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC " p10_query_corecachemma_access_state failed, "
                        " RC=[0x%08X],EqTarget=0x%.8x", fapiRc,eqTgt.get());
                break;
            }

            //For all Core targets associated with the EQ target,call procedure to
            //stop the instructions if scomStateData for the particular core is TRUE
            for (auto coreTgt : eqTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
            {

                //Stop Instructions on the core only if Core Scomable state is TRUE
                if( isCoreOrL2CacheScomEnabled(coreTgt,scomStateData) )
                {
                    fapi2::buffer<uint64_t> data64;
                    uint64_t state;
                    bool warnCheck = true;

                    // Call instruction control stop
                    SBE_EXEC_HWP(fapiRc, threadCntlhwp, coreTgt,
                            static_cast<ThreadSpecifier>(SMT4_THREAD_ALL),
                            PTC_CMD_STOP, warnCheck, data64, state);
                    if(fapiRc != FAPI2_RC_SUCCESS)
                    {
                        SBE_ERROR(SBE_FUNC "p10_thread_control stop Failed for "
                                "Core ALL Thread  RC[0x%08X],CoreTarget=0x%.8x",
                                fapiRc,coreTgt.get());
                        break;
                    }

                }

            }//Core Target Loop
            if(fapiRc)
            {
                break;
            }
        }//EQ Loop

    }while(0);
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
#undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeCntlInst SBE control instructions function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeCntlInst(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeCntlInst "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    // Create the req struct for Control Instructions Chip-op
    sbeCntlInstMsgHdr_t req = {0};

    do
    {
        // Get the Req Struct Data sbeCntlInstMsgHdr_t from upstream Fifo
        uint32_t len2dequeue  = sizeof(req) / sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&req, true);

        // If FIFO failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_INFO(SBE_FUNC "mode[0x%02X] coreId[0x%02X] threadId[0x%02X] "
            "threadOps[0x%04X]",req.mode,req.coreId,req.threadId,req.threadOps);

        //Block start instruction in secure mode/production
        if((SBE_GLOBAL->sbeFWSecurityEnabled) && (!SBE::isSimicsRunning())
                && (req.threadOps == THREAD_START_INS))
        {
            SBE_ERROR(SBE_FUNC "threadOps[0x%02X] blocked in secure mode",(uint8_t)req.threadOps);
            respHdr.setStatus( SBE_PRI_UNSECURE_ACCESS_DENIED, SBE_SEC_BLACKLISTED_CHIPOP_ACCESS);
            break;
        }

        // Validate Input Args
        if( false == req.validateInputArgs())
        {
            SBE_ERROR(SBE_FUNC "ValidateAndMapInputArgs failed");
            respHdr.setStatus( SBE_PRI_INVALID_DATA, SBE_SEC_INVALID_PARAMS );
            break;
        }

        // Fetch HWP mapped values
        bool warnCheck = getWarnCheckFlag(req);
        ThreadCommands cmd = getThreadCommand(req);

        // Default assignment not required since it is assigned below
        uint8_t core, coreCntMax, threadMask;
        req.processInputDataToIterate(core,coreCntMax,threadMask);

        fapi2::buffer<uint64_t> data64;
        uint64_t state;

        // Do Special Wakeup Assert only for Stop Instruction, For Rest of the
        // control instruction commands, it's a no-op
        if( req.threadOps == THREAD_STOP_INS )
        {

            //Stop Instruction Chip-OP is a NOOP when SBE in the MPIPL path
            //Special Wake Asserts should not be done in the MPIPL path
            if(  (SbeRegAccess::theSbeRegAccess().getSbePrevState() == SBE_STATE_RUNTIME) &&
                 (SbeRegAccess::theSbeRegAccess().getSbeState() ==  SBE_STATE_MPIPL) )
            {
                SBE_INFO("Stopping Instructions is NOOP in the MPIPL path");
                rc = SBE_SEC_OPERATION_SUCCESSFUL;
                break;
            }

            do //Iterate over all cores for special wakeup assert
            {
                //TODO: Skip doing special wakeup on Simics
                if(SBE::isSimicsRunning())
                {
                    SBE_INFO("Special Wakeup is NOOP on simics for Stop Instructions");
                    break;
                }

                fapi2::Target<fapi2::TARGET_TYPE_CORE>coreTgt(
                        plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(core));
                if(!coreTgt.isFunctional())
                {
                    continue;
                }

                rc = specialWakeUpCoreAssert(coreTgt, fapiRc);
                if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "Assert failed for core[0x%2x]", core);
                    respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,rc);
                    // Revert back the local RC to success
                    rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    ffdc.setRc(fapiRc);
                    if(!(IGNORE_HW_ERRORS & req.mode))
                    {
                        break;
                    }
                    SBE_INFO(SBE_FUNC "Continuing in case of HW Errors"
                            " As user has passed to ignore errors.");
                }
                else
                {
                    SBE_INFO(SBE_FUNC "Assert succeeded for core[0x%2x]", core);
                }
            }while(++core < coreCntMax);

            if ( (fapiRc) && !(IGNORE_HW_ERRORS & req.mode) )
            {
                // If FapiRc from the inner loop (thread loop), just break here
                break; // From core while loop
            }
        }

        req.processInputDataToIterate(core,coreCntMax,threadMask);

        do //Iterate over all cores to execute control instruction procedure
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE>coreTgt(
              plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(core));
            if(!coreTgt.isFunctional())
            {
                continue;
            }

            // Call the Thread Control Procedure
            SBE_EXEC_HWP(fapiRc, threadCntlhwp, coreTgt,
                         static_cast<ThreadSpecifier>(threadMask),
                         cmd, warnCheck, data64, state)

            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed for Core[%d] ThreadMask [%d] "
                    "Cmd[%d] Mode[%d]",core,threadMask,req.threadOps,req.mode);
                if(IGNORE_HW_ERRORS & req.mode)
                {
                    // No need to delete the fapiRc handle,it will get
                    // over-written
                    SBE_INFO(SBE_FUNC "Continuing in case of HW Errors"
                        " As user has passed to ignore errors.");
                    continue;
                }
                else
                {
                    SBE_ERROR(SBE_FUNC "Breaking out, since User has "
                        "selected the mode to exit on first error.");
                    respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                      SBE_SEC_THREAD_CONTROL_INSTR_FAILED);
                    ffdc.setRc(fapiRc);
                    break;
                }
            }
        } while(++core < coreCntMax);

        if ( (fapiRc) && !(IGNORE_HW_ERRORS & req.mode) )
        {
            // If FapiRc from the inner loop (thread loop), just break here
            break; // From core while loop
        }

        req.processInputDataToIterate(core,coreCntMax,threadMask);

        // Do Special Wakeup De-assert only for Start/SReset Instruction,
        // For Rest of the control instruction commands, it's a no-op
        if(req.threadOps == THREAD_START_INS || req.threadOps == THREAD_SRESET_INS)
        {
            do //Iterate over all cores for special wakeup de-assert
            {
                fapi2::Target<fapi2::TARGET_TYPE_CORE>coreTgt(
                        plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(core));
                if(!coreTgt.isFunctional())
                {
                    continue;
                }

                rc = specialWakeUpCoreDeAssert(coreTgt, fapiRc);
                if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "De-asssert failed for core[0x%2x]",core);
                    respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE, rc);
                    rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    ffdc.setRc(fapiRc);
                    if(!(IGNORE_HW_ERRORS & req.mode))
                    {
                        break;
                    }
                    SBE_INFO(SBE_FUNC "Continuing in case of HW Errors"
                            " As user has passed to ignore errors.");
                }
                else
                {
                    SBE_INFO(SBE_FUNC "De-assert succeeded for core[0x%2x]",core);
                }
            } while(++core < coreCntMax);

            if ( (fapiRc) && !(IGNORE_HW_ERRORS & req.mode) )
            {
                // If FapiRc from the inner loop (thread loop), just break here
                break; // From core while loop
            }
        }

    }while(0);

    // Create the Response to caller
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        if ( SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            break;
        }

        rc = sbeDsSendRespHdr(respHdr, &ffdc);
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
//
// @brief Check for local checkstop pressence with respect to the
//        Core and mark it non functional
///////////////////////////////////////////////////////////////////////
fapi2::ReturnCode checkCoreCheckStopAndUpdateFuncState()
{
#define SBE_FUNC "checkCoreCheckStopAndUpdateFuncState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint8_t coreId = 0;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    //Fetch all EQ Targets
    uint8_t fuseMode = 0;
    FAPI_ATTR_GET(ATTR_FUSED_CORE_MODE,Target<TARGET_TYPE_SYSTEM>(),fuseMode);

    for(auto eqTgt: procTgt.getChildren<fapi2::TARGET_TYPE_EQ>())
    {
        //Check for Core Checkstop
        fapi2::buffer<uint64_t> data64 =0x0;
        fapiRc = getscom_abs_wrap (&eqTgt,scomt::eq::LOCAL_XSTOP , &data64());
        for (auto coreTgt : eqTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            //getChildren() returns functional core targets, but the logic below
            //can mark the paired fused core as non-functional. This check is to
            //skip acting upon those cores which were marked non-functional due
            //to the pressence of local checkstop on their parter core Targets.
            bool coreFuncState = static_cast<plat_target_handle_t&>(
                                 coreTgt.operator ()()).getFunctional();
            //Check for core checkstop pressence only if the core is functional
            if( coreFuncState && isCoreCheckStopped(coreTgt,data64))
            {
              FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,coreTgt, coreId);
              G_vec_targets[CORE_TARGET_OFFSET+ coreId].setFunctional(false);
              SBE_INFO("Core:0x%.8x marked non-functional due to local checkstop pressence",
                        (fapi2::plat_target_handle_t)(coreTgt.get()));
              //If in fused mode, update the pair code as non-functional
              if(fuseMode)
              {
                  fapi2::ATTR_CHIP_UNIT_POS_Type pairCoreId =
                                                 (coreId % 2) ? (coreId - 1) : (coreId + 1);
                  G_vec_targets[CORE_TARGET_OFFSET+ pairCoreId].setFunctional(false);
                  SBE_INFO("Paired Core:0x%.8x marked non-functional due to local checkstop"
                  " pressence",static_cast<uint32_t>(
                                       G_vec_targets[CORE_TARGET_OFFSET+ pairCoreId].value));
              }
            }
        }
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
#undef SBE_FUNC
}
