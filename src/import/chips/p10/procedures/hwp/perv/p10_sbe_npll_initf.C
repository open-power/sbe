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
#include "p10_scom_perv_8.H"
#include "p10_scom_perv_a.H"
#include "p10_perv_sbe_cmn.H"
#include <target_filters.H>
#include <p10_frequency_buckets.H>
#include "hw540133.H"


fapi2::ReturnCode p10_sbe_npll_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];
    fapi2::ATTR_FILTER_PLL_BUCKET_Type l_filter_pll_bucket;
    fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133_Type l_filter_pll_hw540133;

    static const RingID perv_pll_bndy_ring_id[P10_MAX_FILTER_PLL_BUCKETS] = { perv_pll_bndy_bucket_0,
                                                                              perv_pll_bndy_bucket_1,
                                                                              perv_pll_bndy_bucket_2,
                                                                              perv_pll_bndy_bucket_3,
                                                                            };
    FAPI_INF("p10_sbe_npll_initf: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FILTER_PLL_BUCKET, i_target_chip, l_filter_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_FILTER_PLL_BUCKET)");

    FAPI_ASSERT(l_filter_pll_bucket < P10_MAX_FILTER_PLL_BUCKETS,
                fapi2::P10_SBE_PLL_INITF_UNSUPPORTED_PLL_BUCKET().
                set_BUCKET_INDEX(l_filter_pll_bucket).
                set_CHIPLET_ID(0x1),
                "Unsupported PLL bucket value!");

    FAPI_TRY(fapi2::putRing(i_target_chip,
                            perv_pll_bndy_ring_id[l_filter_pll_bucket],
                            fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (perv_pll_bndy)");

    FAPI_TRY(fapi2::putRing(i_target_chip,
                            perv_dpll_time),
             "Error from putRing (perv_dpll_time)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133,
                           i_target_chip,
                           l_filter_pll_hw540133));

    if (l_filter_pll_hw540133)
    {
        // HW540133 requires that we release the filter PLLs from reset in order to sample resolved BANDSEL &
        // write back adjusted value to CCALBANDSEL.  Shift code which releases PLL reset from p10_sbe_npll_setup
        // to this HWP, as we can't read the bndy ring after switch gears flips the nest grid to 2:1
        fapi2::ATTR_CP_PLLTODFLT_BYPASS_Type l_attr_plltodflt_bypass;
        fapi2::ATTR_CP_PLLNESTFLT_BYPASS_Type l_attr_pllnestflt_bypass;
        fapi2::ATTR_CP_PLLIOFLT_BYPASS_Type l_attr_pllioflt_bypass;
        fapi2::ATTR_CP_PLLIOSSFLT_BYPASS_Type l_attr_plliossflt_bypass;
        fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLTODFLT_BYPASS,  i_target_chip, l_attr_plltodflt_bypass));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLNESTFLT_BYPASS, i_target_chip, l_attr_pllnestflt_bypass));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLIOFLT_BYPASS,   i_target_chip, l_attr_pllioflt_bypass));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLIOSSFLT_BYPASS, i_target_chip, l_attr_plliossflt_bypass));

        // run workaround sequence only if all PLLs are going to be removed from bypass
        if (!(l_attr_plltodflt_bypass || l_attr_pllnestflt_bypass || l_attr_pllioflt_bypass || l_attr_plliossflt_bypass))
        {
            FAPI_DBG("Release PLL test enable for filter PLLs");
            // RC3 bits 10,14,18,22
            l_data64.flush<0>();
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLTODFLT_TEST_EN_DC>(!l_attr_plltodflt_bypass);
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLNESTFLT_TEST_EN_DC>(!l_attr_pllnestflt_bypass);
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOFLT_TEST_EN_DC>(!l_attr_pllioflt_bypass);
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOSSFLT_TEST_EN_DC>(!l_attr_plliossflt_bypass);
            FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR, l_data64));

            FAPI_DBG("Release PLL reset for filter PLLs");
            // RC3 bits 8,12,16,20
            l_data64.flush<0>();
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLTODFLT_RESET_DC>(!l_attr_plltodflt_bypass);
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLNESTFLT_RESET_DC>(!l_attr_pllnestflt_bypass);
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOFLT_RESET_DC>(!l_attr_pllioflt_bypass);
            l_data64.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOSSFLT_RESET_DC>(!l_attr_plliossflt_bypass);
            FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR, l_data64));

            FAPI_TRY(hw540133::apply_workaround(l_tpchiplet, hw540133::perv_plls));
        }
    }

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
