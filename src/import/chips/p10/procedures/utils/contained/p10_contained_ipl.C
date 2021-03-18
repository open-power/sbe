/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_ipl.C $ */
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
#include <p10_contained.H>
#include <p10_contained_sim.H>
#include <p10_contained_ipl_istep.H>

#include <fapi2.H>
#include <p10_scom_c.H>
#include <p10_scom_perv.H>
#include <p10_perv_sbe_cmn.H>
#include <multicast_group_defs.H>
#include <multicast_defs.H>
#include <p10_hang_pulse_mc_setup_tables.H>
#include <p10_hcd_core_stopclocks.H>
#include <p10_hcd_cache_stopclocks.H>
#ifndef DFT
    #include <p10_dyninit_bitvec_utils.H>
#endif
#include <p10_dynamic.H>

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
static fapi2::ReturnCode dyn_inits_setup(const bool i_runn,
        const bool i_is_dump_ipl)
{
#ifndef DFT
    using namespace p10_dyninit_bitvec_utils;

    fapi2::ATTR_RUNN_SRESET_THREADS_BVEC_Type sthreads;
    fapi2::ATTR_RUNN_USE_QME_TIMEBASE_Type use_qme_tb;
    p10_dyninit_bitvec plat_feature_bvec;

    // initialize bit vector from platform -- content will reflect
    // curent value of platform dynamic init feature vector attribute
    FAPI_TRY(init_bitvec_from_plat(SYS, FEATURE, plat_feature_bvec));
    dump_bitvec(plat_feature_bvec);

    // adjust dynamic init features based on current state/paramters
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_SRESET_THREADS_BVEC, SYS, sthreads));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_USE_QME_TIMEBASE, SYS, use_qme_tb));

    FAPI_TRY(clear_bit(plat_feature_bvec, HOSTBOOT, "HOSTBOOT"));

    if (i_runn)
    {
        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T0 & sthreads)
        {
            FAPI_TRY(set_bit(plat_feature_bvec, RUNN_SRESET_THREAD0, "RUNN_SRESET_THREAD0"));
        }

        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T1 & sthreads)
        {
            FAPI_TRY(set_bit(plat_feature_bvec, RUNN_SRESET_THREAD1, "RUNN_SRESET_THREAD1"));
        }

        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T2 & sthreads)
        {
            FAPI_TRY(set_bit(plat_feature_bvec, RUNN_SRESET_THREAD2, "RUNN_SRESET_THREAD2"));
        }

        if (fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T3 & sthreads)
        {
            FAPI_TRY(set_bit(plat_feature_bvec, RUNN_SRESET_THREAD3, "RUNN_SRESET_THREAD3"));
        }

        if (use_qme_tb == fapi2::ENUM_ATTR_RUNN_USE_QME_TIMEBASE_ON)
        {
            FAPI_TRY(set_bit(plat_feature_bvec, RUNN_USE_QME_TB_SRC, "RUNN_USE_QME_TB_SRC"));
        }
    }

    if (i_is_dump_ipl)
    {
        FAPI_TRY(set_bit(plat_feature_bvec, RUNN_CONTAINED_DUMP, "RUNN_CONTAINED_DUMP"));
        FAPI_TRY(clear_bit(plat_feature_bvec, RUNN_SRESET_THREAD0, "RUNN_SRESET_THREAD0"));
        FAPI_TRY(clear_bit(plat_feature_bvec, RUNN_SRESET_THREAD1, "RUNN_SRESET_THREAD1"));
        FAPI_TRY(clear_bit(plat_feature_bvec, RUNN_SRESET_THREAD2, "RUNN_SRESET_THREAD2"));
        FAPI_TRY(clear_bit(plat_feature_bvec, RUNN_SRESET_THREAD3, "RUNN_SRESET_THREAD3"));
    }

    // save state back to platform attribute
    dump_bitvec(plat_feature_bvec);
    FAPI_TRY(save_bitvec_to_plat(SYS, plat_feature_bvec));

fapi_try_exit:
    return fapi2::current_err;
#else
    return fapi2::FAPI2_RC_SUCCESS;
