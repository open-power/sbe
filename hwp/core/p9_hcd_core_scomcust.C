///
/// @file  p9_hcd_core_scomcust.C
/// @brief Core Customization SCOMs
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///
/// Procedure Summary:
///   Dynamically built (and installed) routine that is inserted by the .XIP
///   Customization. process. (New for P9) (TODO: this part of the process is
///   a placeholder at this point)
///   Dynamically built pointer where a NULL is checked before execution
///   If NULL (a potential early value); return
///   Else call the function at the pointer;
///   pointer is filled in by XIP Customization
///

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_core_scomcust.H"

//-----------------------------------------------------------------------------
// Constant Definitions: Core Customization SCOMs
//-----------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_hcd_core_scomcust(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
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


