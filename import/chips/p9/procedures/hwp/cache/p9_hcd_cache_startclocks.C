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

#include <p9_misc_scom_addresses.H>
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
    FAPI_INF(">>p9_hcd_cache_startclocks");
    fapi2::buffer<uint64_t>                     l_qcsr;
    fapi2::buffer<uint64_t>                     l_data64;
    uint64_t                                    l_l2sync_clock;
    uint64_t                                    l_region_clock;
    uint32_t                                    l_timeout;
    uint32_t                                    l_attr_system_id     = 0;
    uint8_t                                     l_attr_group_id      = 0;
    uint8_t                                     l_attr_chip_id       = 0;
    uint8_t                                     l_attr_chip_unit_pos = 0;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID,  l_chip,
                           l_attr_group_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID,   l_chip,
                           l_attr_chip_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_SYSTEM_ID, l_chip,
                           l_attr_system_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,         l_perv,
                           l_attr_chip_unit_pos));
    l_attr_chip_unit_pos = l_attr_chip_unit_pos - p9hcd::PERV_TO_EQ_POS_OFFSET;

    FAPI_TRY(getScom(l_chip, PU_OCB_OCI_QCSR_SCOM, l_qcsr));
    FAPI_DBG("Working on cache[%d], good EXs in QCSR[%016llX]",
             l_attr_chip_unit_pos, l_qcsr);

    // -----------------------------
    // Prepare to start cache clocks
    // -----------------------------

    l_region_clock = p9hcd::CLK_REGION_ALL_BUT_EX_DPLL;
    l_l2sync_clock = 0;

    if (l_qcsr & BIT64(l_attr_chip_unit_pos << 1))
    {
        l_region_clock |= p9hcd::CLK_REGION_EX0_L2_L3_REFR;
        l_l2sync_clock |= BIT64(36);
        FAPI_DBG("Sequence EX0 EDRAM enables via QPPM_QCCR[0-3]");
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(0)));
        FAPI_TRY(fapi2::delay(12000, 200));
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(1)));
        FAPI_TRY(fapi2::delay(1000, 200));
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(2)));
        FAPI_TRY(fapi2::delay(4000, 200));
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(3)));
        FAPI_TRY(fapi2::delay(1000, 200));
    }

    if (l_qcsr & BIT64((l_attr_chip_unit_pos << 1) + 1))
    {
        l_region_clock |= p9hcd::CLK_REGION_EX1_L2_L3_REFR;
        l_l2sync_clock |= BIT64(37);
        FAPI_DBG("Sequence EX1 EDRAM enables via QPPM_QCCR[4-7]");
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(4)));
        FAPI_TRY(fapi2::delay(12000, 200));
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(5)));
        FAPI_TRY(fapi2::delay(1000, 200));
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(6)));
        FAPI_TRY(fapi2::delay(4000, 200));
        FAPI_TRY(putScom(i_target, EQ_QPPM_QCCR_WOR, MASK_SET(7)));
        FAPI_TRY(fapi2::delay(1000, 200));
    }

    FAPI_DBG("Set inop_align/wait/wait_cycles via OPCG_ALIGN[0-3,12-19,52-63]");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<0, 4>(0x5).
    insertFromRight<12, 8>(0x0).
    insertFromRight<52, 12>(0x10);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

    FAPI_DBG("Drop partial good fences via CPLT_CTRL1[3-14]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, l_region_clock));

    FAPI_DBG("Drop vital fence via CPLT_CTRL1[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(3)));

    FAPI_DBG("Assert EX-L2 clock sync enables via QPPM_EXCGCR[36,37]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, l_l2sync_clock));

    FAPI_DBG("Poll for EX-L2 clock sync dones via QPPM_QACSR[36,37]");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_SYNC_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_QPPM_QACSR, l_data64));
    }
    while(((l_data64 & l_l2sync_clock) != l_l2sync_clock) &&
          ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSYNC_TIMEOUT().set_EQPPMQACSR(l_data64),
                "EX-L2 Clock Sync Timeout");
    FAPI_DBG("EX-L2 clock sync done");

    FAPI_DBG("Reset abstclk & syncclk muxsel(io_clk_sel) via CPLT_CTRL0[0:1]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_CLR(0, 2, 3)));

    FAPI_DBG("Set fabric group ID[%x] chip ID[%x] system ID[%x]",
             l_attr_group_id, l_attr_chip_id, l_attr_system_id);
    FAPI_TRY(getScom(i_target, EQ_CPLT_CONF0, l_data64));
    l_data64.insertFromRight<48, 4>(l_attr_group_id).
    insertFromRight<52, 3>(l_attr_chip_id).
    insertFromRight<56, 5>(l_attr_system_id);
    FAPI_TRY(putScom(i_target, EQ_CPLT_CONF0, l_data64));

    // -------------------------------
    // Align chiplets
    // -------------------------------

    FAPI_DBG("Assert flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(2)));

    FAPI_DBG("Assert force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(3)));

    FAPI_DBG("Set then unset clear_chiplet_is_aligned via SYNC_CONFIG[7]");
    FAPI_TRY(getScom(i_target, EQ_SYNC_CONFIG, l_data64));
    FAPI_TRY(putScom(i_target, EQ_SYNC_CONFIG, DATA_SET(7)));
    FAPI_TRY(putScom(i_target, EQ_SYNC_CONFIG, DATA_UNSET(7)));

    FAPI_DBG("Poll for cache chiplet aligned");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
    }
    while((l_data64.getBit<9>() != 1) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECPLTALIGN_TIMEOUT()
                .set_EQCPLTSTAT0(l_data64),
                "Cache Chiplets Aligned Timeout");
    FAPI_DBG("Cache chiplets aligned now");

    FAPI_DBG("Drop force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(3)));

    FAPI_TRY(fapi2::delay(0, 900));

    // -------------------------------
    // Start cache clocks
    // -------------------------------

    FAPI_DBG("Clear all bits prior start cache clocks via SCAN_REGION_TYPE");
    FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

    FAPI_DBG("Start cache clocks(arrays+nsl clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_NONE_THOLD_NSL_ARY | l_region_clock;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    /// @todo parameterize delay
    FAPI_TRY(fapi2::delay(0, 1000000));

    FAPI_DBG("Start cache clocks(sl+refresh clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_NONE_THOLD_ALL | l_region_clock;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    FAPI_DBG("Poll for cache clocks running via CLOCK_STAT_SL[4-14]");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & l_region_clock) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CACHECLKSTART_TIMEOUT().set_EQCLKSTAT(l_data64),
                "Cache Clock Start Timeout");
    FAPI_DBG("Cache clocks running now");

    // -------------------------------
    // Cleaning up
    // -------------------------------

    FAPI_DBG("Drop chiplet fence via NET_CTRL0[18]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(18)));

    /// @todo ignore xstop checkstop in sim, review for lab
    /*
    FAPI_DBG("Check the Global Checkstop FIR");
    FAPI_TRY(getScom(i_target, EQ_XFIR, l_data64));
    FAPI_ASSERT(((l_data64 & BITS64(0, 27)) != 0),
                fapi2::PMPROC_CACHE_XSTOP().set_EQXFIR(l_data64),
                "Cache Chiplet Checkstop");
    */

    FAPI_DBG("Drop flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(2)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_startclocks");
    return fapi2::current_err;
}

