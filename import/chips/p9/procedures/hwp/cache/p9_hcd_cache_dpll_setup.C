/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_dpll_setup.C $     */
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
/// @file  p9_hcd_cache_dpll_setup.C
/// @brief Quad DPLL Setup
///
/// Procedure Summary:
///   Note:
///     Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///     DPLL tune bits are not dependent on frequency
///     Frequency is controlled by the Quad PPM
///       Actual frequency value for boot is stored into the Quad PPM by
///         p9_hcd_setup_evid.C in istep 2
///       In real cache STOP exit, the frequency value is persistent
///
///   Pre-Scan:
///
///   Scan:
///     (TODO) Set clock controller scan ratio to 1:1 as this is done at refclk
///     (TODO) scan0 (region = DPLL and ANEP, scan_type = GPTR)
///     (TODO) scan0 (region = DPLL and ANEP, scan_type = FUNC)
///     (TODO) Set clock controller scan ratio to 8:1 for future scans
///
///   Setup:
///     (TODO) set DPLL FREQ CTRL regitster
///     (TODO) set DPLL CTRL register
///     (Done) Drop DPLL test mode;
///     (Done) Drop DPLL into Reset;
///     (Done) Start DPLL clock via quad clock controller
///     (Done) Check for DPLL lock, Timeout: 200us
///     (Done) Remove DPLL bypass
///     (Done) Switch cache glitchless mux to use the DPLL
///     (Done) Drop ff_bypass to enable slewing
///
/// 1) reset, dpll_func_clksel, and all SL_HLD inputs are asserted
/// 2) If grid clock connected to dpll clkout,
///    bypass also has to be asserted to allow refclk on grid

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_dpll_setup.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

enum P9_HCD_CACHE_DPLL_SETUP_CONSTANTS
{
    CACHE_DPLL_LOCK_TIMEOUT_IN_MS      = 1,
    CACHE_DPLL_CLK_START_TIMEOUT_IN_MS = 1,
    CACHE_ANEP_CLK_START_TIMEOUT_IN_MS = 1
};

//-----------------------------------------------------------------------------
// Procedure: Quad DPLL Setup
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_dpll_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    fapi2::buffer<uint64_t> l_data64;
    uint32_t                l_timeout;

    FAPI_INF(">>p9_hcd_cache_dpll_setup");

    // --------------
    // DPLL SCAN
    // --------------
    FAPI_DBG("Set scan ratio to 1:1 in bypass mode");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<47, 5>(0x0);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

#ifndef P9_HCD_STOP_SKIP_FLUSH
    // Putting in block to avoid c++ crosses initialization compile error
    {
        //--------------------------------------------
        // Perform scan0 module for pervasive chiplet
        //--------------------------------------------
        //Each scan0 will rotate the ring 8191 latches (2**13-1) and the longest
        //ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
        //all stumps less than 8191, the repeat can be removed.
        uint32_t l_loop;
        fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
            i_target.getParent<fapi2::TARGET_TYPE_PERV>();

        FAPI_DBG("Scan0 the DPLL/ANEP rings(GPTR type)");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_DPLL_ANEP,
                                                  p9hcd::SCAN0_TYPE_GPTR));

        FAPI_DBG("Scan0 all DPLL/ANEP rings(func type)");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_DPLL_ANEP,
                                                  p9hcd::SCAN0_TYPE_FUNC));
    }
#endif

#ifndef P9_HCD_STOP_SKIP_SCAN

    /// @todo putRing(DPLL,FUNC) here

