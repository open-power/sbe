/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_gptr_time_initf.C $ */
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
/// @file  p9_hcd_cache_gptr_time_initf.C
/// @brief Load GPTR and Time for EX non-core
///
/// Procedure Summary:
///   Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///     to produce #G VPD contents
///   Check for the presence of core override GPTR ring from image
///     (this is new fvor P9)
///   if found, apply;  if not, apply core GPTR from image
///     Check for the presence of core override TIME ring from image;
///   if found, apply;  if not, apply core base TIME from image

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_ringId.H>
#include "p9_hcd_cache_gptr_time_initf.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Load GPTR and Time for EX non-core
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_gptr_time_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_gptr_time_initf");

#ifndef P9_HCD_STOP_SKIP_SCAN

    auto l_ex_targets = i_target.getChildren<fapi2::TARGET_TYPE_EX>();

    FAPI_DBG("Scanning Cache GPTR Rings");
    FAPI_TRY(fapi2::putRing(i_target, eq_gptr,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning Cache TIME Rings");
    FAPI_TRY(fapi2::putRing(i_target, eq_time,
                            fapi2::RING_MODE_HEADER_CHECK));

    for (auto l_ex : l_ex_targets)
    {
        FAPI_DBG("Scanning EX L3 GPTR Rings");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_gptr,
                                fapi2::RING_MODE_HEADER_CHECK));

        FAPI_DBG("Scanning EX L2 GPTR Rings");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l2_gptr,
                                fapi2::RING_MODE_HEADER_CHECK));

        FAPI_DBG("Scanning EX L3 Refresh GPTR Rings");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_refr_gptr,
                                fapi2::RING_MODE_HEADER_CHECK));

        FAPI_DBG("Scanning EX L3 TIME Rings");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_time,
                                fapi2::RING_MODE_HEADER_CHECK));

        FAPI_DBG("Scanning EX L2 TIME Rings");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l2_time,
                                fapi2::RING_MODE_HEADER_CHECK));

        FAPI_DBG("Scanning EX L3 Refresh TIME Rings");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_refr_time,
                                fapi2::RING_MODE_HEADER_CHECK));
    }

    FAPI_DBG("Scanning Cache DPLL GPTR Rings");
    FAPI_TRY(fapi2::putRing(i_target, eq_dpll_gptr,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning Cache Analog GPTR Rings");
    FAPI_TRY(fapi2::putRing(i_target, eq_ana_gptr,
                            fapi2::RING_MODE_HEADER_CHECK));

fapi_try_exit:

#endif

    FAPI_INF("<<p9_hcd_cache_gptr_time_initf");
    return fapi2::current_err;
}

