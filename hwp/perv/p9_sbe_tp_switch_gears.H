//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_switch_gears.H
///
/// @brief Switch from refclock to PLL AND adjust I2C
//------------------------------------------------------------------------------
// *HWP HW Owner        : abagarw8 <srinivan@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumarj8@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#ifndef _P9_SBE_TP_SWITCH_GEARS_H_
#define _P9_SBE_TP_SWITCH_GEARS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_switch_gears_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> &);

/// @brief Switch from refclcok to PLL speed (leave bypass)
/// Read new I2C Bit Rate Divisor setting from mailbox
/// Adjust I2C bit rate divisor setting in I2CM B mode reg
/// Send a stop sequence on I2C
/// Poll for stop command completion
/// Check for magic number
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_tp_switch_gears(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target_chip);
}

#endif
