/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_gptr_time_initf.C $ */
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
#include <target_filters.H>
#include <multicast_defs.H>
#include <multicast_group_defs.H>

static const ring_setup_t ISTEP3_GPTR_TIME_RINGS_UNICAST[] =
{
    { n0_gptr,        IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n0_time,        IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n1_gptr,        IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_time,        IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_gptr,  0x9,         TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_time,  0x9,         TARGET_CHIP,    0x3,  0x3,  0},
    { iohs0_gptr,     IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_time,     IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_ndl_gptr, 0x8,         TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_ndl_time, 0x8,         TARGET_CHIP,    0x18, 0x18, 0},
    { iohs1_gptr,     IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_time,     IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_ndl_gptr, 0x8,         TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_ndl_time, 0x8,         TARGET_CHIP,    0x19, 0x19, 0},
    { iohs2_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_time,     IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_ndl_gptr, 0x8,         TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_ndl_time, 0x8,         TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs3_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_time,     IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_ndl_gptr, 0x8,         TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_ndl_time, 0x8,         TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs4_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_time,     IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_ndl_gptr, 0x8,         TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_ndl_time, 0x8,         TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs5_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_time,     IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_ndl_gptr, 0x8,         TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_ndl_time, 0x8,         TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs6_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_time,     IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_ndl_gptr, 0x8,         TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_ndl_time, 0x8,         TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs7_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_time,     IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_ndl_gptr, 0x8,         TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_ndl_time, 0x8,         TARGET_CHIP,    0x1F, 0x1F, 0},
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
    { pau3_pau7_time, 0x6,         TARGET_CHIP,    0x13, 0x13, 1},
};

static const mc_ring_setup_t ISTEP3_EQ_GPTR_TIME_RINGS_MULTICAST[] =
{
    { eq_gptr,        0},
    { eq_time,        0},
    { eq_clkadj_gptr, 0},
    { eq_clkadj_time, 1},
};

static const mc_ring_setup_t ISTEP3_PCI_GPTR_TIME_RINGS_MULTICAST[] =
{
    { pci_gptr,       0},
    { pci_time,       0},
    { pci_pll_gptr,   1},
};

static const mc_ring_setup_t ISTEP3_MC_GPTR_TIME_RINGS_MULTICAST[] =
{
    { mc_gptr,        0},
    { mc_time,        0},
    { mc_pll_gptr,    1},
};

fapi2::ReturnCode p10_sbe_gptr_time_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_gptr_time_initf: Entering ...");

    auto l_mc = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                    static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC),
                    fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_pci = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
                     fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_GPTR_TIME_RINGS_UNICAST));

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_ALL_EQ,
             ISTEP3_EQ_GPTR_TIME_RINGS_MULTICAST));

    if (l_mc.size())
    {
        FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_GOOD_MC,
                 ISTEP3_MC_GPTR_TIME_RINGS_MULTICAST));
    }

    if (l_pci.size())
    {
        FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_GOOD_PCI,
                 ISTEP3_PCI_GPTR_TIME_RINGS_MULTICAST));
    }

    FAPI_INF("p10_sbe_gptr_time_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
