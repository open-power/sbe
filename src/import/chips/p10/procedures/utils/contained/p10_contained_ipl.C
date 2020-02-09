/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_ipl.C $ */
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
#include <p10_contained.H>
#include <p10_contained_sim.H>
#include <p10_contained_ipl_istep.H>

#include <fapi2.H>
#include <p10_scom_perv.H>
#include <p10_perv_sbe_cmn.H>
#include <multicast_group_defs.H>
#include <multicast_defs.H>
#include <p10_hang_pulse_mc_setup_tables.H>
#include <p10_hcd_core_stopclocks.H>
#include <p10_hcd_cache_stopclocks.H>

///
/// @brief Stopclocks for all regions required for either a cache- or
///        chip-contained IPL
///
/// @param[in] i_chip Reference to chip target
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{
    FAPI_INF(">> %s", __func__);

    if (sim::skip_stopclocks())
    {
        // Use FAPI_ERR(...) to force print this message
        FAPI_ERR("Skipping stopclocks");
        return fapi2::FAPI2_RC_SUCCESS;
    }

    using namespace scomt::perv;

    auto data = CONTAINED_MC_REGIONS;
    const auto all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);

    // Raise fences (including perv region fence)
    FAPI_TRY(fapi2::putScom(all, CPLT_CTRL1_WO_OR, data));

    // Stop clocks
    FAPI_TRY(PREP_CLK_REGION(all));
    SET_CLK_REGION_CLOCK_CMD(0b10, data);
    SET_CLK_REGION_SEL_THOLD_SL(data);
    SET_CLK_REGION_SEL_THOLD_NSL(data);
    SET_CLK_REGION_SEL_THOLD_ARY(data);
    FAPI_TRY(fapi2::putScom(all, CLK_REGION, data));

    // Check clock status
    {
        const uint64_t addrs[3] = { CLOCK_STAT_SL, CLOCK_STAT_NSL, CLOCK_STAT_ARY };
        // When reading the CLOCK_STAT_* registers via multicast-AND, partial bad
        // regions act "neutral" and return one (ie. "stopped"). This lets us get
        // away with just using the "usual" region mask(s).
        auto mask = CONTAINED_MC_REGIONS;

        fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > all_and = all;

        for (const auto& addr : addrs)
        {
            FAPI_TRY(fapi2::getScom(all_and, addr, data));
            // Stopped clocks are indicated by ones in the appropriate region bit(s)
            FAPI_ASSERT((data & mask) == mask,
                        fapi2::P10_CONTAINED_IPL_CLKS_ON_AFTER_CLOCK_STOP()
                        .set_ACTUAL(data)
                        .set_ADDR(addr)
                        .set_EXPECTED(mask),
                        "Some clocks are still running after stopping clocks");
        }
    }

    // Restore pre-startclocks CPLT_CTRL0 settings
    data = 0;
    FAPI_TRY(PREP_CPLT_CTRL0_WO_CLEAR(all));
    SET_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC(data);
    FAPI_TRY(PUT_CPLT_CTRL0_WO_CLEAR(all, data));
    data = 0;
    FAPI_TRY(PREP_CPLT_CTRL0_WO_OR(all));
    SET_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC(data);
    FAPI_TRY(PUT_CPLT_CTRL0_WO_OR(all, data));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

///
/// @brief Restart PERV and QME region clocks in EQs
///
/// @param[in] i_chip Reference to chip target
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode restart_perv_qme_clocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    const auto all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);
    auto data = fapi2::buffer<uint64_t>(0)
                .setBit<CPLT_CTRL1_REGION0_FENCE_DC>()
                .setBit<CPLT_CTRL1_REGION9_FENCE_DC>();

    //  Lower perv region fence
    FAPI_TRY(fapi2::putScom(all, CPLT_CTRL1_WO_CLEAR, data));

    data = 0;
    FAPI_TRY(PREP_CPLT_CTRL0_WO_CLEAR(all));
    SET_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC(data);
    SET_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC(data);
    FAPI_TRY(PUT_CPLT_CTRL0_WO_CLEAR(all, data));

    data = 0;
    data.setBit<CPLT_CTRL1_REGION0_FENCE_DC>();
    data.setBit<CPLT_CTRL1_REGION9_FENCE_DC>();
    // Start clocks
    FAPI_TRY(PREP_CLK_REGION(all));
    SET_CLK_REGION_CLOCK_CMD(0b01, data);
    SET_CLK_REGION_SEL_THOLD_SL(data);
    SET_CLK_REGION_SEL_THOLD_NSL(data);
    SET_CLK_REGION_SEL_THOLD_ARY(data);
    FAPI_TRY(fapi2::putScom(all, CLK_REGION, data));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

