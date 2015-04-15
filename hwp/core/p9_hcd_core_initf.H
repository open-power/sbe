///
/// @file  p9_hcd_core_initf.H
/// @brief Core scan init
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///

#ifndef __P9_HCD_CORE_INITF_H__
#define __P9_HCD_CORE_INITF_H__

extern "C"
{

/// @typedef p9_hcd_core_initf_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_core_initf_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>&);


/// @brief Core scan init
///
/// @param [in] i_target TARGET_TYPE_CORE target
/// @attr
/// @attritem ATTR_CORE_FUNC_RING - EC target, uint32
///               pointer to RS4 content<br>
///
/// @retval FAPI_RC_SUCCESS
fapi2::ReturnCode
p9_hcd_core_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);


} // extern C

#endif  // __P9_HCD_CORE_INITF_H__