#endif
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
    fapi2::ATTR_CONTAINED_FENCE_HANGPULSE_Type fence_hangpulse;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_FENCE_HANGPULSE, i_chip, fence_hangpulse));

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

    if (fence_hangpulse)
    {
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
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

static fapi2::ReturnCode save_eq_pgoods(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
                                        fapi2::TARGET_TYPE_MULTICAST > & i_perv_eqs_w_cores)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    const size_t perv_eq_cplt_id_start = 32;
    fapi2::buffer<uint64_t> tmp;
    fapi2::ATTR_CHIP_UNIT_POS_Type quadnum;
    fapi2::ATTR_CONTAINED_EQ_PGOODS_Type eq_pgoods;

    for (const auto& eq : i_perv_eqs_w_cores.getChildren<fapi2::TARGET_TYPE_PERV>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, eq, quadnum));
        FAPI_TRY(GET_CPLT_CTRL2_RW(eq, tmp));
        eq_pgoods[quadnum - perv_eq_cplt_id_start] = tmp;
    }

    FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CONTAINED_EQ_PGOODS,
                           i_perv_eqs_w_cores.getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                           eq_pgoods));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

static fapi2::ReturnCode restore_eq_pgoods(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST > & i_perv_eqs_w_cores)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    const size_t perv_eq_cplt_id_start = 32;
    fapi2::buffer<uint64_t> tmp;
    fapi2::ATTR_CHIP_UNIT_POS_Type quadnum;
    fapi2::ATTR_CONTAINED_EQ_PGOODS_Type eq_pgoods;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_EQ_PGOODS,
                           i_perv_eqs_w_cores.getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                           eq_pgoods));

    for (const auto& eq : i_perv_eqs_w_cores.getChildren<fapi2::TARGET_TYPE_PERV>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, eq, quadnum));
        tmp = eq_pgoods[quadnum - perv_eq_cplt_id_start];
        FAPI_TRY(PREP_CPLT_CTRL2_RW(eq));
        FAPI_TRY(PUT_CPLT_CTRL2_RW(eq, tmp));
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

