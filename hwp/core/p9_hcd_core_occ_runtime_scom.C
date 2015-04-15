///
/// @file  p9_hcd_core_occ_runtime_scom.C
/// @brief Core OCC runtime SCOMS
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///
/// Procedure Summary:
///   Run-time updates from OCC code that are put  somewhere TBD
///   (TODO . revisit with OCC FW team)
///   OCC FW sets up value in the TBD SCOM section
///   This was not leverage in P8 with the demise of CPMs
///   Placeholder at this point
///

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_core_occ_runtime_scom.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define host_runtime_scom 0

//-----------------------------------------------------------------------------
// Procedure: Core OCC runtime SCOMS
//-----------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_hcd_core_occ_runtime_scom(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{

#if 0

    fapi2::buffer<uint64_t> data;

    // Run the SCOM sequence if the SCOM procedure is defined
    // -   la      A0, occ_runtime_scom
    // -   ld      D0, 0, A0
    // -   braz    D0, 1f
    //FAPI_INF("Launching OCC Runtime SCOM routine")
    // -   bsrd    D0
    // -   1:

    return fapi2::FAPI2_RC_SUCCESS;

    FAPI_CLEANUP();
    return fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

#endif

    return fapi2::FAPI2_RC_SUCCESS;

} // Procedure


} // extern C


