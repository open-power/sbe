/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_occ_runtime_scom.H $ */
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
/// @file  p9_hcd_core_occ_runtime_scom.H
/// @brief Core OCC runtime SCOMS
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///


#ifndef __P9_HCD_CORE_OCC_RUNTIME_SCOM_H__
#define __P9_HCD_CORE_OCC_RUNTIME_SCOM_H__

extern "C"
{

/// @typedef p9_hcd_core_occ_runtime_scom_FP_t
/// function pointer typedef definition for HWP call support
    typedef fapi2::ReturnCode (*p9_hcd_core_occ_runtime_scom_FP_t) (
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>&);


/// @brief Core OCC runtime SCOMS
///
/// @param [in] i_target TARGET_TYPE_CORE target
///
/// @attr
/// @attritem ATTR_CORE_OCC_SCOM_LOC - EC target, uint32
///
/// @retval FAPI_RC_SUCCESS
    fapi2::ReturnCode
    p9_hcd_core_occ_runtime_scom(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);


} // extern C

#endif  // __P9_HCD_CORE_OCC_RUNTIME_SCOM_H__
