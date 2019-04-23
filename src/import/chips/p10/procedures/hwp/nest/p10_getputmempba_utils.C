/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_getputmempba_utils.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file p10_getputmempba_utils.C
///
/// @brief Common code to support PBA get/putmem procedures.
///        Note: This file is not intended to be imported to SBE platform
///              to save its space.
///
/// *HWP HW Maintainer: Thi Tran <thi@us.ibm.com>
/// *HWP FW Maintainer: N/A
/// *HWP Consumed by: Cronus
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_getputmempba_utils.H>
#include <fapi2_mem_access.H>
#include <p10_pba_coherent_utils.H>

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////
// p10_validateSbeMemoryAccessFlagsPBA
////////////////////////////////////////////////////////
fapi2::ReturnCode p10_validateSbeMemoryAccessFlagsPBA(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_address,
    const uint32_t i_bytes,
    const uint32_t i_mem_flags)
{
    FAPI_DBG("Start");
    FAPI_DBG("p10_validateSbeMemoryAccessFlagsPBA: i_mem_flags 0x%.8X", i_mem_flags);
    FAPI_ASSERT( ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA)  == fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PROC) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PB_DIS_MODE) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_SWITCH_MODE) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PB_INIT_MODE) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PRE_SWITCH_CD_MODE) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PRE_SWITCH_AB_MODE) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_POST_SWITCH_MODE) == 0) &&
                 ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_AUTO_INCR_ON) == 0),
                 fapi2::P10_GETPUTMEMPBA_INVALID_FLAGS().
                 set_TARGET(i_target).
                 set_ADDRESS(i_address).
                 set_BYTES(i_bytes).
                 set_FLAGS(i_mem_flags).
                 set_FLAG_CHECK_OP_TYPE(((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA) ==
                                         fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA) &&
                                        ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PROC)  == 0)).
                 set_FLAG_CHECK_PB_DIS_MODE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PB_DIS_MODE) == 0).
                 set_FLAG_CHECK_SWITCH_MODE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_SWITCH_MODE) == 0).
                 set_FLAG_CHECK_PB_INIT_MODE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PB_INIT_MODE) == 0).
                 set_FLAG_CHECK_PRE_SWITCH_CD_MODE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PRE_SWITCH_CD_MODE) == 0).
                 set_FLAG_CHECK_PRE_SWITCH_AB_MODE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_PRE_SWITCH_AB_MODE) == 0).
                 set_FLAG_CHECK_POST_SWITCH_MODE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_POST_SWITCH_MODE) == 0).
                 set_FLAG_CHECK_AUTO_INCR_ON((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_AUTO_INCR_ON) == 0),
                 "Invalid flag specified for PBA access");

    FAPI_ASSERT(((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_ECC_OVERRIDE) == 0) &&
                ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_TAG) == 0) &&
                ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_HOST_PASS_THROUGH) == 0),
                fapi2::P10_GETPUTMEMPBA_UNSUPPORTED_FLAGS().
                set_TARGET(i_target).
                set_ADDRESS(i_address).
                set_BYTES(i_bytes).
                set_FLAGS(i_mem_flags).
                set_FLAG_CHECK_ECC_OVERRIDE((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_ECC_OVERRIDE) == 0).
                set_FLAG_CHECK_TAG((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_TAG) == 0).
                set_FLAG_CHECK_HOST_PASS_THROUGH((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_HOST_PASS_THROUGH) == 0),
                "Unsupported flag specified for PBA access");
fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

////////////////////////////////////////////////////////
// p10_getPBAFlags
////////////////////////////////////////////////////////
fapi2::ReturnCode p10_getPBAFlags(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_address,
    const uint32_t i_bytes,
    const uint32_t i_mem_flags,
    p10_PBA_oper_flag& o_pbaFlags)
{
    FAPI_DBG("Start");

    // set fast mode
    o_pbaFlags.setFastMode((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON) ==
                           fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON);

    // set operation type
    if ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE) ==
        fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE)
    {
        o_pbaFlags.setOperationType(p10_PBA_oper_flag::LCO);
    }
    else if ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_ATOMIC_MODE) ==
             fapi2::SBE_MEM_ACCESS_FLAGS_ATOMIC_MODE)
    {
        o_pbaFlags.setOperationType(p10_PBA_oper_flag::ATOMIC);
        // set Atomic type
        uint8_t l_atomicType = (i_mem_flags >> 17) & 0x3; // Bits 13-14

        if (l_atomicType == 0)
        {
            o_pbaFlags.setAtomicOpType(p10_PBA_oper_flag::ADD);
        }
        else if (l_atomicType == 1)
        {
            o_pbaFlags.setAtomicOpType(p10_PBA_oper_flag::AND);
        }
        else if (l_atomicType == 2)
        {
            o_pbaFlags.setAtomicOpType(p10_PBA_oper_flag::OR);
        }
        else
        {
            o_pbaFlags.setAtomicOpType(p10_PBA_oper_flag::XOR);
        }
    }
    else if ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_CACHE_INJECT_MODE) ==
             fapi2::SBE_MEM_ACCESS_FLAGS_CACHE_INJECT_MODE)
    {
        o_pbaFlags.setOperationType(p10_PBA_oper_flag::INJ);
    }
    else if ((i_mem_flags & fapi2::SBE_MEM_ACCESS_FLAGS_CACHE_INHIBITED_MODE) ==
             fapi2::SBE_MEM_ACCESS_FLAGS_CACHE_INHIBITED_MODE)
    {
        o_pbaFlags.setOperationType(p10_PBA_oper_flag::CI);
    }
    else // Default to DMA mode
    {
        o_pbaFlags.setOperationType(p10_PBA_oper_flag::DMA);
    }

    FAPI_DBG("End");
    return fapi2::current_err;
}
