//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_chiplet_init2.H
///
/// @brief IPL STEP 2.10 : SBE TP Chiplet Init2 :: Run scan 0 module for pervasive
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


#ifndef _P9_SBE_TP_CHIPLET_INIT2_H_
#define _P9_SBE_TP_CHIPLET_INIT2_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_chiplet_init2_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief -- Initialize TP Hangcounter 6
/// -- Scan Repair, Time and GPTR for PRV Chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_tp_chiplet_init2(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
