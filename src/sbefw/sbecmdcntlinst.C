/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdcntlinst.C $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"

#include "fapi2.H"
#include "p9_thread_control.H"

using namespace fapi2;

// TODO via RTC 152424
// Currently all proecdures in core directory are in seeprom.
// So we have to use function pointer to force a long call.
#ifdef SEEPROM_IMAGE
p9_thread_control_FP_t threadCntlhwp = &p9_thread_control;
#endif

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
    if( EXIT_ON_FIRST_ERROR != i_req.mode )
    {
        l_warnCheck = true;
    }
    return l_warnCheck;
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
                    if(IGNORE_HW_ERRORS == l_req.mode)
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

            // If FapiRc from the inner loop (thread loop), just break here
            if ((l_fapiRc) && (IGNORE_HW_ERRORS != l_req.mode))
            {
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

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

