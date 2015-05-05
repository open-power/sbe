//------------------------------------------------------------------------------
/// @file  p9_start_cbs.H
///
/// @brief Start CBS : Trigger CBS
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


#ifndef _P9_START_CBS_H_
#define _P9_START_CBS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_start_cbs_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief --check/wait for VDN_PGOOD = 1
/// --check for OSC ok
/// --check for VDD (VDD status check)
/// --start CBS(CBS runs thru default path)
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_start_cbs(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>
                                   & i_target_chip);
}

#endif
