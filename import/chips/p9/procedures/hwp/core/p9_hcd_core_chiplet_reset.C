/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_chiplet_reset.C $    */
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
/// @file  p9_hcd_core_chiplet_reset.C
/// @brief Core Chiplet Reset
///
/// Procedure Summary:
///   Reset core chiplet logic
///     (TODO: check with Andreas on the effect of a CME based Endpoint reset
///            relative to the CorePPM path)
///   Clocking:
///    - setup cache sector buffer strength,
///      pulse mode and pulsed mode enable values
///      (attribute dependency Nimbus/Cumulus)
///    - Drop glsmux async reset
///   Scan0 flush entire core chiplet

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
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_core_chiplet_reset.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Core Chiplet Reset
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_chiplet_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_chiplet_reset");
    fapi2::buffer<uint64_t> l_data64;

    //--------------------------
    // Reset core chiplet logic
    //--------------------------
    // If there is an unused, powered-off core chiplet which needs to be
    // configured in the following steps to setup the PCB endpoint.

    FAPI_DBG("Init NET_CTRL0[1,3-5,11-14,18,22,26],step needed for hotplug");
    l_data64 = p9hcd::C_NET_CTRL0_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, l_data64));
    l_data64 |= BIT64(2); // make sure bit 2 is untouched
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("Flip core glsmux to refclk via PPM_CGCR[3]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, MASK_SET(0)));

    FAPI_DBG("Assert core progdly and DCC bypass via NET_CTRL1[1,2]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL1_WOR, MASK_OR(1, 2, 3)));

    FAPI_DBG("Drop vital thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(16)));

    /// @todo setup sector buffer strength, pulse mode and pulsed mode enable

    FAPI_DBG("Drop core glsmux reset via PPM_CGCR[0]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, 0));
    FAPI_TRY(fapi2::delay(0, 800));

    FAPI_DBG("Flip core glsmux to DPLL via PPM_CGCR[3]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, MASK_SET(3)));

    FAPI_DBG("Assert chiplet enable via NET_CTRL0[0]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, MASK_SET(0)));

    FAPI_DBG("Drop PCB endpoint reset via NET_CTRL0[1]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(1)));

    FAPI_DBG("Drop chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(26)));

    FAPI_DBG("Drop PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(25)));

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
        fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
            i_target.getParent<fapi2::TARGET_TYPE_PERV>();

        FAPI_DBG("Scan0 region:all_but_vital type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_PERV_CORE,
                                                  p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:all_but_vital type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_PERV_CORE,
                                                  p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }
#endif

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_chiplet_reset");
    return fapi2::current_err;
}

