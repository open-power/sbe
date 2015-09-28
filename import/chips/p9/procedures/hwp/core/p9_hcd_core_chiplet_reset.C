/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_chiplet_reset.C $    */
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
/// @file  p9_hcd_core_chiplet_reset.C
/// @brief Core Chiplet Reset
///
/// Procedure Summary:
///   Reset core chiplet logic
///     (TODO: check with Andreas on the effect of a CME based Endpoint reset
///            relative to the CorePPM path)
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

    FAPI_DBG("Init NETWORK_CONTROL0, step needed for hotplug");
    fapi2::buffer<uint64_t> l_data64 = p9hcd::NET_CTRL0_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, C_NET_CTRL0, l_data64));

    FAPI_DBG("Init Core Glitchless Mux Reset/Select via CLOCK_GRID_CTRL[0:3]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR_OR, MASK_OR(0, 4, 0x8)));

    FAPI_DBG("Clear Core Glitchless Mux Async Reset via CLOCK_GRID_CTRL[0]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR_CLEAR, MASK_SET(0)));

    FAPI_DBG("Clear PCB Endpoint Reset via NET_CTRL0[1]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(1)));

    FAPI_DBG("Reset PCB Slave Error Register");
    FAPI_TRY(putScom(i_target, C_ERROR_REG, MASK_ALL));

    /// @todo FAPI_DBG("Remove pervasive ECO fence;");
    /// @todo FAPI_DBG("Remove logical pervasive/pcbs-pm fence");

    FAPI_DBG("Remove chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(26)));

    FAPI_DBG("Configure HANG_PULSE1 for chiplet hang counters");
    l_data64 = p9hcd::HANG_PULSE1_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, C_HANG_PULSE_1_REG, l_data64));

#ifndef P9_HCD_STOP_SKIP_FLUSH

    //--------------------------------------------
    // Perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the repeat can be removed.
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_target;
    FAPI_DBG("Scan0 the GPTR/TIME/REP rings");

    for(uint32_t l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_target,
                                              p9hcd::SCAN0_REGION_CORE_ONLY,
                                              p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));

#endif

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_chiplet_reset");

    return fapi2::current_err;
}

