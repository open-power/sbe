//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_enable_ridi.H
///
/// @brief enables ridi bits in RC regs after scan initialize and start clock the pervasive chiplet
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


#ifndef _P9_SBE_TP_ENABLE_RIDI_H_
#define _P9_SBE_TP_ENABLE_RIDI_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_enable_ridi_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief Procedure enables ri_di bits
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_tp_enable_ridi(const
                                            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
