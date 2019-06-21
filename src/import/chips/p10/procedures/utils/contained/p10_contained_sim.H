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
/// @brief Applies a resetfile (defined via environment variables) using a
///        simulator batch command.
///
/// Environment variables:
/// P10_CONTAINED_SIM_SCAN_VIA_RESETFILE         - export as =1 to enable, unset to disable
/// P10_CONTAINED_SIM_ISTEP4_CAC_CACHE_RESETFILE - export as =<path to resetfile>
/// P10_CONTAINED_SIM_ISTEP4_CHC_CACHE_RESETFILE - export as =<path to resetfile>
/// P10_CONTAINED_SIM_ISTEP4_CAC_CORE_RESETFILE  - export as =<path to resetfile>
/// P10_CONTAINED_SIM_ISTEP4_CHC_CORE_RESETFILE  - export as =<path to resetfile>
/// P10_CONTAINED_SIM_ISTEP3_CHC_RESETFILE       - export as =<path to resetfile>
///
/// @param[in] i_stage Stage of contained IPL (ISTEP4_CORE, ISTEP4_CACHE, ISTEP3)
/// @param[in] i_chc Indicate if chip-contained IPL
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode scan_via_resetfile(stage i_stage, bool i_chc);

#else // P10_CONTAINED_SIM

constexpr bool skip_stopclocks()
{
    return false;
}
constexpr bool skip_l3purge()
{
    return false;
}
inline fapi2::ReturnCode scan_via_resetfile(stage i_stage, bool i_chc)
{
    return fapi2::FAPI2_RC_SUCCESS;
}

#endif

} // namespace sim

#endif // _P10_CONTAINED_SIM_H_
