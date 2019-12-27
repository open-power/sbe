/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_initf.C $    */
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
/// @file  p10_sbe_initf.C
///
/// @brief Initialize necessary latches in available chiplets
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_initf.H"
#include "p10_perv_sbe_cmn.H"
#include <target_filters.H>
#include <multicast_defs.H>
#include <multicast_group_defs.H>

static const ring_setup_t ISTEP3_FURE_RINGS_UNICAST[] =
{
    { n0_fure,        IGNORE_PG, TARGET_CHIP,    0x2,  0x2,  0},
    { n1_fure,        IGNORE_PG, TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_fure,  0x9,       TARGET_CHIP,    0x3,  0x3,  0},
    { occ_fure,       IGNORE_PG, TARGET_CHIP,    0x1,  0x1,  0},
    { iohs0_fure,     IGNORE_PG, TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_ndl_fure, 0x8,       TARGET_CHIP,    0x18, 0x18, 0},
    { iohs1_fure,     IGNORE_PG, TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_ndl_fure, 0x8,       TARGET_CHIP,    0x19, 0x19, 0},
    { iohs2_fure,     IGNORE_PG, TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_ndl_fure, 0x8,       TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs3_fure,     IGNORE_PG, TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_ndl_fure, 0x8,       TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs4_fure,     IGNORE_PG, TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_ndl_fure, 0x8,       TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs5_fure,     IGNORE_PG, TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_ndl_fure, 0x8,       TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs6_fure,     IGNORE_PG, TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_ndl_fure, 0x8,       TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs7_fure,     IGNORE_PG, TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pdl_fure, IGNORE_PG, TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_ndl_fure, 0x8,       TARGET_CHIP,    0x1F, 0x1F, 0},
    { pau0_fure,      IGNORE_PG, TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_fure,      IGNORE_PG, TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_fure,      IGNORE_PG, TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_fure,      IGNORE_PG, TARGET_CHIP,    0x13, 0x13, 0},
    { pau0_pau0_fure, 0x5,       TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_pau3_fure, 0x5,       TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_pau4_fure, 0x5,       TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_pau5_fure, 0x6,       TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_pau6_fure, 0x5,       TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_pau7_fure, 0x6,       TARGET_CHIP,    0x13, 0x13, 1},
};

static const mc_ring_setup_t ISTEP3_EQ_FURE_RINGS_MULTICAST[] =
{
    { eq_fure,        0},
    { eq_mode,        0},
    { eq_clkadj_fure, 0},
    { eq_clkadj_mode, 1},
};

static const mc_ring_setup_t ISTEP3_PCI_FURE_RINGS_MULTICAST[] =
{
    { pci_fure,       1},
};

static const mc_ring_setup_t ISTEP3_MC_FURE_RINGS_MULTICAST[] =
{
    { mc_fure,        1},
};

fapi2::ReturnCode p10_sbe_initf(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_initf: Entering ...");

    auto l_mc = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                    static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC),
                    fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_pci = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
                     fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_FURE_RINGS_UNICAST));

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_ALL_EQ,
             ISTEP3_EQ_FURE_RINGS_MULTICAST));

    if (l_mc.size())
    {
        FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_GOOD_MC,
                 ISTEP3_MC_FURE_RINGS_MULTICAST));
    }

    if (l_pci.size())
    {
        FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_GOOD_PCI,
                 ISTEP3_PCI_FURE_RINGS_MULTICAST));
    }

    FAPI_INF("p10_sbe_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
