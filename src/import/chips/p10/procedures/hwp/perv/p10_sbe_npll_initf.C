/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_npll_initf.C $ */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_npll_initf.C
///
/// @brief apply initfile for level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_npll_initf.H"
#include "p10_scom_perv_6.H"
#include "p10_perv_sbe_cmn.H"
#include <target_filters.H>


fapi2::ReturnCode p10_sbe_npll_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_npll_initf: Entering ...");


    FAPI_TRY(fapi2::putRing(i_target_chip,
                            perv_pll_bndy,
                            fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (perv_pll_bndy)");

    FAPI_TRY(fapi2::putRing(i_target_chip,
                            perv_dpll_time),
             "Error from putRing (perv_dpll_time)");

    FAPI_DBG("Drop clock region fences for DPLL_PAU, DPLL_NEST and Filter PLL regions");
    l_data64.flush<0>()
    .setBit<CPLT_CTRL1_REGION8_FENCE_DC>()
    .setBit<CPLT_CTRL1_REGION9_FENCE_DC>()
    .setBit<CPLT_CTRL1_REGION10_FENCE_DC>();
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL1_WO_CLEAR, l_data64));

    FAPI_INF("p10_sbe_npll_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
