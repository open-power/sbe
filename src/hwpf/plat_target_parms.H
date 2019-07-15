/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_target_parms.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2019                        */
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

// Nest targets - Group 1
CONST_UINT32_T(NEST_GROUP1_TARGET_OFFSET,  CHIP_TARGET_OFFSET + CHIP_TARGET_COUNT);
CONST_UINT32_T(NEST_GROUP1_CHIPLET_OFFSET, 0x1);
CONST_UINT32_T(NEST_GROUP1_TARGET_COUNT,   6);

// MCBIST/MC targets
CONST_UINT32_T(MCBIST_TARGET_OFFSET, NEST_GROUP1_TARGET_OFFSET + NEST_GROUP1_TARGET_COUNT);
CONST_UINT32_T(MCBIST_CHIPLET_OFFSET,      0x7);
CONST_UINT32_T(MCBIST_TARGET_COUNT,        2);
CONST_UINT32_T(MC_TARGET_OFFSET, NEST_GROUP1_TARGET_OFFSET + NEST_GROUP1_TARGET_COUNT);
CONST_UINT32_T(MC_CHIPLET_OFFSET,      0x7);
CONST_UINT32_T(MC_TARGET_COUNT,        2);

// Nest targets - Group 2
CONST_UINT32_T(NEST_GROUP2_TARGET_OFFSET, MCBIST_TARGET_OFFSET + MCBIST_TARGET_COUNT);
CONST_UINT32_T(NEST_GROUP2_TARGET_COUNT,   7);
CONST_UINT32_T(NEST_GROUP2_CHIPLET_OFFSET, 0x9);

// Cache Targets
CONST_UINT32_T(EQ_TARGET_OFFSET, NEST_GROUP2_TARGET_OFFSET + NEST_GROUP2_TARGET_COUNT);
CONST_UINT32_T(EQ_CHIPLET_OFFSET,       0x10);
CONST_UINT32_T(EQ_TARGET_COUNT,         6);

// Core Targets
CONST_UINT32_T(CORE_TARGET_OFFSET, EQ_TARGET_OFFSET + EQ_TARGET_COUNT);
CONST_UINT32_T(CORE_CHIPLET_OFFSET,     0x20);
CONST_UINT32_T(CORE_TARGET_COUNT,       24);

// Ex Targets
CONST_UINT32_T(EX_TARGET_OFFSET, CORE_TARGET_OFFSET + CORE_TARGET_COUNT);
CONST_UINT32_T(EX_CHIPLET_OFFSET,       0x10);
CONST_UINT32_T(EX_TARGET_COUNT,         12);

// MCS/MI Targets
CONST_UINT32_T(MCS_TARGET_OFFSET, EX_TARGET_OFFSET + EX_TARGET_COUNT);
CONST_UINT32_T(MCS_TARGET_COUNT,         4);
CONST_UINT32_T(MI_TARGET_OFFSET, EX_TARGET_OFFSET + EX_TARGET_COUNT);
CONST_UINT32_T(MI_TARGET_COUNT,         4);

// PHB Targets
CONST_UINT32_T(PHB_TARGET_OFFSET, MCS_TARGET_OFFSET + MCS_TARGET_COUNT);
CONST_UINT32_T(PHB_TARGET_COUNT,         6);

// System Target
CONST_UINT32_T(SYSTEM_TARGET_OFFSET, PHB_TARGET_OFFSET + PHB_TARGET_COUNT);
CONST_UINT32_T(SYSTEM_TARGET_COUNT,     1);

CONST_UINT32_T(MCAST_TARGET_OFFSET, SYSTEM_TARGET_OFFSET + SYSTEM_TARGET_COUNT);
CONST_UINT32_T(MCAST_CHIPLET_OFFSET,    4);
CONST_UINT32_T(MCAST_TARGET_COUNT,      3);  // PPE only needs multicast groups 4-6

CONST_UINT32_T(OCMB_TARGET_OFFSET, MCAST_TARGET_OFFSET + MCAST_TARGET_COUNT);
CONST_UINT32_T(OCMB_TARGET_COUNT,      16);

// Total number of pervasive targets (Both NEST groups + EQs + COREs +MCBISTs)
CONST_UINT32_T(PERV_TARGET_COUNT, NEST_GROUP1_TARGET_COUNT + NEST_GROUP2_TARGET_COUNT +
        MCBIST_TARGET_COUNT + EQ_TARGET_COUNT + CORE_TARGET_COUNT);

// Total Target Count
CONST_UINT32_T(TARGET_COUNT,            CHIP_TARGET_COUNT +
                                        PERV_TARGET_COUNT +
                                        EX_TARGET_COUNT +
                                        MCS_TARGET_COUNT +
                                        PHB_TARGET_COUNT +
                                        SYSTEM_TARGET_COUNT +
                                        MCAST_TARGET_COUNT +
                                        OCMB_TARGET_COUNT);

// Maximum possible Pervasive chiplets
CONST_UINT32_T(MAX_PERV_CHIPLETS, 64);

#endif  // __FAPI2_PPE_TARGET_PARMS__
