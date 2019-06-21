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
#include <p10_hang_pulse_mc_setup_tables.H>
#include <p10_hcd_core_stopclocks.H>
#include <p10_hcd_cache_stopclocks.H>

namespace
{
///
/// @brief Stopclocks for all regions required for either a cache- or
///        chip-contained IPL
///
/// @param[in] i_chip Reference to chip target
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
inline fapi2::ReturnCode stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{

    if (sim::skip_stopclocks())
    {
        // Use FAPI_ERR(...) to force print this message
        FAPI_ERR("Skipping stopclocks");
        return fapi2::FAPI2_RC_SUCCESS;
    }

    using namespace scomt::perv;

    auto data = CHC_MC_REGIONS_NO_PERV;
    const auto all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);

    // Raise fences (including perv region fence)
    FAPI_TRY(fapi2::putScom(all, CPLT_CTRL1_WO_OR,
                            data.setBit<CPLT_CTRL1_REGION0_FENCE_DC>()));

    // Stop clocks
    FAPI_TRY(PREP_CLK_REGION(all));
    SET_CLK_REGION_CLOCK_CMD(0b10, data);
    CLEAR_CLK_REGION_CLOCK_REGION_PERV(data);
    SET_CLK_REGION_SEL_THOLD_SL(data);
    SET_CLK_REGION_SEL_THOLD_NSL(data);
    SET_CLK_REGION_SEL_THOLD_ARY(data);
    FAPI_TRY(fapi2::putScom(all, CLK_REGION, data));

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
inline fapi2::ReturnCode contained_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        bool i_chc)
{
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
        FAPI_TRY(PREP_CPLT_CTRL0_WO_OR(all));
        SET_CPLT_CTRL0_TC_UNIT_AVP_MODE(data);
        FAPI_TRY(PUT_CPLT_CTRL0_WO_OR(all, data));
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
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0070, data));
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0071, data));
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0072, data));
    FAPI_TRY(fapi2::putScom(i_chip, 0xd0073, data));
fapi_try_exit:
    return fapi2::current_err;
}

}; // namespace

extern "C" {
    fapi2::ReturnCode p10_contained_ipl(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        bool chc;
        bool cac;
        bool runn;
        const auto eqs = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
        const auto eqs_all_cores = i_target.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
        fapi2::ATTR_SYSTEM_IPL_PHASE_Type ipl_phase = fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CONTAINED_IPL;

        FAPI_INF("Switching to ATTR_SYSTEM_IPL_PHASE[CONTAINED_IPL]");
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SYSTEM_IPL_PHASE, SYS, ipl_phase));
        FAPI_TRY(get_contained_ipl_type(cac, chc));
        FAPI_TRY(get_contained_run_mode(runn));

        FAPI_TRY(stopclocks(i_target));
        FAPI_TRY(contained_setup(i_target, chc));

        if (chc)
        {
            FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                     ISTEP3_MC_GROUPS));
            FAPI_TRY(p10_contained_ipl_istep3(i_target, runn));
        }
        else
        {
            // In chip-contained mode the EQ regions are scan-zeroed during
            // the istep3 re-IPL. The istep3 re-IPL is skipped for cache-contained
            // mode so we scan-zero the EQ regions here.
            const auto regions = fapi2::buffer<uint16_t>(CHC_EQ_REGIONS_NO_PERV.getBit<4, 15>());
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(eqs, regions,
                                                   p10SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(eqs, regions,
                                                   p10SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));
        }

        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 ISTEP4_MC_GROUPS));
        FAPI_TRY(p10_contained_ipl_istep4(eqs_all_cores, chc, runn));

    fapi_try_exit:
        return fapi2::current_err;
    }
} // extern "C"
