/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_nest_enable_ridi.C $ */
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
/// @file  p10_sbe_nest_enable_ridi.C
///
/// @brief Enable ridi controls for NEST logic
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_nest_enable_ridi.H"
#include "p10_scom_perv_3.H"
#include "p10_scom_perv_4.H"
#include "p10_scom_perv_f.H"
#include <target_filters.H>
#include <p10_perv_sbe_cmn.H>
#include <p10_enable_ridi.H>

static fapi2::ReturnCode p10_sbe_nest_enable_ridi_tp_enable_ridi(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_sbe_nest_enable_ridi(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;

    FAPI_INF("p10_sbe_nest_enable_ridi: Entering ...");

    auto l_perv_nest = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                           static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST),
                           fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_DBG("Release perst override");
    l_data64.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL1_TPFSI_TP_GLB_PERST_OVR_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL1_CLEAR_WO_CLEAR, l_data64));

    FAPI_DBG("Enabling TP drivers and receivers");
    FAPI_TRY(p10_sbe_nest_enable_ridi_tp_enable_ridi(i_target_chip));

    FAPI_DBG("Enabling Nest drivers and receivers");

    for (auto& targ : l_perv_nest)
    {
        FAPI_TRY(p10_enable_ridi(targ));
    }

    FAPI_DBG("p10_sbe_nest_enable_ridi: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief enables TP ridi bits in RC regs
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_nest_enable_ridi_tp_enable_ridi(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_nest_enable_ridi_tp_enable_ridi: Entering ...");

    //Setting ROOT_CTRL1 register value
    l_data64.flush<0>()
    .setBit<FSXCOMP_FSXLOG_ROOT_CTRL1_TP_RI_DC_B>()    // 19
    .setBit<FSXCOMP_FSXLOG_ROOT_CTRL1_TP_DI1_DC_B>()   // 20
    .setBit<FSXCOMP_FSXLOG_ROOT_CTRL1_TP_DI2_DC_B>();  // 21
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL1_SET_WO_OR, l_data64));

    FAPI_INF("p10_sbe_nest_enable_ridi_tp_enable_ridi: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
