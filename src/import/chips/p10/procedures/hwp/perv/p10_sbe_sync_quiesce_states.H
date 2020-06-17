/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_sync_quiesce_states.H $ */
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
/// @file  p10_sbe_sync_quiesce_states.H
///
/// @brief To Check on Quiescing of all Chips in a System by Local SBE
///     Local SBE to poll LQA Bit(Bit3) of Security Swich register(x10005) all other chips on the system
///     FW code to handle timeout scenarios if a chip isn't quiescing - LQA bit not getting set
///
//------------------------------------------------------------------------------
// *HWP HW Owner        : Santosh Balasubramanian <sbalasub@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Murulidhar Nataraju <murulidhar@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#ifndef P10_SBE_SYNC_QUIESCE_STATES_H_
#define P10_SBE_SYNC_QUIESCE_STATES_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p10_sbe_sync_quiesce_states_FP_t)(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
        uint8_t&);
/// @brief To Check on Quiescing of all Chips in a System by Local SBE
///     Local SBE to poll LQA Bit(Bit3) of Security Swich register(x10005) all other chips on the system
///     FW code to handle timeout scenarios if a chip isn't quiescing - LQA bit not getting set
///
/// @param[in]     i_target_chip      Reference to TARGET_TYPE_PROC_CHIP target
/// @param[out]    o_quiesced_status  Quiesce Status
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p10_sbe_sync_quiesce_states(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
            uint8_t& o_quiesced_status);
}

#endif
