/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdupdatecorefuncstates.C $            */
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
#include "ffdc.H"
#include "sbeglobals.H"
#include "sbeHostUtils.H"
#include "sbeHostMsg.H"

#include "sbetrace.H"
#include "sbeglobals.H"
#include "sbeFifoMsgUtils.H"
#include "sbeFFDC.H"
#include "chipop_handler.H"
#include "sbecmdupdatecorefuncstates.H"
#include "ipl_table.H"


uint32_t sbeSyncCoreTargetPSU(uint8_t *i_pArg){

    #define SBE_FUNC " sbeSyncCoreTargetPSU "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    psu2SbeCoreFunctionalState_t req = {};
        do
        {
            l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                      sizeof(req)/sizeof(uint64_t),
                                      (uint64_t*)&req,
                                      true);
            if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc ){
                SBE_ERROR(SBE_FUNC "Failed to extract SBE_HOST_PSU_MBOX_REG1");
                break;
            }
            uint64_t coreFuncState = req.coreFunctionalState();
            SBE_INFO(SBE_FUNC " core functional state = 0x%08x_%08x",
                     coreFuncState>>32, coreFuncState);
            updateCoreFunctionStates(coreFuncState);
        }while(false);
        sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, l_rc);
        SBE_EXIT(SBE_FUNC);
        return l_rc;
        #undef SBE_FUNC

}
