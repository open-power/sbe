///
/// @file  p9_common_pro_epi_log.C
/// @brief common procedure prologue/epilogue routines
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE
/// *HWP Level       : 1
///
/// Procedure Summary:
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_common_pro_epi_log.H"

//------------------------------------------------------------------------------
// Constant Definitions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure:
//------------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_common_pro_epi_log(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ |
                        fapi2::TARGET_TYPE_CORE>& i_target,
    int i_operation)
{
    return fapi2::FAPI2_RC_SUCCESS;

} // Procedure


} // extern C