///
/// @brief Setup the dynamic scaninit feature-select vector for contained a IPL
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode dyn_inits_setup(const bool i_runn)
{
    fapi2::ATTR_DYNAMIC_INIT_FEATURE_VEC_Type dyninits;
    fapi2::ATTR_RUNN_SRESET_THREADS_BVEC_Type sthreads;
    fapi2::ATTR_RUNN_USE_QME_TIMEBASE_Type use_qme_tb;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DYNAMIC_INIT_FEATURE_VEC, SYS, dyninits));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_SRESET_THREADS_BVEC, SYS, sthreads));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_USE_QME_TIMEBASE, SYS, use_qme_tb));
    dyninits &= ~fapi2::ENUM_ATTR_DYNAMIC_INIT_FEATURE_VEC_HOSTBOOT;

    if (i_runn)
    {
        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T0 & sthreads)
        {
            dyninits |= fapi2::ENUM_ATTR_DYNAMIC_INIT_FEATURE_VEC_RUNN_SRESET_T0;
        }

        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T1 & sthreads)
        {
            dyninits |= fapi2::ENUM_ATTR_DYNAMIC_INIT_FEATURE_VEC_RUNN_SRESET_T1;
        }

        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T2 & sthreads)
        {
            dyninits |= fapi2::ENUM_ATTR_DYNAMIC_INIT_FEATURE_VEC_RUNN_SRESET_T2;
        }

        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T3 & sthreads)
        {
            dyninits |= fapi2::ENUM_ATTR_DYNAMIC_INIT_FEATURE_VEC_RUNN_SRESET_T3;
        }

        if (use_qme_tb == fapi2::ENUM_ATTR_RUNN_USE_QME_TIMEBASE_ON)
        {
            dyninits |= fapi2::ENUM_ATTR_DYNAMIC_INIT_FEATURE_VEC_RUNN_USE_QME_TB_SRC;
        }
    }

    FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_DYNAMIC_INIT_FEATURE_VEC, SYS, dyninits));

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Disable non-synchronous hangpulses and set AVP mode in cache-contained
///        mode
///
/// @param[in] i_chip Reference to chip target
/// @param[in] i_chc  Indicate chip-contained mode
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode contained_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        bool i_chc)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    fapi2::buffer<uint64_t> data;
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > all;

    if (i_chc)
    {
        all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    }
    else
    {
        all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
    }

    if (!i_chc)
    {
        // Enable AVP mode
        data = 0;
        FAPI_TRY(PREP_CPLT_CTRL0_WO_OR(all));
        SET_CPLT_CTRL0_TC_UNIT_AVP_MODE(data);
        FAPI_TRY(PUT_CPLT_CTRL0_WO_OR(all, data));

        // Raise EQ chiplet fence to force core ids to zero
        data = 0;
        FAPI_TRY(PREP_NET_CTRL0_RW_WOR(all));
        SET_NET_CTRL0_FENCE_EN(data);
        FAPI_TRY(PUT_NET_CTRL0_RW_WOR(all, data));
    }

    // Disable all hang pulses except heartbeat
    data = 0;
    FAPI_TRY(fapi2::putScom(all, HANG_PULSE_0_REG, data));
    FAPI_TRY(fapi2::putScom(all, HANG_PULSE_1_REG, data));
    FAPI_TRY(fapi2::putScom(all, HANG_PULSE_2_REG, data));
    FAPI_TRY(fapi2::putScom(all, HANG_PULSE_3_REG, data));
    FAPI_TRY(fapi2::putScom(all, HANG_PULSE_4_REG, data));
    FAPI_TRY(fapi2::putScom(all, HANG_PULSE_5_REG, data));

    // Disable all constant hang pulses
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0071, data));
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0073, data));
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0075, data));
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0077, data));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

///
/// @brief Hold EQ partial-good configuration from CPLT_CTRL2 registers which
///        need to be saved and restored during the contained IPL.
///
/// eq_pgoods: [quadnum] -> CPLT_CTRL2 register value
///
using eq_pgoods = std::map<size_t, uint64_t>;

