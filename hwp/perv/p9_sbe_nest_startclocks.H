//------------------------------------------------------------------------------
/// @file  p9_sbe_nest_startclocks.H
///
/// @brief start clocks for nest chiplets
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


#ifndef _P9_SBE_NEST_STARTCLOCKS_H_
#define _P9_SBE_NEST_STARTCLOCKS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_nest_startclocks_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief --drop vital fence
/// --reset abstclk muxsel and syncclk muxsel
/// --Module align chiplets
/// --Module clock start stop
/// --Check clock stat SL, NSL , ARY
/// --drop chiplet fence
/// --check checkstop register
/// --clear flush inhibit to go into flush mode
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_nest_startclocks(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
