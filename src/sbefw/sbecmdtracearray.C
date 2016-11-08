/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdtracearray.C $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
 * @file: ppe/sbe/sbefw/sbecmdtracearray.C
 *
 * @brief This file contains the SBE Control Tracearray chipOp
 *
 */
#include "sbecmdtracearray.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbeutil.H"
#include "fapi2.H"

using namespace fapi2;

static const uint32_t SIZE_OF_LENGTH_INWORDS = 1;

uint32_t sbeControlTraceArray(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeControlTraceArray"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    sbeControlTraceArrayCMD_t l_req = {};
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t l_ffdc;
    ReturnCode l_fapiRc;
    uint32_t l_NumWordsRead = 0;
    uint32_t l_len = 0;

    do
    {
        l_len = sizeof(sbeControlTraceArrayCMD_t)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len, (uint32_t *)&l_req); //EOT fetch

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                         " traceArrayId [0x%04X] operation [0x%04X]",
                         l_req.targetType,
                         l_req.chipletId,
                         l_req.traceArrayId,
                         l_req.operation);

        // Call trace array HWP in a loop

    } while(false);

    // Now build and enqueue response into downstream FIFO
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if ( SBE_SEC_OPERATION_SUCCESSFUL == l_rc )
    {
        l_len = SIZE_OF_LENGTH_INWORDS;
        l_rc = sbeDownFifoEnq_mult (l_len, &(l_NumWordsRead));
        if(SBE_SEC_OPERATION_SUCCESSFUL == l_rc)
        {
            l_rc = sbeDsSendRespHdr( respHdr, &l_ffdc);
        }
    }
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
