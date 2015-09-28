/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_dpll_setup.C $     */
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
/// @file  p9_hcd_cache_dpll_setup.C
/// @brief Quad DPLL Setup
///
// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2
//
// Procedure Summary:
//   Note:
//     Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
//     DPLL tune bits are not dependent on frequency
//     Frequency is controlled by the Quad PPM
//       Actual frequency value for boot is stored into the Quad PPM by
//         p9_hcd_setup_evid.C in istep 2
//       In real cache STOP exit, the frequency value is persistent
//
//   Pre-Scan:
//     (Done) +Drop DPLL test mode;
//     (Done) +Drop DPLL into Reset;
//     (Done) Put DPLL into bypass;
//     (Done) +Set ff_bypass = 1;
//     (Done) Set DPLL syncmux sel;
//
//   Scan:
//     (TBD)  Set clock controller scan ratio to 1:1
//            as this is done at refclk speeds
//     (TBD)  Load the EX DPLL scan ring
//     (TBD)  Set clock controller scan ratio to 8:1 for future scans
//     (Done) +Start DPLL clock via quad clock controller
//
//   Setup:
//     (Done) +Ensure ff_bypass = 1, should this be scaned?
//     (Done) Enable the DPLL in the correct mode: non-dynamic(b1=0 by default?)
//     (Done) Slew rate established per DPLL team
//     (Done) +Clear DPLL syncmux sel
//     (TBD)  *Take the cache glitchless mux out of reset (in chiplet_reset?)
//     (Done) Remove DPLL bypass
//     (TBD)  Drop DPLL Tholds (still in p9?)
//     (Done) Check for DPLL lock, Timeout: 200us
//     (Done) +Recycle DPLL in and out of bypass
//     (Done) Switch cache glitchless mux to use the DPLL (in startclocks?)
//     (Done) +Drop ff_bypass to enable slewing
//
//   Notion: + means the step isnt on p9 ipl flow doc but     in p8 code
//           * means the step is   on p9 ipl flow doc but not in p8 code

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_dpll_setup.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
// Setup DPLL like this: (from dpll_spec_14nm_20140107)
//  (marked as IPL from IPL document)
// Initial State:  (Do we need these (1-3) in IPL?)
// 1) reset, dpll_func_clksel, and all SL_HLD inputs are asserted
// 2) If grid clock connected to dpll clkout,
//    bypass also has to be asserted to allow refclk on grid
// 3) Scan init:
//    - All scan rings in DPLL are scanned with zeroes
//    - Scan in VPD table values into the DPLL_MODE ring (config registers)
//      lf_param_dc<0:15> = b1010001010000000#
//      will change based on HW characterization
//     lf_sdorder_dc<0:1> = b10
//     cd_div124_dc<0:1> = b10
//     cd_dpllout124_dc<0:1> = b10
//     vc_vtune_dc<0:2> = 0b100 # may change based on HW characterization
//     ref_div_dc<0:5> = b000001
//     ref_div_vreg_dc<0:4> = b000010
//     refclk_sel_dc = b1
//     All configuration register bits not specified should be set to b0

enum P9_HCD_CACHE_DPLL_SETUP_CONSTANTS
{
    CACHE_DPLL_LOCK_TIMEOUT_IN_MS = 1
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
    // PRE-SCAN
    // --------------

    FAPI_DBG("Drop DPLL Test Mode and Reset");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_AND(3, 2, 0)));

    FAPI_DBG("Put DPLL into bypass");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR,  MASK_SET(5)));

    FAPI_DBG("Put DPLL into Mode 1 by asserting ff_bypass");
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, MASK_SET(2)));

    /// @todo Is there a dpllclk_muxsel in p9?
    FAPI_DBG("Set syncclk_muxsel and dpllclk_muxsel");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(1)));

    // --------------
    // DPLL SCAN
    // --------------
    /// @todo scan dpll here
    /// @todo start dpll clock here?
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

    // --------------
    // DPLL SETUP
    // --------------

    // This is necessary to ensure that the DPLL is in Mode 1.
    // If not, the lock times will go from ~30us to 3-5ms
    FAPI_DBG("Ensure DPLL in Mode 1, and set slew rate to a modest value");
    l_data64.flush<0>().setBit<2>().insertFromRight<6, 10>(0x40);
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, l_data64));

    /// @todo Is there a dpllclk_muxsel in p9?
    FAPI_DBG("Clear syncclk_muxsel and dpllclk_muxsel");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(1)));

    /// @todo Already done in chiplet_reset?
    FAPI_DBG("Drop glitchless mux async reset");

    FAPI_DBG("Take DPLL out of bypass");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(5)));

    /// @todo Is there a dpll_thold in p9?
    FAPI_DBG("Drop internal DPLL THOLD");

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

    FAPI_DBG("Recycle DPLL in and out of bypass");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR,  MASK_SET(5)));
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(5)));

    FAPI_DBG("DPLL is locked");

    /// @todo Later done in startclocks?
    FAPI_DBG("Set glitchless mux select to dpll");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR_OR, MASK_SET(3)));

    FAPI_DBG("Drop ff_bypass to switch into slew-controlled mode")
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_CLEAR, MASK_SET(2)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_dpll_setup");

    return fapi2::current_err;

} // Procedure



