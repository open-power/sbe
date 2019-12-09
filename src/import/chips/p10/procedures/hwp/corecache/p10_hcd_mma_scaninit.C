/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_mma_scaninit.C $ */
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
/// @file  p10_hcd_mma_scaninit.C
/// @brief
///

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : QME
// *HWP Level              : 2


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_mma_scaninit.H"
#include "p10_hcd_common.H"
#include "p10_ring_id.H"

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

#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
    //    #include <p10_perv_sbe_cmn.H>
#endif


//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_MMA_ARRAYINIT_CONSTANTS
{
    LOOP_COUNTER = 0x0000000000042FFF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_mma_scaninit
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_hcd_mma_scaninit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    /*#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
        fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
            i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
        fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
            eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
        uint32_t l_regions  = i_target.getCoreSelect();
        uint32_t l_loop;
    #endif
    */
    FAPI_INF(">>p10_hcd_mma_scaninit");
    /*
    #ifndef P10_HCD_CORECACHE_SKIP_FLUSH

        FAPI_DBG("Scan0 region:mma type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(15)),
                                                   HCD_SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:mma type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(15)),
                                                   HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

    #endif

    #ifndef P10_HCD_CORECACHE_SKIP_INITF

        FAPI_DBG("Scan ec_mma_gptr ring");
        FAPI_TRY(fapi2::putRing(i_target, ec_mma_gptr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_gptr)");

        FAPI_DBG("Scan ec_mma_time ring");
        FAPI_TRY(fapi2::putRing(i_target, ec_mma_time,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_time)");

        FAPI_DBG("Scan ec_mma_fure ring");
        FAPI_TRY(fapi2::putRing(i_target, ec_mma_fure,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_fure)");

        FAPI_DBG("Scan ec_mma_repr ring");
        FAPI_TRY(fapi2::putRing(l_core, ec_mma_repr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_repr)");

    #endif

    #ifndef P10_HCD_CORECACHE_SKIP_ARRAY

        FAPI_DBG("Arrayinit selected MMA regions");

        FAPI_TRY(p10_perv_sbe_cmn_array_init_module(perv_target,
                 (l_regions << SHIFT16(15)),
                 LOOP_COUNTER,
                 START_ABIST_MATCH_VALUE));

    #endif

    #ifndef P10_HCD_CORECACHE_SKIP_FLUSH

        FAPI_DBG("Scan0 region:mma type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(15)),
                                                   HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

    fapi_try_exit:

    #endif
    */
    FAPI_INF("<<p10_hcd_mma_scaninit");
    return fapi2::current_err;
}
