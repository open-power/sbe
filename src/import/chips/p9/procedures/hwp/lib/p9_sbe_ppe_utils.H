/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_sbe_ppe_utils.H $   */
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
/// @file  p9_sbe_ppe_utils.H
/// @brief PPE RAM utility functions for SBE
///
/// *HWP HW Owner        : Greg Still <stillgs@us.ibm.com>
/// *HWP HW Backup Owner : Brian Vanderpool <vanderp@us.ibm.com>
/// *HWP FW Owner        : Amit Tendolkar <amit.tendolkar@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 1
/// *HWP Consumed by     : SBE
#if 0

#ifndef __P9_SBE_PPE_UTILS_H__
#define __P9_SBE_PPE_UTILS_H__

#include <return_code.H>
#include <target.H>

/**
 * @brief enumerates opcodes for few instructions.
 */
enum
{
    OPCODE_31         = 31,
    MTSPR_CONST1      = 467,
    MFSPR_CONST1      = 339,
    MTSPR_BASE_OPCODE = (OPCODE_31 << (31 - 5)) | (MTSPR_CONST1 << (31 - 30)),
    MFSPR_BASE_OPCODE = (OPCODE_31 << (31 - 5)) | (MFSPR_CONST1 << (31 - 30)),
};


/**
 * @brief Offsets from base address for XIRs.
 */
const static uint64_t PPE_XIXCR    = 0x0; //XCR_NONE
const static uint64_t PPE_XIRAMDBG = 0x3; //XSR_SPRG0
const static uint64_t PPE_XIRAMEDR = 0x4; //IR_EDR
const static uint64_t PPE_XIDBGPRO = 0x5; //XSR_IAR

enum PPE_SPRS
{
    LR      = 8,
    SRR0    = 26,
    SRR1    = 27,
    SPRG0   = 272,
};

/**
 * @brief generates instruction for mtspr
 * @param[in] i_Rs      source register number
 * @param[in] i_Spr represents spr where data is to be moved.
 * @return returns 32 bit instruction representing mtspr instruction.
 */
uint32_t ppe_getMtsprInstruction( const uint16_t i_Rs, const uint16_t i_Spr );

/**
 * @brief generates instruction for mfspr
 * @param[in] i_Rt      target register number
 * @param[in] i_Spr represents spr where data is to sourced
 * @return returns 32 bit instruction representing mfspr instruction.
 */
uint32_t ppe_getMfsprInstruction( const uint16_t i_Rt, const uint16_t i_Spr );

/**
 * @brief poll for Halt state
 * @param[in]   i_Rt     target register number
 * @return  fapi2::ReturnCode
 * @note    moves contents of register MSR to i_Rt register.
 */
fapi2::ReturnCode ppe_pollHaltState(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address);

/**
 * @brief halt the engine
 * @param[in]   i_target  target register number
 * @return  fapi2::ReturnCode
 * @note    programs XCR with halt bit to halt the engine.
 */
fapi2::ReturnCode ppe_halt(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address);

/**
 * @brief Perform RAM "read" operation
 * @param[in]   i_target        Chip Target
 * @param[in]   i_base_address  Base SCOM address of the PPE
 * @param[in]   i_instruction   RAM instruction to move a register
 * @param[out]  o_data          Returned data
 * @return  fapi2::ReturnCode
 */
fapi2::ReturnCode ppe_RAMRead(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address,
    const fapi2::buffer<uint64_t> i_instruction,
    fapi2::buffer<uint32_t>& o_data);


#endif  // __P9_SBE_PPE_UTILS_H__
#endif

