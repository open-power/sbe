///
/// @file  p9_hcd_cache_chiplet_reset.H
/// @brief Cache Chiplet Reset
///
/// *HWP HWP Owner   : David Du       <daviddu@us.ibm.com>
/// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE
/// *HWP Level       : 1
///

#ifndef __P9_HCD_CACHE_CHIPLET_RESET_H__
#define __P9_HCD_CACHE_CHIPLET_RESET_H__

extern "C"
{

/// @typedef p9_hcd_cache_chiplet_reset_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_cache_chiplet_reset_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>&);

/// @brief Cache Chiplet Reset
///
/// @param [in] i_target TARGET_TYPE_EQ target
///
/// @attr
/// @attritem NONE
///
/// @retval FAPI_RC_SUCCESS
fapi2::ReturnCode
p9_hcd_cache_chiplet_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target);


} // extern C

#endif  // __P9_HCD_CACHE_CHIPLET_RESET_H__
