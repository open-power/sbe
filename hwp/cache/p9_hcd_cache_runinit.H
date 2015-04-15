///
/// @file  p9_hcd_cache_runinit.H
/// @brief execute all cache init procedures
///
/// *HWP HWP Owner   : David Du       <daviddu@us.ibm.com>
/// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SGPE
/// *HWP Level       : 1
///

#ifndef __P9_HCD_CACHE_RUNINIT_H__
#define __P9_HCD_CACHE_RUNINIT_H__

extern "C"
{

/// @typedef p9_hcd_cache_runinit_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_cache_runinit_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);


/// @brief execute all cache init procedures
///
/// @param [in] i_target TARGET_TYPE_PROC_CHIP target
///
/// @attr
/// @attritem NONE
///
/// @retval FAPI_RC_SUCCESS
fapi2::ReturnCode
p9_hcd_cache_runinit(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);


} // extern C

#endif  // __P9_HCD_CACHE_RUNINIT_H__
