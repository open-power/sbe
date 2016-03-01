/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_poweron.H $        */
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
/// @file  p9_hcd_cache_poweron.H
/// @brief Cache Chiplet Power-on
///

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Level              : 2
// *HWP Consumed by        : SBE:SGPE

#ifndef __P9_HCD_CACHE_POWERON_H__
#define __P9_HCD_CACHE_POWERON_H__

#include <fapi2.H>

/// @typedef p9_hcd_cache_poweron_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_cache_poweron_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>&);

extern "C"
{
/// @brief Cache Chiplet Power-on
/// @param [in] i_target    TARGET_TYPE_EQ target
/// @param [in] i_operation ENUM(ON,OFF)
///
/// @attr
/// @attritem ATTR_PFET_*
///
/// @retval FAPI2_RC_SUCCESS if success, else error code

    fapi2::ReturnCode
    p9_hcd_cache_poweron(
        const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target);
}

#endif  // __P9_HCD_CACHE_POWERON_H__
