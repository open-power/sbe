/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_scomcust.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_hcd_cache_scomcust.C
/// @brief Cache Customization SCOMs
///

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_scomcust.H"

//------------------------------------------------------------------------------
// Constant Definitions: Core Chiplet PCB Arbitration
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_scomcust(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_scomcust");
    fapi2::buffer<uint64_t>                     l_data64;
    uint8_t                                     l_attr_system_ipl_phase;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>    l_sys;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, l_sys,
                           l_attr_system_ipl_phase));

    if (l_attr_system_ipl_phase != fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED)
    {
        FAPI_DBG("Drop chiplet fence via NET_CTRL0[18]");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(18)));
    }

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_scomcust");
    return fapi2::current_err;
}

