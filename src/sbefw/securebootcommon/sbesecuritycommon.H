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

#include <stdint.h>
#include "sha512.H"
#include "p10_sbe_spi_cmd.H"
#include "p10_ipl_image.H"
#include "plat_hwp_data_stream.H"

/**
 * @brief Get the Xip offset(Start location) from SBE Image for SPI.
 *
 * @return Xip offset(Start location) SPI
 */
uint32_t getXipOffset(p9_xip_section_sbe_t xipSection);

/**
 * @brief Get the Absolute Xip offset(Start location) from SBE Image.
 *
 * @return Xip offset(Start location)
 */
uint32_t getXipOffsetAbs(p9_xip_section_sbe_t xipSection);

/**
 * @brief Get the Xip Size.
 *
 * @return Xip Size
 */
uint32_t getXipSize(p9_xip_section_sbe_t xipSection);

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
