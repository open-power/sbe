/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdcntlinst.C $                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
//#include "p9_query_core_access_state.H"


using namespace fapi2;

//Utility function to mask special attention
extern ReturnCode maskSpecialAttn( const Target<TARGET_TYPE_CORE>& i_target);

// TODO via RTC 152424
// Currently all proecdures in core directory are in seeprom.
// So we have to use function pointer to force a long call.
#if 0
#ifdef SEEPROM_IMAGE
p9_thread_control_FP_t threadCntlhwp = &p9_thread_control;
#endif

static const uint64_t SPWKUP_ASSERT   = 0x8000000000000000ull;
static const uint64_t SPWKUP_DEASSERT = 0x0000000000000000ull;
static const uint64_t GPMMR_SPWKUP_DONE = 0x8000000000000000ull;
static const uint32_t SPECIAL_WAKE_UP_POLL_INTERVAL_NS = 1000000;   //1ms
static const uint32_t SPECIAL_WAKE_UP_POLL_INTERVAL_SIMICS = 1000000;
static const uint32_t SPECIAL_WAKEUP_TIMEOUT_COUNT = 100; // 100 * 1ms



/* @brief Map User Thread Command to Hwp ThreadCommands Enum */
ThreadCommands getThreadCommand(const sbeCntlInstRegMsgHdr_t & i_req)
{
    ThreadCommands l_cmd = PTC_CMD_START;
    switch(i_req.threadOps)
    {
        case THREAD_START_INS:  l_cmd = PTC_CMD_START;  break;
        case THREAD_STOP_INS:   l_cmd = PTC_CMD_STOP;   break;
        case THREAD_STEP_INS:   l_cmd = PTC_CMD_STEP;   break;
        case THREAD_SRESET_INS: l_cmd = PTC_CMD_SRESET; break;
    }
    return l_cmd;
}

