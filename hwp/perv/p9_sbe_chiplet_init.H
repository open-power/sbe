//------------------------------------------------------------------------------
/// @file  p9_sbe_chiplet_init.H
///
/// @brief init procedure for all enabled chiplets
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


#ifndef _P9_SBE_CHIPLET_INIT_H_
#define _P9_SBE_CHIPLET_INIT_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_chiplet_init_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief Scan 0 all rings (except time, repair, gptr) on all enabled chiplets
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_chiplet_init(const
                                          fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
