///
/// @file  p9_hcd_core_pcb_arb.C
/// @brief Core Chiplet PCB Arbitration
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///
/// Procedure Summary:
///   If CME,
///     1.Request PCB Mux, via write to PCB_MUX_REQ_C0 @ CCSCR_OR
///       - setBit(5) @ CME_LOCAL_CORE_STOP_CONTROL_REGISTER_OR_0510
///     2.Poll for PCB Mux grant, via read from
///       Polled Timeout: ns
///       - getBit() @
///   Else (SBE),
///     Nop (as the CME is not running in bringing up the first Core)
///

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_core_pcb_arb.H"

//-----------------------------------------------------------------------------
// Constant Definitions: Core Chiplet PCB Arbitration
//-----------------------------------------------------------------------------

extern "C"
{

fapi2::ReturnCode
p9_hcd_core_pcb_arb(
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

