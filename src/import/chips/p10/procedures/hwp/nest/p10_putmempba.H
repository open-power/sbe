/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_putmempba.H $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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

/// @file p10_putmempba.C
/// @brief Invoke PBA putmem chipop.
///
/// *HWP HW Maintainer: Thi Tran <thi@us.ibm.com>
/// *HWP FW Maintainer: Ilya Smirnov <ismirno@us.ibm.com>
/// *HWP Consumed by: CRONUS
///

#ifndef _P10_PUTMEMPBA_H_
#define _P10_PUTMEMPBA_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------
// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode(*p10_putmempba_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
    const uint64_t,
    const uint32_t,
    const uint8_t*,
    const uint32_t);

//------------------------------------------------------------------------------
// Function prototype
//------------------------------------------------------------------------------
//

extern "C" {

///
/// @brief Invoke PBA putmem chipop
///
/// @param[in] i_target Reference to processor chip target
/// @param[in] i_core_target Reference to core target
/// @param[in] i_address Base address for write operation
/// @param[in] i_bytes Size of write data, in B
/// @param[in] i_data Pointer to write data
/// @param[in] i_mem_flags Flags to pass to chipop
///
/// @return FAPI_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_putmempba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
        const uint64_t i_address,
        const uint32_t i_bytes,
        const uint8_t* i_data,
        const uint32_t i_mem_flags);

} //extern "C"

#endif //_P10_PUTMEMPBA_H_