/* @brief Map User Mode Command to Hwp Warn Check flag */
inline bool getWarnCheckFlag(const sbeCntlInstRegMsgHdr_t & i_req)
{
    bool l_warnCheck = false;

    // EXIT_ON_FIRST_ERROR and IGNORE_HW_ERRORS are mutually exclusive
    if( EXIT_ON_FIRST_ERROR != (i_req.mode & 0x1) )
    {
        l_warnCheck = true;
    }
    return l_warnCheck;
}

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
        o_fapiRc = putscom_abs_wrap(&i_target, C_PPM_SPWKUP_FSP, data);
        if(o_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " special wakeup putscom failed");
            rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
            break;
        }

        do
        {
            delay(SPECIAL_WAKE_UP_POLL_INTERVAL_NS, SPECIAL_WAKE_UP_POLL_INTERVAL_SIMICS);
            o_fapiRc = getscom_abs_wrap(&i_target, C_PPM_GPMMR_SCOM, &data);
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

    do
    {
        uint64_t data = SPWKUP_DEASSERT; // deassert
        o_fapiRc = putscom_abs_wrap(&i_target, C_PPM_SPWKUP_FSP, data);
        if(o_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " special wakeup putscom failed");
            rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
            break;
        }

        // This puts an inherent delay in the propagation of the reset transition.
        o_fapiRc = getscom_abs_wrap(&i_target, C_PPM_SPWKUP_FSP, &data);
        if(o_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " special wakeup getscom failed");
            rc = SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE;
            break;
        }
    } while(false);

    return rc;
    #undef SBE_FUNC
}
#endif
///////////////////////////////////////////////////////////////////////
// @brief stopAllCoreInstructions  Stop all core instructions function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
ReturnCode stopAllCoreInstructions( )
{
    #define SBE_FUNC "stopAllCoreInstructions"
    SBE_ENTER(SBE_FUNC);
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
#if 0
    Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
    for (auto l_coreTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        bool l_isCoreScomEnabled = false;
        if(SBE::isSimicsRunning())
        {
            l_isCoreScomEnabled = true;
        }
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
        if(l_isCoreScomEnabled) //true
        {
            uint8_t l_thread = SMT4_THREAD0;
            fapi2::buffer<uint64_t> l_data64;
            uint64_t l_state;
            bool l_warnCheck = true;
            do
            {
                // Call instruction control stop
                // TODO RTC 164425 - Can we pass in 1111 i.e. all threads at the
                // same time instead of individual threads
                SBE_EXEC_HWP(l_fapiRc,
                             threadCntlhwp,
                             l_coreTgt,
                             (SINGLE_THREAD_BIT_MASK >> l_thread),
                             PTC_CMD_STOP, l_warnCheck,l_data64, l_state)
                if(l_fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC "p9_thread_control stop Failed for "
                        "Core Thread  RC[0x%08X]", l_fapiRc);
                    break;
                }
            }while(++l_thread < SMT4_THREAD_MAX);

            l_fapiRc = maskSpecialAttn(l_coreTgt);
            if( l_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "maskSpecialAttn failed");
                break;
            }
        }
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return l_fapiRc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeCntlInst Sbe control instructions function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeCntlInst(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeCntlInst "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Create the req struct for Control Instructions Chip-op
    sbeCntlInstRegMsgHdr_t l_req = {0};

    do
    {
        // Get the Req Struct Data sbeCntlInstRegMsgHdr_t from upstream Fifo
        uint32_t l_len2dequeue  = sizeof(l_req) / sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_req, true);

        // If FIFO failure
        if ( SBE_SEC_OPERATION_SUCCESSFUL != l_rc )
        {
            // Let command processor routine to handle the RC.
            break;
        }

        SBE_INFO("mode[0x%04X] coreChipletId[0x%08X] threadNum[0x%04X] "
            "threadOps[0x%04X] ", l_req.mode, l_req.coreChipletId,
            l_req.threadNum, l_req.threadOps);

        // Validate Input Args
        if( false == l_req.validateInputArgs())
        {
            SBE_ERROR(SBE_FUNC "ValidateAndMapInputArgs failed");
            l_respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                 SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            break;
        }

        // Fetch HWP mapped values
        bool l_warnCheck = getWarnCheckFlag(l_req);
        ThreadCommands l_cmd = getThreadCommand(l_req);

        // Default assignment not required since it is assigned below
        uint8_t l_core, l_coreCntMax;
        uint8_t l_threadCnt, l_threadCntMax;

        l_req.processInputDataToIterate(l_core, l_coreCntMax,
                                        l_threadCnt, l_threadCntMax);
        fapi2::buffer<uint64_t> l_data64;
        uint64_t l_state;
        do
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE>
                l_coreTgt(plat_getTargetHandleByChipletNumber
                        <fapi2::TARGET_TYPE_CORE>(l_core));
            if(!l_coreTgt.isFunctional())
            {
                continue;
            }

            bool deassertRequired = false;
            if(l_req.isSpecialWakeUpRequired())
            {
                l_rc = specialWakeUpCoreAssert(l_coreTgt, l_fapiRc);
                if(l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "Special Wakeup Assert failed for core[0x%2x]",
                                                 (uint8_t)l_req.coreChipletId);
                    l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                        l_rc);
                    l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    l_ffdc.setRc(l_fapiRc);
                    if(!(IGNORE_HW_ERRORS & l_req.mode))
                    {
                        break;
                    }
                    SBE_INFO(SBE_FUNC "Continuing in case of HW Errors"
                        " As user has passed to ignore errors.");
                }
                else
                {
                    SBE_INFO(SBE_FUNC "Special Wakeup assert succeeded for core[0x%2x]",
                                                     (uint8_t)l_req.coreChipletId);
                    deassertRequired = true;
                }
            }

            uint8_t l_thread = l_threadCnt;
            do
            {
                // Call the Procedure
                SBE_EXEC_HWP(l_fapiRc,
                             threadCntlhwp,
                             l_coreTgt,
                              (SINGLE_THREAD_BIT_MASK >> l_thread),
                              l_cmd, l_warnCheck,
                              l_data64, l_state)

                if(l_fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC "Failed for Core[%d] Thread [%d] "
                        "Cmd[%d] Mode[%d]", l_core, l_thread, l_req.threadOps,
                        l_req.mode);
                    if(IGNORE_HW_ERRORS & l_req.mode)
                    {
                        // No need to delete the l_fapiRc handle,it will get
                        // over-written
                        SBE_INFO(SBE_FUNC "Continuing in case of HW Errors"
                            " As user has passed to ignore errors.");
                        continue;
                    }
                    else
                    {
                        SBE_ERROR(SBE_FUNC "Breaking out, since User has "
                            "Selected the mode to exit on first error.");
                        l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                         SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                        l_ffdc.setRc(l_fapiRc);
                        break;
                    }
                }
            }while(++l_thread < l_threadCntMax);

            if(deassertRequired)
            {
                ReturnCode fapiRc = FAPI2_RC_SUCCESS;
                l_rc = specialWakeUpCoreDeAssert(l_coreTgt, fapiRc);
                if(l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "Special Wakeup de-asssert failed for core[0x%2x]",
                                                 (uint8_t)l_req.coreChipletId);
                    l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                        l_rc);
                    l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    l_ffdc.setRc(fapiRc);
                    if(!(IGNORE_HW_ERRORS & l_req.mode))
                    {
                        break;
                    }
                    SBE_INFO(SBE_FUNC "Continuing in case of HW Errors"
                        " As user has passed to ignore errors.");
                }
                else
                {
                    SBE_INFO(SBE_FUNC "Special Wakeup de-assert succeeded for core[0x%2x]",
                                                     (uint8_t)l_req.coreChipletId);
                }
            }

            if ((l_fapiRc) && !(IGNORE_HW_ERRORS & l_req.mode))
            {
                // If FapiRc from the inner loop (thread loop), just break here
                break; // From core while loop
            }
        }while(++l_core < l_coreCntMax);

    }while(0);

    // Create the Response to caller
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        if ( SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            break;
        }

        l_rc = sbeDsSendRespHdr(l_respHdr, &l_ffdc);
    }while(0);
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

