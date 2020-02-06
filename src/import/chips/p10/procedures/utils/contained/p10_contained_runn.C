/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_runn.C $ */
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

#include <fapi2.H>
#include <p10_scom_perv.H>
#include <p10_scom_eq.H>
#include <multicast_group_defs.H>
#include <queue>

enum opcg_capt_region
{
    CORE,
    L3,
    MMA
};

struct opcg_capt_regs
{
    uint64_t capt1;
    uint64_t capt2;
    uint64_t capt3;
};

///
/// @brief Compute the left-shift required for each region's 2b value into the
///        OPCG_CAPT* registers.
///
/// @param[in] i_corenum Associated core instance number (0-3) for region
/// @param[in] i_region  Region
///
/// @return Value representing amount to left-shift a 2b value into a region's
///         2b field in the OPCG_CAPT* registers.
///
static inline size_t __attribute__((always_inline))
opcg_capt_region_shift (size_t i_corenum, opcg_capt_region i_region)
{
    size_t region_shift = (i_region == CORE) ? 0 : (i_region == L3) ? 8 : 20;
    return (63 - 3 - 2 * (i_corenum % 4) - region_shift);
}

///
/// @brief Update OPCG_CAPT[0-3] register values to match a core's specified
///        RUNN cycle offset (HLD delay, additional cycle(s) delay after RUNN
///        counter expires). Note: register values are OR'd in so this function
///        can be called iteratively with the same register structure for each
///        core in a single EQ.
///
/// @param[in] i_core  Reference to core target
/// @param[io] io_regs Updated OPCT_CAPT[0-3] register values
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode update_opcg_capt_regs(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core,
        opcg_capt_regs& io_regs)
{
    fapi2::ATTR_RUNN_CORE_CYCLE_OFFSET_Type offset;
    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_CORE_CYCLE_OFFSET, i_core,
                           offset));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_core, corenum));

    FAPI_ASSERT(offset >= 0 && offset <= 4,
                fapi2::P10_CONTAINED_RUN_INVALID_CORE_RUNN_OFFSET()
                .set_CORE(corenum),
                "ATTR_RUNN_CORE_CYCLE_OFFSET is not 0,1,2,3, or 4");

    // XXX replace switch-statement with maths (see P10_EPS.pptx)
    switch(offset)
    {
        case 1:
            io_regs.capt2 |= 0b01ull << opcg_capt_region_shift(corenum, CORE) |
                             0b01ull << opcg_capt_region_shift(corenum, MMA);
            break;

        case 2:
            io_regs.capt1 |= 0b01ull << opcg_capt_region_shift(corenum, L3);
            io_regs.capt2 |= 0b01ull << opcg_capt_region_shift(corenum, CORE) |
                             0b01ull << opcg_capt_region_shift(corenum, MMA);
            io_regs.capt3 |= 0b01ull << opcg_capt_region_shift(corenum, CORE) |
                             0b01ull << opcg_capt_region_shift(corenum, MMA);
            break;

        case 3:
            io_regs.capt1 |= 0b01ull << opcg_capt_region_shift(corenum, L3);
            io_regs.capt2 |= 0b10ull << opcg_capt_region_shift(corenum, CORE) |
                             0b10ull << opcg_capt_region_shift(corenum, MMA);
            io_regs.capt3 |= 0b01ull << opcg_capt_region_shift(corenum, CORE) |
                             0b01ull << opcg_capt_region_shift(corenum, MMA);
            break;

        case 4:
            io_regs.capt1 |= 0b10ull << opcg_capt_region_shift(corenum, L3);
            io_regs.capt2 |= 0b10ull << opcg_capt_region_shift(corenum, CORE) |
                             0b10ull << opcg_capt_region_shift(corenum, MMA);
            io_regs.capt3 |= 0b10ull << opcg_capt_region_shift(corenum, CORE) |
                             0b10ull << opcg_capt_region_shift(corenum, MMA);
            break;
    }

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Read the system RUNN cycle count and apply the chip RUNN cycle offset.
///
/// @param[in] i_chip     Reference to chip target
/// @param[in] o_runn_cnt RUNN count
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode init_runn_cnt(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                       uint64_t& o_runn_cnt)
{
    fapi2::ATTR_RUNN_CYCLE_COUNT_Type runn_sys_cnt;
    fapi2::ATTR_RUNN_CHIP_CYCLE_OFFSET_Type runn_chip_offset;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_CYCLE_COUNT, SYS, runn_sys_cnt));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_CHIP_CYCLE_OFFSET, i_chip,
                           runn_chip_offset));

    o_runn_cnt = runn_sys_cnt + runn_chip_offset;

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Apply the EQ RUNN cycle offset to a RUNN count.
///
/// @param[in]  i_quad     Reference to quad target
/// @param[in]  i_runn_cnt Base RUNN count
/// @param[out] o_runn_cnt EQ RUNN count
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode quad_runn_cnt(const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_quad,
                                       uint64_t i_runn_cnt, uint64_t& o_runn_cnt)
{
    fapi2::ATTR_RUNN_QUAD_CYCLE_OFFSET_Type runn_quad_offset = 0; // -Werror=maybe-uninitialized

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_QUAD_CYCLE_OFFSET, i_quad,
                           runn_quad_offset));

    o_runn_cnt = i_runn_cnt + runn_quad_offset;

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Check that clocks are running for expected cache-/chip-contained
///        regions.
///
/// @param[in]  i_chip Reference to chip target
/// @param[in]  i_chc  Indicate chip-contained mode
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode runn_check_clks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        bool i_chc)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    const uint64_t addrs[3] = { CLOCK_STAT_SL, CLOCK_STAT_NSL, CLOCK_STAT_ARY };
    fapi2::buffer<uint64_t> data;
    // When reading the CLOCK_STAT_* registers via multicast-OR, partial bad
    // regions act "neutral" and return zero (ie. "running"). This lets us get
    // away with just using the "usual" region mask(s).
    auto mask = CONTAINED_EQ_REGIONS;
    mask.setBit<CPLT_CTRL1_REGION0_FENCE_DC>();
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > all;

    if (!i_chc)
    {
        all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
    }
    else
    {
        mask |= CONTAINED_N0_REGIONS | CONTAINED_N1_REGIONS;
        all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    }

    for (const auto& addr : addrs)
    {
        FAPI_TRY(fapi2::getScom(all, addr, data));
        // Running clocks are indicated by zeros in the appropriate region bit(s)
        FAPI_ASSERT((data & mask) == 0,
                    fapi2::P10_CONTAINED_RUN_CLKS_OFF_AFTER_RUNN_START()
                    .set_ACTUAL(data)
                    .set_ADDR(addr)
                    .set_EXPECTED(mask),
                    "Expected clocks are not running after RUNN start");
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

///
/// @brief Check that clocks were started in-phase by comparing the captured
///        phase counter values from each chiplet's CC in chip-contained mode.
///
/// @param[in]  i_chip Reference to chip target
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode runn_check_sync(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    fapi2::buffer<uint64_t> data;
    const auto all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST,
          fapi2::MULTICAST_COMPARE > comp = all;
    auto rc = GET_SYNC_CONFIG(comp, data);

    // Multicast-compare sets each bit where the comparison is true
    FAPI_ASSERT(rc == fapi2::FAPI2_RC_SUCCESS,
                fapi2::P10_CONTAINED_RUN_PHASE_COUNTERS_NOT_EQUAL(),
                "Phase counters were not equal when RUNN started");

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

///
/// @brief Write OPCG_REG0 to initiate a RUNN count clock-start.
///
/// @param[in]  i_perv           Reference to perv target
/// @param[in]  i_set_hld_dly_en Set OPCG_REG0_RUNN_HLD_DLY_EN
/// @param[in]  i_stop_on_xstop  Set OPCG_REG0_STOP_RUNN_ON_XSTOP
/// @param[in]  i_runn_cnt       RUNN cycle count to program
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode runn_opcg_start(const fapi2::Target < fapi2::TARGET_TYPE_PERV |
        fapi2::TARGET_TYPE_MULTICAST > & i_perv,
        bool i_set_hld_dly_en, bool i_stop_on_xstop, uint64_t i_runn_cnt)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    fapi2::buffer<uint64_t> data = 0;

    FAPI_TRY(PREP_OPCG_REG0(i_perv));
    SET_OPCG_REG0_RUNN_MODE(data);
    SET_OPCG_REG0_OPCG_GO(data);

    if (i_set_hld_dly_en)
    {
        data.setBit<15>(); // OPCG_REG0_RUNN_HLD_DLY_EN
    }


    // Convert core RUNN cycles to nest RUNN cycles (what the OPCG wants)
    i_runn_cnt = i_runn_cnt / 2;

    SET_OPCG_REG0_LOOP_COUNT(i_runn_cnt, data);

    if (i_stop_on_xstop)
    {
        SET_OPCG_REG0_STOP_RUNN_ON_XSTOP(data);
    }

    FAPI_TRY(PUT_OPCG_REG0(i_perv, data));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

///
/// @brief Setup OPCG HLD_DLY functionality if required due to an odd number of
///        "core" RUNN cycles or explicit per-core HLD_DLY offset.
///
/// @param[in]  i_perv_eq        Reference to perv EQ target
/// @param[in]  i_runn_cnt       RUNN cycle count
/// @param[out] o_set_hld_dly_en OPCG is setup to use HLD_DLY functionality
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode runn_opcg_setup(const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_perv_eq,
        const uint64_t i_runn_cnt,
        bool& o_set_hld_dly_en)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::eq;

    const auto quad = i_perv_eq.getChildren<fapi2::TARGET_TYPE_EQ>()[0];
    opcg_capt_regs regs = {0, 0, 0};

    for (auto const& core : quad.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        if (i_runn_cnt % 2 == 1)
        {
            fapi2::ATTR_RUNN_CORE_CYCLE_OFFSET_Type offset;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_CORE_CYCLE_OFFSET,
                                   core, offset));
            offset += 1;
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_RUNN_CORE_CYCLE_OFFSET,
                                   core, offset));
        }

        FAPI_TRY(update_opcg_capt_regs(core, regs));
    }

    // Program per-core offsets
    FAPI_TRY(PUT_OPCG_CAPT1(quad, regs.capt1));
    FAPI_TRY(PUT_OPCG_CAPT2(quad, regs.capt2));
    FAPI_TRY(PUT_OPCG_CAPT3(quad, regs.capt3));

    o_set_hld_dly_en = regs.capt1 != 0 || regs.capt2 != 0 || regs.capt3 != 0;

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode runn_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                             const bool i_chc)
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

    // Setup clock-controller
    FAPI_TRY(PREP_SYNC_CONFIG(all));
    // This allows us to later read out the phase counters and make sure all
    // clocks actually started in-phase
    SET_SYNC_CONFIG_PHASE_COUNTER_ON_CLKCHANGE_EN(data);
    FAPI_TRY(PUT_SYNC_CONFIG(all, data));

    data = 0;
    FAPI_TRY(fapi2::putScom(all, OPCG_REG1, data));
    FAPI_TRY(fapi2::putScom(all, OPCG_REG2, data));
    FAPI_TRY(fapi2::putScom(all, OPCG_CAPT1, data));
    FAPI_TRY(fapi2::putScom(all, OPCG_CAPT2, data));
    FAPI_TRY(fapi2::putScom(all, OPCG_CAPT3, data));

    data = CONTAINED_EQ_REGIONS;

    if (i_chc)
    {
        data |= CONTAINED_N0_REGIONS | CONTAINED_N1_REGIONS;
    }

    FAPI_TRY(PREP_CLK_REGION(all));
    SET_CLK_REGION_CLOCK_CMD(0b00, data);
    SET_CLK_REGION_SEL_THOLD_SL(data);
    SET_CLK_REGION_SEL_THOLD_NSL(data);
    SET_CLK_REGION_SEL_THOLD_ARY(data);
    FAPI_TRY(PUT_CLK_REGION(all, data));

    data = CONTAINED_EQ_REGIONS;

    if (i_chc)
    {
        data |= CONTAINED_N0_REGIONS | CONTAINED_N1_REGIONS;
    }

    FAPI_TRY(PUT_CPLT_CTRL1_WO_CLEAR(all, data));

    // Align chiplets
    {
        data = 0;
        data.setBit<CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH>();
        FAPI_TRY(PUT_CPLT_CTRL0_WO_OR(all, data));

        data = 0;
        data.setBit<CPLT_CTRL0_CTRL_CC_FORCE_ALIGN>();
        FAPI_TRY(PUT_CPLT_CTRL0_WO_OR(all, data));

        FAPI_TRY(GET_CPLT_STAT0(all, data));
        FAPI_ASSERT(GET_CPLT_STAT0_CC_CTRL_CHIPLET_IS_ALIGNED_DC(data),
                    fapi2::CPLT_NOT_ALIGNED_ERR()
                    .set_PERV_CPLT_STAT0(data)
                    .set_LOOP_COUNT(0)
                    .set_HW_DELAY(0),
                    "Some chiplet is not aligned after setting FORCE_ALIGN");

        data = 0;
        data.setBit<CPLT_CTRL0_CTRL_CC_FORCE_ALIGN>();
        FAPI_TRY(PUT_CPLT_CTRL0_WO_CLEAR(all, data));

        data = 0;
        data.setBit<CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH>();
        FAPI_TRY(PUT_CPLT_CTRL0_WO_CLEAR(all, data));

        FAPI_TRY(GET_CPLT_STAT0(all, data));
        FAPI_ASSERT(GET_CPLT_STAT0_CC_CTRL_CHIPLET_IS_ALIGNED_DC(data),
                    fapi2::CPLT_NOT_ALIGNED_ERR()
                    .set_PERV_CPLT_STAT0(data)
                    .set_LOOP_COUNT(0)
                    .set_HW_DELAY(0),
                    "Some chiplet is not aligned after clearing FORCE_ALIGN");
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode runn_start(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                             bool i_chc)
{
    FAPI_INF(">> %s", __func__);

    auto all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    // Only want EQs with actively-configured cores - the obvious way to get
    // there is to convert the multicast group containing all "good" EQs
    // down to individual EQ targets.
    auto quads = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
    uint64_t base_runn_cnt = 0;
    fapi2::buffer<uint64_t> data;
    fapi2::ATTR_RUNN_DO_CONFIG_CHECKS_Type do_checks;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_DO_CONFIG_CHECKS, SYS, do_checks));
    FAPI_TRY(init_runn_cnt(i_chip, base_runn_cnt));

    if (!i_chc)
    {
        // We need to track which EQ clock-controllers are supposed to enable
        // the per core offset delay after the initial RUNN count expires.
        std::queue<bool> set_hld_dly_en;

        // Program any per-core HLD_DLY offsets
        for (auto const& perv_eq : quads.getChildren<fapi2::TARGET_TYPE_PERV>())
        {
            const auto quad = perv_eq.getChildren<fapi2::TARGET_TYPE_EQ>()[0];
            uint64_t runn_cnt = 0;
            bool tmp;

            FAPI_TRY(quad_runn_cnt(quad, base_runn_cnt, runn_cnt));
            FAPI_TRY(runn_opcg_setup(perv_eq, runn_cnt, tmp));

            set_hld_dly_en.push(tmp);
        }

        // Program the OPCG to start the RUNN. Do *not* merge this loop with
        // the previous so that all EQs are "primed" to go and only require the
        // OPCG_REG0 to be written at this point - no other registers need to
        // be configured. This means we can take checkpoints before the RUNN
        // start per EQ in simulation and only a single SCOM is required to go.
        for (auto const& perv_eq : quads.getChildren<fapi2::TARGET_TYPE_PERV>())
        {
            using namespace scomt::eq;

            const auto quad = perv_eq.getChildren<fapi2::TARGET_TYPE_EQ>()[0];
            uint64_t runn_cnt = 0;
            fapi2::ATTR_RUNN_STOP_ON_XSTOP_Type use_stop_on_xstop;

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_STOP_ON_XSTOP, perv_eq,
                                   use_stop_on_xstop));
            FAPI_TRY(quad_runn_cnt(quad, base_runn_cnt, runn_cnt));

            {
                fapi2::ATTR_CHIP_UNIT_POS_Type quad_num;
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, quad, quad_num));
                std::string tmp = "before_opcg_start_eq" + std::to_string(quad_num);
                FAPI_TRY(sim::checkpoint(tmp));
            }

            // Start RUNN
            FAPI_TRY(runn_opcg_start(perv_eq, set_hld_dly_en.front(),
                                     use_stop_on_xstop == fapi2::ENUM_ATTR_RUNN_STOP_ON_XSTOP_ON,
                                     runn_cnt));
            set_hld_dly_en.pop();
        }
    }
    else
    {
        bool set_hld_dly_en;
        bool stop_on_xstop = false; //-Werror=maybe-uninitialized

        for (auto const& perv_eq : quads.getChildren<fapi2::TARGET_TYPE_PERV>())
        {
            fapi2::ATTR_RUNN_STOP_ON_XSTOP_Type use_stop_on_xstop;

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_STOP_ON_XSTOP, perv_eq,
                                   use_stop_on_xstop));
            FAPI_TRY(runn_opcg_setup(perv_eq, base_runn_cnt, set_hld_dly_en));

            stop_on_xstop = stop_on_xstop ||
                            use_stop_on_xstop == fapi2::ENUM_ATTR_RUNN_STOP_ON_XSTOP_ON;
        }

        FAPI_TRY(sim::checkpoint(std::string("before_opcg_start")));
        FAPI_TRY(runn_opcg_start(all, set_hld_dly_en, stop_on_xstop, base_runn_cnt));
    }

    if (do_checks == fapi2::ENUM_ATTR_RUNN_DO_CONFIG_CHECKS_ON)
    {
        FAPI_TRY(runn_check_clks(i_chip, i_chc));

        if (i_chc)
        {
            FAPI_TRY(runn_check_sync(i_chip));
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}
