/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_dpll_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/// @file  p9_hcd_cache_dpll_initf.C
/// @brief Load DPLL ring for EX non-core
///
/// Procedure Summary:
///   Load cache ring images from MVPD
///   These rings must contain ALL chip customization data.
///   This includes the following:  DPLL Power headers, and DTS
///   Historically this was stored in MVPD keywords are #R, #G. Still stored in
///     MVPD, but SBE image is customized with rings for booting cores

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_ring_id.h>
#include "p9_hcd_cache_dpll_initf.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Load DPLL ring for cache
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_dpll_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_dpll_initf");

    FAPI_DBG("Scan eq_dpll_func ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_dpll_func),
             "Error from putRing (eq_dpll_func)");

    FAPI_DBG("Scan eq_ana_func ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_ana_func),
             "Error from putRing (eq_ana_func)");

fapi_try_exit:
    FAPI_INF("<<p9_hcd_cache_dpll_initf");
    return fapi2::current_err;
}
