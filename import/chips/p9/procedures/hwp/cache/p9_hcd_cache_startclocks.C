/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_startclocks.C $    */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
///
/// @file  p9_hcd_cache_startclocks.C
/// @brief Quad Clock Start
///
/// Procedure Summary:
///   Set (to be sure they are set under all conditions) core logical fences
///   (new for P9)
///   Drop pervasive thold
///   (Done) Setup L3 EDRAM/LCO
///   Drop pervasive fence
///   (Done) Reset abst clock muxsel, sync muxsel
///   (Done) Set fabric node/chip ID from the nest version
///   (Done) Clear clock controller scan register before start
///   (Done) Start arrays + nsl regions
///   (Done) Start sl + refresh clock regions
///   (Done) Check for clocks started
///   (Done) If not, error
///   (Done) Clear force align
///   (Done) Clear flush mode
///   (Done) Drop the chiplet fence to allow PowerBus traffic
///

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_startclocks.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P9_HCD_CACHE_STARTCLOCKS_CONSTANTS
{
    CACHE_CLK_SYNC_TIMEOUT_IN_MS = 1,
    CACHE_CLK_START_TIMEOUT_IN_MS = 1
};

//------------------------------------------------------------------------------
// Procedure: Quad Clock Start
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_startclocks(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    fapi2::buffer<uint64_t> l_data64;
    uint32_t                l_timeout;

    FAPI_INF(">>p9_hcd_cache_startclocks");

    // -------------------------------
    // Prepare to cache startclocks
    // -------------------------------

    /// @todo Drop the Pervasive THOLD, was in p8 code, where in p9?

    FAPI_DBG("Enable L3 EDRAM/LCO setup on both EXs");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_OR(23, 2, 0x3)));
    // 0x0 -> 0x8 -> 0xC -> 0xE -> 0xF to turn on edram
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_OR(0, 8, 0x88)));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_OR(0, 8, 0xCC)));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_OR(0, 8, 0xEE)));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_OR(0, 8, 0xFF)));

    /// @todo get next step from perv, but not in p8 code, necessary?
    FAPI_DBG("Drop Vital Fence via CPLT_CTRL1[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(3)));

    FAPI_DBG("Reset abstclk & syncclk muxsel(io_clk_sel) via CPLT_CTRL0[0:1]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_CLR(0, 2, 3)));

    FAPI_DBG("Set abist_mode_dc for cache chiplet(cache recovery) via BIST[1]");
    FAPI_TRY(getScom(i_target, EQ_BIST, l_data64));
    FAPI_TRY(putScom(i_target, EQ_BIST, DATA_SET(1)));

    /// @todo set fabric node/chip ID values(read from nest chiplet) still need?
    /// @todo force chiplet out of flush?

    // -------------------------------
    // Start L3 Clock
    // -------------------------------

    // @todo put this into dpll_setup?
    //FAPI_DBG("Switch L3 glsmux to DPLL output");
    //FAPI_TRY(putScom(i_target, EQ_PPM_CGCR_OR, MASK_SET(3)));

    FAPI_DBG("Set all bits to zero prior clock start via SCAN_REGION_TYPE");
    FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

    FAPI_DBG("Start clock(arrays+nsl clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_ALL_BUT_DPLL_L2_THOLD_NSL_ARY;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    /// @todo parameterize delay
    FAPI_TRY(fapi2::delay(0, 1000000));

    FAPI_DBG("Start clock(sl+refresh clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_ALL_BUT_DPLL_L2_THOLD_ALL;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    // Read Clock Status Register (Cache chiplet)
    // check for bits 4:14 eq. zero, no tholds on
    FAPI_DBG("Poll for L3 clock running");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & BITS64(4, 11)) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSTART_TIMEOUT()
                .set_EQCLKSTATREGIONS(l_data64),
                "L3 Clock Start Timeout");
    FAPI_DBG("L3 clock running now");

    // -------------------------------
    // Start L2 Clock
    // -------------------------------

    FAPI_DBG("Switch L2 glsmux to DPLL output");
    FAPI_TRY(putScom(i_target, EQ_QPPM_QACCR_SCOM2, MASK_SET(19)));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QACCR_SCOM2, MASK_SET(39)));

    FAPI_DBG("Raise L2 clock sync enable");
    FAPI_TRY(putScom(i_target, EQ_QPPM_QACCR_SCOM2, MASK_SET(13)));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QACCR_SCOM2, MASK_SET(33)));

    FAPI_DBG("Poll for clock sync done to raise on EX L2s");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_SYNC_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_QPPM_QACSR, l_data64));
    }
    while(((l_data64 & 0x3) != 3) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSYNC_TIMEOUT()
                .set_EXL2CLKSYNCDONE(l_data64),
                "L2 Clock Sync Timeout");
    FAPI_DBG("EX L2s clock sync done");

    FAPI_DBG("Start clock(arrays+nsl clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_L2_THOLD_NSL_ARY;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    /// @todo parameterize delay
    FAPI_TRY(fapi2::delay(0, 1000000));

    FAPI_DBG("Start clock(sl+refresh clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_L2_THOLD_ALL;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    // Read Clock Status Register (Cache chiplet)
    // check for bits 4:14 eq. zero, no tholds on
    FAPI_DBG("Poll for L2 clock running");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & BITS64(4, 11)) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSTART_TIMEOUT()
                .set_EQCLKSTATREGIONS(l_data64),
                "L2 Clock Start Timeout");
    FAPI_DBG("L2 clock running now");

    // -------------------------------
    // Cleaning up
    // -------------------------------

    /// @todo Check the Global Checkstop FIR of dedicated EX chiplet
    /// @todo Ben's workaround at model e9025, move clear align/flush to end

    /// @todo what are the remaining fences to drop?
    FAPI_DBG("Drop remaining fences via CPLT_CTRL1");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, 0xEFFF700000000000));

    FAPI_DBG("Drop chiplet fence via NET_CTRL0[18]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(18)));

    FAPI_DBG("Drop fence to allow PCB operations to chiplet via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(25)));

    FAPI_DBG("Clear force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(3)));

    FAPI_DBG("Clear flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(2)));

    FAPI_INF("<<p9_hcd_cache_startclocks");

fapi_try_exit:
    return fapi2::current_err;
}






