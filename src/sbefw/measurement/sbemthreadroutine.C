/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemthreadroutine.C $                   */
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

#include "sbemtrace.H"
#include "sbesecuritycommon.H"
#include "sbemsecuritysetting.H"
#include "sbeTPMCommand.H"
#include "sbemPcrStates.H"
#include "sbeOtpromMeasurementReg.H"
#include "sbestates.H"
#include "sbemutil.H"
#include "tpmStatusCodes.H"
#include "sbemTPMSequences.H"

#define TPM_DECONFIG_BIT_SHIFT 51

extern uint32_t g_sbeRole;

void writeTruncatedVerificationImageHash(SHA512truncated_t truncatedResult)
{
    uint64_t hashData = 0x00;
    memcpy(&hashData, &truncatedResult[sizeof(uint64_t) * 0], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG4), hashData);
    memcpy(&hashData, &truncatedResult[sizeof(uint64_t) * 1], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG5), hashData);
    memcpy(&hashData, &truncatedResult[sizeof(uint64_t) * 2], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG6), hashData);
    memcpy(&hashData, &truncatedResult[sizeof(uint64_t) * 3], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG7), hashData);
}

void sbemthreadroutine(void *i_pArg)
{
    #define SBEM_FUNC " sbeSecureBoot_routine "
    SBEM_ENTER(SBEM_FUNC);

    int rc = 0;
    uint64_t data = 0;
    uint64_t securityReg = 0;
    SHA512_t result;
    SHA512truncated_t truncatedResult;
    SHA512truncated_t truncatedResultSbVerif;
    securityState_PCR6_t securityStatePCR6;
    securityState_PCR1_t securityStatePCR1;
    uint32_t tpmRespCode = SBEM_TPM_OPERATION_SUCCESSFUL;

    do{
        //TODO: This needs to be moved into verification flow.
        sbemSetSecureAccessBit();

        // Update the Code Flow status in messaging register 50009
        uint64_t scratchMsgReg = (uint64_t)(SBE_CODE_MEASURMENT_TPM_INIT_SEQUENCE_MSG)<<32;
        PPE_STVD(0x50009, scratchMsgReg);

        // Reset the MAILBOX_SCRATCH_REG_11.
        PPE_STVD(0x50182, securityReg);

        // Startup TPM Sequence for Master Chip, Poison for Alt-master and Deconfig Bit for Secondary chips
        rc = performTPMSequences(g_sbeRole);
        if (rc)
        {
            SBEM_ERROR(SBEM_FUNC "performTPMSequences failed with rc 0x%08X. "
                                 "Deconfigured TPM and updated scratch 11", rc);
        }

        //Write extendSecurityStatePCR6 into measurement register x10010
        securityStatePCR6.update(g_sbeRole);
        data = 0;
        memcpy(&data, (uint8_t *)&securityStatePCR6, sizeof(securityState_PCR6_t));
        SBEM_INFO("Writing extendSecurityStatePCR6 details [0x%08X 0x%08X] into Register [0x%08X]",
                SBE::higher32BWord(data),SBE::lower32BWord(data), OTPROM_MEASUREMENT_REG0);
        putscom_abs(OTPROM_MEASUREMENT_REG0, data);

        //Write extendSecurityStatePCR1 into measurement register x10012
        securityStatePCR1.update(g_sbeRole);
        data = 0;
        memcpy(&data, (uint8_t *)&securityStatePCR1, sizeof(securityState_PCR1_t));
        SBEM_INFO("Writing extendSecurityStatePCR1 details [0x%08X 0x%08X] into Register [0x%08X]",
                SBE::higher32BWord(data),SBE::lower32BWord(data), OTPROM_MEASUREMENT_REG2);
        putscom_abs(OTPROM_MEASUREMENT_REG2, data);

        //Measure/Calculate SHA512 of .sb_verification XIP Section
        memset(truncatedResultSbVerif, 0x00, sizeof(SHA512truncated_t));
        if(getXipSize(P9_XIP_SECTION_SBE_SB_VERIFICATION) != 0x00)
        {
            SBEM_INFO("Measure/Calculate SHA512 of .sb_verification XIP Section");
            SHA512_XIP_section(P9_XIP_SECTION_SBE_SB_VERIFICATION, &result);
            for (uint8_t i=0; i<sizeof(result); i=i+4)
            {
                SBEM_INFO("SHA512 of .sb_verification XIP Section is %x %x %x %x",
                        result[i],result[i+1],result[i+2],result[i+3]);
            }
            memcpy(truncatedResultSbVerif, result, sizeof(SHA512truncated_t));
        }
        else
        {
            SBEM_ERROR(".sb_verification XIP section not found. Verification code not found.");
            SBEM_ERROR("SHA512 not calculated for .sb_verification XIP section.");
        }
        writeTruncatedVerificationImageHash(truncatedResultSbVerif);

        // Read the Security Switch Register for the TPM Deconfig Bit incase TPM is already
        // deconfigured at this point
        PPE_LVD(0x10005, securityReg);

        //Skip if error/rc/deconfig bit set in TPM sequence.
        if((g_sbeRole == SBE_ROLE_MASTER) && (!(securityReg >> TPM_DECONFIG_BIT_SHIFT)) )
        {
            memset(truncatedResult, 0x00, sizeof(SHA512truncated_t));
            //Extend HW key hash to PCR6 and PCR1 if SBE role is master.
            if(getXipSize(P9_XIP_SECTION_SBE_SB_SETTINGS) != 0x00)
            {
                memcpy(truncatedResult, (uint8_t *)(getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS) + sizeof(SHA512truncated_t)), sizeof(SHA512truncated_t));
            }
            else
            {
                SBEM_ERROR(".sb_settings XIP section not found. HW key hash not found. Extending 0x00 into TPM_PCR6 and TPM_PCR1");
            }

            SBEM_INFO("Extending HW key hash into TPM_PCR6");
            rc = tpmExtendPCR(TPM_PCR6, truncatedResult, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed while extending HW key hash into PCR6");
                tpmRespCode = SBEM_TPM_EXTEND_HW_KEY_HASH_PCR6_FAILURE;
                break;
            }
            SBEM_INFO("Extending HW key hash into TPM_PCR1");
            rc = tpmExtendPCR(TPM_PCR1, truncatedResult, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed while extending HW key hash into PCR1");
                tpmRespCode = SBEM_TPM_EXTEND_HW_KEY_HASH_PCR1_FAILURE;
                break;
            }

            //Extend Security state to PCR6.i.e Jumper State and MSV.
            memset(truncatedResult, 0x00, sizeof(SHA512truncated_t));
            memcpy(truncatedResult, &securityStatePCR6, sizeof(securityStatePCR6));
            SBEM_INFO("Extending extendSecurityStatePCR6 details into TPM_PCR6");
            rc = tpmExtendPCR(TPM_PCR6, truncatedResult, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed while extending SecurityState into PCR6");
                tpmRespCode = SBEM_TPM_EXTEND_SECURITY_STATE_PCR6_FAILURE;
                break;
            }

            //Extend Security state to PCR1.
            memset(truncatedResult, 0x00, sizeof(SHA512truncated_t));
            memcpy(truncatedResult, &securityStatePCR1, sizeof(securityStatePCR1));
            SBEM_INFO("Extending extendSecurityStatePCR1 details into TPM_PCR1");
            rc = tpmExtendPCR(TPM_PCR1, truncatedResult, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed while extending extending SecurityState into PCR1");
                tpmRespCode = SBEM_TPM_EXTEND_SECURITY_STATE_PCR1_FAILURE;
                break;
            }

            //Extend MSB 32 Bytes of result into TPM PCR0 and PCR6 if SBE
            //role is master.
            SBEM_INFO("Extending .sb_verification XIP Section hash into TPM_PCR0");
            rc = tpmExtendPCR(TPM_PCR0, truncatedResultSbVerif, sizeof(SHA512truncated_t));
            if(rc)
            {
                    SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed while extending extending Verification image has key into PCR0");
                    tpmRespCode = SBEM_TPM_EXTEND_VERIFICATION_IMAGE_HASH_PCR0_FAILURE;
                    break;
            }
            SBEM_INFO("Extending .sb_verification XIP Section hash into TPM_PCR6");
            rc = tpmExtendPCR(TPM_PCR6, truncatedResultSbVerif, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed while extending extending Verification image hash key into PCR6");
                tpmRespCode = SBEM_TPM_EXTEND_VERIFICATION_IMAGE_HASH_PCR6_FAILURE;
                break;
            }
        }
    }while(false);
    if((g_sbeRole == SBE_ROLE_MASTER) && (tpmRespCode != SBEM_TPM_OPERATION_SUCCESSFUL))
    {
        rc = setTPMDeconfigBit();
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "Failed to set the deconfig bit with rc 0x%08X", rc);
        }
        rc = setTPMFailureRespCode(tpmRespCode);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "Failed to set the scratch reg with the response code, rc 0x%08X", rc);
        }
    }

    //TODO: Once all systems are updated with latest measurement and boots
    //seeprom jump2bootImage(); needs to be removed.We will have to jump to
    //verification image.
