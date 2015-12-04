/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_startclocks.C $      */
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
    CORE_CLK_SYNC_TIMEOUT_IN_MS = 1,
    CORE_CLK_START_TIMEOUT_IN_MS = 1
};

//------------------------------------------------------------------------------
// Procedure: Core Clock Start
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_startclocks(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    fapi2::buffer<uint64_t> l_data64;
    uint32_t                l_timeout;

    FAPI_INF(">>p9_hcd_core_startclocks");

    // -------------------------------
    // Prepare to core startclocks
    // -------------------------------

    FAPI_DBG("Setup OPCG_ALIGN Register");
    FAPI_TRY(getScom(i_target, C_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<0, 4>(0x5).
    insertFromRight<12, 8>(0x0).
    insertFromRight<52, 12>(0x10);
    FAPI_TRY(putScom(i_target, C_OPCG_ALIGN, l_data64));

    /// @todo partial good information via attribute, drop all fences for now
    FAPI_DBG("Drop partial good fences via CPLT_CTRL1");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL1_CLEAR, 0xFFFF700000000000));

    FAPI_DBG("Drop vital fence via CPLT_CTRL1[3]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL1_CLEAR, MASK_SET(3)));

    FAPI_DBG("Raise core clock sync enable");
    FAPI_TRY(putScom(i_target, C_CPPM_CACCR_OR, MASK_SET(15)));

    FAPI_DBG("Poll for core clock sync done to raise");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CORE_CLK_START_TIMEOUT_IN_MS;
    /*
    do
    {
        FAPI_TRY(getScom(i_target, C_CPPM_CACSR, l_data64));
    }
    while((l_data64.getBit<13>() != 1) && ((--l_timeout) != 0));
    */
    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CORECLKSYNC_TIMEOUT().set_COREPPMCACSR(l_data64),
                "Core Clock Sync Timeout");
    FAPI_DBG("Core clock sync done");

    FAPI_DBG("Reset abstclk & syncclk muxsel(io_clk_sel) via CPLT_CTRL0[0:1]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_CLEAR, MASK_CLR(0, 2, 3)));

    /// @todo set fabric node/chip ID values (read from nest chiplet)

    // -------------------------------
    // Align Core Chiplets
    // -------------------------------

    FAPI_DBG("Set flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_OR, MASK_SET(2)));

    FAPI_DBG("Set force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_OR, MASK_SET(3)));

    FAPI_DBG("Set/Unset clear_chiplet_is_aligned via SYNC_CONFIG[7]");
    FAPI_TRY(getScom(i_target, C_SYNC_CONFIG, l_data64));
    FAPI_TRY(putScom(i_target, C_SYNC_CONFIG, DATA_SET(7)));
    FAPI_TRY(putScom(i_target, C_SYNC_CONFIG, DATA_UNSET(7)));

    /// @todo wait for how long
    /*
        FAPI_DBG("Poll for core chiplet aligned");
        l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                    CORE_CLK_START_TIMEOUT_IN_MS;

        do
        {
            FAPI_TRY(getScom(i_target, C_CPLT_STAT0, l_data64));
        }
        while(((l_data64 & BIT64(9)) != 0) && ((--l_timeout) != 0));

        FAPI_ASSERT((l_timeout != 0),
                    fapi2::PMPROC_CORECPLTALIGN_TIMEOUT()
                    .set_CORECPLTSTAT0(l_data64),
                    "Core Chiplets Aligned Timeout");
        FAPI_DBG("Core chiplets aligned now");
    */
    FAPI_DBG("Clear force_align via CPLT_CTRL0[3]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_CLEAR, MASK_SET(3)));

    FAPI_TRY(fapi2::delay(0, 900));

    // -------------------------------
    // Start Core Clock
    // -------------------------------

    FAPI_DBG("Set all bits to zero prior clock start via SCAN_REGION_TYPE");
    FAPI_TRY(putScom(i_target, C_SCAN_REGION_TYPE, MASK_ZERO));

    FAPI_DBG("Start clock(arrays+nsl clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_ALL_THOLD_NSL_ARY;
    FAPI_TRY(putScom(i_target, C_CLK_REGION, l_data64));

    /// @todo parameterize delay
    FAPI_TRY(fapi2::delay(0, 1000000));

    FAPI_DBG("Start clock(sl+refresh clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_ALL_THOLD_ALL;
    FAPI_TRY(putScom(i_target, C_CLK_REGION, l_data64));

    FAPI_DBG("Poll for core clock running");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CORE_CLK_START_TIMEOUT_IN_MS;

    // Read Clock Status Register (Core chiplet)
    // check for bits 4:14 eq. zero, no tholds on
    do
    {
        FAPI_TRY(getScom(i_target, C_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & BITS64(4, 11)) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_CORECLKSTART_TIMEOUT().set_CORECLKSTAT(l_data64),
                "Core Clock Start Timeout");
    FAPI_DBG("Core clock running now");

    // -------------------------------
    // Cleaning up
    // -------------------------------

    FAPI_DBG("Drop chiplet fence via NET_CTRL0[18]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(18)));

    /*
    FAPI_DBG("Check the Global Checkstop FIR");
    FAPI_TRY(getScom(i_target, C_XFIR, l_data64));
    FAPI_ASSERT(((l_data64 & BITS64(0, 27)) != 0),
                fapi2::PMPROC_CORE_XSTOP().set_COREXFIR(l_data64),
                "Core Chiplet Checkstop");
    */
    FAPI_DBG("Clear flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, C_CPLT_CTRL0_CLEAR, MASK_SET(2)));
    /*
    FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
    FAPI_ASSERT(((l_data64 & BIT64(9)) != 0),
                fapi2::PMPROC_CORECPLTALIGN_TIMEOUT()
                .set_CORECPLTSTAT0(l_data64),
                "Core Chiplets Aligned Timeout");
    FAPI_TRY(getScom(i_target, C_CPLT_STAT0, l_data64));
    FAPI_ASSERT(((l_data64 & BIT64(9)) != 0),
                fapi2::PMPROC_CORECPLTALIGN_TIMEOUT()
                .set_CORECPLTSTAT0(l_data64),
                "Core Chiplets Aligned Timeout");
    */
fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_startclocks");

    return fapi2::current_err;
}




