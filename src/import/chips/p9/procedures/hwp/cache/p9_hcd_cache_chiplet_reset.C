/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_chiplet_reset.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
///
/// @file  p9_hcd_cache_chiplet_reset.C
/// @brief Cache Chiplet Reset
///
/// Procedure Summary:
///   Reset quad chiplet logic
///   Clocking:
///    - setup cache sector buffer strength,
///      pulse mode and pulsed mode enable values
///      (attribute dependency Nimbus/Cumulus)
///    - Drop glsmux async reset
///   Scan0 flush entire cache chiplet

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_misc_scom_addresses.H>
#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_chiplet_reset.H"
#include <p9_common_poweronoff.H>

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P9_HCD_CACHE_CHIPLET_RESET_CONSTANTS
{
    // (1)PCB_EP_RESET
    // (2)CLK_ASYNC_RESET
    // (3)PLL_TEST_EN
    // (4)PLLRST
    // (5)PLLBYP
    // (11)EDIS
    // (12)VITL_MPW1
    // (13)VITL_MPW2
    // (14)VITL_MPW3
    // (16)VITL_THOLD
    // (18)FENCE_EN
    // (22)FUNC_CLKSEL
    // (25)PCB_FENCE
    // (26)LVLTRANS_FENCE
    Q_NET_CTRL0_INIT_VECTOR = (BITS64(1, 5) | BITS64(11, 4) | BIT64(16) |
                               BIT64(18) | BIT64(22) | BITS64(25, 2)),
    CACHE_GLSMUX_RESET_DELAY_REF_CYCLES = 40
};

// This workaround is disabled on DD2+, the ring length data below is from DD1
enum HW388878_DD1_FIX_CONSTATNS
{
    CACHE_CLK_START_POLL_TIMEOUT_HW_NS       = 1000000, // 10^6ns = 1ms timeout
    CACHE_CLK_START_POLL_DELAY_HW_NS         = 10000,   // 10us poll loop delay
    CACHE_CLK_START_POLL_DELAY_SIM_CYCLE     = 32000,   // 320k sim cycle delay

    // Eq_fure + Ex_l2_fure(ex0) + Ex_l2_fure(ex1)
    DD1_EQ_FURE_RING_LENGTH = (46532 + 119192 + 119192)
};

