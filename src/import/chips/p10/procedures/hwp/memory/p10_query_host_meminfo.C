/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/memory/p10_query_host_meminfo.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
// EKB-Mirror-To: hw/ppe

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "p10_query_host_meminfo.H"
#include <p10_scom_c_d.H>

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

fapi2::ReturnCode p10_query_host_meminfo(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE >& i_bootCore,
    fapi2::Target < fapi2::TARGET_TYPE_PROC_CHIP >& o_target,
    uint32_t& o_sizeHostMem, uint64_t& o_hrmor, const uint32_t i_mode)
{
    FAPI_INF("p10_query_host_meminfo: Entering ...");

    fapi2::buffer<uint64_t> l_data64;
    fapi2::ATTR_ECO_MODE_Type l_eco_mode;
    // size of host memory in MB
    o_sizeHostMem = 0x0;
    o_hrmor = 0x0;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ECO_MODE, i_bootCore, l_eco_mode));
    FAPI_ASSERT(l_eco_mode == fapi2::ENUM_ATTR_ECO_MODE_DISABLED,
                fapi2::P10_QUERY_HOST_MEMINFO_CORE_ECO_ERROR().
                set_MASTER_CORE(i_bootCore),
                "p10_query_host_meminfo: Input target is in ECO mode!");

    // reflect back parent target
    o_target = i_bootCore.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    // read hostboot state from core (scratch #1)
    FAPI_TRY(fapi2::getScom( i_bootCore,
                             scomt::c::EC_PC_SCR1,
                             l_data64 ));

    FAPI_IMP("getScom Done: Address:[0x%08X%08X]: Data Read[0x%08X%08X]",
             (scomt::c::EC_PC_SCR1 >> 32), (scomt::c::EC_PC_SCR1 & 0xFFFFFFFF),
             (l_data64 >> 32), (l_data64 & 0xFFFFFFFF) );

    FAPI_ASSERT(l_data64 != 0x0ULL,
                fapi2::P10_QUERY_HOST_MEMINFO_STATE_DETERMINATION_ERR().
                set_MASTER_CORE(i_bootCore),
                "p10_query_host_meminfo: Unable to determine hostboot state");

    // extract hostboot state data
    l_data64.extractToRight < 52, (63 - 52) + 1 > (o_sizeHostMem);
    // extract hrmor written by hostboot
    l_data64.extractToRight < 4 , (51 - 4 ) + 1 > (o_hrmor);

    FAPI_ASSERT(o_sizeHostMem,
                fapi2::P10_QUERY_HOST_MEMINFO_INVALID_SIZE().
                set_MASTER_CORE(i_bootCore),
                "p10_query_host_meminfo: Core Scratch 1 says Host memory size is 0, which is wrong");

    FAPI_ASSERT(o_hrmor,
                fapi2::P10_QUERY_HOST_MEMINFO_INVALID_HRMOR().
                set_MASTER_CORE(i_bootCore),
                "p10_query_host_meminfo: Core Scratch 1 says HRMOR is 0, which is wrong");

    // l_sizeHostMem = Number of MBs of memory to dump (hex format)
    FAPI_IMP("p10_query_host_meminfo: size of hostboot memory[0x%08X], HRMOR[0x%08X%08X]",
             o_sizeHostMem, (o_hrmor >> 32), (o_hrmor & 0xFFFFFFFF));

fapi_try_exit:
    FAPI_INF("p10_query_host_meminfo: Exiting...");
    return fapi2::current_err;
}
