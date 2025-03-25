/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbesecuritycommon.H $              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2025                        */
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
 * @file: ppe/sbe/sbefw/sbesecureboot.H
 *
 * @brief This file contains the Interfaces for SBE Secure Boot Verification
 *
 */

#ifndef __SBEFW_SBESECURITYCOMMON_H
#define __SBEFW_SBESECURITYCOMMON_H

#include "fapi2.H"
#include <stdint.h>
#include "sha512.H"
#include "p10_ipl_image.H"
#include "sbeXipUtils.H"
#include "ROM.H"
#include "p10_scom_pibms.H"
#include "sbevutil.H"

#define TPM_DECONFIG_BIT 12

typedef uint8_t SHA512truncated_t[SHA512_DIGEST_LENGTH/2];
extern const SHA512truncated_t SHA256separator;

//TPM PCR
#define TPM_PCR0 0x00
#define TPM_PCR1 0x01
#define TPM_PCR2 0x02
#define TPM_PCR3 0x03
#define TPM_PCR4 0x04
#define TPM_PCR5 0x05
#define TPM_PCR6 0x06
#define TPM_PCR7 0x07

#define UPDATE_ERROR_REG_SBEFW(err_code)                                     \
            getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);\
            secureBootStatus.status.sbeFWSecureHdrStatus = err_code;         \
            putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg)

#define UPDATE_ERROR_REG_HBBL(err_code)                                      \
            getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);\
            secureBootStatus.status.hbblSecureHdrStatus = err_code;          \
            putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg)

#define UPDATE_ERROR_REG_VERIFICATION_STATUS(err_code)                                      \
            getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);\
            secureBootStatus.status.loadVStatus = err_code;          \
            putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg)

#define UPDATE_ERROR_REG_VERIFICATION_STATUS_AND_HALT(err_code) \
            UPDATE_ERROR_REG_VERIFICATION_STATUS(err_code);     \
            pk_halt()

//Secure boot status Mailbox scratch 11 (SCOM x50182)
typedef union secureBootStatus
{
    uint64_t statusReg;
    struct
    {
        uint8_t loadVStatus;
        uint8_t tpmStatus;
        uint8_t hbblSecureHdrStatus;
        uint8_t sbeFWSecureHdrStatus;
        uint32_t unused;
    }status;

    //Constructor
    secureBootStatus()
    {
        statusReg = 0x00;
    };
}secureBootStatus_t;

/**
 * @brief Perform SHA512 update for particular XIP section.
 *        Data is read from seeprom via SPI.
 *        SHA512 is calculated always for a 8byte aligned region internally.
 *        If unaligned section it is a must for padding bytes to be zero's.
 *
 * @Param p9_xip_section_sbe_t XIP section for which SHA512 needs to be calculated
 *
 * @Param SHA512_CTX* SHA512 contest struct pointer used during SHA512 Init
 *
 * @Param sectionSize Returns byte size of XIP section included in hash calculation
 *
 * @return None
 */
void SHA512UpdateXipSection(p9_xip_section_sbe_t xipSection, SHA512_CTX* context, uint64_t* sectionSize);

/**
 * @brief Calculate SHA512 for a given XIP section.
 *
 * @Param p9_xip_section_sbe_t XIP section for which SHA512 needs to be calculated
 *
 * @Param SHA512_t Output SHA512 of XIP Section
 *
 * @return None
 */
void SHA512_XIP_section(p9_xip_section_sbe_t xipSection, SHA512_t *result);

/**
 * @brief Verify .sbe_fw or .hbbl payload size and update status reg in case of error
 *
 * @param i_sections XIP section for which payload size check is to be done
 * @param i_calPayloadSize actual payload size during image load
 * @param i_shPayloadSize expected payload size from secure header
 * @return ROM_response
 */
ROM_response verifyPayloadSize( const p9_xip_section_sbe_t i_sections,
                                uint32_t i_calPayloadSize,
                                uint32_t i_shPayloadSize);

/**
 * @brief Verify .sbe_fw or .hbbl payload hash and update status reg in case of error
 *
 * @param i_sections XIP section for which payload hash check is to be done
 * @param i_calPayloadHash actual payload hash calculated during image load
 * @param i_shPayloadHash expected payload hash from secure header
 * @return ROM_response
 */
ROM_response verifyPayloadHash( const p9_xip_section_sbe_t i_sections,
                                SHA_DIGEST_t& i_calPayloadHash,
                                SHA_DIGEST_t& i_shPayloadHash);

/**
 * @brief Write the truncated payload hash of .sbe_fw or .hbbl into OTP regs
 *
 * @param regs List to OTP regs into which truncated payload hash has to be written
 * @param regCount No of OTP regs
 * @param i_sha512Truncated truncated value of the payload hash to be written into OTP regs
 * @return true
 * @return false
 */
bool writeandverifytruncatedsha512(uint32_t regs[], int regCount, SHA512truncated_t i_sha512Truncated);

#endif /* __SBEFW_SBESECURITYCOMMON_H */
