/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_load_bootloader.H $ */
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
/// @file p10_sbe_load_bootloader.H
/// @brief Shift HB bootloader payload from SEEPROM to L3 cache of master core
/// via PBA
///
/// *HWP HW Maintainer : Doug Holtsinger <Douglas.Holtsinger@ibm.com>
/// *HWP FW Maintainer : Raja Das        <rajadas2@in.ibm.com>
/// *HWP Consumed by   : SBE
///

#ifndef _SBE_BOOTLOADER_H_
#define _SBE_BOOTLOADER_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------
// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode(*p10_sbe_load_bootloader_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
    const fapi2::Target<fapi2::TARGET_TYPE_EX>&,
    const uint64_t,
    uint8_t*);

//------------------------------------------------------------------------------
// Function prototype
//------------------------------------------------------------------------------
//

extern "C" {

///
/// @brief Shift HB bootloader payload from SEEPROM to L3 cache of master core
///        via PBA
/// @param[in] i_master_chip_target Reference to master processor chip target
/// @param[in] i_master_ex_target Reference to master EX unit target
/// @param[in] i_payload_size Size of image payload data, in B
/// @param[in] i_payload_data Pointer to image payload data
///
/// @return FAPI_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p10_sbe_load_bootloader(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_master_ex_target,
        const uint64_t i_payload_size,
        uint8_t* i_payload_data);

} //extern "C"

#endif //_SBE_BOOTLOADER_H_
