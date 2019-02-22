/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_nest_enable_ridi.C $ */
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
/// @file  p10_sbe_nest_enable_ridi.C
///
/// @brief Enable ridi controls for NEST logic
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_nest_enable_ridi.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include <target_filters.H>

static fapi2::ReturnCode p10_sbe_nest_enable_ridi_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p10_sbe_nest_enable_ridi_tp_enable_ridi(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_sbe_nest_enable_ridi(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    FAPI_INF("p10_sbe_nest_enable_ridi: Entering ...");

    auto l_perv_nest = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                           static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST),
                           fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_DBG("Enabling TP drivers and receivers");
    FAPI_TRY(p10_sbe_nest_enable_ridi_tp_enable_ridi(i_target_chip));

    FAPI_DBG("Enabling Nest drivers and receivers");

    for (auto& targ : l_perv_nest)
    {
        FAPI_TRY(p10_sbe_nest_enable_ridi_setup(targ));
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
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_nest_enable_ridi_tp_enable_ridi: Entering ...");

    //Setting ROOT_CTRL1 register value
    l_data64.flush<0>()
    .setBit<PERV_ROOT_CTRL1_TP_RI_DC_B>()    // 19
    .setBit<PERV_ROOT_CTRL1_TP_DI1_DC_B>()   // 20
    .setBit<PERV_ROOT_CTRL1_TP_DI2_DC_B>();  // 21
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL1_SET_SCOM, l_data64));

    FAPI_INF("p10_sbe_nest_enable_ridi_tp_enable_ridi: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Enable Drivers/Recievers of Nest chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_nest_enable_ridi_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p10_sbe_nest_enable_ridi_setup: Entering ...");

    FAPI_INF("Check for chiplet enable");
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL0, l_data64));
    l_read_reg = l_data64.getBit<0>();

    if ( l_read_reg )
    {
        FAPI_INF("Enable Recievers, Drivers DI1 & DI2");
        l_data64.flush<0>();
        l_data64.setBit<19>();
        l_data64.setBit<20>();
        l_data64.setBit<21>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));
    }

    FAPI_DBG("p10_sbe_nest_enable_ridi_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