//------------------------------------------------------------------------------
// Procedure: Cache Chiplet Reset
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_chiplet_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_chiplet_reset");
    fapi2::buffer<uint64_t>                     l_data64;
    uint16_t                                    l_region_scan0;
    uint64_t                                    l_l2gmux_input       = 0;
    uint64_t                                    l_l2gmux_reset       = 0;
    uint8_t                                     l_attr_chip_unit_pos = 0;
    fapi2::buffer<uint8_t>                      l_attr_dd1_vcs_workaround;

    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();
    auto l_core_functional_vector =
        i_target.getChildren<fapi2::TARGET_TYPE_CORE>
        (fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW388878, l_chip,
                           l_attr_dd1_vcs_workaround));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_perv,
                           l_attr_chip_unit_pos));
    l_attr_chip_unit_pos = l_attr_chip_unit_pos - p9hcd::PERV_TO_EQ_POS_OFFSET;

    FAPI_TRY(getScom(l_chip, PU_OCB_OCI_QCSR_SCOM, l_data64));
    FAPI_DBG("Working on cache[%d], good EXs in QCSR[%016llX]",
             l_attr_chip_unit_pos, l_data64);

    l_region_scan0 = p9hcd::SCAN0_REGION_ALL_BUT_EX;

    if (l_data64 & BIT64(l_attr_chip_unit_pos << 1))
    {
        l_region_scan0 |= p9hcd::SCAN0_REGION_EX0_L2_L3_REFR;
        l_l2gmux_reset |= BIT64(32);
        l_l2gmux_input |= BIT64(34);
    }

    if (l_data64 & BIT64((l_attr_chip_unit_pos << 1) + 1))
    {
        l_region_scan0 |= p9hcd::SCAN0_REGION_EX1_L2_L3_REFR;
        l_l2gmux_reset |= BIT64(33);
        l_l2gmux_input |= BIT64(35);
    }

    //--------------------------
    // Reset cache chiplet logic
    //--------------------------
    // If there is an unused, powered-off cache chiplet which needs to be
    // configured in the following steps to setup the PCB endpoint.

    for(auto& it : l_core_functional_vector)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it.getParent<fapi2::TARGET_TYPE_PERV>(),
                               l_attr_chip_unit_pos));
        FAPI_DBG("Assert core[%d] DCC reset via NET_CTRL0[2]",
                 (l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET));
        FAPI_TRY(putScom(l_chip, (C_NET_CTRL0_WOR + (0x1000000 *
                                  (l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET))),
                         MASK_SET(2)));
    }

    /// @todo RTC158181 needs to revisit this sim workaround
    FAPI_DBG("Init heartbeat hang counter via HANG_PULSE_6[2]");
    FAPI_TRY(putScom(i_target, EQ_HANG_PULSE_6_REG, MASK_SET(2)));

    FAPI_DBG("Init NET_CTRL0[1-5,11-14,16,18,22,25,26],step needed for hotplug");
    l_data64 = Q_NET_CTRL0_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0, l_data64));

    FAPI_DBG("Assert progdly/DCC bypass,L2 DCC reset via NET_CTRL1[1,2,23,24]");
    l_data64.flush<0>().insertFromRight<1, 2>(0x3).insertFromRight<23, 2>(0x3);
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL1_WOR, l_data64));

    FAPI_DBG("Flip cache glsmux to DPLL input via PPM_CGCR[3]");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_OR(0, 4, 0x9)));

    FAPI_DBG("Flip L2 glsmux to DPLL input via QPPM_EXCGCR[34:35]");

    if (l_attr_dd1_vcs_workaround)
    {
        FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, MASK_OR(34, 2, 3)));
    }
    else
    {
        FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, l_l2gmux_input));
    }

    FAPI_DBG("Assert DPLL ff_bypass via QPPM_DPLL_CTRL[2]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, MASK_SET(2)));

    FAPI_DBG("Drop vital thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(16)));

    /// @todo RTC158181 setup sector buffer strength, pulse mode and pulsed mode enable

    FAPI_DBG("Drop cache glsmux reset via PPM_CGCR[0]");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_SET(3)));

    FAPI_DBG("Drop L2 glsmux reset via QPPM_EXCGCR[32:33]");

    if (l_attr_dd1_vcs_workaround)
    {
        FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_CLEAR, MASK_CLR(32, 2, 3)));
    }
    else
    {
        FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_CLEAR, l_l2gmux_reset));
    }

    FAPI_TRY(fapi2::delay(
                 CACHE_GLSMUX_RESET_DELAY_REF_CYCLES * p9hcd::CLK_PERIOD_10NS,
                 CACHE_GLSMUX_RESET_DELAY_REF_CYCLES * p9hcd::SIM_CYCLE_200UD));

    FAPI_DBG("Assert chiplet enable via NET_CTRL0[0]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_SET(0)));

    FAPI_DBG("Drop PCB endpoint reset via NET_CTRL0[1]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(1)));

    FAPI_DBG("Drop chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(26)));

    FAPI_DBG("Drop PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(25)));

    FAPI_DBG("Assert sram_enable via NET_CTRL0[23]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_SET(23)));

    FAPI_DBG("Set scan ratio to 1:1 in bypass mode via OPCG_ALIGN[47-51]");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<47, 5>(0x0);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

#ifndef P9_HCD_STOP_SKIP_FLUSH
    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    // Putting in block to avoid c++ crosses initialization compile error
    {
        uint32_t l_loop;

        FAPI_DBG("Scan0 region:all_but_vital type:gptr_repr_time rings");

        if (l_attr_dd1_vcs_workaround)
        {
            for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
                FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                      p9hcd::SCAN0_REGION_ALL,
                                                      p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));

            FAPI_DBG("Enable DD1 VCS workaround");
            FAPI_TRY(p9_hcd_dd1_vcs_workaround(i_target));
        }
        else
        {
            for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
                FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                      l_region_scan0,
                                                      p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));
        }

        FAPI_DBG("Scan0 region:all_but_vital type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  l_region_scan0,
                                                  p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }
#endif

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_chiplet_reset");
    return fapi2::current_err;
}




// ----------------------------------------------------
// HW388878
// ----------------------------------------------------
fapi2::ReturnCode
p9_hcd_dd1_vcs_workaround(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_dd1_vcs_workaround");
    fapi2::buffer<uint64_t>                     l_data64;
    uint64_t                                    l_regions;
    uint32_t                                    l_timeout;
    uint32_t                                    l_poll_loops;
    uint32_t                                    l_loop;

#ifndef __PPE__
    //Skip the entire work-around in simulation to save time.
    //Only flushing part of the rings to 1's causes initR issues
    //because the fences are being dropped at the end of the workaround.
    fapi2::buffer<uint8_t>                      l_attr_is_simulation;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_SIMULATION, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                           l_attr_is_simulation));

    if (!l_attr_is_simulation)
    {

#endif

        l_regions = p9hcd::CLK_REGION_PERV   |
                    p9hcd::CLK_REGION_EX0_L2 |
                    p9hcd::CLK_REGION_EX1_L2;

        // ----------------------------------------------------
        // Scan1 initialize region:Perv/L20/L21 type:Fure rings
        // Note: must also scan partial good "bad" L2 rings,
        // and clock start&stop their latches, as well
        // ----------------------------------------------------

        FAPI_DBG("Assert Vital clock regional fence via CPLT_CTRL1[3]");
        FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_OR, MASK_SET(3)));

        FAPI_DBG("Assert regional fences of scanned regions via CPLT_CTRL1[4,8,9]");
        FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_OR, l_regions));

        FAPI_DBG("Clear clock region register via CLK_REGION");
        FAPI_TRY(putScom(i_target, EQ_CLK_REGION, MASK_ZERO));

        FAPI_DBG("Setup scan select register via SCAN_REGION_TYPE[4,8,9,48,51]");
        FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE,
                         (l_regions | p9hcd::SCAN_TYPE_FUNC | p9hcd::SCAN_TYPE_REGF)));

        FAPI_DBG("Write scan data register via 0x1003E040");


        for (l_loop = 0; l_loop <= DD1_EQ_FURE_RING_LENGTH / 64; l_loop++)
        {
            FAPI_DBG("Loop Count: %d", l_loop);
            FAPI_TRY(putScom(i_target, 0x1003E040, MASK_ALL));
        }


        // -------------------------------
        // Start Perv/L20/L21 clocks
        // -------------------------------

        FAPI_DBG("Clear all SCAN_REGION_TYPE bits");
        FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

        FAPI_DBG("Start cache clocks(perv/l20/l21) via CLK_REGION");
        l_data64 = (p9hcd::CLK_START_CMD | l_regions | p9hcd::CLK_THOLD_ARY);
        FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

        FAPI_DBG("Poll for perv/l20/l21 clocks running via CPLT_STAT0[8]");
        l_poll_loops = CACHE_CLK_START_POLL_TIMEOUT_HW_NS /
                       CACHE_CLK_START_POLL_DELAY_HW_NS;

        do
        {
            fapi2::delay(CACHE_CLK_START_POLL_DELAY_HW_NS,
                         CACHE_CLK_START_POLL_DELAY_SIM_CYCLE);

            FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
        }
        while((l_data64.getBit<8>() != 1) && ((--l_poll_loops) != 0));

        FAPI_ASSERT((l_poll_loops != 0),
                    fapi2::CACHE_CLK_START_TIMEOUT()
                    .set_EQ_CPLT_STAT(l_data64)
                    .set_CACHE_CLK_START_POLL_DELAY_HW_NS(CACHE_CLK_START_POLL_DELAY_HW_NS)
                    .set_CACHE_CLK_START_POLL_TIMEOUT_HW_NS(CACHE_CLK_START_POLL_TIMEOUT_HW_NS)
                    .set_CACHE_TARGET(i_target),
                    "perv/l20/l21 Clock Start Timeout");

        FAPI_DBG("Check perv/l20/l21 clocks running");
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_ARY, l_data64));

        FAPI_ASSERT(((l_data64 & l_regions) == 0),
                    fapi2::CACHE_CLK_START_FAILED()
                    .set_EQ_CLK_STAT(l_data64)
                    .set_CACHE_TARGET(i_target),
                    "perv/l20/l21 Clock Start Failed");
        FAPI_DBG("perv/l20/l21 clocks running now");

        // -------------------------------
        // Turn on power headers for VCS
        // -------------------------------

        FAPI_TRY(p9_common_poweronoff<fapi2::TARGET_TYPE_EQ>(i_target, p9power::POWER_ON_VCS));

        // Because common module raises those fences, we need to lower them here.
        FAPI_DBG("Drop vital thold via NET_CTRL0[16]");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(16)));

        FAPI_DBG("Drop chiplet electrical fence via NET_CTRL0[26]");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(26)));

        FAPI_DBG("Drop PCB fence via NET_CTRL0[25]");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(25)));

        // -------------------------------
        // Stop Perv/L20/L21 clocks
        // -------------------------------

        FAPI_DBG("Clear all SCAN_REGION_TYPE bits");
        FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

        FAPI_DBG("Stop perv/l20/l21 clocks via CLK_REGION");
        l_data64 = (p9hcd::CLK_STOP_CMD | l_regions | p9hcd::CLK_THOLD_ARY);
        FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

        FAPI_DBG("Poll for perv/l20/l21 clocks stopped via CPLT_STAT0[8]");
        l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP);

        do
        {
            FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
        }
        while((l_data64.getBit<8>() != 1) && ((--l_timeout) != 0));

        FAPI_ASSERT((l_timeout != 0),
                    fapi2::PMPROC_CACHECLKSTOP_TIMEOUT().set_EQCPLTSTAT(l_data64),
                    "perv/l20/l21 Clock Stop Timeout");

        FAPI_DBG("Check perv/l20/l21 clocks stopped");
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_ARY, l_data64));

        FAPI_ASSERT((((~l_data64) & l_regions) == 0),
                    fapi2::PMPROC_CACHECLKSTOP_FAILED().set_EQCLKSTAT(l_data64),
                    "perv/l20/l21 Clock Stop Failed");
        FAPI_DBG("perv/l20/l21 clocks stopped now");

        // -------------------------------
        // Clean up
        // -------------------------------

        FAPI_DBG("Drop Vital clock regional fence via CPLT_CTRL1[3]");
        FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(3)));

        FAPI_DBG("Drop Perv/L20/L21 regional fences via CPLT_CTRL1[4,8,9]");
        FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, l_regions));

#ifndef __PPE__
    }

#endif

fapi_try_exit:

    FAPI_INF("<<p9_hcd_dd1_vcs_workaround");
    return fapi2::current_err;
}

