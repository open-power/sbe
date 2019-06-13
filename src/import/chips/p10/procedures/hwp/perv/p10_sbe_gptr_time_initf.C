/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_gptr_time_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file  p10_sbe_gptr_time_initf.C
///
/// @brief Load time and GPTR rings for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------


#include "p10_sbe_gptr_time_initf.H"
#include "p10_perv_sbe_cmn.H"

static const ring_setup_t ISTEP3_GPTR_TIME_RINGS[] =
{
    { n0_gptr,        IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n0_time,        IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n1_gptr,        IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_time,        IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_gptr,  0x9,         TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_time,  0x9,         TARGET_CHIP,    0x3,  0x3,  0},
    { pci_gptr,       IGNORE_PG,   TARGET_CHIPLET, 0x8,  0x9,  0},
    { pci_time,       IGNORE_PG,   TARGET_CHIPLET, 0x8,  0x9,  0},
    { pci_pll_gptr,   IGNORE_PG,   TARGET_CHIPLET, 0x8,  0x9,  0},
    { pci_ph5_gptr,   IGNORE_PG,   TARGET_CHIPLET, 0x8,  0x9,  0},
    { pci_ph5_time,   IGNORE_PG,   TARGET_CHIPLET, 0x8,  0x9,  0},
    { mc_gptr,        IGNORE_PG,   TARGET_CHIPLET, 0xC,  0xF,  0},
    { mc_time,        IGNORE_PG,   TARGET_CHIPLET, 0xC,  0xF,  0},
    { mc_pll_gptr,    IGNORE_PG,   TARGET_CHIPLET, 0xC,  0xF,  0},
    { mc_emo_gptr,    IGNORE_PG,   TARGET_CHIPLET, 0xC,  0xF,  0},
    { mc_emo_time,    IGNORE_PG,   TARGET_CHIPLET, 0xC,  0xF,  0},
    { iohs_gptr,      IGNORE_PG,   TARGET_CHIPLET, 0x18, 0x1F, 0},
    { iohs_time,      IGNORE_PG,   TARGET_CHIPLET, 0x18, 0x1F, 0},
    { iohs_pll_gptr,  IGNORE_PG,   TARGET_CHIPLET, 0x18, 0x1F, 0},
    { iohs_pdl_gptr,  IGNORE_PG,   TARGET_CHIPLET, 0x18, 0x1F, 0},
    { iohs_pdl_time,  IGNORE_PG,   TARGET_CHIPLET, 0x18, 0x1F, 0},
    { iohs_ndl_gptr,  0x8,         TARGET_CHIPLET, 0x18, 0x1F, 0},
    { iohs_ndl_time,  0x8,         TARGET_CHIPLET, 0x18, 0x1F, 0},
    { pau0_gptr,      IGNORE_PG,   TARGET_CHIP,    0x10, 0x10, 0},
    { pau0_time,      IGNORE_PG,   TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_gptr,      IGNORE_PG,   TARGET_CHIP,    0x11, 0x11, 0},
    { pau1_time,      IGNORE_PG,   TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_gptr,      IGNORE_PG,   TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_time,      IGNORE_PG,   TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_gptr,      IGNORE_PG,   TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_time,      IGNORE_PG,   TARGET_CHIP,    0x13, 0x13, 0},
    { pau0_pau0_gptr, 0x5,         TARGET_CHIP,    0x10, 0x10, 0},
    { pau0_pau0_time, 0x5,         TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_pau3_gptr, 0x5,         TARGET_CHIP,    0x11, 0x11, 0},
    { pau1_pau3_time, 0x5,         TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_pau4_gptr, 0x5,         TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_pau4_time, 0x5,         TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_pau5_gptr, 0x6,         TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_pau5_time, 0x6,         TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_pau6_gptr, 0x5,         TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_pau6_time, 0x5,         TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_pau7_gptr, 0x6,         TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_pau7_time, 0x6,         TARGET_CHIP,    0x13, 0x13, 0},
    { eq_gptr,        IGNORE_PG,   TARGET_CHIPLET, 0x20, 0x27, 0},
    { eq_time,        IGNORE_PG,   TARGET_CHIPLET, 0x20, 0x27, 1},
};

fapi2::ReturnCode p10_sbe_gptr_time_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    FAPI_INF("p10_sbe_gptr_time_initf: Entering ...");

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_GPTR_TIME_RINGS));

    FAPI_INF("p10_sbe_gptr_time_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
