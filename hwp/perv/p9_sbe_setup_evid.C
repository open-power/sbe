///
/// @file  p9_sbe_setup_evid.C
/// @brief Setup External Voltage IDs and Boot Frequency
///
/// *HW Owner    : Greg Still <stillgs@us.ibm.com>
/// *FW Owner    : TBD
/// *Team        : Power Management
/// *Consumed by : SBE
/// *Level       : 1
///
/// Procedure Summary:
///   Use Attributes to send VDD, VCS via the AVS bus to VRMs
///   Use Attributes to adjust the VDN and send via I2C to VRM
///   Read core frequency ATTR and write to the Quad PPM

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
#include "p9_sbe_setup_evid.H"

//-----------------------------------------------------------------------------
// Procedure
//-----------------------------------------------------------------------------

using fapi2::ReturnCode;
using fapi2::Target;
using fapi2::TARGET_TYPE_PROC_CHIP;
using fapi2::FAPI2_RC_SUCCESS;

extern "C"
{


ReturnCode
p9_sbe_setup_evid(const Target<TARGET_TYPE_PROC_CHIP>& i_target)
{
    // Substep indicators
    
    // commented out in Level 1 to not have "unused variable" warnings
    // until the SBE substep management "macro" or "call" is defined.
   
    // const uint32_t STEP_SBE_EVID_START              = 0x1;
    // const uint32_t STEP_SBE_EVID_CONFIG             = 0x2;
    // const uint32_t STEP_SBE_EVID_WRITE_VDN          = 0x3;
    // const uint32_t STEP_SBE_EVID_POLL_VDN_STATUS    = 0x4;
    // const uint32_t STEP_SBE_EVID_WRITE_VDD          = 0x5;
    // const uint32_t STEP_SBE_EVID_POLL_VDD_STATUS    = 0x6;
    // const uint32_t STEP_SBE_EVID_WRITE_VCS          = 0x7;
    // const uint32_t STEP_SBE_EVID_POLL_VCS_STATUS    = 0x8;
    // const uint32_t STEP_SBE_EVID_TIMEOUT            = 0x9;
    // const uint32_t STEP_SBE_EVID_BOOT_FREQ          = 0xA;
    // const uint32_t STEP_SBE_EVID_COMPLETE           = 0xB;

    return FAPI2_RC_SUCCESS;

} // Procedure


} // extern C

