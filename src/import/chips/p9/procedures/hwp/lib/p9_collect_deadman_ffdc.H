/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_collect_deadman_ffdc.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
/// @file  p9_collect_deadman_ffdc.H
/// @brief Collects PPE State FFDC based on fails in the Deadman Timer
///
/// *HWP HW Owner        : Greg Still <stillgs@us.ibm.com>
/// *HWP HW Backup Owner : Brian Vanderpool <vanderp@us.ibm.com>
/// *HWP FW Owner        : Amit Tendolkar <amit.tendolkar@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 2
/// *HWP Consumed by     : SBE
#ifndef __P9_COLLECT_DEADMAN_FFDC_H__
#define __P9_COLLECT_DEADMAN_FFDC_H__

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <return_code.H>
#include <hwp_return_codes.H>

typedef enum
{
    CHECK_MASTER_STOP15_INVALID_STATE,
    CHECK_MASTER_STOP15_TIMEDOUT,
    CHECK_MASTER_STOP15_UNKNOWN
}   p9SbeCheckMasterStop15RC_t;

/// @typedef p9_ppe_state_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_collect_deadman_ffdc_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>&,
    const p9SbeCheckMasterStop15RC_t
);

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

/// @brief Collects FFDC related to the deadman failure
/// @param [in] i_target Master core target which failed entering STOP15
/// @param [in] i_reason Reason code to be added to the FFDC data
/// @return On success, a ReturnCode object with the input HWP return code
///                     and relevant FFDC added to it
///         On failure, a ReturnCode object with the input HWP return code
///                     and partial or default FFDC added to it
fapi2::ReturnCode
p9_collect_deadman_ffdc (
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
    const p9SbeCheckMasterStop15RC_t              i_reason );

#endif  // __P9_COLLECT_DEADMAN_FFDC_H__
