/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdCntrlTimer.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2022                        */
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
 * @file: src/sbefw/sbecmdCntrlTimer.C
 *
 * @brief This file contains the SBE Timer Commands
 *
 */

#include "sbecmdCntrlTimer.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFFDC.H"
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sbeTimerSvc.H"
#include "sbeglobals.H"
#include "sbes1handler.H"

#include "fapi2.H"
using namespace fapi2;

// Global instance to track PK timer
static timerService g_hostTimerSvc;
// Callback
void sbeTimerExpiryCallback(void *)
{
    #define SBE_FUNC "sbeTimerExpiryCallback "
    SBE_ENTER(SBE_FUNC);

    do
    {
        // indicate the Host via Bit SBE_SBE2PSU_DOORBELL_SET_BIT14
        // that Timer has expired
        SBE_INFO(SBE_FUNC "Updating door bell bit 14");
        uint32_t l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT14);
        if(l_rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Write "
                                "SBE_SBE2PSU_DOORBELL_SET_BIT14");
            __wait_for_s1();
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC

}
/////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
uint32_t sbeCntrlTimer( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeCntrlTimer "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_START_TIMER)
        {
            // Disable interrupts before sending the ACK for timer chipop.
            // Otherwise other high priority thread can interrupt it if
            // there is a interrupt on slow soft-fsi based systems,
            // taking initial data from FIFO may take time in command receiver
            // thread.
            PkMachineContext  ctx;
            pk_critical_section_enter(&ctx);
            do
            {
                uint64_t time = 0;
                l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                        (sizeof(time)/sizeof(uint64_t)),
                                        &time, true);

                if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
                {
                    SBE_ERROR(SBE_FUNC" Failed to extract "
                                                "SBE_HOST_PSU_MBOX_REG1");
                    break;
                }

                l_rc = g_hostTimerSvc.stopTimer( );
                if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
                {
                    SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                                            SBE_PRI_INTERNAL_ERROR, l_rc);
                    SBE_ERROR(SBE_FUNC" g_hostTimerSvc.stopTimer failed");
                    l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    break;
                }

                l_rc = g_hostTimerSvc.startTimer( (uint32_t )time,
                                     (PkTimerCallback)&sbeTimerExpiryCallback);
                if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
                {
                    SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                                            SBE_PRI_INTERNAL_ERROR, l_rc);
                    SBE_ERROR(SBE_FUNC" g_hostTimerSvc.startTimer failed");
                    l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    break;
                }
            }while(0);
            pk_critical_section_exit(&ctx);
            break;
        }
        // Acknowledge host
        l_rc = sbeAcknowledgeHost();
        if(l_rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                      "SBE_SBE2PSU_DOORBELL_SET_BIT1");
            break;
        }

        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_STOP_TIMER)
        {
            SBE_INFO(SBE_FUNC "Stop Timer.");
            l_rc = g_hostTimerSvc.stopTimer( );
            if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
            {
                SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INTERNAL_ERROR, l_rc);
                SBE_ERROR(SBE_FUNC" g_hostTimerSvc.stopTimer failed");
                l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                break;
            }
            break;
        }

        SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus( SBE_PRI_INVALID_COMMAND,
                                       SBE_SEC_COMMAND_NOT_SUPPORTED);
        SBE_ERROR(SBE_FUNC" Not a valid flag 0x%4X",
                  (uint16_t) SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags);
    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, l_rc);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

