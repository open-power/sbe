/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_arrayinit.C $ */
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
/// @file  p10_hcd_core_arrayinit.C
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

#include "p10_hcd_core_arrayinit.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_scom_eq.H"
    using namespace scomt::eq;
#else
    #include "p10_scom_eq.H"
    using namespace scomt::eq;
#endif

#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
    #include <p10_perv_sbe_cmn.H>
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_CORE_ARRAYINIT_CONSTANTS
{
    LOOP_COUNTER = 0x0000000000042FFF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_arrayinit
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_core_arrayinit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND  > & i_target)
{
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    fapi2::buffer<uint64_t> l_scomData = 0;
#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
        eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                l_regions  = i_target.getCoreSelect();
    /*
    #ifdef __PPE_QME
    uint8_t                 l_attr_mma_poweron_disable = 0;
    uint8_t                 l_attr_mma_poweroff_disable = 0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    // it is confirmed that we do not need to arrayinit mma, keep the code here in case we need to do so
    FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_SYSTEM_MMA_POWERON_DISABLE,  l_sys, l_attr_mma_poweron_disable ) );
    FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_SYSTEM_MMA_POWEROFF_DISABLE, l_sys, l_attr_mma_poweroff_disable ) );

    // PowerON_Dis = 0 and PowerOFF_Dis = 0 do not start mma
    // PowerON_Dis = 0 and PowerOFF_Dis = 1 do start mma
    // PowerON_Dis = 1 and PowerOFF_Dis = 0 do not start mma
    // PowerON_Dis = 1 and PowerOFF_Dis = 1 do not start mma
    if( !l_attr_mma_poweron_disable && l_attr_mma_poweroff_disable )
    {
    #endif
        l_regions = ( l_regions << SHIFT16(5) | l_regions << SHIFT16(15) );
    #ifdef __PPE_QME
    }
    else
    {
    */
    l_regions = l_regions << SHIFT16(5);
    /*
    }
    #endif
    */

#endif

    FAPI_INF(">>p10_hcd_core_arrayinit");

#ifndef EPM_TUNING

    FAPI_DBG("Scan ec_cl2_abst");
    FAPI_TRY(fapi2::putRing(i_target, ec_cl2_abst,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_cl2_abst)");

#endif

    FAPI_DBG("Assert sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CONF0_WO_OR, SCOM_1BIT(34) ) );

#ifndef P10_HCD_CORECACHE_SKIP_ARRAY

    FAPI_DBG("Arrayinit selected ECL2 regions");
    FAPI_TRY(p10_perv_sbe_cmn_array_init_module(perv_target,
             l_regions,
             LOOP_COUNTER,
             START_ABIST_MATCH_VALUE,
             false, false));

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

    FAPI_INF("<<p10_hcd_core_arrayinit");
    return fapi2::current_err;
}