#endif

    FAPI_DBG("Set scan ratio to 8:1 in bypass mode");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<49, 3>(0x7);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

    // --------------
    // DPLL SETUP
    // --------------

    FAPI_DBG("Ensure DPLL in Mode 1, and set slew rate to a modest value");
    l_data64.flush<0>().setBit<2>().insertFromRight<6, 10>(0x40);
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, l_data64));

    FAPI_DBG("Drop DPLL Test Mode and Reset");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_AND(3, 2, 0)));

    FAPI_DBG("Drop DPLL Clock Region Fence");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(14)));

    // ----------------
    // Start DPLL clock
    // ----------------

    FAPI_DBG("Set all bits to zero prior clock start via SCAN_REGION_TYPE");
    FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

    FAPI_DBG("Start clock(arrays+nsl clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_DPLL_THOLD_NSL_ARY;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    /// @todo parameterize delay
    FAPI_TRY(fapi2::delay(0, 1000000));

    FAPI_DBG("Start clock(sl+refresh clock region) via CLK_REGION");
    l_data64 = p9hcd::CLK_START_REGION_DPLL_THOLD_ALL;
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    FAPI_DBG("Poll for DPLL clock running");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_DPLL_CLK_START_TIMEOUT_IN_MS;

    // Read Clock Status Register (Cache chiplet)
    // check for bits 4:14 eq. zero, no tholds on
    do
    {
        FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
    }
    while(((l_data64 & BIT64(14)) != 0) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_DPLLCLKSTART_TIMEOUT().set_EQCLKSTAT(l_data64),
                "DPLL Clock Start Timeout");
    FAPI_DBG("DPLL clock running now");

    // This is necessary to ensure that the DPLL is in Mode 1(ff_bypass = 1)
    // If not, the lock times will go from ~30us to 3-5ms
    /// @todo Determine whether or not we should POLL instead of put delay here.
    FAPI_DBG("Wait for DPLL to lock");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_DPLL_LOCK_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_QPPM_DPLL_STAT, l_data64));
        break; /// @todo Skipping the lock checking until model is ready
    }
    while ((l_data64.getBit<63>() != 1 ) && (--l_timeout != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_DPLL_LOCK_TIMEOUT()
                .set_EQQPPMDPLLSTAT(l_data64),
                "DPLL lock timeout");
    FAPI_DBG("DPLL is locked");

    FAPI_DBG("Take DPLL out of bypass");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(5)));

    FAPI_DBG("Switch L3 glsmux select to DPLL output");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_SET(3)));

    FAPI_DBG("Switch L2 glsmux select to DPLL output");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, MASK_OR(34, 2, 3)));

    FAPI_DBG("Drop ff_bypass to switch into slew-controlled mode")
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_CLEAR, MASK_SET(2)));
    /*
        FAPI_DBG("Drop ANEP Clock Region Fence");
        FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(10)));

        // ----------------
        // Start ANEP clock
        // ----------------

        FAPI_DBG("Set all bits to zero prior clock start via SCAN_REGION_TYPE");
        FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

        FAPI_DBG("Start clock(arrays+nsl clock region) via CLK_REGION");
        l_data64 = p9hcd::CLK_START_REGION_ANEP_THOLD_NSL_ARY;
        FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

        /// @todo parameterize delay
        FAPI_TRY(fapi2::delay(0, 1000000));

        FAPI_DBG("Start clock(sl+refresh clock region) via CLK_REGION");
        l_data64 = p9hcd::CLK_START_REGION_ANEP_THOLD_ALL;
        FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

        FAPI_DBG("Poll for ANEP clock running");
        l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                    CACHE_ANEP_CLK_START_TIMEOUT_IN_MS;

        // Read Clock Status Register (Cache chiplet)
        // check for bits 4:14 eq. zero, no tholds on
        do
        {
            FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));
        }
        while(((l_data64 & BIT64(10)) != 0) && ((--l_timeout) != 0));

        FAPI_ASSERT((l_timeout != 0),
                    fapi2::PMPROC_ANEPCLKSTART_TIMEOUT().set_EQCLKSTAT(l_data64),
                    "ANEP Clock Start Timeout");
        FAPI_DBG("ANEP clock running now");

        // @todo is this bit really skew adjust? note it is set in chiplet reset
        FAPI_DBG("Release skew adjust reset");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(2)));
    */
fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_dpll_setup");

    return fapi2::current_err;

} // Procedure



