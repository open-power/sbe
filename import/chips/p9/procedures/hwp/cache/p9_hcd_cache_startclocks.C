/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_startclocks.C $    */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
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
///   (Done) Setup L3 EDRAM/LCO
///   (Done) Setup OPCG_ALIGN
///   (Done) Drop partial good regional fences(always drop vital and pervasive)
///   (Done) Drop Vital fence
///   (Done) Reset abst clock muxsel, sync muxsel
///   (TODO) Set fabric node/chip ID from the nest version
///   (Done) module align_chiplets
///   (Done)  - set flushmode_inh to exit flush mode
///   (Done)  - set force align
///   (Done)  - set chiplet_is_aligned
///   (Done)  - clear chiplet_is_aligned
///   (Done)  - wait
///   (Done)  - check chiplet is aligned
///   (Done)  - clear force align
///   (Done) module start_clocks
///   (Done)  - Clear clock controller scan register before start
///   (Done)  - Start arrays + nsl regions
///   (Done)  - Start sl + refresh clock regions
///   (Done) Check for clocks started. If not, error
///   (Done) Drop the cache to PowerBus logical fence
///   (Done) Check for cache xstop, If so, error
///   (Done) Clear flushmode_inh to go into flush mode

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

    FAPI_DBG("Enable L3 EDRAM/LCO setup on both EXs");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_OR(23, 2, 0x3)));
    // 0x0 -> 0x8 -> 0xC -> 0xE -> 0xF to turn on edram
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(0)));
    FAPI_TRY(fapi2::delay(12000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(1)));
    FAPI_TRY(fapi2::delay(1000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(2)));
    FAPI_TRY(fapi2::delay(4000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(3)));
    FAPI_TRY(fapi2::delay(1000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(4)));
    FAPI_TRY(fapi2::delay(12000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(5)));
    FAPI_TRY(fapi2::delay(1000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(6)));
    FAPI_TRY(fapi2::delay(4000, 200));
    FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(7)));
    FAPI_TRY(fapi2::delay(1000, 200));

    FAPI_DBG("Setup OPCG_ALIGN Register");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<0, 4>(0x5).
    insertFromRight<12, 8>(0x0).
    insertFromRight<52, 12>(0x10);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

    /// @todo partial good information via attribute, drop all fences for now
    FAPI_DBG("Drop partial good fence via CPLT_CTRL1");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, 0xFFFF700000000000));

    FAPI_DBG("Drop vital fence via CPLT_CTRL1[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(3)));

    FAPI_DBG("Reset abstclk & syncclk muxsel(io_clk_sel) via CPLT_CTRL0[0:1]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_CLR(0, 2, 3)));

    /// @todo set fabric node/chip ID values(read from nest chiplet)
    // FAPI_DBG("setup fabric group/unit/sys ids");
    // FAPI_TRY(putScom(i_target, EQ_CPLT_CONF0, <attributes>))

    // -------------------------------
    // Align Chiplets
    // -------------------------------

    FAPI_DBG("Set flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(2)));

    FAPI_DBG("Set force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(3)));

    /// @todo wait for how long
    /*
        FAPI_DBG("Poll for cache chiplet aligned");
        l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                    CACHE_CLK_START_TIMEOUT_IN_MS;

        do
        {
            FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
        }
        while(((l_data64 & BIT64(9)) != 0) && ((--l_timeout) != 0));

        FAPI_ASSERT((l_timeout != 0),
                    fapi2::PMPROC_CACHECPLTALIGN_TIMEOUT()
                    .set_EQCPLTSTAT0(l_data64),
                    "Cache Chiplets Aligned Timeout");
        FAPI_DBG("Cache chiplets aligned now");
    */
    FAPI_DBG("Clear force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(3)));

    // -------------------------------
    // Start Cache Clock
    // -------------------------------

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

    FAPI_DBG("Poll for L3 clock running");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_START_TIMEOUT_IN_MS;

    // Read Clock Status Register (Cache chiplet)
    // check for bits 4:14 eq. zero, no tholds on
    do
    {
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & (BITS64(4, 4) | BITS64(10, 4))) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSTART_TIMEOUT().set_EQCLKSTAT(l_data64),
                "Cache Clock Start Timeout");
    FAPI_DBG("Cache clock running now");

    // -------------------------------
    // Start L2 Clock
    // -------------------------------

    FAPI_DBG("Raise L2 clock sync enable");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, MASK_OR(36, 2, 3)));

    FAPI_DBG("Poll for clock sync done to raise on EX L2s");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_SYNC_TIMEOUT_IN_MS;
    /*
    do
    {
        FAPI_TRY(getScom(i_target, EQ_QPPM_QACSR, l_data64));
    }
    while(((l_data64 & 0x3) != 3) && ((--l_timeout) != 0));
    */
    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSYNC_TIMEOUT().set_EQPPMQACSR(l_data64),
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

    FAPI_DBG("Poll for L2 clock running");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_START_TIMEOUT_IN_MS;

    // Read Clock Status Register (Cache chiplet)
    // check for bits 4:14 eq. zero, no tholds on
    do
    {
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & BITS64(8, 2)) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSTART_TIMEOUT().set_EQCLKSTAT(l_data64),
                "L2 Clock Start Timeout");
    FAPI_DBG("L2 clock running now");

    // -------------------------------
    // Cleaning up
    // -------------------------------

    FAPI_DBG("Drop chiplet fence via NET_CTRL0[18]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(18)));

    /*
    FAPI_DBG("Check the Global Checkstop FIR");
    FAPI_TRY(getScom(i_target, EQ_XFIR, l_data64));
    FAPI_ASSERT(((l_data64 & BITS64(0, 27)) != 0),
                fapi2::PMPROC_CACHE_XSTOP().set_EQXFIR(l_data64),
                "Cache Chiplet Checkstop");
    */
    FAPI_DBG("Clear flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(2)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_startclocks");

    return fapi2::current_err;
}






