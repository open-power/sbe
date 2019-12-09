/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_mma_startclocks.C $ */
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
/// @file  p10_hcd_mma_startclocks.C
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

#include "p10_hcd_mma_startclocks.H"
#include "p10_hcd_corecache_clock_control.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_scom_eq.H"
    #include "p10_ppe_c.H"
    using namespace scomt::eq;
    using namespace scomt::ppe_c;
#else
    #include "p10_scom_eq.H"
    #include "p10_scom_c.H"
    using namespace scomt::eq;
    using namespace scomt::c;
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_MMA_STARTCLOCKS_CONSTANTS
{
    HCD_MMA_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS        = 100000, // 10^5ns = 100us timeout
    HCD_MMA_CLK_SYNC_DONE_POLL_DELAY_HW_NS          = 1000,   // 1us poll loop delay
    HCD_MMA_CLK_SYNC_DONE_POLL_DELAY_SIM_CYCLE      = 32000,  // 32k sim cycle delay
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_mma_startclocks
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_hcd_mma_startclocks(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                l_regions  = i_target.getCoreSelect() << SHIFT32(18);
    fapi2::buffer<uint64_t> l_scomData = 0;
//    fapi2::buffer<buffer_t> l_mmioData = 0;
    /*
    #ifndef EQ_SKEW_ADJUST_DISABLE
        uint32_t                l_timeout = 0;
        fapi2::Target < fapi2::TARGET_TYPE_SYSTEM > l_sys;
        fapi2::ATTR_RUNN_MODE_Type                  l_attr_runn_mode;
        FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_RUNN_MODE, l_sys, l_attr_runn_mode ) );
    #endif
    */
    FAPI_INF(">>p10_hcd_mma_startclocks");
    /*
        FAPI_DBG("Enable MMA Skewadjust via CPMS_CGCSR[1:CL2_CLK_SYNC_ENABLE]");
        FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_OR, MMIO_1BIT(1) ) );

    #ifndef EQ_SKEW_ADJUST_DISABLE

        FAPI_DBG("Check MMA Skewadjust Sync Done via CPMS_CGCSR[33:CL2_CLK_SYNC_DONE]");
        l_timeout = HCD_MMA_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS /
                    HCD_MMA_CLK_SYNC_DONE_POLL_DELAY_HW_NS;

        do
        {
            if (!l_attr_runn_mode)
            {
                FAPI_TRY( HCD_GETMMIO_C( i_target, MMIO_LOWADDR(CPMS_CGCSR), l_mmioData ) );

                // use multicastAND to check 1
                if( MMIO_GET(MMIO_LOWBIT(33)) == 1 )
                {
                    break;
                }
            }

            fapi2::delay(HCD_MMA_CLK_SYNC_DONE_POLL_DELAY_HW_NS,
                         HCD_MMA_CLK_SYNC_DONE_POLL_DELAY_SIM_CYCLE);
        }
        while( (--l_timeout) != 0 );

        FAPI_ASSERT((l_timeout != 0),
                    fapi2::MMA_CLK_SYNC_DONE_TIMEOUT()
                    .set_MMA_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS(HCD_MMA_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS)
                    .set_CPMS_CGCSR(l_mmioData)
                    .set_MMA_TARGET(i_target),
                    "MMA Clock Sync Done Timeout");

    #endif
    */
    FAPI_TRY( p10_hcd_corecache_clock_control(eq_target, l_regions, HCD_CLK_START ) );

    FAPI_DBG("Disable MMA Regional Fences via CPLT_CTRL1[5-8:MMA_FENCES]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CTRL1_WO_CLEAR, SCOM_LOAD32H(l_regions) ) );

    FAPI_DBG("Enable MMA Regional PSCOMs via CPLT_CTRL3[5-8:MMA_REGIONS]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CTRL3_WO_OR,  SCOM_LOAD32H(l_regions) ) );

fapi_try_exit:

    FAPI_INF("<<p10_hcd_mma_startclocks");

    return fapi2::current_err;

}
