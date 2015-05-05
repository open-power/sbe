//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_chiplet_init3.H
///
/// @brief SBE Pervasive Init Procedure 3
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


#ifndef _P9_SBE_TP_CHIPLET_INIT3_H_
#define _P9_SBE_TP_CHIPLET_INIT3_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_chiplet_init3_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief -- Switches PRV Chiplet OOB mux
/// -- Reset PCB Master Interrupt Register
/// -- Drop Pervasive and OCC2PIB Fence in GP0 (bits 19 & 63)
/// --"Clock Start" command (all other clk domains)
/// -- Clear force_align in chiplet GP0
/// -- Clear flushmode_inhibit in chiplet GP0
/// -- Drop FSI fence 5 (checkstop, interrupt conditions)
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_tp_chiplet_init3(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
