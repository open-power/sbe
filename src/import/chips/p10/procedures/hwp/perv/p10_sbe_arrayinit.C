/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_arrayinit.C $ */
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
/// @file  p10_sbe_arrayinit.C
///
/// @brief array init procedure to be called with any chiplet target except TP,EQ
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_arrayinit.H"
#include "p10_scom_perv_0.H"
#include "p10_scom_perv_6.H"
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_ARRAYINIT_Private_Constants
{
    LOOP_COUNTER = 0x0000000000042FFF,
    REGIONS_EXCEPT_VITAL_AND_PLL = 0x7FEF,
    REGIONS_OCC = 0x0800,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

static fapi2::ReturnCode p10_sbe_arrayinit_sdisn_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target,
    const bool i_set);

static fapi2::ReturnCode p10_sbe_arrayinit_occ(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_sbe_arrayinit(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_arrayinit: Entering ...");

    auto l_mc_all = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);

    FAPI_DBG("set sdis_n");
    FAPI_TRY(p10_sbe_arrayinit_sdisn_setup(l_mc_all, true));

    FAPI_DBG("Run Arrayinit module for all chiplets except TP and EQ");

    FAPI_TRY(p10_perv_sbe_cmn_array_init_module(l_mc_all, REGIONS_EXCEPT_VITAL_AND_PLL,
             LOOP_COUNTER, START_ABIST_MATCH_VALUE));

    FAPI_DBG("Run scan0 module again for all regions except GPTR, TIME and REPAIR");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_mc_all, REGIONS_EXCEPT_VITAL_AND_PLL,
                                           SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_DBG("clear sdis_n");
    FAPI_TRY(p10_sbe_arrayinit_sdisn_setup(l_mc_all, false));

    // OCC shares dual-clocked arrays with N1 so we can't run array init before N1 GPTR, TIME, REPR are initialized
    FAPI_TRY(p10_sbe_arrayinit_occ(i_target_chip));

    FAPI_INF("p10_sbe_arrayinit: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Run arrayinit on the OCC
///
/// @param[in]     i_target_chip   Chip to operate on
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_arrayinit_occ(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_perv_fence, l_pibmem_control;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_arrayinit_occ: Entering ...");

    FAPI_DBG("Exclude PIBMEM from TP array init");
    FAPI_TRY(fapi2::getScom(i_target_chip, 0x0008FFF0, l_pibmem_control ));
    l_pibmem_control.setBit<5>();
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x0008FFF0, l_pibmem_control ));

    FAPI_DBG("Raise TP perv region fence so abist_done can propagate");
    l_perv_fence.flush<0>().setBit<CPLT_CTRL1_REGION0_FENCE_DC>();
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL1_WO_OR, l_perv_fence));

    // Not dropping OCC region fence during Arrayinit call
    FAPI_DBG("run array_init module for OCC");
    FAPI_TRY(p10_perv_sbe_cmn_array_init_module(l_tpchiplet, REGIONS_OCC,
             LOOP_COUNTER, START_ABIST_MATCH_VALUE, false));

    FAPI_DBG("Drop TP perv region fence");
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL1_WO_CLEAR, l_perv_fence));

    FAPI_DBG("Add PIBMEM back to TP array init");
    l_pibmem_control.clearBit<5>();
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x0008FFF0, l_pibmem_control));

    FAPI_DBG("run scan0 module for OCC");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_tpchiplet, REGIONS_OCC,
                                           SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_INF("p10_sbe_arrayinit_occ: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Sdis_n setup
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target or multicast target
/// @param[in]     i_set           set or clear the LCBES condition
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_arrayinit_sdisn_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target,
    const bool i_set)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_arrayinit_sdisn_setup: Entering ...");

    l_data64.flush<0>();
    l_data64.setBit<CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
    FAPI_TRY(fapi2::putScom(i_mcast_target,
                            i_set ? CPLT_CONF0_WO_OR : CPLT_CONF0_WO_CLEAR, l_data64));

    FAPI_INF("p10_sbe_arrayinit_sdisn_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
