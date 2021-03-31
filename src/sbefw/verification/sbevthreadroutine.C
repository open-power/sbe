/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevthreadroutine.C $                  */
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
#include "sbevtrace.H"
#include "sbevutil.H"
#include "sbesecureboot.H"
//#include "sbeTPMCommand.H"
#include "sbestates.H"

extern uint32_t g_sbevRole;

void sbevthreadroutine(void *i_pArg)
{
    #define SBEV_FUNC " sbevthreadroutine "
    SBEV_ENTER(SBEV_FUNC);

    //secureHdrResponse_t hbblSecureHdrResponse;
    //secureHdrResponse_t sbeFwSecureHdrResponse;
    uint64_t securityReg = 0;

    do{
        //Check if TPM Deconfig bit is set.
        PPE_LVD(0x10005, securityReg);

        SBEV_INFO(SBEV_FUNC "Verify Secure containers");
#if 0
        SBEV_INFO("Verify SBE-FW secure header.");
        verifySecureHdr(P9_XIP_SECTION_SBE_SBH_FIRMWARE, VERIFY_HW_SIG_C_SBE_FW, &sbeFwSecureHdrResponse);
        SBEV_INFO("Completed SBE-FW secure header verification. Status: %x", sbeFwSecureHdrResponse.statusCode);

        SBEV_INFO("Verify HBBL secure header.");
        verifySecureHdr(P9_XIP_SECTION_SBE_SBH_HBBL, VERIFY_HW_SIG_A_HBBL, &hbblSecureHdrResponse);
        SBEV_INFO("Completed HBBL secure header verification.Status: %x", hbblSecureHdrResponse.statusCode);

        //Skip if error/rc/deconfig bit set in TPM sequence.
        if((g_sbevRole == SBE_ROLE_MASTER) && (!(securityReg >> TPM_DECONFIG_BIT_SHIFT)))
        {
            //Extend calculated truncated hash of SBE_FW secure Hdr into PCR0
            SBEV_INFO("Extending calculated truncated hash of SBE_FW secure Hdr into PCR0");
            tpmExtendPCR(TPM_PCR0, sbeFwSecureHdrResponse.SHA512TruncatedResult, sizeof(SHA512truncated_t));

            //Extend calculated truncated hash of HBBL secure Hdr into PCR0
            SBEV_INFO("Extending calculated truncated hash of HBBL secure Hdr into PCR0");
            tpmExtendPCR(TPM_PCR0, hbblSecureHdrResponse.SHA512TruncatedResult, sizeof(SHA512truncated_t));
        }
#endif
        SBEV_INFO(SBEV_FUNC "Jump to Boot Seeprom Image.");
        jump2boot();

    }while(false);

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}
