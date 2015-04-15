///
/// @file  p9_hcd_core_scominit.H
/// @brief Core SCOM Inits
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///


#ifndef __P9_HCD_CORE_SCOMINIT_H__
#define __P9_HCD_CORE_SCOMINIT_H__

extern "C"
{

/// @typedef p9_hcd_core_scominit_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_core_scominit_FP_t) (
     const fapi2::Target<fapi2::TARGET_TYPE_CORE>&);


/// @brief Core SCOM Inits
///
/// @param [in] i_target TARGET_TYPE_CORE target
/// @attr
/// @attritem NONE
///
/// @retval FAPI_RC_SUCCESS
fapi2::ReturnCode
p9_hcd_core_scominit(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);


} // extern C

#endif  // __P9_HCD_CORE_SCOMINIT_H__
