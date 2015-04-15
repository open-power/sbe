///
/// @file  p9_hcd_cache_poweron.C
/// @brief Cache Chiplet Power-on
///
// *HWP HWP Owner   : David Young       <davidy@us.ibm.com>
// *HWP FW Owner    : Sangeetha T S     <sangeet2@in.ibm.com>
// *HWP Team        : PM
// *HWP Consumed by : SBE:SGPE
// *HWP Level       : 2
//
// Procedure Summary:
//   Command the cache PFET controller to power-on
//   Check for valid power on completion
//   Polled Timeout:  100us
//

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_cache_poweron.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Chiplet Power-on
//------------------------------------------------------------------------------
#define FAPI_CLEANUP() fapi_try_exit:

fapi2::ReturnCode
p9_hcd_cache_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    FAPI_EXEC_HWP(l_rc, p9_common_poweronoff, i_target, p9power::POWER_ON);
    return l_rc;
} // Procedure
