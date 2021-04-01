/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbesecuritycommon.H $              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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

#define TPM_DECONFIG_BIT 12

typedef uint8_t SHA512truncated_t[SHA512_DIGEST_LENGTH/2];

//TPM PCR
#define TPM_PCR0 0x00
#define TPM_PCR1 0x01
#define TPM_PCR2 0x02
#define TPM_PCR3 0x03
#define TPM_PCR4 0x04
#define TPM_PCR5 0x05
#define TPM_PCR6 0x06
#define TPM_PCR7 0x07

//Secure boot status Mailbox scratch 11 (SCOM x50182)
typedef union secureBootStatus
{
    uint64_t statusReg;
    struct
    {
        uint8_t reserved;
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
 * @return None
 */
void SHA512UpdateXipSection(p9_xip_section_sbe_t xipSection, SHA512_CTX* context);

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

#endif /* __SBEFW_SBESECURITYCOMMON_H */
