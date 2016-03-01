/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_initf.C $          */
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
/// @file  p9_hcd_cache_initf.C
/// @brief EX (non-core) scan init
///
/// Procedure Summary:
///   Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///   Check for the presence of cache FUNC override rings from image;
///   if found, apply;  if not, apply cache base FUNC rings from image
///   Note:  FASTINIT ring (eg CMSK ring) is setup at this point to limit the
///   stumps that participate in FUNC ring scanning (this is new for P9).
///   (TODO to make sure the image build support is in place)
///   Note: all caches that are in the Cache Multicast group will be
///   initialized to the same values via multicast scans

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p9_hcd_cache_initf.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: EX (non-core) scan init
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_initf");

#ifndef P9_HCD_STOP_SKIP_SCAN

    FAPI_DBG("Scanning Cache FUNC Rings");
    FAPI_TRY(fapi2::putRing(i_target, EQ_FURE,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning EX L3 FUNC Rings");
    FAPI_TRY(fapi2::putRing(i_target, EX_L3_FURE,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning EX L2 FUNC Rings");
    FAPI_TRY(fapi2::putRing(i_target, EX_L2_FURE,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning EX L3 Refresh FUNC Rings");
    FAPI_TRY(fapi2::putRing(i_target, EX_L3_REFR_FURE,
                            fapi2::RING_MODE_HEADER_CHECK));

    FAPI_DBG("Scanning Cache Analog FUNC Rings");
    FAPI_TRY(fapi2::putRing(i_target, EQ_ANA_FUNC,
                            fapi2::RING_MODE_HEADER_CHECK));

fapi_try_exit:

#endif

    FAPI_INF("<<p9_hcd_cache_initf");
    return fapi2::current_err;
}




