/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained.H $ */
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

#ifndef _P10_CONTAINED_H_
#define _P10_CONTAINED_H_

#include <fapi2.H>
#include <p10_scom_perv.H>

const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> SYS;
const fapi2::buffer<uint64_t> CHC_N0_REGIONS_NO_PERV = (fapi2::buffer<uint64_t>(0)
        .setBit<scomt::perv::CPLT_CTRL1_REGION1_FENCE_DC>()   // int/nmmu/nx/vas
        .setBit<scomt::perv::CPLT_CTRL1_REGION4_FENCE_DC>()); // pe
const fapi2::buffer<uint64_t> CHC_N1_REGIONS_NO_PERV = (fapi2::buffer<uint64_t>(0)
        .setBit<scomt::perv::CPLT_CTRL1_REGION2_FENCE_DC>()   // fbc/mcd
        .setBit<scomt::perv::CPLT_CTRL1_REGION4_FENCE_DC>()   // pe
        .setBit<scomt::perv::CPLT_CTRL1_REGION5_FENCE_DC>()   // nmmu
        .setBit<scomt::perv::CPLT_CTRL1_REGION6_FENCE_DC>()); // psi
const fapi2::buffer<uint64_t> CHC_EQ_REGIONS_NO_PERV = (fapi2::buffer<uint64_t>(0)
        .setBit<scomt::perv::CPLT_CTRL1_REGION1_FENCE_DC>()    // ecl20
        .setBit<scomt::perv::CPLT_CTRL1_REGION2_FENCE_DC>()    // ecl21
        .setBit<scomt::perv::CPLT_CTRL1_REGION3_FENCE_DC>()    // ecl22
        .setBit<scomt::perv::CPLT_CTRL1_REGION4_FENCE_DC>()    // ecl23
        .setBit<scomt::perv::CPLT_CTRL1_REGION5_FENCE_DC>()    // l30
        .setBit<scomt::perv::CPLT_CTRL1_REGION6_FENCE_DC>()    // l31
        .setBit<scomt::perv::CPLT_CTRL1_REGION7_FENCE_DC>()    // l32
        .setBit<scomt::perv::CPLT_CTRL1_REGION8_FENCE_DC>()    // l33
        .setBit<scomt::perv::CPLT_CTRL1_REGION9_FENCE_DC>()    // qme
        .setBit<scomt::perv::CPLT_CTRL1_REGION11_FENCE_DC>()   // mma0
        .setBit<scomt::perv::CPLT_CTRL1_REGION12_FENCE_DC>()   // mma1
        .setBit<scomt::perv::CPLT_CTRL1_REGION13_FENCE_DC>()   // mma2
        .setBit<scomt::perv::CPLT_CTRL1_REGION14_FENCE_DC>()); // mma3
// Encapsulate all regions in all chiplets which participate in cache-contained
// mode in a single constant for multicast operations on all chiplets.
// Note: the "extra" EQ regions specified are unused in the N0/N1 chiplets.
const auto CHC_MC_REGIONS_NO_PERV(CHC_EQ_REGIONS_NO_PERV);

///
/// @brief Read attribute(s) to determine if clocks should be started in RUNN
///        or free-running mode
///
/// @param[o] o_is_runn Indicates clocks should be started in RUNN mode
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode get_contained_run_mode(bool& o_is_runn);

///
/// @brief Read attribute(s) to determine contained IPL type
///
/// @param[o] o_is_cache_contained Indicates cache-contained boot
/// @param[o] o_is_chip_contained Indiciates chip-contained boot
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode get_contained_ipl_type(bool& o_is_cache_contained,
        bool& o_is_chip_contained);

///
/// @brief Determine if a core's cache is configured as "active"
///
/// @param[in] i_corenum Number of core to check (0-31)
/// @param[in] i_active_bvec Bitvector of cores configured to be active
///
/// @return true if core is configured as active else false
///
inline bool is_active_core(const size_t i_corenum, const uint32_t i_active_bvec)
{
    return ((1ul << (31 - i_corenum)) & i_active_bvec);
}

#endif
