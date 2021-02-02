/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_dft.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
/* EKB-Mirror-To: hw/ppe                                                  */
///----------------------------------------------------------------------------
///
/// @file p10_contained_dft.H
///
/// @brief Discretized procedures from p10_contained_ipl.H
//         Used to reduce a long contained ipl procedure into manageable
//         procedures for SBE consumption at manufacturing
///
/// *HWP HW Maintainer: Justin Ginn <justin.ginn@ibm.com>
/// *HWP Consumed by  : SBE
///----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi2.H>

typedef fapi2::ReturnCode (*p10_cotnained_save_config_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

typedef fapi2::ReturnCode (*p10_cotnained_stopclocks_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

typedef fapi2::ReturnCode (*p10_contained_setup_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

typedef fapi2::ReturnCode (*p10_contained_scan0_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

typedef fapi2::ReturnCode (*p10_restore_l3_config_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
extern "C"
{
///
/// @brief
///
/// @param[in] i_runn        Specify whether we are in runn mode
///
/// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_contained_save_config(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///
/// @brief
///
/// @param[in] i_runn        Specify whether we are in runn mode
///
/// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_contained_stopclocks(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///
/// @brief
///
/// @param[in] i_runn        Specify whether we are in runn mode
///
/// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_contained_setup(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///
/// @brief
///
///
/// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_contained_restore(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

///
/// @brief Restore L3 configuration during contained IPL
///
/// @param[in] i_target      Reference to core target
/// @param[in] i_thread_mask Threads to start
///
/// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_contained_restore_l3_config(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);
} // extern "C"
