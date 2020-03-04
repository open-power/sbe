/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_reset.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_chiplet_reset.H
///
/// @brief
///
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------


#ifndef _P10_SBE_CHIPLET_RESET_H_
#define _P10_SBE_CHIPLET_RESET_H_


#include <fapi2.H>

namespace p10SbeChipletReset
{
enum P10_SBE_CHIPLET_RESET_Public_Constants
{
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    REGIONS_EXCEPT_VITAL = 0x7FFF,
    SCAN_TYPES_TIME_GPTR_REPR = 0x230,
    SYNC_CONFIG_DEFAULT_VAL = 0xA000000000000000,
    OPCG_ALIGN_DEFAULT_VAL = 0x5000000000000020,
    OPCG_ALIGN_INOP_ALIGN = 0x5,
    OPCG_ALIGN_INOP_WAIT = 0x0,
    OPCG_ALIGN_SCANRATIO_4to1 = 0x03,
    OPCG_ALIGN_WAIT_CYCLES = 0x020,
};
}

const uint8_t n0_chiplet_delay = 0;
const uint8_t n1_chiplet_delay = 4;
const uint8_t eq0_chiplet_delay = 1;
const uint8_t eq1_chiplet_delay = 1;
const uint8_t eq2_chiplet_delay = 2;
const uint8_t eq3_chiplet_delay = 2;
const uint8_t eq4_chiplet_delay = 3;
const uint8_t eq5_chiplet_delay = 3;
const uint8_t eq6_chiplet_delay = 4;
const uint8_t eq7_chiplet_delay = 4;

typedef fapi2::ReturnCode (*p10_sbe_chiplet_reset_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

/// For all good chiplets excluding EQ/EC
/// -- Setup Chiplet GP3 regs
///  -- Reset to default state
///  -- Set chiplet enable on all all good chiplets excluding EQ/EC
/// For all enabled chiplets excluding EQ/EC/Buses
///  -- Start vital clocks and release endpoint reset
///  -- PCB Slave error register Reset
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p10_sbe_chiplet_reset(const
                                            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);
}

#endif
