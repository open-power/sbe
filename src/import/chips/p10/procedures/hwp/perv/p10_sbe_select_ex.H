/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_select_ex.H $ */
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
///
/// @file p10_sbe_select_ex.H
/// @brief Select the Hostboot core from the available cores on the chip
///
// *HWP Owner       : Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner    : Prasad Bg Ranganath <prasadpbr@in.ibm.com>
// *HWP Team        : PM
// *HWP Consumed by : SBE
///

#ifndef _SBE_SELECT_EX_H_
#define _SBE_SELECT_EX_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi2.H>
#if 0
    #include <p9_misc_scom_addresses.H>
    #include <p9_quad_scom_addresses.H>
    #include <p9_perv_scom_addresses.H>
    #include <p9n2_perv_scom_addresses_fld.H>
    #include <p9_quad_scom_addresses_fld.H>
#endif

namespace selectex
{
// valid domain options
enum  MODE
{
    SINGLE,         // Only the first core
    SINGLE_NONE_OK, // Only the first core, don't error if none are present
    ALL             // All Core
};

} // namespace p10selectex


// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p10_sbe_select_ex_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
    selectex::MODE);

extern "C" {

// -----------------------------------------------------------------------------
//  Function prototype
// -----------------------------------------------------------------------------

/// @brief Select the Hostboot core from the available cores on the chip
///
/// @param [in] i_target    Chip target
/// @param [in] i_mode      SINGLE core (enable only the first core set found);
///                         ALL cores (enable all configured cores found)
///
    fapi2::ReturnCode p10_sbe_select_ex(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        selectex::MODE i_mode);

} // extern "C"

#endif // _SBE_SELECT_EX_H_
