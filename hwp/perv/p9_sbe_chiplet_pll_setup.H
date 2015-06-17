//------------------------------------------------------------------------------
/// @file  p9_sbe_chiplet_pll_setup.H
///
/// @brief PLL set up for L2 and L3 plls
// *!
// *! OWNER NAME  : Abhishek Agarwal  Email: abagarw8@in.ibm.com
// *! BACKUP NAME :                   Email:
//------------------------------------------------------------------------------
// *HWP HWP Owner   : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP FW Owner    : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team        : Perv
// *HWP Level       : 1
// *HWP Consumed by : SBE
//------------------------------------------------------------------------------


#ifndef _P9_SBE_CHIPLET_PLL_SETUP_H_
#define _P9_SBE_CHIPLET_PLL_SETUP_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_chiplet_pll_setup_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief PLL setup for L2 and L3 levels
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_chiplet_pll_setup(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
