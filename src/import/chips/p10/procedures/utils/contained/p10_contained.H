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

#include <map>
#include <fapi2.H>
#include <p10_scom_perv.H>

const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> SYS;
const fapi2::buffer<uint64_t> CONTAINED_N0_REGIONS = (fapi2::buffer<uint64_t>(0)
        .setBit<scomt::perv::CPLT_CTRL1_REGION0_FENCE_DC>()
        .setBit<scomt::perv::CPLT_CTRL1_REGION1_FENCE_DC>()   // int/nmmu/nx/vas
        .setBit<scomt::perv::CPLT_CTRL1_REGION4_FENCE_DC>()); // pe
const fapi2::buffer<uint64_t> CONTAINED_N1_REGIONS = (fapi2::buffer<uint64_t>(0)
        .setBit<scomt::perv::CPLT_CTRL1_REGION0_FENCE_DC>()
        .setBit<scomt::perv::CPLT_CTRL1_REGION2_FENCE_DC>()   // fbc/mcd
        .setBit<scomt::perv::CPLT_CTRL1_REGION4_FENCE_DC>()   // pe
        .setBit<scomt::perv::CPLT_CTRL1_REGION5_FENCE_DC>()); // nmmu
const fapi2::buffer<uint64_t> CONTAINED_EQ_REGIONS = (fapi2::buffer<uint64_t>(0)
        .setBit<scomt::perv::CPLT_CTRL1_REGION0_FENCE_DC>()
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
// Encapsulate all regions in all chiplets which participate in chip-contained
// mode in a single constant for multicast operations on all chiplets.
// Note: the "extra" EQ regions specified are unused in the N0/N1 chiplets.
const auto CONTAINED_MC_REGIONS(CONTAINED_EQ_REGIONS);

///
/// @brief Indicate an IPL where clocks are started in RUNN mode
///
/// @param[o] o_yes
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode is_runn_ipl(bool& o_yes);

///
/// @brief Indicate a cache-contained IPL
///
/// @param[o] o_yes
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode is_cac_ipl(bool& o_yes);

///
/// @brief Indicate a chip-contained IPL
///
/// @param[o] o_yes
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode is_chc_ipl(bool& o_yes);

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

fapi2::ReturnCode save_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                 const uint32_t i_active_bvec);
fapi2::ReturnCode restore_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                    const uint32_t i_active_bvec, const bool i_runn,
                                    const bool i_chc);

///
/// @brief Disable sreset-on-decrementer for all threads on cores behind a backing cache
///
/// @param[in] i_chip        Reference to chip target
/// @param[in] i_active_bvec Bitvector of cores configured to be active
///
/// Note: Only applies to chip-contained RUNN IPL
///
fapi2::ReturnCode disable_sreset_on_decr(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        const uint32_t i_active_bvec);

#ifdef P10_CONTAINED_ENABLE_SEEDING

///
/// @brief Set T0-T3 SIER registers via scan to seed exercisers
///
/// @param[in] i_chip        Reference to chip target
///
///
fapi2::ReturnCode seed_exer_threads(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip);

#else

inline fapi2::ReturnCode seed_exer_threads(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{
    return fapi2::FAPI2_RC_SUCCESS;
}

#endif // P10_CONTAINED_ENABLE_SEEDING

#endif