static fapi2::ReturnCode save_eq_pgoods(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
                                        fapi2::TARGET_TYPE_MULTICAST > & i_perv_eqs_w_cores,
                                        eq_pgoods& io_eq_pgoods)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    const size_t perv_eq_cplt_id_start = 32;
    fapi2::buffer<uint64_t> tmp;
    fapi2::ATTR_CHIP_UNIT_POS_Type quadnum;

    for (const auto& eq : i_perv_eqs_w_cores.getChildren<fapi2::TARGET_TYPE_PERV>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, eq, quadnum));
        FAPI_TRY(GET_CPLT_CTRL2_RW(eq, tmp));
        io_eq_pgoods[quadnum - perv_eq_cplt_id_start] = tmp;
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

static fapi2::ReturnCode restore_eq_pgoods(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST > & i_perv_eqs_w_cores,
        const eq_pgoods& i_eq_pgoods)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    const size_t perv_eq_cplt_id_start = 32;
    fapi2::buffer<uint64_t> tmp;
    fapi2::ATTR_CHIP_UNIT_POS_Type quadnum;

    for (const auto& eq : i_perv_eqs_w_cores.getChildren<fapi2::TARGET_TYPE_PERV>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, eq, quadnum));
        tmp = i_eq_pgoods.at(quadnum - perv_eq_cplt_id_start);
        FAPI_TRY(PREP_CPLT_CTRL2_RW(eq));
        FAPI_TRY(PUT_CPLT_CTRL2_RW(eq, tmp));
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

extern "C" {
    fapi2::ReturnCode p10_contained_ipl(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        bool chc;
        bool runn;
        const auto eqs_all_cores = i_target.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
        const auto perv_eqs_w_cores = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
        fapi2::ATTR_ACTIVE_CORES_VEC_Type active_bvec = 0;
        fapi2::ATTR_SYSTEM_IPL_PHASE_Type ipl_phase = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CONTAINED_IPL;

        l3_config l3_cache_config;
        eq_pgoods eq_pgood_config;

        FAPI_INF("Switching to ATTR_SYSTEM_IPL_PHASE[CONTAINED_IPL]");
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYSTEM_IPL_PHASE, SYS, ipl_phase));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_target, active_bvec));
        FAPI_TRY(is_chc_ipl(chc));
        FAPI_TRY(is_runn_ipl(runn));
        FAPI_TRY(dyn_inits_setup(runn));

        FAPI_TRY(save_l3_config(i_target, active_bvec, l3_cache_config));
        FAPI_TRY(save_eq_pgoods(perv_eqs_w_cores, eq_pgood_config));
        FAPI_TRY(stopclocks(i_target));
        FAPI_TRY(contained_setup(i_target, chc));

        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 ISTEP3_MC_GROUPS));

        if (chc)
        {
            // istep3 arrayinit expects the EQ chiplets' partial-good config
            // to mark all cores as 'bad'.
            const auto istep3_cplt_ctrl2 = fapi2::buffer<uint64_t>(0)
                                           .setBit<scomt::perv::CPLT_CTRL1_REGION0_FENCE_DC>()   // perv
                                           .setBit<scomt::perv::CPLT_CTRL1_REGION9_FENCE_DC>()   // qme
                                           .setBit<scomt::perv::CPLT_CTRL1_REGION10_FENCE_DC>(); // clkadj
            const auto perv_eqs = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);
            FAPI_TRY(fapi2::putScom(perv_eqs, scomt::perv::CPLT_CTRL2_RW, istep3_cplt_ctrl2));
            FAPI_TRY(p10_contained_ipl_istep3(i_target, runn));
        }
        else
        {
#undef P10_CACHE_CONTAINED_IPL_SCAN0_EQ
#ifdef P10_CACHE_CONTAINED_IPL_SCAN0_EQ
            const auto eqs = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
            // In chip-contained mode the EQ regions are scan-zeroed during
            // the istep3 re-IPL. The istep3 re-IPL is skipped for cache-contained
            // mode so we scan-zero the EQ regions here.
            const auto regions = fapi2::buffer<uint16_t>(CONTAINED_EQ_REGIONS.getBit<4, 15>());
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(eqs, regions,
                                                   p10SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(eqs, regions,
                                                   p10SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));
#endif

            if (!runn)
            {
                FAPI_TRY(restart_perv_qme_clocks(i_target));
            }
        }

        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 ISTEP4_MC_GROUPS));
        FAPI_TRY(restore_eq_pgoods(perv_eqs_w_cores, eq_pgood_config));
        FAPI_TRY(p10_contained_ipl_istep4(eqs_all_cores, chc, runn,
                                          l3_cache_config, active_bvec));

    fapi_try_exit:
        return fapi2::current_err;
    }
} // extern "C"
