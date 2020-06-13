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

static const ring_setup_t ISTEP3_TP_FUNC_RINGS_UNICAST[] =
{
    { occ_func,       IGNORE_PG, TARGET_CHIP,    0x1,  0x1,  1},
};

static const ring_setup_t ISTEP3_FUNC_RINGS_UNICAST[] =
{
    { n0_func,        IGNORE_PG, TARGET_CHIP,    0x2,  0x2,  0},
    { n1_func,        IGNORE_PG, TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_func,  0x9,       TARGET_CHIP,    0x3,  0x3,  0},
    { iohs0_func,     IGNORE_PG, TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pdl_func, IGNORE_PG, TARGET_CHIP,    0x18, 0x18, 0},
    { iohs1_func,     IGNORE_PG, TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pdl_func, IGNORE_PG, TARGET_CHIP,    0x19, 0x19, 0},
    { iohs2_func,     IGNORE_PG, TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pdl_func, IGNORE_PG, TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs3_func,     IGNORE_PG, TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pdl_func, IGNORE_PG, TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs4_func,     IGNORE_PG, TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pdl_func, IGNORE_PG, TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs5_func,     IGNORE_PG, TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pdl_func, IGNORE_PG, TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs6_func,     IGNORE_PG, TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pdl_func, IGNORE_PG, TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs7_func,     IGNORE_PG, TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pdl_func, IGNORE_PG, TARGET_CHIP,    0x1F, 0x1F, 0},
    { pau0_func,      IGNORE_PG, TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_func,      IGNORE_PG, TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_func,      IGNORE_PG, TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_func,      IGNORE_PG, TARGET_CHIP,    0x13, 0x13, 0},
    { pau0_pau0_func, 0x5,       TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_pau3_func, 0x5,       TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_pau4_func, 0x5,       TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_pau5_func, 0x6,       TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_pau6_func, 0x5,       TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_pau7_func, 0x6,       TARGET_CHIP,    0x13, 0x13, 1},
};

static const mc_ring_setup_t ISTEP3_EQ_FUNC_RINGS_MULTICAST[] =
{
    { eq_func,        0},
    { eq_mode,        0},
    { eq_clkadj_func, 1},
};

static const mc_ring_setup_t ISTEP3_EQ_FUNC_RINGS_MULTICAST_NO_CLKADJ[] =
{
    { eq_func,        0},
    { eq_mode,        1},
    { eq_clkadj_func, 0},
};

static const mc_ring_setup_t ISTEP3_PCI_FUNC_RINGS_MULTICAST[] =
{
    { pci_func,       1},
};

static const mc_ring_setup_t ISTEP3_MC_FUNC_RINGS_MULTICAST[] =
{
    { mc_func,        1},
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

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYS;
    const auto* l_eq_func_rings_mc = &ISTEP3_EQ_FUNC_RINGS_MULTICAST;

    fapi2::ATTR_CONTAINED_IPL_TYPE_Type ipl_type;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type ipl_phase;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYS,
                           ipl_type));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYS,
                           ipl_phase));

    if (ipl_type != fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE &&
        ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CONTAINED_IPL)
    {
        l_eq_func_rings_mc = &ISTEP3_EQ_FUNC_RINGS_MULTICAST_NO_CLKADJ;
    }

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_TP_FUNC_RINGS_UNICAST, true));
    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_FUNC_RINGS_UNICAST));

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_ALL_EQ,
             *l_eq_func_rings_mc));

    if (l_mc.size())
    {
        FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_GOOD_MC,
                 ISTEP3_MC_FUNC_RINGS_MULTICAST));
    }

    if (l_pci.size())
    {
        FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_GOOD_PCI,
                 ISTEP3_PCI_FUNC_RINGS_MULTICAST));
    }

    FAPI_INF("p10_sbe_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
