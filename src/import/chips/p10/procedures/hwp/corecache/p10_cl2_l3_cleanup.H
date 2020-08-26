/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_cl2_l3_cleanup.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
/// @file  p10_cl2_l3_cleanup.C
/// @brief In mpipl down path verify cores chiplet state..if it is still running then
///        will put the core to stop 11 state
///        will put the core to stop 11 state
/// *HWP HW Owner        : Greg Still <stillgs@us.ibm.com>
/// *HWP FW Owner        : Prasad Bg Ranganath <prasadbgr@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 2
/// *HWP Consumed by     : HB,CRO
///
// EKB-Mirror-To: hw/ppe

#ifndef _p10_cl2_l3_cleanup_H_
#define _p10_cl2_l3_cleanup_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi2.H>


// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p10_cl2_l3_cleanup_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);



extern "C" {

// -----------------------------------------------------------------------------
//  Function prototype
// -----------------------------------------------------------------------------

/// @brief Select the Hostboot core from the available cores on the chip
///
/// @param [in] i_target    Chip target
///

    fapi2::ReturnCode p10_cl2_l3_cleanup(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

} // extern "C"

#endif // _p10_cl2_l3_cleanup_H_
