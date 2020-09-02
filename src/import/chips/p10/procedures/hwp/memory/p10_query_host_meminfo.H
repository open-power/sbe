/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/memory/p10_query_host_meminfo.H $ */
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
/// @file  p10_query_host_meminfo.H
///
/// @brief Returns the list of hostboot memory ranges to dump
//------------------------------------------------------------------------------
// *HWP HW Owner        : Alpana Kumari <alpankum@in.ibm.com>
// *HWP FW Owner        : Daniel M. Crowell   <dcrowell@us.ibm.com>
// *HWP Team            : MEMORY
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#ifndef P10_QUERY_HOST_MEMINFO_H_
#define P10_QUERY_HOST_MEMINFO_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// size constants
const uint64_t P10_QUERY_HOST_MEMINFO_KB = 1024ULL;
const uint64_t P10_QUERY_HOST_MEMINFO_MB = 1024ULL * P10_QUERY_HOST_MEMINFO_KB;

// NO_MEM:
// No memory has been initialized so we can only dump our static
// code load up to 904KB.
const uint8_t P10_QUERY_HOST_MEMINFO_NO_MEM_RANGE_COUNT = 0x1;
const std::pair<uint64_t, uint64_t> P10_QUERY_HOST_MEMINFO_NO_MEM_RANGES[P10_QUERY_HOST_MEMINFO_NO_MEM_RANGE_COUNT] =
{
    std::make_pair(0, (904 * P10_QUERY_HOST_MEMINFO_KB))
};

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------

// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode
(*p10_query_host_meminfo_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& o_target,
    std::vector< std::pair<uint64_t, uint64_t> >& o_ranges,
    const uint32_t i_mode);

extern "C" {

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

    /**
     * @brief HWP which returns target/list of hostboot memory ranges to dump
     *
     * @param[in]  i_target Reference to multicast core chiplet targets
     * @param[out] o_target Reference to chip target which should be
     *                      used to extract dump content
     * @param[out] o_ranges Vector of ranges (base address/size) to dump
     *
     * @return ReturnCode
     */

    fapi2::ReturnCode p10_query_host_meminfo(
        const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target,
        fapi2::Target < fapi2::TARGET_TYPE_PROC_CHIP | fapi2::TARGET_TYPE_MULTICAST > & o_target,
        std::vector< std::pair<uint64_t, uint64_t> >& o_ranges);

} // extern "C"

#endif
// P10_QUERY_HOST_MEMINFO_H_
