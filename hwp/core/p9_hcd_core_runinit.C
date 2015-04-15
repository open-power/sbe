///
/// @file  p9_hcd_core_runinit.C
/// @brief execute all core init procedures
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : CME
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
#include "p9_hcd_core_runinit.H"

//------------------------------------------------------------------------------
// Constant Definitions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure:
//------------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_hcd_core_runinit(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    return fapi2::FAPI2_RC_SUCCESS;

#if 0

    FAPI_CLEANUP();
    return fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

#endif

} // Procedure


} // extern C


