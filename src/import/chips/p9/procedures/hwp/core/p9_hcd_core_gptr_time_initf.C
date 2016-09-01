/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/core/p9_hcd_core_gptr_time_initf.C $ */
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
/// @file  p9_hcd_core_gptr_time_initf.C
/// @brief Load Core GPTR and Time rings
///
/// Procedure Summary:
///   initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///     to produce #G VPD contents
///   Check for the presence of core override GPTR ring from image
///     (this is new for P9)
///   if found, apply;  if not, apply core GPTR from image
///   Check for the presence of core override TIME ring from image;
///   if found, apply;  if not, apply core base TIME from image

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <p9_ring_id.h>
#include "p9_hcd_core_gptr_time_initf.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Load Core GPTR and Time rings
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_gptr_time_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_gptr_time_initf");

    FAPI_DBG("Scan ec_gptr ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_gptr,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_gptr)");

    FAPI_DBG("Scan ec_time ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_time,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_time)");

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_gptr_time_initf");
    return fapi2::current_err;
}

