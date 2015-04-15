///
/// @file  p9_common_poweronoff.H
/// @brief common procedure for power on/off
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE:CME
/// *HWP Level       : 1
///

#ifndef __P9_COMMON_POWERONOFF_H__
#define __P9_COMMON_POWERONOFF_H__

extern "C"
{

/// @typedef p9_common_proweronoff_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_common_proweronoff_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_EQ|
                        fapi2::TARGET_TYPE_CORE>&,
    int);


/// @brief common procedure for power on/off
///
/// @param [in] i_target    TARGET_TYPE_EQ|TARGET_TYPE_CORE target
/// @param [in] i_operation ENUM(ON,OFF)
///
/// @attr
/// @attritem ATTR_PFET_TIMING - EX target, uint32
///
/// @retval FAPI_RC_SUCCESS
fapi2::ReturnCode
p9_common_proweronoff(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ|
                        fapi2::TARGET_TYPE_CORE>& i_target,
    int i_operation);


} // extern C

#endif  // __P9_COMMON_POWERONOFF_H__
