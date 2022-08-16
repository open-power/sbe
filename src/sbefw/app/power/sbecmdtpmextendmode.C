/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdtpmextendmode.C $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
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

#include "sbecmdtpmextendmode.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_host_intf.H"
#include "sbeglobals.H"
#include "sbeHostUtils.H"
#include "sbestates.H"

uint32_t sbeTpmExtendMode(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeTpmExtendMode "
    SBE_ENTER(SBE_FUNC)

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_ENTER_TPM_EXTEND_MODE)
        {
            break;
        }

        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_EXIT_TPM_EXTEND_MODE)
        {
            break;
        }

        SBE_ERROR(SBE_FUNC" Not a valid command ");
        rc = SBE_SEC_COMMAND_NOT_SUPPORTED;

        // Send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        // This util method will check internally on the mbox0 register if ACK
        // is requested.
        rc = sbeAcknowledgeHost();
        if(rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                    "SBE_SBE2PSU_DOORBELL_SET_BIT1");
            break;
        }
    }while(0);

    if( (fapiRc != FAPI2_RC_SUCCESS) && (rc == SBE_SEC_OPERATION_SUCCESSFUL) )
    {
        SBE_ERROR(SBE_FUNC "fapiRc=0x%.8x rc[0x%X]",fapiRc,rc);
    }

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC" Failed to write SBE_HOST_PSU_MBOX_REG4. rc[0x%X]", rc);
        (void)SbeRegAccess::theSbeRegAccess().
              updateSbeState(SBE_STATE_TPM_EXTEND_MODE_HALT);
        SBE_INFO("Halting PPE...");
        pk_halt();
    }

    SBE_EXIT(SBE_FUNC)
    return rc;
    #undef SBE_FUNC
}