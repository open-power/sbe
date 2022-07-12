/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeXipUtils.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2022                        */
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
#ifndef SBE_XIP_UTILS_H
#define SBE_XIP_UTILS_H

#include "p10_ipl_image.H"
#include "ppe42_scom.h"  // for PPE_STVD
#include "sbe_link.H"  // for SBE_BASE_ORIGIN, SBE_SEEPROM_BASE_ORIGIN

// Unconditional jump to a memory location
#define JUMP_TO_ADDR(VAL ) \
{ \
asm volatile ( \
    "mtlr  %0 \n\t" \
    :   \
    : "r" (VAL)); \
asm volatile ( \
    "blr  \n\t" \
    :   \
    );\
}

// PIBMEM start address
const uint32_t g_pibMemAddr = SBE_BASE_ORIGIN;
// SEEPROM start address
const uint32_t g_headerAddr = SBE_SEEPROM_BASE_ORIGIN;
// Measurement Seeprom start address
const uint32_t g_mseeprom_headerAddr = SBE_MEASUREMENT_BASE_ORIGIN;
// IVPR register address
const uint32_t g_ivprLoc = 0xC0000160;

// Get Measurement Seeprom XIP image header
inline P9XipHeader * getMSeepromXipHdr()
{
    P9XipHeader *hdr = (P9XipHeader *)( g_mseeprom_headerAddr );
    return hdr;
}

// Get Measurement Seeprom Section start addess
inline uint8_t * getMSeepromSectionAddr( P9XipSection * i_section )
{
    return ( (uint8_t *)( g_mseeprom_headerAddr + i_section->iv_offset));
}

// Get XIP image header
inline P9XipHeader * getXipHdr()
{
    P9XipHeader *hdr = (P9XipHeader *)( g_headerAddr );
    return hdr;
}

// Get Section start addess
inline uint8_t * getSectionAddr( P9XipSection * i_section )
{
    return ( (uint8_t *)( g_headerAddr + i_section->iv_offset));
}

/*
 *  @brief Get the Xip offset(Start location) from SBE Image for SPI.
 *
 *  @param[in] xipSection XIP section
 *
 *  @return Xip offset(Start location) SPI
 */
uint32_t getXipOffset(p9_xip_section_sbe_t xipSection);

/*
 *  @brief Get the Absolute Xip offset(Start location) from SBE Image.
 *
 *  @param[in] xipSection XIP section
 *
 *  @return Xip offset(Start location)
 */
uint32_t getXipOffsetAbs(p9_xip_section_sbe_t xipSection);

/*
 *  @brief Get the Xip Size.
 *
 *  @param[in] xipSection XIP section
 *
 *  @return Xip Size
 */
uint32_t getXipSize(p9_xip_section_sbe_t xipSection);

/*
 *  @brief Get the Xip Size for the Measurement SEEPROM.
 *
 *  @param[in] xipSection XIP section
 *
 *  @return Xip Size
 */
uint32_t getXipSizeMeasurement(p9_xip_section_sbe_t xipSection);

//TODO:Use common function for measurement and boots .
/*
 * @brief Get the Absolute Xip offset(Start location) from SBE Measurement Image.
 *
 * @param[in] xipSection XIP section
 *
 * @return Xip offset(Start location) for measurement image
 */
uint32_t getXipOffsetAbsMeasurement(p9_xip_section_sbe_t xipSection);

#endif // SBE_XIP_UTILS_H
