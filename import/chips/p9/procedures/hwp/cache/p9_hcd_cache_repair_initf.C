/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_repair_initf.C $   */
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
/// @file  p9_hcd_cache_repair_initf.C
/// @brief Load Repair ring for EX non-core
///
/// Procedure Summary:
///   Load cache ring images from MVPD
///   These rings must contain ALL chip customization data.
///   This includes the following:  Repair Power headers, and DTS
///   Historically this was stored in MVPD keywords are #R, #G. Still stored in
///     MVPD, but SBE image is customized with rings for booting cores

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_ring_id.h>
#include "p9_hcd_cache_repair_initf.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Load Repair ring for cache
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_repair_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_repair_initf");

    auto l_ex_targets = i_target.getChildren<fapi2::TARGET_TYPE_EX>();

    FAPI_DBG("Scan eq_repr ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_repr,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (eq_repr)");

    for (auto l_ex : l_ex_targets)
    {
        FAPI_DBG("Scan ex_l3_repr ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_repr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l3_repr)");

        FAPI_DBG("Scan ex_l2_repr ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l2_repr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l2_repr)");

        FAPI_DBG("Scan ex_l3_refr_repr ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_refr_repr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l3_refr_repr)");
    }

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_repair_initf");
    return fapi2::current_err;
}



