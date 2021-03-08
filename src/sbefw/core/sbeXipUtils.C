/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeXipUtils.C $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
 * @file: ppe/sbe/sbefw/core/sbeXipUtils.C
 *
 * @brief This file contains the SBE Secure Boot Verification Code
 *
 */

#include "sbeXipUtils.H"

uint32_t getXipOffset(p9_xip_section_sbe_t xipSection)
{
#if defined DFT || defined PIBMEM_ONLY_IMAGE
    uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
    uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif
    P9XipHeader* imgHdr = (P9XipHeader*)(base);
    p9_xip_section_sbe_t sectionName = xipSection;
    P9XipSection* pSection = &imgHdr->iv_section[sectionName];
    return pSection->iv_offset;
}

uint32_t getXipOffsetAbs(p9_xip_section_sbe_t xipSection)
{
#if defined DFT || defined PIBMEM_ONLY_IMAGE
    uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
    uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif

    return ( (uint32_t)base + getXipOffset(xipSection));
}

uint32_t getXipSize(p9_xip_section_sbe_t xipSection)
{
#if defined DFT || defined PIBMEM_ONLY_IMAGE
    uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
    uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif
    P9XipHeader* imgHdr = (P9XipHeader*)(base);
    p9_xip_section_sbe_t sectionName = xipSection;
    P9XipSection* pSection = &imgHdr->iv_section[sectionName];
    return (pSection->iv_size);
}
