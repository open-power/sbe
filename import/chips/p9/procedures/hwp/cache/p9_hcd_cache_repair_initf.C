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

#ifndef P9_HCD_STOP_SKIP_SCAN

    FAPI_DBG("Scanning Cache REPAIR Rings");
    FAPI_TRY(fapi2::putRing(i_target, EQ_REPR,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning EX L3 REPAIR Rings");
    FAPI_TRY(fapi2::putRing(i_target, EX_L3_REPR,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning EX L2 REPAIR Rings");
    FAPI_TRY(fapi2::putRing(i_target, EX_L2_REPR,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning EX L3 Refresh REPAIR Rings");
    FAPI_TRY(fapi2::putRing(i_target, EX_L3_REFR_REPR,
                            fapi2::RING_MODE_HEADER_CHECK));

fapi_try_exit:

#endif

    FAPI_INF("<<p9_hcd_cache_repair_initf");
    return fapi2::current_err;
}



