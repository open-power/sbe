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
 * @file plat_ppe_target.H
 * @brief Definitions for fapi2 PPE targets
 */

#ifndef __FAPI2_PPE_TARGET_PARMS__
#define __FAPI2_PPE_TARGET_PARMS__

#include "fapi_sbe_common.H"


CONST_UINT32_T(CHIP_TARGET_OFFSET,      0);
CONST_UINT32_T(CHIP_TARGET_COUNT ,      1);

// Pervasive Targets (note; these include the MCS targets as well)
CONST_UINT32_T(PERV_TARGET_OFFSET, CHIP_TARGET_OFFSET + CHIP_TARGET_COUNT);
CONST_UINT32_T(PERV_CHIPLET_OFFSET,     0x1);
CONST_UINT32_T(PERV_TARGET_COUNT,       15);

// Cache Targets
CONST_UINT32_T(EQ_TARGET_OFFSET, PERV_TARGET_OFFSET + PERV_TARGET_COUNT);
CONST_UINT32_T(EQ_CHIPLET_OFFSET,       0x10);
CONST_UINT32_T(EQ_TARGET_COUNT,         6);

// Core Targets
CONST_UINT32_T(CORE_TARGET_OFFSET, EQ_TARGET_OFFSET + EQ_TARGET_COUNT);
CONST_UINT32_T(CORE_CHIPLET_OFFSET,     0x20);
CONST_UINT32_T(CORE_TARGET_COUNT,       24);

// MCS Targets (note: these are phyically Pervasive targets)
CONST_UINT32_T(MCS_TARGET_OFFSET, CORE_TARGET_OFFSET + CORE_TARGET_COUNT);
CONST_UINT32_T(MCS_CHIPLET_OFFSET,      0x7);
CONST_UINT32_T(MCS_TARGET_COUNT,        2);

CONST_UINT32_T(EX_TARGET_OFFSET, MCS_TARGET_OFFSET + MCS_TARGET_COUNT);
CONST_UINT32_T(EX_CHIPLET_OFFSET,       0x10);
CONST_UINT32_T(EX_TARGET_COUNT,         12);

CONST_UINT32_T(SYSTEM_TARGET_OFFSET, EX_TARGET_OFFSET + EX_TARGET_COUNT);
CONST_UINT32_T(SYSTEM_TARGET_COUNT,     1);

CONST_UINT32_T(MCAST_TARGET_OFFSET, SYSTEM_TARGET_OFFSET + SYSTEM_TARGET_COUNT);
CONST_UINT32_T(MCAST_CHIPLET_OFFSET,    4);
CONST_UINT32_T(MCAST_TARGET_COUNT,      3);  // PPE only needs multicast groups 4-6

// Total Target Count
CONST_UINT32_T(TARGET_COUNT,            CHIP_TARGET_COUNT +
                                        PERV_TARGET_COUNT +
                                        EQ_TARGET_COUNT +
                                        CORE_TARGET_COUNT +
                                        MCS_TARGET_COUNT +
                                        EX_TARGET_COUNT +
                                        SYSTEM_TARGET_COUNT +
                                        MCAST_TARGET_COUNT);

/*
enum TargetFilter
{
    TARGET_FILTER_ALL_NEST   ,
    TARGET_FILTER_NEST_NORTH ,
    TARGET_FILTER_NEST_SOUTH ,
    TARGET_FILTER_NEST_EAST  ,
    TARGET_FILTER_NEST_WEST  ,
    TARGET_FILTER_XBUS       ,
    TARGET_FILTER_ALL_OBUS   ,
    TARGET_FILTER_OBUS0      ,
    TARGET_FILTER_OBUS1      ,
    TARGET_FILTER_OBUS2      ,
    TARGET_FILTER_OBUS3      ,
    TARGET_FILTER_ALL_PCI    ,
    TARGET_FILTER_PCI0       ,
    TARGET_FILTER_PCI1       ,
    TARGET_FILTER_PCI2       ,
    TARGET_FILTER_ALL_EC     ,
    TARGET_FILTER_ALL_EP     ,
    TARGET_FILTER_ALL_MC     ,
    TARGET_FILTER_MC_WEST    ,
    TARGET_FILTER_MC_EAST    ,
    TARGET_FILTER_TP         ,
};

// The order of this MUST match the order of the TargetFilter enum above
// The bit vectors represent
const uint64_t TargetFilters[] =
{
    BITS(2,4),   // TARGET_FILTER_ALL_NEST
    BIT(2),      // TARGET_FILTER_NEST_NORTH
    BIT(3),      // TARGET_FILTER_NEST_SOUTH
    BIT(4),      // TARGET_FILTER_NEST_EAST
    BIT(5),      // TARGET_FILTER_NEST_WEST
    BIT(6),      // TARGET_FILTER_XBUS
    BITS(9,4),   // TARGET_FILTER_ALL_OBUS
    BIT(9),      // TARGET_FILTER_OBUS0
    BIT(10),     // TARGET_FILTER_OBUS1
    BIT(11),     // TARGET_FILTER_OBUS2
    BIT(12),     // TARGET_FILTER_OBUS3
    BITS(13,3),  // TARGET_FILTER_ALL_PCI
    BIT(12),     // TARGET_FILTER_PCI0
    BIT(13),     // TARGET_FILTER_PCI1
    BIT(14),     // TARGET_FILTER_PCI2
    BITS(32,24), // TARGET_FILTER_ALL_EC
    BITS(16,6),  // TARGET_FILTER_ALL_EP
    BITS(7,2),   // TARGET_FILTER_ALL_MC
    BIT(7),      // TARGET_FILTER_MC_WEST
    BIT(8),      // TARGET_FILTER_MC_EAST
    BIT(1),      // TARGET_FILTER_TP
};
*/
#endif  // __FAPI2_PPE_TARGET_PARMS__
