/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbehostfifomsg.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
 * @file: ppe/sbe/sbefw/sbehostfifomsg.C
 *
 * @brief This file contains the SBE Host Fifo Chip-ops
 *
 */

#include "sbehostfifomsg.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sberegaccess.H"
#include "sbestates.H"

#include "fapi2.H"
#include "chipop_handler.H"
using namespace fapi2;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeHostHaltReq (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeHostHaltReq "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);
    SBE_INFO(SBE_FUNC "Fifo Type is:[%02X]",type);
    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        if(type != SBE_HB_FIFO)
        {
            //N-Ack this command and don't process
            SBE_INFO(SBE_FUNC "Request not allowed via fifo [%02X]", type);
            respHdr.setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED,SBE_SEC_NOT_ALLOWED_VIA_FIFO_1);
            break;
        }

        //Update SBE state to HALT state
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(SBE_HALT_VIA_HOST_EVENT);

        SBE_INFO(SBE_FUNC "PK HALT");
        pk_halt();
    } while(0);

    rc = sbeDsSendRespHdr(respHdr, NULL, type);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC"sbeDsSendRespHdr failed. rc[0x%X]", rc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}


