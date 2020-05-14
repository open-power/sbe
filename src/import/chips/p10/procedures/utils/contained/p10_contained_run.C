/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_run.C $ */
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
#include <p10_contained.H>
#include <p10_contained_runn.H>

#include <p10_scom_perv.H>
#include <multicast_group_defs.H>

///
/// @brief Lower fences which are assumed low by istep3/4 procedures during
///        p10_contained_ipl. These fences were raised during p10_contained_ipl
///        to manually stop clocks. Note: only needed for non-RUNN/free-clocks.
///
/// @param[in] i_chip Reference to chip target
/// @param[in] i_chc  Indicate chip-contained mode (vs. cache-contained mode)
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode cleanup_for_free_clks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        const bool i_chc)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    auto all = i_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    fapi2::buffer<uint64_t> data;

    FAPI_TRY(PREP_CPLT_CTRL1_WO_CLEAR(all));
    SET_CPLT_CTRL1_REGION0_FENCE_DC(data); // perv

    if (i_chc)
    {
        SET_CPLT_CTRL1_REGION9_FENCE_DC(data); // qme
    }

    FAPI_TRY(PUT_CPLT_CTRL1_WO_CLEAR(all, data));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

extern "C" {
    fapi2::ReturnCode p10_contained_run(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        bool chc;
        bool runn;

        FAPI_TRY(is_runn_ipl(runn));
        FAPI_TRY(is_chc_ipl(chc));

        if (runn)
        {
            FAPI_TRY(runn_setup(i_target, chc));
            FAPI_TRY(runn_start(i_target, chc));
        }
        else
        {
            FAPI_TRY(cleanup_for_free_clks(i_target, chc));
        }

    fapi_try_exit:
        return fapi2::current_err;
    }
} // extern "C"
