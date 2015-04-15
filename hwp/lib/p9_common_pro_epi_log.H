///
/// @file  p9_common_pro_epi_log.H
/// @brief common procedure prologue/epilogue routines
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE
/// *HWP Level       : 1
///

#ifndef __P9_COMMON_PRO_EPI_LOG_H__
#define __P9_COMMON_PRO_EPI_LOG_H__

extern "C"
{

/// @typedef p9_common_pro_epi_log_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_common_pro_epi_log_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_EQ|
                        fapi2::TARGET_TYPE_CORE>&,
    int);


/// @brief common procedure prologue/epilogue routines
///
/// @param [in] i_target    TARGET_TYPE_EQ|TARGET_TYPE_CORE target
/// @param [in] i_operation ENUM(PROLOG, EPILOG)
///
/// @attr
/// @attritem ATTR_EX_PARIAL_GOOD - EX target, uint32
///
/// @retval FAPI_RC_SUCCESS
fapi2::ReturnCode
p9_common_pro_epi_log(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ|
                        fapi2::TARGET_TYPE_CORE>& i_target,
    int i_operation);


} // extern C

#endif  // __P9_COMMON_PRO_EPI_LOG_H__
