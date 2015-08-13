///
/// @file  p9_hcd_cache_dpll_setup.H
/// @brief Quad DPLL Setup
///
/// *HWP HWP Owner   : David Du       <daviddu@us.ibm.com>
/// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE
/// *HWP Level       : 1
///

#ifndef __P9_HCD_CACHE_DPLL_SETUP_H__
#define __P9_HCD_CACHE_DPLL_SETUP_H__

/// @typedef p9_hcd_cache_dpll_setup_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_cache_dpll_setup_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>&);

extern "C"
{
/// @brief Quad DPLL Setup
///
/// @param [in] i_target TARGET_TYPE_EQ target
///
/// @attr
/// @attritem ATTR_DPLL_REPAIR_RING - EQ target, uint32
///               repair dpll ring content<br>
///
/// @retval FAPI2_RC_SUCCESS
fapi2::ReturnCode
p9_hcd_cache_dpll_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target);


} // extern C

#endif  // __P9_HCD_CACHE_DPLL_SETUP_H__
