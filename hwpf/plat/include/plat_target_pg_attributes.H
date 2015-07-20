/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: $                                                             */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2014                        */
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
/**
 * @file plat_target_pg_attribute.H
 * @brief Definitions for fapi2 PPE targets' partial good attribute mapping
 */

#ifndef __FAPI2_PPE_TARGET_PG_ATTR__
#define __FAPI2_PPE_TARGET_PG_ATTR__


typedef struct chiplet_pg_entry_t
{
    // char[16]  pg_attribute;
    uint32_t  pg_attribute;
    uint8_t   target_type;
    uint16_t  relative_target_num;
} chiplet_pg_entry_t;

const chiplet_pg_entry_t CHIPLET_PG_ARRAY[] =
{
     // Pervasive Chiplets
     { fapi2::ATTR_PG_PRV , fapi2::TARGET_TYPE_PERV,      0x01 },
     { fapi2::ATTR_PG_N0  , fapi2::TARGET_TYPE_PERV,      0x02 },
     { fapi2::ATTR_PG_N1  , fapi2::TARGET_TYPE_PERV,      0x03 },
     { fapi2::ATTR_PG_N2  , fapi2::TARGET_TYPE_PERV,      0x04 },
     { fapi2::ATTR_PG_N3  , fapi2::TARGET_TYPE_PERV,      0x05 },
     { fapi2::ATTR_PG_XB  , fapi2::TARGET_TYPE_PERV,      0x06 },
     { fapi2::ATTR_PG_MC01, fapi2::TARGET_TYPE_PERV,      0x07 },
     { fapi2::ATTR_PG_MC23, fapi2::TARGET_TYPE_PERV,      0x08 },
     { fapi2::ATTR_PG_OB0 , fapi2::TARGET_TYPE_PERV,      0x09 },
     { fapi2::ATTR_PG_OB1 , fapi2::TARGET_TYPE_PERV,      0x0A },
     { fapi2::ATTR_PG_OB2 , fapi2::TARGET_TYPE_PERV,      0x0B },
     { fapi2::ATTR_PG_OB3 , fapi2::TARGET_TYPE_PERV,      0x0C },
     { fapi2::ATTR_PG_PCI0, fapi2::TARGET_TYPE_PERV,      0x0D },
     { fapi2::ATTR_PG_PCI1, fapi2::TARGET_TYPE_PERV,      0x0E },
     { fapi2::ATTR_PG_PCI2, fapi2::TARGET_TYPE_PERV,      0x0F },
     // EQ Chiplets
     { fapi2::ATTR_PG_EQ0 , fapi2::TARGET_TYPE_EQ,        0x00 },
     { fapi2::ATTR_PG_EQ1 , fapi2::TARGET_TYPE_EQ,        0x01 },
     { fapi2::ATTR_PG_EQ2 , fapi2::TARGET_TYPE_EQ,        0x02 },
     { fapi2::ATTR_PG_EQ3 , fapi2::TARGET_TYPE_EQ,        0x03 },
     { fapi2::ATTR_PG_EQ4 , fapi2::TARGET_TYPE_EQ,        0x04 },
     { fapi2::ATTR_PG_EQ5 , fapi2::TARGET_TYPE_EQ,        0x05 },
     // Core Chiplets
     { fapi2::ATTR_PG_EC00, fapi2::TARGET_TYPE_CORE,      0x00 },
     { fapi2::ATTR_PG_EC01, fapi2::TARGET_TYPE_CORE,      0x01 },
     { fapi2::ATTR_PG_EC02, fapi2::TARGET_TYPE_CORE,      0x02 },
     { fapi2::ATTR_PG_EC03, fapi2::TARGET_TYPE_CORE,      0x03 },
     { fapi2::ATTR_PG_EC04, fapi2::TARGET_TYPE_CORE,      0x04 },
     { fapi2::ATTR_PG_EC05, fapi2::TARGET_TYPE_CORE,      0x05 },
     { fapi2::ATTR_PG_EC06, fapi2::TARGET_TYPE_CORE,      0x06 },
     { fapi2::ATTR_PG_EC07, fapi2::TARGET_TYPE_CORE,      0x07 },
     { fapi2::ATTR_PG_EC08, fapi2::TARGET_TYPE_CORE,      0x08 },
     { fapi2::ATTR_PG_EC09, fapi2::TARGET_TYPE_CORE,      0x09 },
     { fapi2::ATTR_PG_EC10, fapi2::TARGET_TYPE_CORE,      0x0A },
     { fapi2::ATTR_PG_EC11, fapi2::TARGET_TYPE_CORE,      0x0B },
     { fapi2::ATTR_PG_EC12, fapi2::TARGET_TYPE_CORE,      0x0C },
     { fapi2::ATTR_PG_EC13, fapi2::TARGET_TYPE_CORE,      0x0D },
     { fapi2::ATTR_PG_EC14, fapi2::TARGET_TYPE_CORE,      0x0E },
     { fapi2::ATTR_PG_EC15, fapi2::TARGET_TYPE_CORE,      0x0F },
     { fapi2::ATTR_PG_EC16, fapi2::TARGET_TYPE_CORE,      0x10 },
     { fapi2::ATTR_PG_EC17, fapi2::TARGET_TYPE_CORE,      0x11 },
     { fapi2::ATTR_PG_EC18, fapi2::TARGET_TYPE_CORE,      0x12 },
     { fapi2::ATTR_PG_EC19, fapi2::TARGET_TYPE_CORE,      0x13 },
     { fapi2::ATTR_PG_EC20, fapi2::TARGET_TYPE_CORE,      0x14 },
     { fapi2::ATTR_PG_EC21, fapi2::TARGET_TYPE_CORE,      0x15 },
     { fapi2::ATTR_PG_EC22, fapi2::TARGET_TYPE_CORE,      0x16 },
     { fapi2::ATTR_PG_EC23, fapi2::TARGET_TYPE_CORE,      0x17 }
};

uint32_t CHIPLET_PG_ARRAY_ENTRIES = sizeof(CHIPLET_PG_ARRAY) /
                                    sizeof(chiplet_pg_entry_t);

#endif  // __FAPI2_PPE_TARGET_PG_ATTR__
