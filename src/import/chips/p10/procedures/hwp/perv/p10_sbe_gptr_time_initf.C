/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_gptr_time_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
#include "p10_scan_via_scom.H"


const uint8_t PAU_DTS_REGS = 3;

// Sort of a "scandef lite" which tells the procedure how many thermal sensors exist
// and where their configuration bits are on the ring
struct pau_dts_ring_settings
{
    uint64_t scan_region_type;            ///< Scan region & type of affected ring
    uint16_t reg_offsets[PAU_DTS_REGS + 1]; ///< Rotate counts to reach each register, plus one to finish spinning the ring
    uint16_t reg_lengths[PAU_DTS_REGS + 1]; ///< Register length, last is unused
};

const pau_dts_ring_settings pau01 =
{
    0x0860000000002000ULL,                ///< perv+tl+ppe, GPTR
    { 1966,   48,   61,    95 },          ///< P, M, B coefficients
    {    6,    7,    9,     0 },
};

const pau_dts_ring_settings pau23 =
{
    0x0860000000002000ULL,                ///< perv+tl+ppe, GPTR
    { 2080,   48,   61,    80 },          ///< P, M, B coefficients
    {    6,    7,    9,     0 },
};

/// @brief Rotate across the GPTR ring and read out the thermal sensor calibration values
static inline fapi2::ReturnCode read_pau_dts_configs(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_cplt,
    const pau_dts_ring_settings& i_pau_ring_settings,
    std::vector<fapi2::buffer<uint16_t>>& o_reg_values)
{
    fapi2::buffer<uint64_t> tmp;

    // Set up clock controller and write the header
    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_pau_ring_settings.scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // For each sensor, rotate to the next position
    for (uint8_t l_dts = 0; l_dts < PAU_DTS_REGS + 1; l_dts++)
    {
        // sample data in scan buffer, store in output structure
        FAPI_TRY(svs::rotate(i_cplt, i_pau_ring_settings.reg_offsets[l_dts], tmp));

        if (l_dts != PAU_DTS_REGS)
        {
            FAPI_TRY(tmp.extract(o_reg_values[l_dts], 0, i_pau_ring_settings.reg_lengths[l_dts], 0));
        }
    }

    // header should be present in scan buffer based on data structure
    if (tmp != 0x00BAD666BAD66600)
    {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Rotate across the GPTR ring and rewrite the thermal sensor calibration values
static inline fapi2::ReturnCode write_pau_dts_configs(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_cplt,
    const pau_dts_ring_settings& i_pau_ring_settings,
    const std::vector<fapi2::buffer<uint16_t>>& i_reg_values)
{
    fapi2::buffer<uint64_t> tmp;

    // Set up clock controller and write the header
    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_pau_ring_settings.scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // For each sensor, rotate to the next position
    for (uint8_t l_dts = 0; l_dts < PAU_DTS_REGS + 1; l_dts++)
    {
        // sample data in scan buffer, update output structure
        FAPI_TRY(svs::rotate(i_cplt, i_pau_ring_settings.reg_offsets[l_dts], tmp));

        if (l_dts != PAU_DTS_REGS)
        {
            FAPI_TRY(tmp.insert(i_reg_values[l_dts], 0, i_pau_ring_settings.reg_lengths[l_dts], 0));
            FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
        }
    }

    if (tmp != 0x00BAD666BAD66600)
    {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}

fapi2::ReturnCode p10_sbe_gptr_time_initf_fixup_dts(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("Start");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_IS_SIMULATION_Type l_attr_is_simulation;
    auto l_pau0_tgt = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                          static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_PAU0),
                          fapi2::TARGET_STATE_FUNCTIONAL).front();

    auto l_pau1_tgt = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                          static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_PAU1),
                          fapi2::TARGET_STATE_FUNCTIONAL).front();

    auto l_pau2_tgt = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                          static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_PAU2),
                          fapi2::TARGET_STATE_FUNCTIONAL).front();

    auto l_pau3_tgt = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                          static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_PAU3),
                          fapi2::TARGET_STATE_FUNCTIONAL).front();

    std::vector<fapi2::buffer<uint16_t>> pau0_dts;
    std::vector<fapi2::buffer<uint16_t>> pau1_dts;
    std::vector<fapi2::buffer<uint16_t>> pau2_dts;
    std::vector<fapi2::buffer<uint16_t>> pau3_dts;

    for (auto ii = 0; ii < PAU_DTS_REGS; ii++)
    {
        pau0_dts.push_back(0);
        pau1_dts.push_back(0);
        pau2_dts.push_back(0);
        pau3_dts.push_back(0);
    }

#ifdef __PPE__

    if (SBE::isSimicsRunning())
    {
        FAPI_INF("Skipping workaround because Simics does not support scanning");
        goto fapi_try_exit;
    }

