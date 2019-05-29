/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/core/p10_sbe_core_spr_setup.H $ */
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
///----------------------------------------------------------------------------
///
/// @file p10_sbe_core_spr_setup.H
///
/// @brief Initializes core SPRs prior to thread execution start
///
/// *HWP HW Maintainer: Nicholas Landi <nlandi@ibm.com>
/// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by  : SBE
///----------------------------------------------------------------------------

#ifndef _PROC_SBE_CORE_SPR_SETUP_H_
#define _PROC_SBE_CORE_SPR_SETUP_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi2.H>

typedef fapi2::ReturnCode
(*p10_sbe_core_spr_setup_FP_t)(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
extern "C"
{
///
/// @brief Prepares core SPRs for Hostboot start
///
/// @param[in] i_target   Reference to core target
///
/// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_sbe_core_spr_setup(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

} // extern "C"

#endif // _PROC_SBE_INSTRUCT_START_H_
