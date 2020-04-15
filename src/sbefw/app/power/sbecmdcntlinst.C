/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdcntlinst.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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

#include "fapi2.H"
#include "p10_thread_control.H"
#include "p10_scom_c.H"

using namespace fapi2;

//Utility function to mask special attention
extern ReturnCode maskSpecialAttn( const Target<TARGET_TYPE_CORE>& i_target);

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
    for (auto coreTgt : procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        // Making all cores enable by default for simics, onces we have the
        // procedure, we should update core state
        bool isCoreScomEnabled = true;
        if(SBE::isSimicsRunning())
        {
            isCoreScomEnabled = true;
        }
#if 0
        // TODO - Check with PM team for the P10 procedure for query_core_state
        // Comeback on this function in MPIPL Phase
        else
        {
            bool l_isScanEnable = false;
            SBE_EXEC_HWP(l_fapiRc, p9_query_core_access_state, l_coreTgt,
                    l_isCoreScomEnabled, l_isScanEnable)
            if(l_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC " p9_query_core_access_state failed, "
                        "RC=[0x%08X]", l_fapiRc);
                break;
            }
        }
#endif
        if(isCoreScomEnabled) //true
        {
            fapi2::buffer<uint64_t> data64;
            uint64_t state;
            bool warnCheck = true;
                
            // Call instruction control stop
            SBE_EXEC_HWP(fapiRc, threadCntlhwp, coreTgt,
                         static_cast<ThreadSpecifier>(SMT4_THREAD_ALL),
                         PTC_CMD_STOP, warnCheck, data64, state)
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_thread_control stop Failed for "
                    "Core ALL Threads, RC[0x%08X]", fapiRc);
                break;
            }

            fapiRc = maskSpecialAttn(coreTgt);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "maskSpecialAttn failed");
                break;
            }
        }
    }
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

        do //Iterate over all cores for special wakeup assert
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE>coreTgt(
              plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_CORE>(core));
            if(!coreTgt.isFunctional())
            {
                continue;
            }

            // TODO - Comeback on this if we need to do this by default
            if(req.isSpecialWakeUpRequired())
            {
                rc = specialWakeUpCoreAssert(coreTgt, fapiRc);
                if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "SpecialWakeup Assert failed for "
                        "core[0x%2x]", core);
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
                    SBE_INFO(SBE_FUNC "SpecialWakeup assert succeeded for "
                        "core[0x%2x]", core);
                }
            }
        } while(++core < coreCntMax);

        if ( (fapiRc) && !(IGNORE_HW_ERRORS & req.mode) )
        {
            // If FapiRc from the inner loop (thread loop), just break here
            break; // From core while loop
        }

        req.processInputDataToIterate(core,coreCntMax,threadMask);

        do //Iterate over all cores to execute control instruction procedure
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE>coreTgt(
              plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_CORE>(core));
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
                        "Selected the mode to exit on first error.");
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

        do //Iterate over all cores for special wakeup de-assert
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE>coreTgt(
                plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_CORE>(core));
            if(!coreTgt.isFunctional())
            {
                continue;
            }

            // TODO - Comeback on this if we need to do this by default
            if(req.isSpecialWakeUpRequired())
            {
                rc = specialWakeUpCoreDeAssert(coreTgt, fapiRc);
                if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "Special Wakeup de-asssert failed for "
                            "core[0x%2x]", core);
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
                    SBE_INFO(SBE_FUNC "Special Wakeup de-assert succeeded for "
                            "core[0x%2x]", core);
                }
            }
        } while(++core < coreCntMax);

        if ( (fapiRc) && !(IGNORE_HW_ERRORS & req.mode) )
        {
            // If FapiRc from the inner loop (thread loop), just break here
            break; // From core while loop
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

