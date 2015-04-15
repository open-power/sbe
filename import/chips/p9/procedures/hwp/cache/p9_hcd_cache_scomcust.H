/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_scomcust.H $       */
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
/// @file  p9_hcd_cache_scomcust.H
/// @brief Core Chiplet PCB Arbitration
///
/// *HWP HWP Owner   : David Du       <daviddu@us.ibm.com>
/// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE
/// *HWP Level       : 1
///

#ifndef __P9_HCD_CACHE_SCOMCUST_H__
#define __P9_HCD_CACHE_SCOMCUST_H__
extern "C"
{

/// @typedef p9_hcd_cache_scomcust_FP_t
/// function pointer typedef definition for HWP call support
    typedef fapi2::ReturnCode (*p9_hcd_cache_scomcust_FP_t) (
        const fapi2::Target<fapi2::TARGET_TYPE_EX>&);


/// @brief Core Chiplet PCB Arbitration
///
/// @param [in] i_target TARGET_TYPE_EX target
///
/// @attr
/// @attritem NONE
///
/// @retval FAPI_RC_SUCCESS
    fapi2::ReturnCode
    p9_hcd_cache_scomcust(
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_target);


} // extern C

#endif  // __P9_HCD_CACHE_SCOMCUST_H__
