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
#include "sbeTPMCommand.H"
#include "sbestates.H"
#include "sbevPcrStates.H"
#include "sbeOtpromMeasurementReg.H"
#include "sbesecuritycommon.H"
#include "p10_scom_pibms.H"
#include "tpmStatusCodes.H"
#include "sbevsecuritysetting.H"

extern uint32_t g_sbevRole;
extern SHA512truncated_t SHA256separator;

static void writeTruncatedSbeFwPayloadHash(SHA512truncated_t i_sha512Truncated)
{
    uint64_t hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 0], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG8), hashData);
    hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 1], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG9), hashData);
    hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 2], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG10), hashData);
    hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 3], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG11), hashData);
}

static void writeTruncatedHbblPayloadHash(SHA512truncated_t i_sha512Truncated)
{
    uint64_t hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 0], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG12), hashData);
    hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 1], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG13), hashData);
    hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 2], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG14), hashData);
    hashData = 0x00;
    memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * 3], sizeof(uint64_t));
    putscom_abs((OTPROM_MEASUREMENT_REG15), hashData);
}

void sbevthreadroutine(void *i_pArg)
{
    #define SBEV_FUNC " sbevthreadroutine "
    SBEV_ENTER(SBEV_FUNC);

    SBEV_INFO("Inside verification thread");

    secureHdrResponse_t hbblSecureHdrResponse;
    secureHdrResponse_t sbeFwSecureHdrResponse;
    securitySwitchReg_PCR1_t securitySwitchReg_PCR1;
    SHA512truncated_t sha512Truncated;
    secureBootStatus_t secureBootStatus;
    ROM_response sbeFwSecureHdrRsp;
    ROM_response sbeHbblSecureHdrRsp;
    fapi2::buffer<uint64_t> securityReg;
    uint64_t data = 0;
    uint32_t isSecureHdrPassed = 0;
    uint32_t tpmRespCode = SBEM_TPM_OPERATION_SUCCESSFUL;
    int rc = 0;

    do{
        SBEV_INFO(SBEV_FUNC "Verify Secure containers");

        SBEV_INFO(SBEV_FUNC "Verify SBE-FW secure header.");
        sbeFwSecureHdrRsp = verifySecureHdr(P9_XIP_SECTION_SBE_SBH_FIRMWARE, VERIFY_HW_SIG_C_SBE_FW, &sbeFwSecureHdrResponse);
        SBEV_INFO(SBEV_FUNC "Completed SBE-FW secure header verification. Response: [0x%08x] Status: [0x%02x]",
            sbeFwSecureHdrRsp, sbeFwSecureHdrResponse.statusCode);

        uint64_t loadValue = (uint64_t)(SBE_CODE_VERIFICATION_SBEFW_SECURE_HDR_DONE)<<32;
        PPE_STVD(0x50009, loadValue);

        //TODO: Set secure acces bit based on prefix header flag in SBE_FW
        //secure container
        if( sbeFwSecureHdrRsp == ROM_DONE )
        {
            isSecureHdrPassed = 1;
            SBEV_INFO(SBEV_FUNC "Prefix header flag in SBE_FW secure container [0x%08x]", sbeFwSecureHdrResponse.flag);
            SBEV_INFO(SBEV_FUNC "Completed SBE_FW secure header verification. Response:[0x%08x] Status:[0x%02x]",
                sbeFwSecureHdrRsp, sbeFwSecureHdrResponse.statusCode);
            sbevSetSecureAccessBit(isSecureHdrPassed, sbeFwSecureHdrResponse.flag);
        }
        else
        {
            SBEV_ERROR(SBEV_FUNC "SBE_FW Secure Header Verification Failed. Response:[0x%08x] Status:[0x%02x]"
                sbeFwSecureHdrRsp, sbeFwSecureHdrResponse.statusCode);
            sbevSetSecureAccessBit(isSecureHdrPassed, sbeFwSecureHdrResponse.flag);
        }

        SBEV_INFO(SBEV_FUNC "Verify HBBL secure header.");
        sbeHbblSecureHdrRsp = verifySecureHdr(P9_XIP_SECTION_SBE_SBH_HBBL, VERIFY_HW_SIG_A_HBBL, &hbblSecureHdrResponse);
        if( sbeHbblSecureHdrRsp == ROM_FAILED )
        {
            SBEV_ERROR(SBEV_FUNC "HBBL Secure Header Verification Failed. Response:[0x%08x] Status:[0x%02x]"
                sbeHbblSecureHdrRsp, hbblSecureHdrResponse.statusCode);
        }
        else
        {
            SBEV_INFO(SBEV_FUNC "Completed HBBL secure header verification. Response:[0x%08x] Status:[0x%02x]",
                sbeHbblSecureHdrRsp, hbblSecureHdrResponse.statusCode);
        }

        loadValue = (uint64_t)(SBE_CODE_VERIFICATION_HBBL_SECURE_HDR_DONE)<<32;
        PPE_STVD(0x50009, loadValue);

        //Update SBE-FW and HBBL secure header status into Mailbox scratch 11.
        getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);
        secureBootStatus.status.sbeFWSecureHdrStatus = sbeFwSecureHdrResponse.statusCode;
        secureBootStatus.status.hbblSecureHdrStatus = hbblSecureHdrResponse.statusCode;
        putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg);
        SBEV_INFO("Updated SBE-FW and HBBL secure header status into Mailbox scratch 11 [0x%08x 0x%08x]",
                SBE::higher32BWord(secureBootStatus.statusReg), SBE::lower32BWord(secureBootStatus.statusReg));

        //Write SBE_FW truncated payload hash into otprom register 8-11 (x10018-x1001B)
        SBEV_INFO("Writing truncated SBE_FW payload hash into otprom register 8-11 (x10018-x1001B)");
        writeTruncatedSbeFwPayloadHash(sbeFwSecureHdrResponse.sha512Truncated);

        //Write HBBL truncated payload hash into otprom register 12-15 (x1001C-x1001F)
        SBEV_INFO("Writing truncated HBBL payload hash into otprom register 12-15 (x1001C-x1001F)");
        writeTruncatedHbblPayloadHash(hbblSecureHdrResponse.sha512Truncated);

        //Check if TPM Deconfig bit is set.(Write the updated value into otprom
        //regs and TPM)
        getscom_abs(0x10005, &securityReg());

        //Write extendSecurityStatePCR1 into measurement register x10013
        securitySwitchReg_PCR1.update(securityReg());
        data = 0;
        memcpy(&data, (uint8_t *)&securitySwitchReg_PCR1, sizeof(securitySwitchReg_PCR1_t));
        SBEV_INFO("Writing securitySwitchReg_PCR1 details [0x%08X 0x%08X] into Register [0x%08X]",
                SBE::higher32BWord(data),SBE::lower32BWord(data), OTPROM_MEASUREMENT_REG3);
        putscom_abs(OTPROM_MEASUREMENT_REG3, data);

        //Skip if error/rc/deconfig bit set in TPM sequence.
        if( (g_sbevRole == SBE_ROLE_MASTER) && (!(securityReg.getBit<TPM_DECONFIG_BIT>())) )
        {
            //Extend calculated truncated hash of SBE_FW secure Hdr into PCR0
            SBEV_INFO("Extending calculated truncated hash of SBE_FW secure Hdr into PCR0");
            rc = tpmExtendPCR(TPM_PCR0, sbeFwSecureHdrResponse.sha512Truncated, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending truncated hash of SBE_FW secure Hdr into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_SBE_FW_PAYLOAD_IMAGE_HASH_PCR0_FAILURE;
                break;
            }

            //Extend calculated truncated hash of HBBL secure Hdr into PCR0
            SBEV_INFO("Extending calculated truncated hash of HBBL secure Hdr into PCR0");
            rc = tpmExtendPCR(TPM_PCR0, hbblSecureHdrResponse.sha512Truncated, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending truncated hash of HBBL secure Hdr into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_HBBL_PAYLOAD_IMAGE_HASH_PCR0_FAILURE;
                break;
            }

            //Extend Security state to PCR1.i.e Security Switch Register
            memset(sha512Truncated, 0x00, sizeof(SHA512truncated_t));
            memcpy(sha512Truncated, &securitySwitchReg_PCR1, sizeof(securitySwitchReg_PCR1_t));
            SBEV_INFO("Extending securitySwitchReg_PCR1 details into TPM_PCR1");
            rc = tpmExtendPCR(TPM_PCR1, sha512Truncated, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending securitySwitchReg_PCR1 details into TPM_PCR1");
                tpmRespCode = SBEV_TPM_EXTEND_SECURITY_SWITCH_REG_PCR1_FAILURE ;
                break;
            }

            /*************************************************TPM_EXTEND_SEPERATOR*******************************************************/

            //Extend separator to denote change of control from measurement
            //SEEProm to Boots SEEProm PCR0 to PCR7
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR0");
            rc = tpmExtendPCR(TPM_PCR0, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR0_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR1");
            rc = tpmExtendPCR(TPM_PCR1, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR1");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR1_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR2");
            rc = tpmExtendPCR(TPM_PCR2, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR2");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR2_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR3");
            rc = tpmExtendPCR(TPM_PCR3, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR3");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR3_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR4");
            rc = tpmExtendPCR(TPM_PCR4, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR4");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR4_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR5");
            rc = tpmExtendPCR(TPM_PCR5, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR5");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR5_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR6");
            rc = tpmExtendPCR(TPM_PCR6, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR6");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR6_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR7");
            rc = tpmExtendPCR(TPM_PCR7, SHA256separator, sizeof(SHA512truncated_t));
            if(rc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR7");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR7_FAILURE;
                break;
            }

            /*************************************************TPM_EXTEND_SEPERATOR_END***************************************************/
        }

    }while(false);

    if( (g_sbevRole == SBE_ROLE_MASTER) &&
        (tpmRespCode != SBEM_TPM_OPERATION_SUCCESSFUL) &&
        (!(securityReg.getBit<TPM_DECONFIG_BIT>())) )
    {
        SBEV_INFO(SBEV_FUNC "Setting the TPM deconfig bit");
        rc = setTPMDeconfigBit();
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEV_ERROR(SBEV_FUNC "Failed to set the deconfig bit with rc 0x%08X", rc);
        }

        SBEV_INFO(SBEV_FUNC "Setting the TPM response code into Scratch Reg 11");
        getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);
        secureBootStatus.status.tpmStatus = (uint8_t)tpmRespCode;
        putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg);
        SBEV_INFO("Updated SBE-FW and HBBL secure header status into Mailbox scratch 11 [0x%08x 0x%08x]",
                SBE::higher32BWord(secureBootStatus.statusReg), SBE::lower32BWord(secureBootStatus.statusReg));
    }
    SBEV_INFO("Jump to Boot from Verification.");
    jump2boot();

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

