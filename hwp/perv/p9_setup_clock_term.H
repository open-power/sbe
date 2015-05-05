//------------------------------------------------------------------------------
/// @file  p9_setup_clock_term.H
///
/// @brief proc setup clock term
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


#ifndef _P9_SETUP_CLOCK_TERM_H_
#define _P9_SETUP_CLOCK_TERM_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_setup_clock_term_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief Setup the clock termination correctly for system/chip type
/// Since this is the first procedure run against the chips it also clears the GP write protect
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_setup_clock_term(const
                                          fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
