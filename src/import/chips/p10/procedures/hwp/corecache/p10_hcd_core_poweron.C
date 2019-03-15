/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_poweron.C $ */
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
/// @file  p10_hcd_core_poweron.C
/// @brief Fencing Core+L2, Power on Core+L2 VDD+VCS PFET headers
///


// *HWP HWP Owner          : David Du               <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still             <stillgs@us.ibm.com>
// *HWP FW Owner           : Prasad Brahmasamurdra  <prasadbgr@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:QME
// *HWP Level              : 2


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_core_poweron.H"
#include "p10_hcd_corecache_power_control.H"
#include "p10_hcd_common.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_poweron
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_core_poweron(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target,
    uint32_t i_regions)
{
    fapi2::buffer<uint64_t> l_data64  = 0;
//     fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
//         i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    fapi2::Target < fapi2::TARGET_TYPE_EQ > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ > ();


    FAPI_INF(">>p10_hcd_core_poweron");

    FAPI_DBG("Assert ECL2 Glsmux Reset via CPMS_CGCSR[8:CL2_CLKGLM_ASYNC_RESET]");
    FAPI_TRY( putScom( i_target, G_QME_CPMS_CGCSR_OR, MASK_SET(8) ) );

    FAPI_DBG("Enable ECL2 Regional Fences via CPLT_CTRL1[5-8:ECL2_FENCES] to regions 0x%08X", i_regions);
    FAPI_TRY( putScom( eq_target, G_CPLT_CTRL1_OR, MASK_H32(i_regions) ) );

    // VDD on first, VCS on after
    FAPI_TRY( p10_hcd_corecache_power_control( i_target, HCD_POWER_CL2_ON ) );

fapi_try_exit:

    FAPI_INF("<<p10_hcd_core_poweron");

    return fapi2::current_err;

}
