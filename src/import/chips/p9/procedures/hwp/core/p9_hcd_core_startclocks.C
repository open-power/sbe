/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/core/p9_hcd_core_startclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file  p9_hcd_core_startclocks.C
/// @brief Core Clock Start
///
/// Procedure Summary:
///   (Done) Drop partial good regional fences(always drop vital and pervasive)
///   (Done) Drop vital fence
///   (Done) Reset abst clock muxsel, sync muxsel
///   (TODO) +set fabric node/chip ID (read from nest chiplet)
///   (Done) module align_chiplets
///   (Done)  - set flushmode_inh to exit flush mode
///   (Done)  - set force_align
///   (Done)  - set clear_chiplet_is_aligned
///   (Done)  - unset clear_chiplet_is_aligned
///   (Done)  - wait
///   (Done)  - check chiplet_is_aligned
///   (Done)  - clear force_align
///   (Done) module start_clocks
///   (Done)  - set flush mode(alerady set in align_chiplets)
///   (Done)  - Clear scan region type register
///   (Done)  - Start arrays + nsl regions
///   (Done)  - Start sl + refresh clock regions
///   (Done) Check for clocks started, If not, error
///   (Done) Drop the core to cache logical fence
///   (Done) Check for core xstop, If so, error
///   (Done) Clear flushmode_inh to go into flush mode
///   (Done) Check cache/core chiplet_is_aligned

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include "p9_hcd_core_startclocks.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P9_HCD_CORE_STARTCLOCKS_CONSTANTS
{
    CORE_CLK_SYNC_TIMEOUT_IN_MS       = 1,
    CORE_CLK_START_TIMEOUT_IN_MS      = 1,
    CORE_CLK_ALIGN_DELAY_CACHE_CYCLES = 255
};

