/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_sbe_ppe_ffdc.H $    */
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
/// @file  p9_sbe_ppe_ffdc.H
/// @brief Dumps a minimal and critical state of a PPE from SBE
///
/// *HWP HW Owner        : Greg Still <stillgs@us.ibm.com>
/// *HWP HW Backup Owner : Brian Vanderpool <vanderp@us.ibm.com>
/// *HWP FW Owner        : Amit Tendolkar <amit.tendolkar@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 1
/// *HWP Consumed by     : SBE
#if 0

#ifndef __P9_SBE_PPE_FFDC_H__
#define __P9_SBE_PPE_FFDC_H__

#include <target.H>
#include <return_code.H>

// PM Stop Recovery State Collection.xlsx defines what FFDC is to be collected
// Members of this enum are ordered to optimize PPE State collection
// and should not change
typedef enum
{
    XIR_IDX_IAR_XSR = 0,
    XIR_IDX_EDR_IR,
    XIR_IDX_SPRG0,
    XIR_IDX_MAX
} SbePpeFfdcXIRIdx_t;

typedef enum
{
    SPR_IDX_LR = 0,
    SPR_IDX_SRR0,
    SPR_IDX_SRR1,
    SPR_IDX_MAX
}   SbePpeFfdcSPRIdx_t;

/// @typedef p9_sbe_ppe_ffdc_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_sbe_ppe_ffdc_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
    const uint64_t,
    std::vector<uint64_t>&,
    std::vector<uint32_t>&
);

/// @brief Collect PPE XIRs and SPRs
/// @param [in]  i_target Chip Target
/// @param [in]  i_base_address Base address of the PPE to be used for access
/// @param [out] o_v_ppe_xirs vector of XIRs collected
/// @param [out] o_v_ppe_sprs vector of SPRs collected
/// @return FAPI2_RC_SUCCESS
fapi2::ReturnCode
p9_sbe_ppe_ffdc (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address,
    std::vector<uint64_t>& o_v_ppe_xirs,
    std::vector<uint32_t>& o_v_ppe_sprs
);

#endif  // __P9_SBE_PPE_FFDC_H__
#endif
