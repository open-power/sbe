//------------------------------------------------------------------------------
/// @file  p9_sbe_check_master.C
///
/// @brief Deremine if this is master SBE -- External FSI/GP bitIf master continue, else enable runtime chipOps
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
#include "p9_sbe_check_master.H"
fapi2::ReturnCode p9_sbe_check_master(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip)
{
    FAPI_DBG("p9_sbe_check_master: Entering ...");

    FAPI_DBG("p9_sbe_check_master: Exiting ...");

    return fapi2::FAPI2_RC_SUCCESS;

}
