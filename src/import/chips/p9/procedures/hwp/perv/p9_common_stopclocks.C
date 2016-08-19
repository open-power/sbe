/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_common_stopclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/// @file  p9_common_stopclocks.C
///
/// @brief Common module for stopclocks
//------------------------------------------------------------------------------
// *HWP HW Owner        : Soma BhanuTej <soma.bhanu@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_common_stopclocks.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


/// @brief --Raise partial good fences
/// --set abstclk muxsel,syncclk_muxsel
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_common_stopclocks_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    // Local variable and constant definition
    fapi2::buffer <uint16_t> l_cplt_ctrl_init;
    fapi2::buffer<uint32_t> l_attr_pg;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering p9_common_stopclocks_cplt_ctrl_action_function...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<20, 11>(l_cplt_ctrl_init);

    FAPI_DBG("Raise partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>
    (l_attr_pg.getBit<19>());  //CPLT_CTRL1.TC_VITL_REGION_FENCE = l_attr_pg.getBit<19>()
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_cplt_ctrl_init
    l_data64.insertFromRight<4, 11>(l_cplt_ctrl_init);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("set abistclk_muxsel and syncclk_muxsel");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
    //CPLT_CTRL0.TC_UNIT_SYNCCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_OR, l_data64));

    FAPI_INF("Exiting p9_common_stopclocks_cplt_ctrl_action_function...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Raise chiplet fence for chiplets
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_common_stopclocks_raise_fence(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering p9_common_stopclocks_raise_fence...");

    FAPI_DBG("Raise chiplet fence");
    //Setting NET_CTRL0 register value
    l_data64.flush<0>();
    l_data64.setBit<PERV_1_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 1
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));

    FAPI_INF("Exiting p9_common_stopclocks_raise_fence...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief -- Assert vital fence
/// -- set flush_inhibit to go out of flush mode
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_common_stopclocks_set_vitalfence_flushmode(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering p9_common_stopclocks_set_vitalfence_flushmode ...");

    FAPI_DBG("Assert Vital Fence");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_VITL_REGION_FENCE = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_OR, l_data64));

    FAPI_DBG("Set flush_inhibit to go in to flush mode");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_FLUSHMODE_INH_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_OR, l_data64));

    FAPI_INF("Exiting p9_common_stopclocks_set_vitalfence_flushmode...");

fapi_try_exit:
    return fapi2::current_err;

}
