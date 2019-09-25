/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_sim.H $ */
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
#ifndef _P10_CONTAINED_SIM_H_
#define _P10_CONTAINED_SIM_H_

#include <fapi2.H>


namespace sim
{
enum stage
{
    ISTEP3,
    ISTEP4_CACHE,
    ISTEP4_CORE
};

enum array
{
    QMEARY,
    L3ARY
};

#ifdef P10_CONTAINED_SIM

#pragma message "p10_contained_* procedures built with simulation-only hooks"


///
/// @brief Checks if environment variable P10_CONTAINED_SIM_SKIP_STOPCLOCKS is
///        set to a value.
///
/// Environment variables:
/// P10_CONTAINED_SIM_SKIP_STOPCLOCKS - export as =1 to enable, unset to disable
///
/// @return true if env[P10_CONTAINED_SIM_SKIP_STOPCLOCKS=<some value>] else false
///
bool skip_stopclocks();

///
/// @brief Checks if environment variable P10_CONTAINED_SIM_SKIP_L3PURGE is
///        set to a value.
///
/// Environment variables:
/// P10_CONTAINED_SIM_SKIP_L3PURGE - export as =1 to enable, unset to disable
///
/// @return true if env[P10_CONTAINED_SIM_SKIP_L3PURGE=<some value>] else false
///
bool skip_l3purge();

///
/// @brief Applies scan inits via dials.
///
/// Environment variables:
/// P10_CONTAINED_SIM_INIT_VIA_DIALS - export as =1 to enable, unset to disable
///
/// @param[in] i_stage Stage of contained IPL (ISTEP4_CORE, ISTEP4_CACHE, ISTEP3)
/// @param[in] i_chc Indicate if chip-contained IPL
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode init_via_dials(stage i_stage, bool i_chc);

///
/// @brief Sticks the inits to control selective skip arrayinit for QME and L3
///        arrays.
///
/// Environment variables:
/// P10_CONTAINED_SIM_APPLY_SELECTIVE_SKIP_ARRAYINIT - export as =1 to enable,
///                                                    unset to disable
///
/// @param[in] i_array Array for which to set/clr the init (QMEARY or L3ARY)
/// @param[in] i_set   Bool which sets the init when true and clears the bit
///                    when false
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode apply_selective_skip_arrayinit(const array i_array, const bool i_set);

fapi2::ReturnCode checkpoint(const bool i_chc, const bool i_runn,
                             const std::string& i_suffix);

#else // P10_CONTAINED_SIM

constexpr bool skip_stopclocks()
{
    return false;
}
constexpr bool skip_l3purge()
{
    return false;
}
inline fapi2::ReturnCode init_via_dials(stage i_stage, bool i_chc)
{
    return fapi2::FAPI2_RC_SUCCESS;
}
inline fapi2::ReturnCode apply_selective_skip_arrayinit(const array i_array, const bool set)
{
    return fapi2::FAPI2_RC_SUCCESS;
}
inline fapi2::ReturnCode checkpoint(const bool i_chc, const bool i_runn,
                                    const std::string& i_suffix)
{
    return fapi2::FAPI2_RC_SUCCESS;
}

#endif

} // namespace sim

#endif // _P10_CONTAINED_SIM_H_
