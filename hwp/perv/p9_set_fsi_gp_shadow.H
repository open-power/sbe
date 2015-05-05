//------------------------------------------------------------------------------
/// @file  p9_set_fsi_gp_shadow.H
///
/// @brief --IPL step 0.8 proc_prep_ipl
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


#ifndef _P9_SET_FSI_GP_SHADOW_H_
#define _P9_SET_FSI_GP_SHADOW_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_set_fsi_gp_shadow_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief --update ROOT CTRL shadows if needed
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_set_fsi_gp_shadow(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
