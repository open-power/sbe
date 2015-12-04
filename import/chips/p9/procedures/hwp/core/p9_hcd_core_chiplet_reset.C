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
///

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

    //--------------------------
    // Reset core chiplet logic
    //--------------------------
    // If there is an unused, powered-off core chiplet which needs to be
    // configured in the following steps to setup the PCB endpoint.

    FAPI_DBG("Init NET_CTRL0[0,1,3,4,5,12,13,14,18], step needed for hotplug");
    fapi2::buffer<uint64_t> l_data64 = p9hcd::NET_CTRL0_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, C_NET_CTRL0, l_data64));

    FAPI_DBG("Assert Core Progdly and DCC Bypass via NET_CTRL1[1,2]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL1_WOR, MASK_OR(1, 2, 3)));

    FAPI_DBG("Assert Core DCC Reset via NET_CTRL0[2]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, MASK_SET(2)));

    FAPI_DBG("Drop Vital Thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(16)));

    /// @todo setup sector buffer strength, pulse mode and pulsed mode enable

    FAPI_DBG("Init core glsmux reset/select via CLOCK_GRID_CTRL[0:3]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, MASK_OR(0, 4, 0x1)));

    FAPI_DBG("Clear PCB Endpoint Reset via NET_CTRL0[1]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(1)));
    FAPI_TRY(fapi2::delay(0, 100));

    FAPI_DBG("Remove chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(26)));

    FAPI_DBG("Remove PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(25)));

#ifndef P9_HCD_STOP_SKIP_FLUSH
    // Putting in block to avoid c++ crosses initialization compile error
    {
        //--------------------------------------------
        // Perform scan0 module for pervasive chiplet
        //--------------------------------------------
        //Each scan0 will rotate the ring 8191 latches (2**13-1) and the longest
        //ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
        // all stumps less than 8191, the repeat can be removed.
        uint32_t l_loop;
        fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
            i_target.getParent<fapi2::TARGET_TYPE_PERV>();

        FAPI_DBG("Scan0 the GPTR/TIME/REP rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_CORE_ONLY,
                                                  p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 all except the GPTR/TIME/REP rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_CORE_ONLY,
                                                  p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }
#endif

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_chiplet_reset");

    return fapi2::current_err;
}

