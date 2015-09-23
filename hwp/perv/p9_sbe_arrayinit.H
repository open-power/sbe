//------------------------------------------------------------------------------
/// @file  p9_sbe_arrayinit.H
///
/// @brief array init procedure to be called with any chiplet target except TP,EP,EC
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#ifndef _P9_SBE_ARRAYINIT_H_
#define _P9_SBE_ARRAYINIT_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_arrayinit_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief Array Init function call for any chiplet Target except TP,EP,EC
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_arrayinit(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
