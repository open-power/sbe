/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/memory/p10_query_host_meminfo.C $ */
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
/// @file  p10_query_host_meminfo.C
///
/// @brief Returns the list of hostboot memory ranges to dump
//------------------------------------------------------------------------------
// *HWP HW Owner        : Alpana Kumari <alpankum@in.ibm.com>
// *HWP FW Owner        : Daniel M. Crowell   <dcrowell@us.ibm.com>
// *HWP Team            : MEMORY
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "p10_query_host_meminfo.H"
#include <p10_scom_c_d_unused.H>

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

fapi2::ReturnCode p10_query_host_meminfo(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target,
    fapi2::Target < fapi2::TARGET_TYPE_PROC_CHIP | fapi2::TARGET_TYPE_MULTICAST > & o_target,
    std::vector< std::pair<uint64_t, uint64_t> >& o_ranges)
{
    FAPI_INF("p10_query_host_meminfo: Entering ...");

    fapi2::ReturnCode l_rc;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > multicast_core_target;
    // size of host memory in MB
    uint64_t l_sizeHostMem = 0x0;
    uint64_t l_hrmor = 0x0;

    multicast_core_target = i_target;

    // reflect back parent target
    o_target = multicast_core_target.getParent
               < fapi2::TARGET_TYPE_PROC_CHIP | fapi2::TARGET_TYPE_MULTICAST > ();

    // read hostboot state from multicast core (scratch #1)
    l_rc = fapi2::getScom(multicast_core_target,
                          EC_PC_SCR1,
                          l_data64);

    if (l_rc)
    {
        FAPI_ERR("p10_query_host_meminfo:: getScom failed to read hostboot state from multicast_core_target, bailing out of function." );
        FAPI_ASSERT( false,
                     fapi2::P10_QUERY_HOST_MEMINFO_CORE_SCOM_ERROR().
                     set_MASTER_CORE(multicast_core_target),
                     "p10_query_host_meminfo: Unable to access core scratch reg 1 on master core");
    }
    else
    {
        FAPI_INF("getScom Done: Address:[0x%016llX]: Data Read[0x%016llX]",
                 HB_STATE, static_cast<uint64_t>(l_data64));

        FAPI_ASSERT( l_data64 != 0x0ULL,
                     fapi2::P10_QUERY_HOST_MEMINFO_STATE_DETERMINATION_ERR().
                     set_MASTER_CORE(multicast_core_target),
                     "p10_query_host_meminfo: Unable to determine hostboot state");

        // extract hostboot state data
        l_data64.extractToRight < 52, (63 - 52) + 1 > (l_sizeHostMem);
        // extract hrmor written by hostboot
        l_data64.extractToRight < 4 , (51 - 4 ) + 1 > (l_hrmor);
    }

    if(!l_sizeHostMem)
    {
        FAPI_ERR("p10_query_host_meminfo:: Core Scratch 1 says hostboot memory size is 0, something is wrong, bailing out of function. " );
        FAPI_ASSERT( false,
                     fapi2::P10_QUERY_HOST_MEMINFO_INVALID_SIZE().
                     set_MASTER_CORE(multicast_core_target),
                     "p10_query_host_meminfo: Core Scratch 1 says Host memory size is 0, which is wrong");
    }
    else if(!l_hrmor)
    {
        FAPI_ERR("p10_query_host_meminfo:: Core Scratch 1 says HRMOR is 0, something is wrong, bailing out of function. " );
        FAPI_ASSERT( false,
                     fapi2::P10_QUERY_HOST_MEMINFO_INVALID_HRMOR().
                     set_MASTER_CORE(multicast_core_target),
                     "p10_query_host_meminfo: Core Scratch 1 says HRMOR is 0, which is wrong");
    }

    FAPI_INF("p10_query_host_meminfo: size of hostboot memory = 0x%16llX   HRMOR = 0x%16llx",
             l_sizeHostMem, l_hrmor);


    // l_sizeHostMem = Number of MBs of memory to dump (hex format)
    for (uint8_t i = 0; i < l_sizeHostMem; i++)
    {
        o_ranges.push_back(std::make_pair( l_hrmor +
                                           (i * P10_QUERY_HOST_MEMINFO_MB),
                                           P10_QUERY_HOST_MEMINFO_MB));
    }

fapi_try_exit:

    FAPI_INF("p10_query_host_meminfo: Exiting...");
    return fapi2::current_err;
}
