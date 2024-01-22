/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/ocmb/odyssey/procedures/hwp/utils/ody_ppe_state.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2024                             */
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
/// @file  ody_ppe_state.H
/// @brief Dump a PPE engine's state.
///
/// *HWP HW Owner        : Greg Still <stillgs.@us.ibm.com>
/// *HWP HW Backup Owner : Brian Vanderpool <vanderp@us.ibm.com>
/// *HWP FW Owner        : Prasad BG Ranganath <prasadbgr@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 2
/// *HWP Consumed by     : SBE, Cronus
//
/// EKB-Mirror-To: hw/ppe
//
#include "ody_ppe_instance_defs.H"

#ifndef __ODY_PPE_STATE_H__
#define __ODY_PPE_STATE_H__

enum ODY_PPE_DUMP_MODE
{
    O_NONE           = 0x0,
    O_XIRS           = 0x4,
    O_SNAPSHOT       = 0x1,
    O_HALT           = 0x2,
    O_FORCE_HALT     = 0x3
};

enum ODY_VERBOSE_MODE
{
    O_NOVERBOSE        = 0x0,
    O_VERBOSE          = 0x1,
    O_VERBOSEP         = 0x2,
    O_VERBOSE1         = 0x3,

};

typedef struct
{
    uint16_t    number;
    uint32_t    value;
    const char*  name;
} Reg32Val_t;



/// @typedef ody_ppe_state_FP_t
/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*ody_ppe_state_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>&,
    enum PPE_TYPES i_ppe_type, uint32_t i_ppe_instance_num, const ODY_PPE_DUMP_MODE,
    std::vector<Reg32Val_t>& v_ppe_gprs_value,
    std::vector<Reg32Val_t>& v_ppe_sprs_value,
    std::vector<Reg32Val_t>& v_ppe_xirs_value);

extern "C"
{

/// @brief Dump the PPE state based on the based base address
/// @param [in] i_target TARGET_TYPE_PROC_CHIP
/// @param [in] i_base_address Base offset to be used for all accesses
/// @return FAPI2_RC_SUCCESS
    fapi2::ReturnCode
    ody_ppe_state(const fapi2::Target<fapi2::TARGET_TYPE_OCMB_CHIP>& i_target,
                  enum PPE_TYPES i_ppe_type, uint32_t i_ppe_instance_num, const ODY_PPE_DUMP_MODE i_mode,
                  std::vector<Reg32Val_t>& v_ppe_gprs_value,
                  std::vector<Reg32Val_t>& v_ppe_sprs_value,
                  std::vector<Reg32Val_t>& v_ppe_xirs_value);

} // extern C

#endif  // __P10_PPE_STATE_H__
