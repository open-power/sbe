/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_check_quiesce.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
//-----------------------------------------------------------------------------------
//
/// @file p10_sbe_check_quiesce.H
/// @brief Check quiesce state for all units on the powerbus
///
/// *HWP HWP Maintainer: Rajees Rahman  <rajerpp1@in.ibm.com>
/// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by: SBE
// ----------------------------------------------------------------------------------
//
// *! ADDITIONAL COMMENTS :
// *!
// *! The purpose of this procedure is to check quiesce state for all units on
// *! the powerbus on its chip if the quiesce fails then this HWP will checkstop
// *! the system
// *!
// *! Succcessful operation assumes that:
// *!   o System clocks are running
// *!   o Fabric is initalized
// *!
//-----------------------------------------------------------------------------------

#ifndef _P10_SBE_CHECK_QUIESCE_H_
#define _P10_SBE_CHECK_QUIESCE_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <fapi2.H>

//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p10_sbe_check_quiesce_FP_t)
(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------

    /// @brief SBE will check quiesce state for all units on the powerbus on
    ///        its chip. If the quiesce fails then this HWP will checkstop
    ///        the system
    /// @param[in] i_target       => Reference to processor chip target
    /// @return FAPI_RC_SUCCESS if the check_quiesce completes successfully
    fapi2::ReturnCode p10_sbe_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

    /// @brief Helper function to check the quiesce for PSIHB
    /// @param[in] i_target       => Reference to processor chip target
    /// @return FAPI_RC_SUCCESS if the check_quiesce completes successfully
    fapi2::ReturnCode p10_psihb_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);
} //extern "C"

#endif //_P10_SBE_CHECK_QUIESCE_H_
