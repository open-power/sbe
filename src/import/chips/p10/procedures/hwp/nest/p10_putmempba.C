/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_putmempba.C $    */
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
///
/// @file p10_putmempba.C
/// @brief Invoke PBA putmem chipop.
///
/// *HWP HW Maintainer: Thi Tran <thi@us.ibm.com>
/// *HWP FW Maintainer: Ilya Smirnov <ismirno@us.ibm.com>
/// *HWP Consumed by: CRONUS
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_putmempba.H>
#include <p10_getputmempba_utils.H>
#include <p10_pba_setup.H>
#include <p10_pba_access.H>
#include <p10_pba_coherent_utils.H>

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

/// NOTE: doxygen in header
fapi2::ReturnCode p10_putmempba(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
    const uint64_t i_address,
    const uint32_t i_bytes,
    const uint8_t* i_data,
    const uint32_t i_mem_flags)
{

    p10_PBA_oper_flag l_flags;
    uint64_t l_target_address = i_address;
    uint64_t l_end_address = i_address + i_bytes;
    uint32_t l_granules_before_setup = 0;
    uint32_t l_granule = 0;
    bool l_first_access = true;
    uint8_t l_granuleSize = 128;

    FAPI_DBG("Start");

    // Validate flags
    FAPI_TRY(p10_validateSbeMemoryAccessFlagsPBA(i_target, i_address, i_bytes, i_mem_flags),
             "Error returned from p10_validateSbeMemoryAccessFlags_pba()");

    FAPI_TRY(p10_getPBAFlags(i_target, i_address, i_bytes, i_mem_flags, l_flags),
             "Error returned from p10_getPBAFlags()");

    // Granule size is 8 bytes for ATOMIC or CI operation
    if ( (l_flags.getOperationType() == p10_PBA_oper_flag::ATOMIC) ||
         (l_flags.getOperationType() == p10_PBA_oper_flag::CI) )
    {
        l_granuleSize = 8;
    }

    // Write data
    while (l_target_address < l_end_address)
    {

        // Invoke PBA setup HWP to prepare current stream of contiguous granules
        FAPI_TRY(p10_pba_setup(i_target,
                               i_core_target,
                               l_target_address,
                               false,
                               l_flags.setFlag(),
                               l_granules_before_setup),
                 "Error from p10_pba_setup");

        FAPI_DBG("Granules before setup: %d", l_granules_before_setup);

        l_first_access = true;

        while (l_granules_before_setup && (l_target_address < l_end_address))
        {
            // invoke PBA access HWP to move one granule
            uint8_t l_data[l_granuleSize];
            memset(l_data, 0, l_granuleSize);

            // Load data into array to write.
            for (uint32_t ii = 0; ii < l_granuleSize; ii++)
            {
                l_data[ii] = i_data[(l_granule * l_granuleSize) + ii];
            }

            FAPI_TRY(p10_pba_access(i_target,
                                    l_target_address,
                                    false,
                                    l_flags.setFlag(),
                                    l_first_access,
                                    (l_granules_before_setup == 1) ||
                                    ((l_target_address + 8) >=
                                     l_end_address),
                                    l_data),
                     "Error from p10_pba_access");
            l_first_access = false;
            l_granules_before_setup--;
            l_target_address += l_granuleSize;
            l_granule++;
        }
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}
