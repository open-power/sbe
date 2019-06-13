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

const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> SYS;

///
/// @brief Read attribute(s) to determine if clocks should be started in RUNN
///        or free-running mode
///
/// @param[io] io_is_runn Indicates clocks should be started in RUNN mode
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode get_contained_run_mode(bool& is_runn);

///
/// @brief Read attribute(s) to determine contained IPL type
///
/// @param[io] io_is_cache_contained Indicates cache-contained boot
/// @param[io] io_is_chip_contained Indiciates chip-contained boot
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode get_contained_ipl_type(bool& io_is_cache_contained,
        bool& io_is_chip_contained);

///
/// @brief Determine if a core's cache is configured as "active"
///
/// @param[in] i_corenum Number of core to check (0-31)
/// @param[in] i_active_bvec Bitvector of cores configured to be active
///
/// @return true if core is configured as active else false
///
inline bool is_active_core(const size_t& i_corenum, const uint32_t& i_active_bvec)
{
    return ((1ul << (31 - i_corenum)) & i_active_bvec);
}


#endif
