/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_target_parms.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2020                        */
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


CONST_UINT32_T(CHIP_TARGET_OFFSET,    0);
CONST_UINT32_T(CHIP_TARGET_COUNT ,    1);

//----------------------------------------------------------------------------------
// Chiplet targets
//----------------------------------------------------------------------------------

// TP target
CONST_UINT32_T(TP_TARGET_OFFSET, CHIP_TARGET_OFFSET + CHIP_TARGET_COUNT);
CONST_UINT32_T(TP_CHIPLET_OFFSET, 0x1);
CONST_UINT32_T(TP_TARGET_COUNT,     1);

// Nest targets
CONST_UINT32_T(NEST_TARGET_OFFSET,  TP_TARGET_OFFSET + TP_TARGET_COUNT);
CONST_UINT32_T(NEST_CHIPLET_OFFSET, 0x2);
CONST_UINT32_T(NEST_TARGET_COUNT,     2);

// PEC (PCI Express Chiplet) targets
CONST_UINT32_T(PEC_TARGET_OFFSET, NEST_TARGET_OFFSET + NEST_TARGET_COUNT);
CONST_UINT32_T(PEC_CHIPLET_OFFSET, 0x8);
CONST_UINT32_T(PEC_TARGET_COUNT,     2);

// MC targets
CONST_UINT32_T(MC_TARGET_OFFSET, PEC_TARGET_OFFSET + PEC_TARGET_COUNT);
CONST_UINT32_T(MC_CHIPLET_OFFSET, 0xC);
CONST_UINT32_T(MC_TARGET_COUNT,     4);

// PAUC (PAU chiplet) targets
CONST_UINT32_T(PAUC_TARGET_OFFSET, MC_TARGET_OFFSET + MC_TARGET_COUNT);
CONST_UINT32_T(PAUC_CHIPLET_OFFSET, 0x10);
CONST_UINT32_T(PAUC_TARGET_COUNT,      4);

// IOHS targets
CONST_UINT32_T(IOHS_TARGET_OFFSET, PAUC_TARGET_OFFSET + PAUC_TARGET_COUNT);
CONST_UINT32_T(IOHS_CHIPLET_OFFSET, 0x18);
CONST_UINT32_T(IOHS_TARGET_COUNT,      8);

// EQ (Quad) Targets
CONST_UINT32_T(EQ_TARGET_OFFSET, IOHS_TARGET_OFFSET + IOHS_TARGET_COUNT);
CONST_UINT32_T(EQ_CHIPLET_OFFSET,   0x20);
CONST_UINT32_T(EQ_TARGET_COUNT,        8);

// Total number of pervasive targets
CONST_UINT32_T(PERV_TARGET_OFFSET,  TP_TARGET_OFFSET);
CONST_UINT32_T(PERV_TARGET_COUNT,   TP_TARGET_COUNT +
                                    NEST_TARGET_COUNT +
                                    PEC_TARGET_COUNT +
                                    MC_TARGET_COUNT +
                                    PAUC_TARGET_COUNT +
                                    IOHS_TARGET_COUNT +
                                    EQ_TARGET_COUNT);

//----------------------------------------------------------------------------------
// Non-chiplet targets
//----------------------------------------------------------------------------------

// Core Targets
CONST_UINT32_T(CORE_TARGET_OFFSET, EQ_TARGET_OFFSET + EQ_TARGET_COUNT);
CONST_UINT32_T(CORE_TARGET_COUNT,       32);

// PHB Targets
CONST_UINT32_T(PHB_TARGET_OFFSET, CORE_TARGET_OFFSET + CORE_TARGET_COUNT);
CONST_UINT32_T(PHB_TARGET_COUNT,         6);

// MI Targets
CONST_UINT32_T(MI_TARGET_OFFSET,  PHB_TARGET_OFFSET + PHB_TARGET_COUNT);
CONST_UINT32_T(MI_TARGET_COUNT,          4);

// NMMU Targets
CONST_UINT32_T(NMMU_TARGET_OFFSET,  MI_TARGET_OFFSET + MI_TARGET_COUNT);
CONST_UINT32_T(NMMU_TARGET_COUNT,          2);

// PAU (PAU unit) Targets
CONST_UINT32_T(PAU_TARGET_OFFSET,  NMMU_TARGET_OFFSET + NMMU_TARGET_COUNT);
CONST_UINT32_T(PAU_TARGET_COUNT,          6);

// System Target
CONST_UINT32_T(SYSTEM_TARGET_OFFSET, PAU_TARGET_OFFSET + PAU_TARGET_COUNT);
CONST_UINT32_T(SYSTEM_TARGET_COUNT,     1);

// First non-chiplet target
CONST_UINT32_T(NON_CHIPLET_TARGET_OFFSET, CORE_TARGET_OFFSET);

// Total Target Count
CONST_UINT32_T(TARGET_COUNT,        CHIP_TARGET_COUNT +
                                    PERV_TARGET_COUNT +
                                    CORE_TARGET_COUNT +
                                    PHB_TARGET_COUNT +
                                    MI_TARGET_COUNT +
                                    NMMU_TARGET_COUNT +
                                    PAU_TARGET_COUNT +
                                    SYSTEM_TARGET_COUNT);

// Maximum possible Pervasive chiplets
CONST_UINT32_T(MAX_PERV_CHIPLETS, 96);

#endif  // __FAPI2_PPE_TARGET_PARMS__
