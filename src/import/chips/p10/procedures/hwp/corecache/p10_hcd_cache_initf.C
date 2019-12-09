/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file  p10_hcd_cache_initf.C
/// @brief
///


// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:QME
// *HWP Level              : 2


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_ring_id.H"
#include "p10_hcd_cache_initf.H"


//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_initf
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_cache_initf(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_cache_initf");

#ifndef P10_HCD_CORECACHE_SKIP_INITF

    FAPI_DBG("Scan ec_l3_fure ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_l3_fure),
             "Error from putRing (ec_l3_fure)");

    FAPI_DBG("Scan ec_l3_mode ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_l3_mode),
             "Error from putRing (ec_l3_mode)");


fapi_try_exit:

#else

#ifdef __PPE_QME

#include "iota_panic_codes.h"

    fapi2::Target < fapi2::TARGET_TYPE_SYSTEM > l_sys;
    fapi2::ATTR_QME_BROADSIDE_SCAN_Type         l_attr_qme_broadside_scan;
    FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_QME_BROADSIDE_SCAN, l_sys, l_attr_qme_broadside_scan ) );

    if (l_attr_qme_broadside_scan)
    {
        FAPI_INF("QME TRAP FOR BROADSIDE SCAN");
        IOTA_PANIC(CORECACHE_BROADSIDE_SCAN);
    }

#endif

#endif

    FAPI_INF("<<p10_hcd_cache_initf");
    return fapi2::current_err;
}
