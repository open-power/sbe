/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbesecureboot.H $                      */
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

#ifndef __SBEFW_SBECMDCNTLINST_H
#define __SBEFW_SBECMDCNTLINST_H

#include <stdint.h>
#include "p10_sbe_spi_cmd.H"
#include "p10_ipl_image.H"
#include "ROM.H"

/**
 * @brief Get the Xip offset(Start location) from SBE Image.
 *
 * @return Xip offset(Start location)
 */
uint32_t getXipOffset(p9_xip_section_sbe_t xipSection);

/**
 * @brief Get the Xip Size.
 *
 * @return Xip Size
 */
uint32_t getXipSize(p9_xip_section_sbe_t xipSection);

/**
 * @brief Calculate SHA512 for sbe_verification XIP-Section.
 *
 * @Param SHA512_t Output SHA512 of XIP Section
 *
 * @return None
 */
void SHA512_Verification_Code_Hash(SHA512_t *result);

/**
 * @brief Verify Secure container.
 *
 * @param ROM_container_raw* Pointer to secure container start address
 *
 * @param ROM_hw_params* Pointer to HW Keys Hash
 *
 * @param hw_sig_to_verify The hardware signature that has to be verified(Among the 3 HW signaturs that are present)
 *
 * @return Secure container verification response/result/status_code.
 */
ROM_response ROM_verify(ROM_container_raw* container, ROM_hw_params* params, int hw_sig_to_verify);

/**
 * @brief Verify HBBL Secure Header.
 *
 * @return Secure container verification response/result/status_code
 */
ROM_response verifyHBBLSecureHdr();

/**
 * @brief Verify SBE-FW Secure Header.
 *
 * @return Secure container verification response/result/status_code
 */
ROM_response verifyFWSecureHdr();





















#endif /* __SBEFW_SBECMDCNTLINST_H */
