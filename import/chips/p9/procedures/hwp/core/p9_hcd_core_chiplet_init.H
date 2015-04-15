/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_chiplet_init.H $     */
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
/// @file  p9_hcd_core_chiplet_init.H
/// @brief Core Flush/Initialize
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///

#ifndef __P9_HCD_CORE_CHIPLET_INIT_H__
#define __P9_HCD_CORE_CHIPLET_INIT_H__

extern "C"
{

/// @typedef p9_hcd_core_chiplet_init_FP_t
/// function pointer typedef definition for HWP call support
    typedef fapi2::ReturnCode (*p9_hcd_core_chiplet_init_FP_t) (
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>&);


/// @brief Core Flush/Initialize
///
/// @param [in] i_target TARGET_TYPE_CORE target
///
/// @attr
/// @attritem NONE
///
/// @retval FAPI_RC_SUCCESS
    fapi2::ReturnCode
    p9_hcd_core_chiplet_init(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);


} // extern C

#endif  // __P9_HCD_CORE_CHIPLET_INIT_H__
