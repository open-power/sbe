/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_initf.C $ */
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
/// @file  p9_hcd_cache_initf.C
/// @brief EX (non-core) scan init
///
/// Procedure Summary:
///   Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///   Check for the presence of cache FUNC override rings from image;
///   if found, apply;  if not, apply cache base FUNC rings from image
///   Note:  FASTINIT ring (eg CMSK ring) is setup at this point to limit the
///   stumps that participate in FUNC ring scanning (this is new for P9).
///   (TODO to make sure the image build support is in place)
///   Note: all caches that are in the Cache Multicast group will be
///   initialized to the same values via multicast scans

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p9_hcd_cache_initf.H"

//------------------------------------------------------------------------------
// Procedure: EX (non-core) scan init
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_initf");

#ifndef __PPE__
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    uint8_t l_attr_system_ipl_phase;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, l_sys,
                           l_attr_system_ipl_phase));
#endif

    FAPI_DBG("Scan eq_fure ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_fure),
             "Error from putRing (eq_fure)");
    FAPI_DBG("Scan eq_ana_func ring");
    FAPI_TRY(fapi2::putRing(i_target, eq_ana_func),
             "Error from putRing (eq_ana_func)");

    for (auto l_ex_target : i_target.getChildren<fapi2::TARGET_TYPE_EX>())
    {
        FAPI_DBG("Scan ex_l2_fure ring");
        FAPI_TRY(fapi2::putRing(l_ex_target, ex_l2_fure),
                 "Error from putRing (ex_l2_fure)");
        FAPI_DBG("Scan ex_l2_mode ring");
        FAPI_TRY(fapi2::putRing(l_ex_target, ex_l2_mode),
                 "Error from putRing (ex_l2_mode)");
        FAPI_DBG("Scan ex_l3_fure ring");
        FAPI_TRY(fapi2::putRing(l_ex_target, ex_l3_fure),
                 "Error from putRing (ex_l3_fure)");

#ifndef __PPE__

        if (l_attr_system_ipl_phase ==
            fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
        {
            FAPI_DBG("Cache contained: Skipping ex_l3_refr ring scan");
        }
        else
        {
#endif
            FAPI_DBG("Scan ex_l3_refr_fure ring");
            FAPI_TRY(fapi2::putRing(l_ex_target, ex_l3_refr_fure),
                     "Error from putRing (ex_l3_refr_fure)");
#ifndef __PPE__
        }

#endif
    }

fapi_try_exit:
    FAPI_INF("<<p9_hcd_cache_initf");
    return fapi2::current_err;
}
