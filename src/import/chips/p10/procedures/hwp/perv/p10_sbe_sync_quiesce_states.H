/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_sync_quiesce_states.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2022                        */
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

///----------------------------------------------------------------------------
/// Constant definitions
///----------------------------------------------------------------------------
const uint64_t SECURITY_SWITCH_REG_ADDR  = 0x00010005;   // Security Switch Register Address
const uint64_t ADU_XSCOM_BASE_ADDR       = 0x000E03FC00000000;
//Each 5-bit topology index addresses a 16 terabyte space
const uint64_t  MMIO_OFFSET_PER_TOPO_INDEX =  0x0000100000000000;

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------

/**
 * @brief bit definitions for 5-bit mode 0/1 topology inde
 */
typedef union
{
    struct // GGG0C
    {
        uint8_t rsvd1: 3; // 0:2
        uint8_t group: 3; // 3:5
        uint8_t rsvd2: 1; // 6
        uint8_t chip : 1; // 7
    } mode0;
    struct // 0GGCC
    {
        uint8_t rsvd1: 4; // 0:3
        uint8_t group: 2; // 4:5
        uint8_t chip : 2; // 6:7
    } mode1;
    uint8_t topoIndex;
} topologyIndexBits_t;

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
