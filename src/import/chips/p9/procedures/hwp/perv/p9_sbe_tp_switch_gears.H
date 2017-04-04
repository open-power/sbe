/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_switch_gears.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_tp_switch_gears.H
///
/// @brief Switch from refclock to PLL AND adjust I2C
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumarj8@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#ifndef _P9_SBE_TP_SWITCH_GEARS_H_
#define _P9_SBE_TP_SWITCH_GEARS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_tp_switch_gears_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

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
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);
}

fapi2::ReturnCode p9_sbe_tp_switch_gears_check_magicnumber(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

#endif
