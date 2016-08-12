/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/core/p9_hcd_core_pcb_arb.H $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
///
/// @file  p9_hcd_core_pcb_arb.H
/// @brief Core Chiplet PCB Arbitration
///

// *HWP HWP Owner          : David Du      <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still    <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

#ifndef __P9_HCD_CORE_PCB_ARB_H__
#define __P9_HCD_CORE_PCB_ARB_H__

#include <fapi2.H>
#include <p9_hcd_common.H>

/// @typedef p9_hcd_core_pcb_arb_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_hcd_core_pcb_arb_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>&,
    const p9hcd::P9_HCD_CME_CORE_MASKS i_core_mask,
    const p9hcd::P9_HCD_PCB_ARBITER_CTRL i_request);

extern "C"
{

/// @brief Core Chiplet PCB Arbitration
///
/// @param [in] i_target TARGET_TYPE_CORE target
/// @attr
/// @attritem NONE
///
/// @retval FAPI_RC_SUCCESS
    fapi2::ReturnCode
    p9_hcd_core_pcb_arb(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
        const p9hcd::P9_HCD_CME_CORE_MASKS i_core_mask,
        const p9hcd::P9_HCD_PCB_ARBITER_CTRL i_request);

}

#endif  // __P9_HCD_CORE_PCB_ARB_H__
