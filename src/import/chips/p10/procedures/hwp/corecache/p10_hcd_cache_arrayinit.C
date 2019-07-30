/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_arrayinit.C $ */
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
/// @file  p10_hcd_cache_arrayinit.C
/// @brief Perform array initialization to the EQ cache region
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

#include "p10_hcd_cache_arrayinit.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_scom_eq.H"
    using namespace scomt::eq;
#else
    #include "p10_scom_eq.H"
    using namespace scomt::eq;
#endif

#if !defined P10_HCD_CORECACHE_SKIP_ARRAY && !defined P10_HCD_CORECACHE_SKIP_FLUSH
    #include <p10_perv_sbe_cmn.H>
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_CACHE_ARRAYINIT_CONSTANTS
{
    LOOP_COUNTER = 0x0000000000042FFF,
    SELECT_SRAM = 0x1,
    SELECT_EDRAM = 0x0,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_arrayinit
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_cache_arrayinit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
#if !defined P10_HCD_CORECACHE_SKIP_ARRAY && !defined P10_HCD_CORECACHE_SKIP_FLUSH
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
        eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                l_regions  = i_target.getCoreSelect();
#endif

    fapi2::buffer<uint64_t> l_scomData = 0;

    FAPI_INF(">>p10_hcd_cache_arrayinit");

    FAPI_DBG("Assert sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CONF0_WO_OR, SCOM_1BIT(34) ) );

#ifndef P10_HCD_CORECACHE_SKIP_ARRAY

    FAPI_DBG("Arrayinit all regions except vital/DPLL");
    FAPI_TRY(p10_perv_sbe_cmn_array_init_module(perv_target,
             l_regions,
             LOOP_COUNTER,
             START_ABIST_MATCH_VALUE));

#endif

#ifndef P10_HCD_CORECACHE_SKIP_FLUSH

    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    FAPI_DBG("Scan0 region:ecl2 type:all_but_gptr_repr_time rings");

    for(uint32_t l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
        FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                               l_regions,
                                               HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

#endif

    FAPI_DBG("Drop sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CONF0_WO_CLEAR, SCOM_1BIT(34) ) );

fapi_try_exit:

    FAPI_INF("<<p10_hcd_cache_arrayinit");

    return fapi2::current_err;

}