#if 0
    // Load .sb_verification section into PIBMEM.
    P9XipHeader *hdr = getXipHdr();
    P9XipSection* pSection = &hdr->iv_section[P9_XIP_SECTION_SBE_SB_VERIFICATION];
    uint32_t dsize = pSection->iv_size;
    if(dsize)
    {
            uint32_t verificationOffset = pSection->iv_offset;;
            uint32_t verificationAddress = (g_headerAddr + verificationOffset);
            P9XipHeader *vhdr = (P9XipHeader *)(verificationAddress);
            P9XipSection* pVBase = &vhdr->iv_section[P9_XIP_SECTION_SBE_BASE];
            uint64_t *srcAddr = (uint64_t *)(pVBase->iv_offset + (uint32_t)vhdr);
            SBEM_INFO("Source addr is 0x%08X", srcAddr);
            loadSectionForVerification(srcAddr, (uint64_t *)(vhdr->iv_L1LoaderAddr));
            SBEM_INFO("Completed Loading of .sb_verification into PIBMEM. Verify the image.");
            jump2verificationImage((uint32_t )vhdr->iv_kernelAddr);
    }
    else
#endif
    {
        SBEM_INFO("No verification image, jump to boot");
        jump2bootImage();
    }

    SBEM_EXIT(SBEM_FUNC);
    #undef SBEM_FUNC
}
