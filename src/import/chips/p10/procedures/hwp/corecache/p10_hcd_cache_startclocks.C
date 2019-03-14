/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_startclocks.C $ */
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
/// @file  p10_hcd_cache_startclocks.C
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

#include "p10_hcd_cache_startclocks.H"
#include "p10_hcd_corecache_clock_control.H"
#include "p10_hcd_common.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_CACHE_STARTCLOCKS_CONSTANTS
{
    HCD_L3_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS        = 100000,  // 10^5ns = 100us timeout
    HCD_L3_CLK_SYNC_DONE_POLL_DELAY_HW_NS          = 1000,    // 1us poll loop delay
    HCD_L3_CLK_SYNC_DONE_POLL_DELAY_SIM_CYCLE      = 32000,   // 32k sim cycle delay
};



//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_startclocks
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_cache_startclocks(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target,
    uint32_t i_regions)
{
// @todo RTC 207921
//     fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
//         i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    fapi2::Target < fapi2::TARGET_TYPE_EQ > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ > ();

    fapi2::buffer<uint64_t> l_data64  = 0;
#ifndef EQ_SKEW_ADJUST_DISABLE
    uint32_t                l_timeout = 0;
#endif
    FAPI_INF(">>p10_hcd_cache_startclocks");

    FAPI_DBG("Enable L3 Skewadjust via CPMS_CGCSR_[0:L3_CLK_SYNC_DONE_ENABLE]");
    FAPI_TRY( putScom( i_target, G_QME_CPMS_CGCSR_OR, MASK_SET(0) ) );

#ifndef EQ_SKEW_ADJUST_DISABLE
    FAPI_DBG("Check L3 Skewadjust Sync Done via CPMS_CGCSR[32:L3_CLK_SYNC_DONE_DONE]");
    l_timeout = HCD_L3_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS /
                HCD_L3_CLK_SYNC_DONE_POLL_DELAY_HW_NS;

    do
    {
        FAPI_TRY( getScom( i_target, G_QME_CPMS_CGCSR, l_data64 ) );

        //use multicastAND to check 1
        if( DATA_GET(32) == 1 )
        {
            break;
        }

        fapi2::delay(HCD_L3_CLK_SYNC_DONE_POLL_DELAY_HW_NS,
                     HCD_L3_CLK_SYNC_DONE_POLL_DELAY_SIM_CYCLE);
    }
    while( (--l_timeout) != 0 );

    FAPI_ASSERT((l_timeout != 0),
                fapi2::L3_CLK_SYNC_DONE_TIMEOUT()
                .set_L3_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS(HCD_L3_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS)
                .set_CPMS_CGCSR(l_data64)
                .set_CORE_TARGET(i_target),
                "L3 Clock Sync Done Timeout");
#endif

    FAPI_TRY( p10_hcd_corecache_clock_control(eq_target, i_regions, HCD_CLK_START ) );

    FAPI_DBG("Disable L3 Regional Fences via CPLT_CTRL1[9-12:L3_FENCES]");
    FAPI_TRY( putScom( eq_target, G_CPLT_CTRL1_CLR, MASK_H32(i_regions) ) );

    FAPI_DBG("Enable L3 Regional PSCOMs via CPLT_CTRL3[9-12:L3_REGIONS]");
    FAPI_TRY( putScom( eq_target, G_CPLT_CTRL3_OR, MASK_H32(i_regions) ) );

fapi_try_exit:

    FAPI_INF("<<p10_hcd_cache_startclocks");

    return fapi2::current_err;

}
