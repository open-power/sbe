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
/// *HWP Level           : 2
/// *HWP Consumed by     : SBE

#ifndef __P9_SBE_PPE_FFDC_H__
#define __P9_SBE_PPE_FFDC_H__

#include <target.H>
#include <return_code.H>

static const uint32_t default_32 = 0xDEADC0DE;
static const uint64_t default_64 = 0xBADFEED0DEADC0DEULL;

// @note This enum is used to order, as well as pack, PPE register FFDC added to
//       a vector  of uint64_t. 32 bit PPE register data is packed into
//       bits 0-31 or 32-63 of each element of the vector of type uint64_t.
//       This order and packing matches that of the SPRS in error xml for
//       RC_CHECK_MASTER_STOP15_FAILED. If need be, both should change in
//       lock-step. XSR, IAR, IR, EDR and SPRG0 are read via PPE XIR SCOMs.
//       LR, SRR0 and SSR1 are read via RAMming the PPE.
typedef enum
{
    REG_FFDC_IDX_XIR_FIRST = 0,
    REG_FFDC_IDX_XSR_IAR = REG_FFDC_IDX_XIR_FIRST, // 0
    REG_FFDC_IDX_IR_EDR,                           // 1
    REG_FFDC_IDX_LR_SPRG0,                         // 2
    REG_FFDC_IDX_SPR_FIRST = REG_FFDC_IDX_LR_SPRG0,
    REG_FFDC_IDX_XIR_MAX,
    REG_FFDC_IDX_SRR0_SRR1 = REG_FFDC_IDX_XIR_MAX, // 3
    REG_FFDC_IDX_SPR_MAX,
    REG_FFDC_IDX_MAX = REG_FFDC_IDX_SPR_MAX
} SbePpeRegFfdcIdx_t;

/// @typedef p9_sbe_ppe_ffdc_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p9_sbe_ppe_ffdc_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
    const uint64_t,
    std::vector<uint64_t>&
);

/// @brief Collects PPE register data as defined by SbePpeRegFfdcIdx_t
/// @param[in]    i_target         Proc Chip Target
/// @param[in]    i_base_address   Base address of the PPE, see p9_ppe_defs.H
/// @param[out]   o_v_ppe_reg_ffdc Vector to collect PPE Internal Registers
///               On input:        An empty vector of type uint64_t with size 0
///               On ouptut:       Updated with REG_FFDC_IDX_MAX elements in the
///                                order defined by SbePpeRegFfdcIdx_t
/// @return       FAPI2_RC_SUCCESS
fapi2::ReturnCode
p9_sbe_ppe_ffdc (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t                              i_base_address,
    std::vector<uint64_t>&                o_v_ppe_reg_ffdc
);

#endif  // __P9_SBE_PPE_FFDC_H__
