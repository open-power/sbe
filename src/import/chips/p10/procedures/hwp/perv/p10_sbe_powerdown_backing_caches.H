/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_powerdown_backing_caches.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file  p10_sbe_powerdown_backing_caches.H
/// @brief  Determine the powered on caches and intimates QME to put the cores
///         to stop 11
// *HWP HWP Owner: Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner : Prasad Bg Ranganath <prasadbgr@in.ibm.com>
// *HWP Team: PM
// *HWP Level: 2
// *HWP Consumed by: SBE
///
///-----------------------------------------------------------------------------

#ifndef _P10_SBE_POWERDOWN_BACKING_CACHES_H_
#define _P10_SBE_POWERDOWN_BACKING_CACHES_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi2.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------


// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p10_sbe_powerdown_backing_caches_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

extern "C"
{

//------------------------------------------------------------------------------
// Function prototype
//------------------------------------------------------------------------------

/// @brief @brief Set/reset the Block Wake-up function in the QME for the
//                targeted core.
///
/// @param[in] i_proc_target    Proc target
///
/// @return FAPI2_RC_SUCCESS if success, else error code.

    fapi2::ReturnCode
    p10_sbe_powerdown_backing_caches(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_proc_target);

} // extern "C"

#endif // _P10_SBE_POWERDOWN_BACKING_CACHES_H_