//------------------------------------------------------------------------------
// Procedure: Core Clock Start
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_startclocks(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_startclocks");
    fapi2::buffer<uint64_t>                     l_data64;
    uint32_t                                    l_timeout;
    uint32_t                                    l_attr_pg;
    uint8_t                                     l_attr_chip_unit_pos;
    uint8_t                                     l_attr_system_ipl_phase;
    uint8_t                                     l_attr_runn_mode;
    fapi2::Target<fapi2::TARGET_TYPE_EQ>        l_quad =
        i_target.getParent<fapi2::TARGET_TYPE_EQ>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_MODE,        l_sys,
                           l_attr_runn_mode));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, l_sys,
                           l_attr_system_ipl_phase));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG,               l_perv,
                           l_attr_pg));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,    l_perv,
                           l_attr_chip_unit_pos));
    l_attr_chip_unit_pos = (l_attr_chip_unit_pos -
                            p9hcd::PERV_TO_CORE_POS_OFFSET) % 4;

    // ----------------------------
    // Prepare to start core clocks
    // ----------------------------

    if (l_attr_system_ipl_phase ==
        fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
    {
        FAPI_DBG("Set CPLT_CTRL0[AVP_MODE] for cache-contained execution");
        FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_OR, MASK_SET(5)));
    }

    /// @todo add DD1 attribute control
    FAPI_DBG("DD1 only: set sdis_n(flushing LCBES condition workaround");
    FAPI_TRY(putScom(i_target, C_CPLT_CONF0_OR, MASK_SET(34)));

    FAPI_DBG("Set inop_align/wait/wait_cycles via OPCG_ALIGN[0-3,12-19,52-63]");
    FAPI_TRY(getScom(i_target, C_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<0, 4>(0x5).
    insertFromRight<12, 8>(0x0).
    insertFromRight<52, 12>(0x10);
    FAPI_TRY(putScom(i_target, C_OPCG_ALIGN, l_data64));

    /// @todo partial good information via attribute, drop all fences for now
    FAPI_DBG("Drop partial good fences via CPLT_CTRL1[4-13]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL1_CLEAR, MASK_CLR(4, 11, 0x7FF)));

    FAPI_DBG("Drop vital fence via CPLT_CTRL1[3]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL1_CLEAR, MASK_SET(3)));

    FAPI_DBG("Drop skew sense to skew adjust fence via NET_CTRL0[22]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(22)));

    FAPI_DBG("Assert core clock sync enable via CPPM_CACCR[15]");
    FAPI_TRY(putScom(i_target, C_CPPM_CACCR_OR, MASK_SET(15)));

    FAPI_DBG("Poll for core clock sync done via CPPM_CACSR[13]");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CORE_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, C_CPPM_CACSR, l_data64));
    }
    while((l_data64.getBit<13>() != 1) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CORECLKSYNC_TIMEOUT().set_COREPPMCACSR(l_data64),
                "Core Clock Sync Timeout");
    FAPI_DBG("Core clock sync done");

    FAPI_DBG("Reset abstclk & syncclk muxsel(io_clk_sel) via CPLT_CTRL0[0:1]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_CLEAR, MASK_CLR(0, 2, 3)));

    // -------------------------------
    // Align chiplets
    // -------------------------------

    FAPI_DBG("Assert flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_OR, MASK_SET(2)));

    FAPI_DBG("Assert force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_OR, MASK_SET(3)));

    FAPI_DBG("Set then unset clear_chiplet_is_aligned via SYNC_CONFIG[7]");
    FAPI_TRY(getScom(i_target, C_SYNC_CONFIG, l_data64));
    FAPI_TRY(putScom(i_target, C_SYNC_CONFIG, DATA_SET(7)));
    FAPI_TRY(putScom(i_target, C_SYNC_CONFIG, DATA_UNSET(7)));

    FAPI_TRY(fapi2::delay(
                 CORE_CLK_ALIGN_DELAY_CACHE_CYCLES * p9hcd::CLK_PERIOD_CORE2CACHE *
                 p9hcd::CLK_PERIOD_250PS / 1000,
                 CORE_CLK_ALIGN_DELAY_CACHE_CYCLES * p9hcd::CLK_PERIOD_CORE2CACHE *
                 p9hcd::SIM_CYCLE_4U4D));

    FAPI_DBG("Poll for core chiplet aligned");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CORE_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, C_CPLT_STAT0, l_data64));
    }
    while((l_data64.getBit<9>() != 1) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CORECPLTALIGN_TIMEOUT()
                .set_CORECPLTSTAT0(l_data64),
                "Core Chiplets Aligned Timeout");
    FAPI_DBG("Core chiplets aligned now");

    FAPI_DBG("Drop force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_CLEAR, MASK_SET(3)));

    // -------------------------------
    // Start core clocks
    // -------------------------------

    FAPI_DBG("Clear all bits prior start core clocks via SCAN_REGION_TYPE");
    FAPI_TRY(putScom(i_target, C_SCAN_REGION_TYPE, MASK_ZERO));

    if (!l_attr_runn_mode)
    {

        FAPI_DBG("Start core clocks(all but pll) via CLK_REGION");
        l_data64 = (p9hcd::CLK_START_CMD          |
                    p9hcd::CLK_REGION_ALL_BUT_PLL |
                    p9hcd::CLK_THOLD_ALL);
        FAPI_TRY(putScom(i_target, C_CLK_REGION, l_data64));

        FAPI_DBG("Poll for core clocks running via CPLT_STAT0[8]");
        l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                    CORE_CLK_START_TIMEOUT_IN_MS;

        do
        {
            FAPI_TRY(getScom(i_target, C_CPLT_STAT0, l_data64));
        }
        while((l_data64.getBit<8>() != 1) && ((--l_timeout) != 0));

        FAPI_ASSERT((l_timeout != 0),
                    fapi2::PMPROC_CORECLKSTART_TIMEOUT().set_CORECPLTSTAT(l_data64),
                    "Core Clock Start Timeout");

        FAPI_DBG("Check core clocks running via CLOCK_STAT_SL[4-13]");
        FAPI_TRY(getScom(i_target, C_CLOCK_STAT_SL, l_data64));

        FAPI_ASSERT(((l_data64 & p9hcd::CLK_REGION_ALL_BUT_PLL) == 0),
                    fapi2::PMPROC_CORECLKSTART_FAILED().set_CORECLKSTAT(l_data64),
                    "Core Clock Start Failed");
        FAPI_DBG("Core clocks running now");

    }

    // -------------------------------
    // Cleaning up
    // -------------------------------

    if ((~l_attr_pg) & BITS32(4, 11))
    {
        FAPI_DBG("Drop chiplet fence via NET_CTRL0[18]");
        FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(18)));
    }

    /// @todo ignore xstop checkstop in sim, review for lab
    /*
    FAPI_DBG("Check the Global Checkstop FIR");
    FAPI_TRY(getScom(i_target, C_XFIR, l_data64));
    FAPI_ASSERT(((l_data64 & BITS64(0, 27)) != 0),
                fapi2::PMPROC_CORE_XSTOP().set_COREXFIR(l_data64),
                "Core Chiplet Checkstop");
    */

    FAPI_DBG("Drop flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_CLEAR, MASK_SET(2)));

    if (!l_attr_runn_mode && l_attr_system_ipl_phase !=
        fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
    {
        FAPI_DBG("Drop Core-L2/CC Quiesces via CME_SCOM_SICR[6,8]/[7,9]");
        FAPI_TRY(putScom(l_quad,
                         (l_attr_chip_unit_pos < 2) ?
                         EX_0_CME_SCOM_SICR_CLEAR : EX_1_CME_SCOM_SICR_CLEAR,
                         (BIT64(6 + (l_attr_chip_unit_pos % 2)) |
                          BIT64(8 + (l_attr_chip_unit_pos % 2)))));
    }

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_startclocks");
    return fapi2::current_err;
}




