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
#include "p10_hcd_corecache_realign.H"
#include "p10_hcd_common.H"
#include "p10_pm_hcd_flags.h"

#ifdef __PPE_QME
    #include "p10_scom_eq.H"
    #include "p10_ppe_c.H"
    using namespace scomt::eq;
    using namespace scomt::ppe_c;
    #define QME_FLAGS_PLATFORM_EPM QME_FLAGS_RUNNING_EPM
    // Due to p10_scom_eq.H has both CPLT_CTRL and QME quad register
    // cannot include p10_ppe_eq.H for QME quad register alone
    #define QME_FLAGS 0xc0001200
#else
    #include "p10_scom_eq.H"
    #include "p10_scom_c.H"
    using namespace scomt::eq;
    using namespace scomt::c;
    #define QME_FLAGS_PLATFORM_EPM p10hcd::QME_FLAGS_RUNNING_EPM
    #define QME_FLAGS QME_FLAGS_RW
#endif

#if !defined P10_HCD_CORECACHE_SKIP_FLUSH
    #include <p10_perv_sbe_cmn.H>
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
    uint32_t                l_regions  = i_target.getCoreSelect();
    fapi2::buffer<buffer_t> l_mmioData = 0;

    FAPI_INF(">>p10_hcd_cache_reset");

    FAPI_DBG("Switch L3 Glsmux to DPLL via CPMS_CGCSR[7:L3_CLKGLM_SEL]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_OR, MMIO_1BIT(7) ) );

    FAPI_DBG("L3 sector buffer control");
    // RTC: 208213  to determine if any value other than 0 is needed

    FAPI_DBG("Drop L3 Glsmux Reset via CPMS_CGCSR[4:L3_CLKGLM_ASYNC_RESET]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_CLEAR, MMIO_1BIT(4) ) );

    //TODO DELAY before scan?

    FAPI_DBG("Assert sram_enable via CPMS_L3_PFETCNTL[63:SRAM_ENABLE]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, MMIO_LOWADDR(CPMS_L3_PFETCNTL_WO_OR), MMIO_1BIT( MMIO_LOWBIT(63) ) ) );

    //TODO SCAN ratio?

    FAPI_TRY( p10_hcd_corecache_realign(eq_target, ( l_regions << SHIFT32(12) ) ) );

#ifndef P10_HCD_CORECACHE_SKIP_FLUSH

    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    FAPI_TRY(HCD_GETMMIO_Q( eq_target, MMIO_LOWADDR(QME_FLAGS), l_mmioData ) );

    if ( MMIO_GET( MMIO_LOWBIT(QME_FLAGS_PLATFORM_EPM) ) == 0 )
    {
        fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
            eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
        uint32_t l_loop;

        FAPI_DBG("Scan0 region:l3 type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(9)),
                                                   HCD_SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:l3 type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(9)),
                                                   HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }

#endif

fapi_try_exit:

    FAPI_INF("<<p10_hcd_cache_reset");

    return fapi2::current_err;

}
