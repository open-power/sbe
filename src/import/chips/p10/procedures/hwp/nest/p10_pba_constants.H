/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_pba_constants.H $ */
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
/// @file p10_pba_constants.H
/// @brief Constant enums to support PBA get/putmem procedures (FAPI)
///
// *HWP HW Maintainer : Chris Riedl (cmr@ibm.com)
// *HWP FW Maintainer : Raja Das    (rajadas2@in.ibm.com)
// *HWP Consumed by   : SBE

#ifndef _P10_PBA_CONSTANTS_H_
#define _P10_PBA_CONSTANTS_H_

//---------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------

extern "C"
{

//----------------------------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------------------------
    enum pba_flags
    {
        // Fastmode flag
        // If this flag is set it means we will not check for errors until
        // the very last granule for the read/write
        FLAG_PBA_FASTMODE = 0x80000000ull,

        // TTYPE flag
        // PBA operation type
        // 0b000: DMA
        // 0b001: LCO_M
        // 0b010: ATOMIC
        // 0b011: CACHE_INJ
        // 0b100: CI_PR_W
        // Same as in the documentation and how they will be passed to the register
        FLAG_PBA_TTYPE    = 0x70000000ull,

        // ATOMIC OP flag
        // PBA Atomic operation type
        // 0b00000010: ADD
        // 0b00100010: AND
        // 0b01000010: OR
        // 0b01100010: XOR
        // Format corresponds to PBASLVCTL[write_tsize]
        FLAG_PBA_ATOMIC_OPTYPE = 0x0ff00000ull,
    };

// TTYPE shift position
    const uint64_t FLAG_PBA_TTYPE_SHIFT = 28;
    const uint64_t FLAG_PBA_ATOMIC_OPTYPE_SHIFT = 20;

} //extern "C"

#endif //_P10_PBA_CONSTANTS_H_
