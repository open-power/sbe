/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevthreadroutine.C $                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021,2022                        */
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
#include "sbeDecompression.h"
#include "base_toc.H"

#define UPDATE_ERROR_REG_SBEFW(err_code)                                     \
            getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);\
            secureBootStatus.status.sbeFWSecureHdrStatus = err_code;         \
            putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg)

#define UPDATE_ERROR_REG_SBEFW_AND_HALT(err_code) \
            UPDATE_ERROR_REG_SBEFW(err_code);     \
            pk_halt()

#define UPDATE_ERROR_REG_HBBL(err_code)                                      \
            getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);\
            secureBootStatus.status.hbblSecureHdrStatus = err_code;          \
            putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg)

#define UPDATE_ERROR_REG_HBBL_AND_HALT(err_code) \
            UPDATE_ERROR_REG_HBBL(err_code);     \
            pk_halt()

#define BUILD_TAG_LENGTH 20

extern uint32_t g_sbevRole;
extern SHA512truncated_t SHA256separator;
extern uint32_t _base_origin __attribute__ ((section (".bss")));

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
    uint32_t fapirc = 0;

    do{
        SBEV_INFO(SBEV_FUNC "Verify Secure containers");

        // Copy the data.compressed to just above the verification
        uint32_t data_end_address = (uint32_t)(&_base_origin); // _base_origin is start of verification image which is defined in verification linker
        uint32_t data_start_address = 0; // will be returned by loadSeepromtoPibmem
        uint32_t data_size = 0;
        fapirc = loadSeepromtoPibmem(
                    P9_XIP_SECTION_SBE_DATA,
                    data_start_address,
                    data_end_address,
                    data_end_address - SBE_BASE_IMAGE_START, // assuming we have space still start of pIBMEM
                    data_size,
                    NULL);
        if(fapirc)
        {
            SBEV_ERROR(SBEV_FUNC "Loading data to pibmem is failed with rc [0x%08X], start [0x%08X] end [0x%08X]"
                fapirc, data_start_address, data_end_address);
            UPDATE_ERROR_REG_SBEFW_AND_HALT(BASE_DATA_DECOMPRESSION_FAILED);
        }

        // Now Copy the base.compressed to just above the data.compressed
        SHA512_CTX context;
        SHA512_t result;
        SHA512_Init(&context);
        uint32_t base_start_address = 0;
        uint32_t payload_size = 0;
        fapirc = loadSeepromtoPibmem(
                                    P9_XIP_SECTION_SBE_BASE,
                                    base_start_address,
                                    data_start_address,
                                    data_start_address - SBE_BASE_IMAGE_START, // assuming we have space still start of pIBMEM
                                    payload_size,
                                    &context);
        if(fapirc)
        {
            SBEV_ERROR(SBEV_FUNC "Loading base to pibmem is failed with rc [0x%08X], start [0x%08X] end [0x%08X]"
                fapirc, base_start_address, data_start_address);
            UPDATE_ERROR_REG_SBEFW_AND_HALT(BASE_CODE_DECOMPRESSION_FAILED);
        }

        SHA512_Final(&context, &result);
        for (uint8_t i=0; i<sizeof(result); i=i+4)
        {
            SBEV_INFO("SHA512 of payload is %x %x %x %x",
                    result[i],result[i+1],result[i+2],result[i+3]);
        }

        SBEV_INFO(SBEV_FUNC "Verify SBE-FW secure header.");
        SBEV_INFO(SBEV_FUNC "payload_size=%u", payload_size);
        payload_size = ALIGN_8_BYTE_CIELING(payload_size);
        SBEV_INFO(SBEV_FUNC "payload_size=%u", payload_size);
        sbeFwSecureHdrRsp = verifySecureHdr(
                                P9_XIP_SECTION_SBE_SBH_FIRMWARE,
                                VERIFY_HW_SIG_C_SBE_FW,
                                &result, payload_size, &sbeFwSecureHdrResponse);
        SBEV_INFO(SBEV_FUNC "Completed SBE-FW secure header verification. Response: [0x%08x] Status: [0x%02x]",
            sbeFwSecureHdrRsp, sbeFwSecureHdrResponse.statusCode);

        if( sbeFwSecureHdrRsp == ROM_DONE )
        {
            isSecureHdrPassed = 1;
            SBEV_INFO(SBEV_FUNC "Prefix header flag in SBE_FW secure container [0x%08x]", sbeFwSecureHdrResponse.flag);
            SBEV_INFO(SBEV_FUNC "Completed SBE_FW secure header verification. Response:[0x%08x] Status:[0x%02x]",
                sbeFwSecureHdrRsp, sbeFwSecureHdrResponse.statusCode);
        }
        if(!isSecureHdrPassed)
        {
            SBEV_ERROR(SBEV_FUNC "SBE_FW Secure Header Verification Failed. Response:[0x%08x] Status:[0x%02x]"
                sbeFwSecureHdrRsp, sbeFwSecureHdrResponse.statusCode);
            UPDATE_ERROR_REG_SBEFW(sbeFwSecureHdrResponse.statusCode);
        }
        // decompress the base section
        uint8_t *decompBuffer = (uint8_t*)SBE_BASE_IMAGE_START;
        SBEV_INFO(SBEV_FUNC "base_start_address=0x%08X, SBE_BASE_IMAGE_START=0x%08X",
                            base_start_address, SBE_BASE_IMAGE_START);
        uint8_t rc = decompress(
                        (uint8_t*)base_start_address,
                        &decompBuffer);
        uint32_t endOffset = (uint32_t)decompBuffer;
        if(rc)
        {
            SBEV_ERROR(SBEV_FUNC "SBE_FW decompression of base section is failed. "
                "rc:[0x%02X] base_start_address:[0x%08X] endOffset[0x%08X]",
                rc, base_start_address, endOffset);
            UPDATE_ERROR_REG_SBEFW_AND_HALT(BASE_CODE_DECOMPRESSION_FAILED);
        }
        // check the end address doesnot cross the 'hdr->iv_dataAddr'
        uint8_t buf[sizeof(P9XipHeader)] __attribute__ ((aligned(8))) = {0};
        getXipHdr(buf);
        P9XipHeader *bSeepromHdr = (P9XipHeader *)buf;
        uint32_t data_address = bSeepromHdr->iv_dataAddr;
        SBEV_INFO(SBEV_FUNC "data_address=0x%08X, endOffset=0x%08X",
                            data_address, endOffset);
        if(endOffset > data_address)
        {
            SBEV_ERROR(SBEV_FUNC "SBE_FW decompression of base section is failed. "
                "base_start_address:[0x%08X] endOffset[0x%08X] iv_dataAddr[0x%08X]",
                base_start_address, endOffset, data_address);
            UPDATE_ERROR_REG_SBEFW_AND_HALT(BASE_HEADER_INVALID_DATA_ADDR);
        }
        // decompress data section
        decompBuffer = (uint8_t*)data_address;
        rc = decompress(
                        (uint8_t*)data_start_address,
                        &decompBuffer);
        endOffset = (uint32_t)decompBuffer;
        if(rc)
        {
            SBEV_ERROR(SBEV_FUNC "SBE_FW decompression of data section is failed. "
                "rc:[0x%02X] data_start_address:[0x%08X] data_address:[0x] endOffset[0x%08X]",
                rc, data_start_address, data_address, endOffset);
            UPDATE_ERROR_REG_SBEFW_AND_HALT(BASE_DATA_DECOMPRESSION_FAILED);
        }

        // reserve the space for bss area.
        //      we are re-using iv_L2LoaderAddr for storing the bss space required
        uint32_t bssSpaceReqd = bSeepromHdr->iv_L2LoaderAddr;
        SBEV_INFO(SBEV_FUNC "bssSpaceReqd=0x%08X", bssSpaceReqd);
        if(endOffset + bssSpaceReqd > (uint32_t)(&_base_origin))
        {
            SBEV_ERROR(SBEV_FUNC "SBE_FW not enough space for bss area of base image "
                "bssSpaceReqd:[0x%08X] endOffset[0x%08X] _base_origin[0x%08X]",
                bssSpaceReqd, endOffset, (uint32_t)(&_base_origin));
            UPDATE_ERROR_REG_SBEFW_AND_HALT(BASE_HEADER_BSS_OVERFLOW);
        }

        ((base_toc_t*)(SBE_BASE_ORIGIN))->hbbl_start = ALIGN_8_BYTE_CIELING(endOffset + bssSpaceReqd);

        uint64_t loadValue = (uint64_t)(SBE_CODE_VERIFICATION_SBEFW_SECURE_HDR_DONE)<<32;
        PPE_STVD(0x50009, loadValue);

        sbevSetSecureAccessBit(isSecureHdrPassed, sbeFwSecureHdrResponse.flag);

        // Copy the HBBL to pibmem
        uint32_t hbbl_end_address = 0;
        uint32_t hbbl_start_address = ((base_toc_t*)(SBE_BASE_ORIGIN))->hbbl_start;
        uint32_t hbbl_size = 0;
        SHA512_Init(&context);
        fapirc = loadSeepromtoPibmem(P9_XIP_SECTION_SBE_HBBL,
                                hbbl_start_address,
                                hbbl_end_address,
                                (uint32_t)(&_base_origin) - hbbl_start_address, // we can load till verification image start
                                hbbl_size,
                                &context);
        if(fapirc)
        {
            SBEV_ERROR(SBEV_FUNC "Loading hbbl to pibmem is failed with rc [0x%08X], start [0x%08X] end [0x%08X]",
                fapirc, hbbl_start_address, hbbl_end_address);
            UPDATE_ERROR_REG_HBBL_AND_HALT(HBBL_LOADING_FAILED);
        }
        SBEV_INFO(SBEV_FUNC "hbbl_end_address=0x%08X, hbbl_size=0x%08X",
                            hbbl_end_address, hbbl_size);

        SHA512_Final(&context, &result);
        for (uint8_t i=0; i<sizeof(result); i=i+4)
        {
            SBEV_INFO("SHA512 of payload is %x %x %x %x",
                    result[i],result[i+1],result[i+2],result[i+3]);
        }
        ((base_toc_t*)(SBE_BASE_ORIGIN))->hbbl_size = hbbl_size;

        SBEV_INFO(SBEV_FUNC "Verify HBBL secure header.");
        sbeHbblSecureHdrRsp = verifySecureHdr(
                                P9_XIP_SECTION_SBE_SBH_HBBL,
                                VERIFY_HW_SIG_A_HBBL,
                                &result, hbbl_size, &hbblSecureHdrResponse);
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

        ((base_toc_t*)(SBE_BASE_ORIGIN))->hdct_start = ALIGN_8_BYTE_CIELING(hbbl_end_address);

        // Load HDCT, RINGS, FASTARRAY from SEEPROM to PIBMEM.
        uint32_t section_start_address = ((base_toc_t*)(SBE_BASE_ORIGIN))->hdct_start;

        // Array of XIP sections.
        // The order should be matched with base_toc_t structure.
        uint32_t section_array[] = {
                                       P9_XIP_SECTION_SBE_HDCT,
                                       P9_XIP_SECTION_SBE_RINGS,
                                       P9_XIP_SECTION_SBE_OVERRIDES,
                                       P9_XIP_SECTION_SBE_FA_EC_CL2_FAR,
                                       P9_XIP_SECTION_SBE_FA_EC_MMA_FAR,
                                       P9_XIP_SECTION_SBE_FA_RING_OVRD,
                                       P9_XIP_SECTION_SBE_SB_SETTINGS       // Boot Seeprom
                                   };

        // Pointer to PIBMEM TOC.
        uint32_t * pibmemPtr = (uint32_t *)SBE_BASE_ORIGIN;
        // Increment the pointer to point to HDCT start.
        pibmemPtr = pibmemPtr + 2;
        uint32_t section_end_address = 0;

        SBEV_INFO("Load all sections.");
        for(uint32_t count = 0; count < sizeof(section_array)/ sizeof(uint32_t); count++)
        {
            section_end_address = 0;
            uint32_t section_size = 0;
            bool measSeeprom = false;
            SBEV_INFO(SBEV_FUNC "Passed section_start_address 0x%08X section_end_address=0x%08X"
                                " section_size=0x%08X",
                                 section_array[count], section_end_address, section_size);

            fapirc = loadSeepromtoPibmem((p9_xip_section_sbe_t)section_array[count],
                                         section_start_address,
                                         section_end_address,
                                         (uint32_t)(&_base_origin) - section_start_address,
                                         section_size,
                                         NULL,
                                         measSeeprom);
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "Loading section from SEEPROM to pibmem is failed for section"
                                     " %d with rc [0x%08X], start [0x%08X] end [0x%08X]",
                                     section_array[count], fapirc, section_start_address, section_end_address);
                UPDATE_ERROR_REG_SBEFW_AND_HALT(HDCT_LOADING_FAILED + count);
            }
            SBEV_INFO(SBEV_FUNC "section %d section_start_address 0x%08X section_end_address=0x%08X"
                                " section_size=0x%08X",
                                 section_array[count], section_start_address, section_end_address, section_size);

            // Make the section offset 0 if the size is 0.
            if(section_size == 0)
            {
                *pibmemPtr = 0;
            }
            else
            {
                *pibmemPtr = section_start_address;
            }

            pibmemPtr++;
            *pibmemPtr = section_size;
            pibmemPtr++;
            section_start_address = section_end_address;
        }
        ((base_toc_t*)(SBE_BASE_ORIGIN))->mSbSettings_start = section_end_address;

        // Load Meas SB SETTINGS.
        SBEV_INFO("Load Meas SB SETTINGS ");
        section_start_address = section_end_address;
        section_end_address = 0;
        uint32_t section_size = 0;
        bool measSeeprom = true;
        SBEV_INFO(SBEV_FUNC "Passed section_start_address 0x%08X section_end_address=0x%08X"
                            " section_size=0x%08X",
                             section_start_address, section_end_address, section_size);

        fapirc = loadSeepromtoPibmem(P9_XIP_SECTION_SBE_SB_SETTINGS,
                                     section_start_address,
                                     section_end_address,
                                     (uint32_t)(&_base_origin) - section_start_address,
                                     section_size,
                                     NULL,
                                     measSeeprom);
        if(fapirc)
        {
            SBEV_ERROR(SBEV_FUNC "Loading section from SEEPROM to pibmem is failed for section"
                                 " %d with rc [0x%08X], start [0x%08X] end [0x%08X]",
                                 fapirc, section_start_address, section_end_address);
            UPDATE_ERROR_REG_SBEFW_AND_HALT(M_SB_SETTINGS_LOADING_FAILED);
        }
        SBEV_INFO(SBEV_FUNC "section %d section_start_address 0x%08X section_end_address=0x%08X"
                            " section_size=0x%08X",
                             P9_XIP_SECTION_SBE_SB_SETTINGS, section_start_address, section_end_address, section_size);

        // Update the base TOC.
        ((base_toc_t*)(SBE_BASE_ORIGIN))->mSbSettings_size = section_size;
        BASE_IMG_TOC->buildTag_start = section_end_address;

        // Copy the buildTag from Seeprom to Pibmem.
        uint8_t *buildPtr = (uint8_t *)(BASE_IMG_TOC->buildTag_start);
        for(uint32_t i = 0; i < BUILD_TAG_LENGTH; i++)
        {
            *(buildPtr + i) = bSeepromHdr->iv_buildTag[i];
        }

        // Update the size;
        BASE_IMG_TOC->buildTag_size = BUILD_TAG_LENGTH;

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
            fapirc = tpmExtendPCR(TPM_PCR0, sbeFwSecureHdrResponse.sha512Truncated, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending truncated hash of SBE_FW secure Hdr into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_SBE_FW_PAYLOAD_IMAGE_HASH_PCR0_FAILURE;
                break;
            }

            //Extend calculated truncated hash of HBBL secure Hdr into PCR0
            SBEV_INFO("Extending calculated truncated hash of HBBL secure Hdr into PCR0");
            fapirc = tpmExtendPCR(TPM_PCR0, hbblSecureHdrResponse.sha512Truncated, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending truncated hash of HBBL secure Hdr into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_HBBL_PAYLOAD_IMAGE_HASH_PCR0_FAILURE;
                break;
            }

            //Extend Security state to PCR1.i.e Security Switch Register
            memset(sha512Truncated, 0x00, sizeof(SHA512truncated_t));
            memcpy(sha512Truncated, &securitySwitchReg_PCR1, sizeof(securitySwitchReg_PCR1_t));
            SBEV_INFO("Extending securitySwitchReg_PCR1 details into TPM_PCR1");
            fapirc = tpmExtendPCR(TPM_PCR1, sha512Truncated, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending securitySwitchReg_PCR1 details into TPM_PCR1");
                tpmRespCode = SBEV_TPM_EXTEND_SECURITY_SWITCH_REG_PCR1_FAILURE ;
                break;
            }

            /*************************************************TPM_EXTEND_SEPERATOR*******************************************************/

            //Extend separator to denote change of control from measurement
            //SEEProm to Boots SEEProm PCR0 to PCR7
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR0");
            fapirc = tpmExtendPCR(TPM_PCR0, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR0_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR1");
            fapirc = tpmExtendPCR(TPM_PCR1, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR1");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR1_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR2");
            fapirc = tpmExtendPCR(TPM_PCR2, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR2");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR2_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR3");
            fapirc = tpmExtendPCR(TPM_PCR3, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR3");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR3_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR4");
            fapirc = tpmExtendPCR(TPM_PCR4, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR4");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR4_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR5");
            fapirc = tpmExtendPCR(TPM_PCR5, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR5");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR5_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR6");
            fapirc = tpmExtendPCR(TPM_PCR6, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
            {
                SBEV_ERROR(SBEV_FUNC "tpmExtendPCR failed while extending seperator into PCR6");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR6_FAILURE;
                break;
            }
            SBEV_INFO("Extending seperator to denote change of control from measurement to boots seeprom into TPM_PCR7");
            fapirc = tpmExtendPCR(TPM_PCR7, SHA256separator, sizeof(SHA512truncated_t));
            if(fapirc)
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
        fapirc = setTPMDeconfigBit();
        if( fapirc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEV_ERROR(SBEV_FUNC "Failed to set the deconfig bit with rc 0x%08X", fapirc);
        }

        SBEV_INFO(SBEV_FUNC "Setting the TPM response code into Scratch Reg 11");
        getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);
        secureBootStatus.status.tpmStatus = (uint8_t)tpmRespCode;
        putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg);
        SBEV_INFO("Updated SBE-FW and HBBL secure header status into Mailbox scratch 11 [0x%08x 0x%08x]",
                SBE::higher32BWord(secureBootStatus.statusReg), SBE::lower32BWord(secureBootStatus.statusReg));
    }

    // Copy remaining sections like hdct and attributes etc.
    // we can re-use same loadSeepromtoPibmem(void* src, void* dest, uint32_t size);

    SBEV_INFO("Jump to Boot from Verification.");
    jump2boot();

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

