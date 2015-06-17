//------------------------------------------------------------------------------
/// @file  p9_sbe_chiplet_reset.C
///
/// @brief Identify all good chiplets excluding EQ/EC
/// Setup multicast groups for all chiplets
/// For all good chiplets excluding EQ/EC
/// For all enabled chiplets
// *!
// *! OWNER NAME  : Abhishek Agarwal  Email: abagarw8@in.ibm.com
// *! BACKUP NAME :                   Email:
//------------------------------------------------------------------------------
// *HWP HWP Owner   : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP FW Owner    : Brian Silver <bsilver@us.ibm.com>
// *HWP Team        : Perv
// *HWP Level       : 1
// *HWP Consumed by : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_reset.H"



fapi2::ReturnCode p9_sbe_chiplet_reset(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip)
{
    FAPI_DBG("Entering ...");

    FAPI_DBG("Exiting ...");

    return fapi2::FAPI2_RC_SUCCESS;

}
