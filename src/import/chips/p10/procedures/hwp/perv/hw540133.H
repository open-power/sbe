/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/hw540133.H $         */
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
#ifndef HW540133_H
#define HW540133_H

#include <fapi2.H>

namespace hw540133
{

// Opaque workaround settings for different rings
struct pll_ring_settings;
extern const pll_ring_settings perv_plls, rcs_pll0, rcs_pll1, pci_plls, iohs_plls, mc_plls;

/// @brief Apply workaround sequence for HW540133
///
/// Check selected PLLs on the given ring for completion of their internal
/// calibration, and once they are all done read out the band they calibrated
/// to and force them to one band below that (band 1 if they're at band 0).
///
/// @param[in] i_mcast_target Target chiplet(s) to apply workaround to
/// @param[in] i_settings     Workaround settings to use, depends on ring
///
fapi2::ReturnCode apply_workaround(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target,
    const pll_ring_settings& i_settings);

}

#endif //HW540133_H
