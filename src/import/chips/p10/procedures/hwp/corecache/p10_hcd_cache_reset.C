/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_reset.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
/// @file  p10_hcd_cache_reset.C
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

#include "p10_hcd_cache_reset.H"
#include <p10_hcd_cache_scan0.H>
#include "p10_hcd_corecache_realign.H"
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


//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_reset
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_cache_reset(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                l_regions  = i_target.getCoreSelect() << SHIFT32(12);
    fapi2::buffer<buffer_t> l_mmioData = 0;
    fapi2::buffer<uint64_t> l_scomData = 0;

    FAPI_INF(">>p10_hcd_cache_reset");

    FAPI_DBG("Switch L3 Glsmux to DPLL via CPMS_CGCSR[7:L3_CLKGLM_SEL]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_OR, MMIO_1BIT(7) ) );

    FAPI_DBG("L3 sector buffer Strength is left as default 0");

    FAPI_DBG("Drop L3 Glsmux Reset via CPMS_CGCSR[4:L3_CLKGLM_ASYNC_RESET]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_CLEAR, MMIO_1BIT(4) ) );

    FAPI_DBG("Assert sram_enable via CPMS_L3_PFETCNTL[63:SRAM_ENABLE]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, MMIO_LOWADDR(CPMS_L3_PFETCNTL_WO_OR), MMIO_1BIT( MMIO_LOWBIT(63) ) ) );

    FAPI_DBG("Drop TC_REGION0_DFT_FENCE_DC via CPLT_CTRL5[9-12:L3_FENCES] to regions 0x%08X", l_regions);
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CTRL5_WO_CLEAR, SCOM_LOAD32H(l_regions) ) );

    FAPI_TRY( p10_hcd_corecache_realign( eq_target, l_regions ) );

#ifndef __PPE_QME

    FAPI_TRY( p10_hcd_cache_scan0(i_target) );

#endif

fapi_try_exit:

    FAPI_INF("<<p10_hcd_cache_reset");
    return fapi2::current_err;
}
