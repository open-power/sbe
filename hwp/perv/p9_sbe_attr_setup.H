//------------------------------------------------------------------------------
/// @file  p9_sbe_attr_setup.H
///
/// @brief If and only if scratch registers are non-zero, HWP will read the contents of the scratch registers and call FAPI2 APIs to set the values into the corresponding platform ATTR
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


#ifndef _P9_SBE_ATTR_SETUP_H_
#define _P9_SBE_ATTR_SETUP_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_attr_setup_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief --Setting All Mailbox scratch register 1, 2, 3, 4, 5, 6, 7, 8
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_attr_setup(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
