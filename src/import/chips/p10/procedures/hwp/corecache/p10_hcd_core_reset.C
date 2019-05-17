/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_reset.C $ */
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
/// @file  p10_hcd_core_reset.C
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

#include "p10_hcd_core_reset.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_hcd_addresses.H"
#else
    #include "p10_scom_eq.H"
    #include "p10_scom_c.H"
    using namespace scomt::eq;
    using namespace scomt::c;
#endif

#if !defined P10_HCD_CORECACHE_SKIP_FLUSH
    #include <p10_perv_sbe_cmn.H>
#endif


//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_reset
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_core_reset(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();

    uint32_t                l_regions  = i_target.getCoreSelect();
    fapi2::buffer<uint64_t> l_scomData = 0;
    fapi2::buffer<buffer_t> l_mmioData = 0;

    FAPI_INF(">>p10_hcd_core_reset");

    FAPI_DBG("Assert ECL2 DC Bypass via NET_CTRL1[4-7]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, NET_CTRL1_RW_WOR, SCOM_LOAD32H( (l_regions << SHIFT32(7)) ) ) );

    FAPI_DBG("Assert ECL2 PDLY Bypass via NET_CTRL1[12-15]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, NET_CTRL1_RW_WOR, SCOM_LOAD32H( (l_regions << SHIFT32(15)) ) ) );

    FAPI_DBG("Assert ECL2 DCC Reset via NET_CTRL1[20-23]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, NET_CTRL1_RW_WOR, SCOM_LOAD32H( (l_regions << SHIFT32(23)) ) ) );

    FAPI_DBG("Switch ECL2 Glsmux to DPLL via CPMS_CGCSR[11:CL2_CLKGLM_SEL]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_OR, MMIO_1BIT(11) ) );

    FAPI_DBG("ECL2 sector buffer control");
    // RTC: 208213  to determine if any value other than 0 is needed

    FAPI_DBG("Drop ECL2 Glsmux Reset via CPMS_CGCSR[8:CL2_CLKGLM_ASYNC_RESET]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_CLEAR, MMIO_1BIT(8) ) );

    //TODO DELAY before scan?

    FAPI_DBG("Assert sram_enable via CPMS_CL2_PFETCNTL[63:SRAM_ENABLE]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, MMIO_LOWADDR(CPMS_CL2_PFETCNTL_WO_OR), MMIO_1BIT( MMIO_LOWBIT(63) ) ) );

    //TODO SCAN ratio?

#ifndef P10_HCD_CORECACHE_SKIP_FLUSH

    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    // Putting in block to avoid c++ crosses initialization compile error
    {
        fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
            eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
        uint32_t l_loop;

        FAPI_DBG("Scan0 region:ecl2 type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   l_regions,
                                                   HCD_SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:ecl2 type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   l_regions,
                                                   HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }

#endif

fapi_try_exit:

    FAPI_INF("<<p10_hcd_core_reset");

    return fapi2::current_err;

}
