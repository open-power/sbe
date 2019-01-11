/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_nest_enable_ridi.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
/// @file  p9_sbe_nest_enable_ridi.C
///
/// @brief Enable ridi controls for NEST logic
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_nest_enable_ridi.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"

static fapi2::ReturnCode p9_sbe_nest_enable_ridi_net_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode tp_enable_ridi(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p9_sbe_nest_enable_ridi(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("p9_sbe_nest_enable_ridi: Entering ...");
    FAPI_DBG("p9_sbe_nest_enable_ridi: Enabling TP RI/DI ...");
    //First enable the TP ri/di, originally they were enabled in
    //a previous istep, but running the steps in that order
    //was allowing LPC traffic to flow while the LPC logic was scanned
    FAPI_TRY(tp_enable_ridi(i_target_chip));
    FAPI_DBG("p9_sbe_nest_enable_ridi: Enabling TP RI/DI Complete ...");

    FAPI_DBG("p9_sbe_nest_enable_ridi: Enabling nest RI/DI ...");

    for (auto& l_chplt_trgt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */)))
        {
            continue;
        }

        FAPI_INF("Call p9_sbe_nest_enable_ridi_net_ctrl_action_function");
        FAPI_TRY(p9_sbe_nest_enable_ridi_net_ctrl_action_function(l_chplt_trgt));
    }

    FAPI_DBG("p9_sbe_nest_enable_ridi: Enabling nest RI/DI Complete ...");
    FAPI_DBG("p9_sbe_nest_enable_ridi: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief enables TP ridi bits in RC regs
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode tp_enable_ridi(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("tp_enable_ridi: Entering ...");

    FAPI_INF("tp_enable_ridi:: Enable Recievers, Drivers DI1 & DI2");
    //Setting ROOT_CTRL1 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL1_SCOM, l_data64));
    l_data64.setBit<PERV_ROOT_CTRL1_TP_RI_DC_B>();   // 19
    l_data64.setBit<PERV_ROOT_CTRL1_TP_DI1_DC_B>();  // 20
    l_data64.setBit<PERV_ROOT_CTRL1_TP_DI2_DC_B>();  // 21
    l_data64.setBit<29>();  // HW476237: Lift inhibit on PCIe refclk drivers
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL1_SCOM, l_data64));

    FAPI_DBG("tp_enable_ridi: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Enable Drivers/Recievers of Nest chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_enable_ridi_net_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p9_sbe_nest_enable_ridi_net_ctrl_action_function: Entering ...");

    FAPI_INF("Check for chiplet enable");
    //Getting NET_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL0, l_data64));
    l_read_reg = l_data64.getBit<0>();  //l_read_reg = NET_CTRL0.CHIPLET_ENABLE

    if ( l_read_reg )
    {
        FAPI_INF("Enable Recievers, Drivers DI1 & DI2");
        //Setting NET_CTRL0 register value
        l_data64.flush<0>();
        l_data64.setBit<19>();  //NET_CTRL0.RI_N = 1
        l_data64.setBit<20>();  //NET_CTRL0.DI1_N = 1
        l_data64.setBit<21>();  //NET_CTRL0.DI2_N = 1
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));
    }

    FAPI_DBG("p9_sbe_nest_enable_ridi_net_ctrl_action_function: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
