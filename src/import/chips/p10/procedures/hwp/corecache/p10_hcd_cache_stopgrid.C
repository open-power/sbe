/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_stopgrid.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
/// @file  p10_hcd_cache_stopgrid.C
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

#include "p10_hcd_cache_stopgrid.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_ppe_c.H"
    using namespace scomt::ppe_c;
#else
    #include "p10_scom_c.H"
    using namespace scomt::c;
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_CACHE_STOPGRID_CONSTANTS
{
    HCD_L3_CLK_SYNC_DROP_POLL_TIMEOUT_HW_NS        = 10000,   // 10^4ns = 10us timeout
    HCD_L3_CLK_SYNC_DROP_POLL_DELAY_HW_NS          = 100,     // 100ns poll loop delay
    HCD_L3_CLK_SYNC_DROP_POLL_DELAY_SIM_CYCLE      = 3200,    // 3.2k sim cycle delay
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_stopgrid
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_hcd_cache_stopgrid(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    fapi2::buffer<buffer_t> l_mmioData = 0;
    fapi2::buffer<uint64_t> l_scomData = 0;
    uint32_t                l_timeout  = 0;
#ifdef USE_RUNN
    fapi2::ATTR_RUNN_MODE_Type                  l_attr_runn_mode;
    fapi2::Target < fapi2::TARGET_TYPE_SYSTEM > l_sys;
    FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_RUNN_MODE, l_sys, l_attr_runn_mode ) );
#endif

    FAPI_INF(">>p10_hcd_cache_stopgrid");

    FAPI_DBG("Disable L3 Skewadjust via CPMS_CGCSR_[0:L3_CLK_SYNC_ENABLE]");
    FAPI_TRY( HCD_PUTMMIO_S( i_target, CPMS_CGCSR_WO_CLEAR, BIT64(0) ) );

    FAPI_DBG("Check L3 Skewadjust Removed via CPMS_CGCSR[32:L3_CLK_SYNC_DONE]");
    l_timeout = HCD_L3_CLK_SYNC_DROP_POLL_TIMEOUT_HW_NS /
                HCD_L3_CLK_SYNC_DROP_POLL_DELAY_HW_NS;

    do
    {
        FAPI_TRY( HCD_GETMMIO_S( i_target, CPMS_CGCSR, l_scomData ) );

        // use multicastOR to check 0
        if(
#ifdef USE_RUNN
            ( !l_attr_runn_mode ) &&
#endif
            ( SCOM_GET(32) == 0 ) )
        {
            break;
        }

        fapi2::delay(HCD_L3_CLK_SYNC_DROP_POLL_DELAY_HW_NS,
                     HCD_L3_CLK_SYNC_DROP_POLL_DELAY_SIM_CYCLE);
    }
    while( (--l_timeout) != 0 );

    HCD_ASSERT4( (
#ifdef USE_RUNN
                     l_attr_runn_mode ? ( SCOM_GET(32) == 0 ) :
#endif
                     (l_timeout != 0) ),
                 L3_CLK_SYNC_DROP_TIMEOUT,
                 set_L3_CLK_SYNC_DROP_POLL_TIMEOUT_HW_NS, HCD_L3_CLK_SYNC_DROP_POLL_TIMEOUT_HW_NS,
                 set_CPMS_CGCSR, l_scomData,
                 set_MC_CORE_TARGET, i_target,
                 set_CORE_SELECT, i_target.getCoreSelect(),
                 "ERROR: L3 Clock Sync Drop Timeout");

    FAPI_DBG("Switch glsmux to refclk to save clock grid power via CPMS_CGCSR[7]");
    FAPI_TRY( HCD_PUTMMIO_S( i_target, CPMS_CGCSR_WO_CLEAR, BIT64(7) ) );

fapi_try_exit:

    FAPI_INF("<<p10_hcd_cache_stopgrid");

    return fapi2::current_err;

}
