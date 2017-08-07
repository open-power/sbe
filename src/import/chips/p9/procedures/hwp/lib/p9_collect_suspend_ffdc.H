/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_collect_suspend_ffdc.H $ */
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
/// @file  p9_collect_suspend_ffdc.H
/// @brief Dumps a minimal and critical state of a PPE from SBE
///
/// *HWP HW Owner        : Adam Hale <Adam.Samuel.Hale@ibm.com>
/// *HWP HW Backup Owner : Brian Vanderpool <vanderp@us.ibm.com>
/// *HWP FW Owner        : Raja Das
/// *HWP Team            : PM
/// *HWP Level           : 3
/// *HWP Consumed by     : SBE
#if 0 // tmp change to enable manual mirror to ppe
#ifndef __P9_COLLECT_SUSPEND_FFDC_H__
#define __P9_COLLECT_SUSPEND_FFDC_H__

#include <error_info_defs.H>
#include <return_code.H>

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

/// @typedef p9_collect_suspend_ffdc_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_collect_suspend_ffdc_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&
);

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

/// @brief Collects FFDC related to the Complex Suspend failure
/// @param [in] i_target Master Chip target which failed entering Complex Suspend
/// @return On success, a ReturnCode object with the input HWP return code
///                     and relevant FFDC added to it
///         On failure, a ReturnCode object with the input HWP return code
///                     and partial or default FFDC added to it
///
fapi2::ReturnCode
p9_collect_suspend_ffdc (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target
);

#endif  // __P9_COLLECT_SUSPEND_FFDC_H__
#endif
