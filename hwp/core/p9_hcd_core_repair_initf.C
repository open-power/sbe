///
/// @file  p9_hcd_core_repair_initf.C
/// @brief Load Repair ring for core
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///
/// Procedure Summary:
///   Load core ring images from that came from MVPD into the image
///     These rings must contain ALL chip customization data. This includes the
///     following:  Array Repair and DTS calibration settings
///     Historically this was stored in MVPD keywords are #R, #G.  Still stored
///     in MVPD, but SBE image is customized with rings for booting cores
///     at build time
///

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_core_repair_initf.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Load Repair ring for core
//-----------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_hcd_core_repair_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{

#if 0

    fapi2::buffer<uint64_t> data;

    // scan chiplet specific ring content
    //FAPI_DBG("Scanning EX core REPAIR rings...")
    // - load_ring_vec_ex ex_repr_core

    return fapi2::FAPI2_RC_SUCCESS;

    FAPI_CLEANUP();
    return fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

#endif

    return fapi2::FAPI2_RC_SUCCESS;

} // Procedure


} // extern C

