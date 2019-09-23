/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_arrayinit.C $ */
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
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_ARRAYINIT_Private_Constants
{
    LOOP_COUNTER = 0x0000000000042FFF,
    REGIONS_EXCEPT_VITAL_AND_PLL = 0x7FEF,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

static fapi2::ReturnCode p10_sbe_arrayinit_sdisn_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target,
    const bool i_set);

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

    FAPI_INF("p10_sbe_arrayinit: Exiting ...");

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
