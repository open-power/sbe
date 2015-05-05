//------------------------------------------------------------------------------
/// @file  p9_sbe_lpc_init.H
///
/// @brief Requirement from the bootloader is that it only uses MMIOs to LPC master, not Xscom
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


#ifndef _P9_SBE_LPC_INIT_H_
#define _P9_SBE_LPC_INIT_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_lpc_init_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief PerfoPerform scoms to setup LPC bus
///   --   Move the LPC clock to external input
/// Pull the LPC unit out of reset
/// Set LPC BAR -- hardcoded like Xscom BAR
///
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_lpc_init(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
