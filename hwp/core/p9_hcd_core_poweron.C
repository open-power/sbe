///
/// @file  p9_hcd_core_poweron.C
/// @brief Core Chiplet Power-on
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///
/// Procedure Summary:
///   1.Command the core PFET controller to power-on, via putscom to CPPM
///     -
///   2.Check for valid power on completion, via getscom from CPPM
///     Polled Timeout:  100us
///

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_core_poweron.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Core Chiplet Power-on
//-----------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_hcd_core_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
    const uint32_t i_operation)
{

#if 0

    fapi2::buffer<uint64_t> data;

    return fapi2::FAPI2_RC_SUCCESS;

    FAPI_CLEANUP();
    return fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

#endif

    return fapi2::FAPI2_RC_SUCCESS;

} // Procedure


} // extern C

