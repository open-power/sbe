///
/// @file  p9_hcd_core_poweron.H
/// @brief Core Chiplet Power-on
///
// *HWP HWP Owner   : David Young      <daviddu@us.ibm.com>
// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team        : PM
// *HWP Consumed by : SBE:CME
// *HWP Level       : 2
//


#ifndef __P9_HCD_CORE_POWERON_H__
#define __P9_HCD_CORE_POWERON_H__

#include "p9_common_poweronoff.H"

/// @typedef p9_hcd_core_poweron_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_core_poweron_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>&);

extern "C"
{
/// @brief Core Chiplet Power-on
///
/// @param [in] i_target    TARGET_TYPE_CORE target
/// @param [in] i_operation ENUM(ON,OFF)
///
/// @attr
/// @attritem ATTR_PFET_*
///
/// @retval FAPI2_RC_SUCCESS if success, else error code
fapi2::ReturnCode
p9_hcd_core_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);
}
#endif  // __P9_HCD_CORE_POWERON_H__
