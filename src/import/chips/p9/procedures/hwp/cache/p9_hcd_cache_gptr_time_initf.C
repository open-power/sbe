/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_gptr_time_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file  p9_hcd_cache_gptr_time_initf.C
/// @brief Load GPTR and Time for EX non-core
///
/// Procedure Summary:
///   Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///     to produce #G VPD contents
///   Check for the presence of core override GPTR ring from image
///     (this is new fvor P9)
///   if found, apply;  if not, apply core GPTR from image
///     Check for the presence of core override TIME ring from image;
///   if found, apply;  if not, apply core base TIME from image

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_ring_id.h>
#include "p9_hcd_cache_gptr_time_initf.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Load GPTR and Time for EX non-core
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_gptr_time_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_gptr_time_initf");

    auto l_ex_targets = i_target.getChildren<fapi2::TARGET_TYPE_EX>();

    FAPI_DBG("Scan eq_gptr ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_gptr,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (eq_gptr)");

    FAPI_DBG("Scan eq_time ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_time,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (eq_time)");

    for (auto& l_ex : l_ex_targets)
    {
        FAPI_DBG("Scan ex_l3_gptr ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_gptr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l3_gptr)");

        FAPI_DBG("Scan ex_l2_gptr ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l2_gptr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l2_gptr)");

        FAPI_DBG("Scan ex_l3_refr_gptr ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_refr_gptr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l3_refr_gptr)");

        FAPI_DBG("Scan ex_l3_time ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_time,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l3_time)");

        FAPI_DBG("Scan ex_l2_time ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l2_time,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l2_time)");

        FAPI_DBG("Scan ex_l3_refr_time ring");
        FAPI_TRY(fapi2::putRing(l_ex, ex_l3_refr_time,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ex_l3_refr_time)");
    }

    FAPI_DBG("Scan eq_dpll_gptr ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_dpll_gptr,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (eq_dpll_gptr)");

    FAPI_DBG("Scan eq_ana_gptr ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_ana_gptr,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (eq_ana_gptr)");

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_gptr_time_initf");
    return fapi2::current_err;
}