static fapi2::ReturnCode connect_single_ec_to_fbc(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{
#ifndef __PPE__ // No such thing as an environment variable on a PPE
    FAPI_INF(">> %s", __func__);

    using namespace scomt::c;

    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;

    std::string dump_core_str;
    int dump_core;

    if (!getenvvar("P10_CONTAINED_DUMP_CORE", dump_core_str))
    {
        FAPI_ERR("P10_CONTAINED_DUMP_CORE envvar not set");
        return fapi2::FAPI2_RC_SUCCESS;
    }

    dump_core = std::stoi(dump_core_str); // YOLO.

    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        // Functionally disconnect all other ECs from the fabric by applying
        // the proper PM controls.
        if (corenum != dump_core)
        {
            fapi2::buffer<uint64_t> tmp;

            tmp.flush<0>()
            .setBit<QME_SCSR_HBUS_DISABLE>()
            .setBit<QME_SCSR_L2RCMD_INTF_QUIESCE>()
            .setBit<QME_SCSR_NCU_TLBIE_QUIESCE>()
            .setBit<QME_SCSR_PB_PURGE_REQ>();
            PREP_QME_SCSR_SCOM2(core);
            FAPI_TRY(PUT_QME_SCSR_SCOM2(core, tmp));

            tmp.flush<0>()
            .setBit<NC_NCMISC_NCSCOMS_NCU_RCMD_QUIESCE_REG_NCU_RCMD_QUIESCE>();
            PREP_NC_NCMISC_NCSCOMS_NCU_RCMD_QUIESCE_REG(core);
            FAPI_TRY(PUT_NC_NCMISC_NCSCOMS_NCU_RCMD_QUIESCE_REG(core, tmp));

            tmp.flush<0>()
            .setBit<L3_MISC_L3CERRS_PM_LCO_DIS_REG_LCO_DIS_CFG>()
            .setBit<L3_MISC_L3CERRS_PM_LCO_DIS_REG_RCMD_DIS_CFG>();
            PREP_L3_MISC_L3CERRS_PM_LCO_DIS_REG(core);
            FAPI_TRY(PUT_L3_MISC_L3CERRS_PM_LCO_DIS_REG(core, tmp));
        }
        else
        {
            // Inits change sometimes so make sure these are clear for the dump core
            fapi2::buffer<uint64_t> tmp;

            tmp.flush<0>()
            .setBit<QME_SCSR_HBUS_DISABLE>()
            .setBit<QME_SCSR_L2RCMD_INTF_QUIESCE>()
            .setBit<QME_SCSR_NCU_TLBIE_QUIESCE>()
            .setBit<QME_SCSR_PB_PURGE_REQ>();
            PREP_QME_SCSR_WO_CLEAR(core);
            FAPI_TRY(PUT_QME_SCSR_WO_CLEAR(core, tmp));

            tmp.flush<0>();
            PREP_NC_NCMISC_NCSCOMS_NCU_RCMD_QUIESCE_REG(core);
            FAPI_TRY(PUT_NC_NCMISC_NCSCOMS_NCU_RCMD_QUIESCE_REG(core, tmp));

            tmp.flush<0>();
            PREP_L3_MISC_L3CERRS_PM_LCO_DIS_REG(core);
            FAPI_TRY(PUT_L3_MISC_L3CERRS_PM_LCO_DIS_REG(core, tmp));
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
#else //ifdef __PPE__
    FAPI_INF("SKIPPING connect_single_ec_to_fbc on PPE plat");
    return fapi2::FAPI2_RC_SUCCESS;
#endif
}

extern "C" {
    fapi2::ReturnCode p10_contained_ipl(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        bool chc;
        bool runn;
        bool is_dump_ipl;
        const auto eqs_all_cores = i_target.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
        const auto perv_eqs_w_cores = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
        fapi2::ATTR_ACTIVE_CORES_VEC_Type active_bvec = 0;
        fapi2::ATTR_SYSTEM_IPL_PHASE_Type ipl_phase;
        fapi2::ATTR_CHIP_EC_FEATURE_CONTAINED_QME_MMA_AVAIL_Type l_contained_qme_mma_avail;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_CONTAINED_QME_MMA_AVAIL, i_target, l_contained_qme_mma_avail));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, SYS, ipl_phase));
        is_dump_ipl = ipl_phase == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CONTAINED_IPL;

        if (is_dump_ipl)
        {
            fapi2::ATTR_RUNN_MODE_Type runn_mode = fapi2::ENUM_ATTR_RUNN_MODE_OFF;
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_RUNN_MODE, SYS, runn_mode));
            FAPI_INF("Running contained dump IPL");
        }
        else
        {
            ipl_phase = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CONTAINED_IPL;
            FAPI_INF("Switching to ATTR_SYSTEM_IPL_PHASE[CONTAINED_IPL]");
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYSTEM_IPL_PHASE, SYS, ipl_phase));
        }

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_target, active_bvec));
        FAPI_TRY(is_chc_ipl(chc));
        FAPI_TRY(is_runn_ipl(runn));
        FAPI_TRY(dyn_inits_setup(runn, is_dump_ipl));

        if (!is_dump_ipl)
        {
            FAPI_TRY(save_l3_config(i_target, active_bvec));
            FAPI_TRY(save_eq_pgoods(perv_eqs_w_cores));
        }

        FAPI_TRY(stopclocks(i_target));
        FAPI_TRY(contained_setup(i_target, chc));

        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 ISTEP3_MC_GROUPS));

        if (is_dump_ipl)
        {
            using namespace scomt::perv;
            const auto all = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
            const uint64_t istep3_sync_config = 0xa800000000000000;

            for (auto const& cplt : all.getChildren<fapi2::TARGET_TYPE_PERV>())
            {
                // EQs are started sequentially in a cache-contained RUNN and
                // other synchronous chiplets remain clocked-off resulting in
                // different phase counter values in SYNC_CONFIG. This needs to
                // be cleaned up before calling sbe_startclocks during a
                // reeeeee-IPL to prevent a multicast read-compare of the
                // SYNC_CONFIG register to fail.
                if (!chc)
                {
                    FAPI_TRY(PREP_SYNC_CONFIG(cplt));
                    FAPI_TRY(PUT_SYNC_CONFIG(cplt, istep3_sync_config));
                }

                // Mask xstop inputs to the OPCG since we do not re-initialize
                // that logic as part of a contained IPL.
                FAPI_TRY(PREP_XSTOP1(cplt));
                FAPI_TRY(PUT_XSTOP1(cplt, 0));
            }
        }

        if (chc || is_dump_ipl)
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
            if (!runn)
            {
                FAPI_TRY(restart_perv_qme_clocks(i_target));
            }
        }

        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 SELECT_EX_MC_GROUPS));
        FAPI_TRY(restore_eq_pgoods(perv_eqs_w_cores));
        FAPI_TRY(p10_contained_ipl_istep4(eqs_all_cores, chc, runn, is_dump_ipl,
                                          active_bvec));

        if (is_dump_ipl && !chc)
        {
            FAPI_TRY(connect_single_ec_to_fbc(i_target));
        }

        // rescan QME to set MMA available
        if (!chc && runn && !is_dump_ipl && l_contained_qme_mma_avail)
        {
            FAPI_TRY(set_mma_available(i_target));
        }

    fapi_try_exit:
        return fapi2::current_err;
    }
} // extern "C"