#endif
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_SIMULATION, FAPI_SYSTEM, l_attr_is_simulation));

    if (l_attr_is_simulation)
    {
        FAPI_INF("Skipping workaround in simulation");
        goto fapi_try_exit;
    }

    // MVPD on all current (and future) parts installs the PAUC DTS coefficients into the wrong
    // physical rings.  read DTS sensor coefficients installed into each PAUC chiplet
    FAPI_TRY(read_pau_dts_configs(l_pau0_tgt, pau01, pau0_dts),
             "Error from read_pau_dts_configs (PAUC0)");
    FAPI_TRY(read_pau_dts_configs(l_pau1_tgt, pau01, pau1_dts),
             "Error from read_pau_dts_configs (PAUC1)");
    FAPI_TRY(read_pau_dts_configs(l_pau2_tgt, pau23, pau2_dts),
             "Error from read_pau_dts_configs (PAUC2)");
    FAPI_TRY(read_pau_dts_configs(l_pau3_tgt, pau23, pau3_dts),
             "Error from read_pau_dts_configs (PAUC3)");

    // rewrite coefficient data reflecting VPD swap:
    //   PAUC0->PAUC3
    //   PAUC2->PAUC0
    //   PAUC1->PAUC2
    //   PAUC3->PAUC1
    FAPI_TRY(write_pau_dts_configs(l_pau0_tgt, pau01, pau2_dts),
             "Error from write_pau_dts_configs (PAUC2->PAUC0)");
    FAPI_TRY(write_pau_dts_configs(l_pau1_tgt, pau01, pau3_dts),
             "Error from write_pau_dts_configs (PAUC3->PAUC1)");
    FAPI_TRY(write_pau_dts_configs(l_pau2_tgt, pau23, pau1_dts),
             "Error from write_pau_dts_configs (PAUC1->PAUC2)");
    FAPI_TRY(write_pau_dts_configs(l_pau3_tgt, pau23, pau0_dts),
             "Error from write_pau_dts_configs (PAUC0->PAUC3)");

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;
}

static const ring_setup_t ISTEP3_GPTR_TIME_RINGS_UNICAST[] =
{
    { n0_gptr,        IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n0_gptr_ovly,   IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n0_time,        IGNORE_PG,   TARGET_CHIP,    0x2,  0x2,  0},
    { n1_gptr,        IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_gptr_ovly,   IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_time,        IGNORE_PG,   TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_gptr,  0x9,         TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_time,  0x9,         TARGET_CHIP,    0x3,  0x3,  0},
    { iohs0_gptr,     IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_time,     IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs0_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x18, 0x18, 0},
    { iohs1_gptr,     IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_time,     IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs1_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x19, 0x19, 0},
    { iohs2_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_time,     IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs2_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1A, 0x1A, 0},
    { iohs3_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_time,     IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs3_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1B, 0x1B, 0},
    { iohs4_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_time,     IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs4_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1C, 0x1C, 0},
    { iohs5_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_time,     IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs5_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1D, 0x1D, 0},
    { iohs6_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_time,     IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs6_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1E, 0x1E, 0},
    { iohs7_gptr,     IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_time,     IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pll_gptr, IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pdl_gptr, IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { iohs7_pdl_time, IGNORE_PG,   TARGET_CHIP,    0x1F, 0x1F, 0},
    { pau0_gptr,      IGNORE_PG,   TARGET_CHIP,    0x10, 0x10, 0},
    { pau0_gptr_ovly, IGNORE_PG,   TARGET_CHIP,    0x10, 0x10, 0},
    { pau0_time,      IGNORE_PG,   TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_gptr,      IGNORE_PG,   TARGET_CHIP,    0x11, 0x11, 0},
    { pau1_gptr_ovly, IGNORE_PG,   TARGET_CHIP,    0x11, 0x11, 0},
    { pau1_time,      IGNORE_PG,   TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_gptr,      IGNORE_PG,   TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_gptr_ovly, IGNORE_PG,   TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_time,      IGNORE_PG,   TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_gptr,      IGNORE_PG,   TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_gptr_ovly, IGNORE_PG,   TARGET_CHIP,    0x13, 0x13, 0},
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
    { eq_clkadj_gptr, 1},
};

static const ring_setup_t ISTEP3_EQ_GPTR_RINGS_UNICAST[] =
{
    { eq_gptr_ovly,   IGNORE_PG,   TARGET_CHIPLET, 0x20, 0x27, 1},
};

static const mc_ring_setup_t ISTEP3_EQ_GPTR_TIME_RINGS_MULTICAST_NO_CLKADJ[] =
{
    { eq_gptr,        0},
    { eq_time,        1},
    { eq_clkadj_gptr, 0},
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

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYS;
    const auto* l_eq_gptr_time_rings_mc = &ISTEP3_EQ_GPTR_TIME_RINGS_MULTICAST;

    fapi2::ATTR_CHIP_EC_FEATURE_PAU_DTS_SWAP_Type l_pau_dts_swap;
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type ipl_type;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type ipl_phase;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYS,
                           ipl_type));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, FAPI_SYS,
                           ipl_phase));

    if (ipl_type != fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE &&
        ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CONTAINED_IPL)
    {
        l_eq_gptr_time_rings_mc = &ISTEP3_EQ_GPTR_TIME_RINGS_MULTICAST_NO_CLKADJ;
    }

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_GPTR_TIME_RINGS_UNICAST));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_PAU_DTS_SWAP, i_target_chip, l_pau_dts_swap));

    if (l_pau_dts_swap)
    {
        FAPI_TRY(p10_sbe_gptr_time_initf_fixup_dts(i_target_chip));
    }

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring_multicast(i_target_chip, fapi2::MCGROUP_ALL_EQ,
             *l_eq_gptr_time_rings_mc));

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_EQ_GPTR_RINGS_UNICAST));

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
