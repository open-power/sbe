/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_poweron.C $        */
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
/// @file  p9_hcd_cache_poweron.C
/// @brief Cache Chiplet Power-on
///
/// Procedure Summary:
///   Set glsmux async reset
///   Set DPLL ff_bypass
///   Command the cache PFET controller to power-on
///   Check for valid power on completion
///   Polled Timeout:  100us

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
#include <p9_common_poweronoff.H>
#include <p9_common_poweronoff.C>
#include "p9_hcd_cache_poweron.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Chiplet Power-on
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_poweron");

    //--------------------------
    // Prepare to cache power on
    //--------------------------

    fapi2::buffer<uint64_t> l_data64;

    FAPI_DBG("Assert PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_SET(25)));

    FAPI_DBG("Assert chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_SET(26)));

    FAPI_DBG("Assert Vital Thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_SET(16)));

    FAPI_DBG("Set L2 glsmux reset via EXCLK_GRID_CTRL[32:33]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, MASK_OR(32, 2, 0x3)));

    FAPI_DBG("Set L3 glsmux reset via CLOCK_GRID_CTRL[0]");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_SET(0)));

    //-----------------------
    // Power on cache chiplet
    //-----------------------

    FAPI_DBG("Power on cache chiplet");
    FAPI_TRY(p9_common_poweronoff<fapi2::TARGET_TYPE_EQ>(i_target, p9power::POWER_ON));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_poweron");

    return fapi2::current_err;
}
