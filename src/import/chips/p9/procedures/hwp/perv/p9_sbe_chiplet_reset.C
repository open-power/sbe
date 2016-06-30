/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_chiplet_reset.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
/// @file  p9_sbe_chiplet_reset.C
///
/// @brief Steps:-
///     1) Identify Partical good chiplet and configure Multicasting register
///     2) Similar way, Configure hang pulse counter for Nest/MC/OBus/XBus/PCIe
///     3) Similar way,  set fence for Nest and MC chiplet
///     4) Similar way,  Reset sys.config and OPCG setting for Nest and MC chiplet in sync mode
///
/// Done
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V. Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Brian Silver <bsilver@us.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_reset.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>


static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint8_t i_reg0_val = 0xff,
    const uint8_t i_reg1_val = 0xff,
    const uint8_t i_reg2_val = 0xff,
    const uint8_t i_reg3_val = 0xff,
    const uint8_t i_reg4_val = 0xff,
    const uint8_t i_reg5_val = 0xff,
    const uint8_t i_reg6_val = 0xff);

static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_MC(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_obus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_pcie(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_xbus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value);

static fapi2::ReturnCode p9_sbe_chiplet_reset_div_clk_bypass(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_listen_to_sync(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_enable);

static fapi2::ReturnCode p9_sbe_chiplet_reset_hsspowergate(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_async_reset_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const bool i_drop);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
        const uint64_t i_mc_grp1_val,
        const uint64_t i_mc_grp2_val = 0x0,
        const uint64_t i_mc_grp3_val = 0x0);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_cache(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_ob_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode
p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_pll_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_enable);

static fapi2::ReturnCode p9_sbe_chiplet_reset_scan0_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip);

static fapi2::ReturnCode p9_sbe_chiplet_reset_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_setup_iop_logic(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip);

fapi2::ReturnCode p9_sbe_chiplet_reset(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    // Local variable
    //uint8_t l_mc_sync_mode = 0;
    fapi2::buffer<uint8_t> l_attr_vitl_setup;
    fapi2::buffer<uint8_t> l_attr_hang_cnt6_setup;
    fapi2::TargetState l_target_state = fapi2::TARGET_STATE_FUNCTIONAL;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_VITL_CLK_SETUP, i_target_chip,
                           l_attr_vitl_setup));

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring chiplet multicasting registers.
        FAPI_DBG("Configuring multicasting registers for Nest,Xb,Obus,pcie chiplets" );
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt,
                                               p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Configuring multicast registers for MC01,MC23");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt,
                                               p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0,
                                               p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP2));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CACHES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring chiplet multicasting registers..
        FAPI_DBG("Configuring cache chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup_cache(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CORES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring chiplet multicasting registers..
        FAPI_DBG("Configuring core chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt,
                                               p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0,
                                               p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP1,
                                               p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP3));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                           fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Configuring NET control registers into Default required value
        FAPI_DBG("Restore NET_CTRL0&1 init value - for all chiplets except TP");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 0 and register 6
        FAPI_DBG("Setup hang pulse counter for Mc");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt,
                 p9SbeChipletReset::HANG_PULSE_0X10, 0xff, 0xff, 0xff, 0xff, 0xff,
                 p9SbeChipletReset::HANG_PULSE_0X08));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_PCI, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 0 and register 6
        FAPI_DBG("Setup hang pulse counter for Pcie - increase in hang_pulse value");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt,
                 p9SbeChipletReset::HANG_PULSE_0X10, 0xff, 0xff, 0xff, 0xff, 0xff,
                 p9SbeChipletReset::HANG_PULSE_0X08));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 0 and register 6
        FAPI_DBG("Setup hang pulse counter for Xbus,Obus");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt,
                 p9SbeChipletReset::HANG_PULSE_0X10, p9SbeChipletReset::HANG_PULSE_0X04, 0xff,
                 0xff, 0xff, 0xff, p9SbeChipletReset::HANG_PULSE_0X08));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_NEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 5
        FAPI_DBG("Setup hang pulse counter for nest chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_nest_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CORES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 5
        FAPI_DBG("Setup hang pulse counter for core chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt,
                 p9SbeChipletReset::HANG_PULSE_0X10, p9SbeChipletReset::HANG_PULSE_0X1A, 0xff,
                 0xff, 0xff, p9SbeChipletReset::HANG_PULSE_0X06,
                 p9SbeChipletReset::HANG_PULSE_0X08));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_CACHES, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        // Setting up hang pulse counter for register 5
        FAPI_DBG("Setup hang pulse counter for cache chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt,
                 p9SbeChipletReset::HANG_PULSE_0X10, p9SbeChipletReset::HANG_PULSE_0X01,
                 p9SbeChipletReset::HANG_PULSE_0X01, p9SbeChipletReset::HANG_PULSE_0X04,
                 p9SbeChipletReset::HANG_PULSE_0X00, p9SbeChipletReset::HANG_PULSE_0X06,
                 p9SbeChipletReset::HANG_PULSE_0X08));
    }

    FAPI_DBG("Clock mux settings");
    FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_call(i_target_chip));

    if ( l_attr_vitl_setup )
    {
        l_target_state = fapi2::TARGET_STATE_PRESENT;
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                           fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS), l_target_state))
    {
        // Setting up partial good fence drop and resetting chiplet.
        FAPI_DBG("PLL Setup : Enable pll");
        FAPI_TRY(p9_sbe_chiplet_reset_pll_setup(l_target_cplt, true));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, l_target_state))
    {
        FAPI_DBG("Drop clk async reset for N3 chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_nest_ob_async_reset(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, l_target_state))
    {
        FAPI_DBG("Drop clk async reset for Mc chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_async_reset_setup(l_target_cplt, true));
    }

    fapi2::delay(10000, (40 * 400));

    if ( l_attr_vitl_setup )
    {
        l_target_state = fapi2::TARGET_STATE_PRESENT;

        for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                               fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                               fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                               fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS), l_target_state))
        {
            // Setting up partial good fence drop and resetting chiplet.
            FAPI_DBG("PLL setup : Disable pll");
            FAPI_TRY(p9_sbe_chiplet_reset_pll_setup(l_target_cplt, false));
        }

        for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (fapi2::TARGET_FILTER_ALL_MC, l_target_state))
        {
            FAPI_DBG("Raise clk async reset for Mc chiplet");
            FAPI_TRY(p9_sbe_chiplet_reset_mc_async_reset_setup(l_target_cplt, false));
        }
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_OBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop clk async reset for N3, Mc  and Obus chiplets");
        FAPI_TRY(p9_sbe_chiplet_reset_nest_ob_async_reset(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop clk_div_bypass for Mc chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_div_clk_bypass(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                           fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Enable chiplet and reset error register");
        FAPI_TRY(p9_sbe_chiplet_reset_setup(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop lvltrans fence and endpoint reset");
        FAPI_TRY(p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
                     l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Initialize OPCG registers for Nest,MC,XB,OB,PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Enable listen to sync for NEST,OB,XB,PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_enable_listen_to_sync(l_target_cplt, true));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI |
                                           fapi2::TARGET_FILTER_XBUS), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Disable listen_to_sync for Nest,MC,XB,OB,PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_enable_listen_to_sync(l_target_cplt, false));
    }

    FAPI_DBG("Set Chip-wide HSSPORWREN gate");
    FAPI_TRY(p9_sbe_chiplet_reset_hsspowergate(i_target_chip));

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_PCI, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Setup IOP Logic for PCIe");
        FAPI_TRY(p9_sbe_chiplet_reset_setup_iop_logic(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("set scan ratio to 1:1 ");
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(l_target_cplt));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_sbe_chiplet_reset_scan0_call(l_target_cplt));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setting up hang pulse counter for all parital good chiplet except for Tp,nest, core and cache
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_reg0_val      value for HANG_PULSE_0_REG
/// @param[in]     i_reg1_val      value for HANG_PULSE_1_REG
/// @param[in]     i_reg2_val      value for HANG_PULSE_2_REG
/// @param[in]     i_reg3_val      value for HANG_PULSE_3_REG
/// @param[in]     i_reg4_val      value for HANG_PULSE_4_REG
/// @param[in]     i_reg5_val      value for HANG_PULSE_5_REG
/// @param[in]     i_reg6_val      Hang pulse reg 6 value - for heartbeat
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint8_t i_reg0_val,
    const uint8_t i_reg1_val,
    const uint8_t i_reg2_val,
    const uint8_t i_reg3_val,
    const uint8_t i_reg4_val,
    const uint8_t i_reg5_val,
    const uint8_t i_reg6_val)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    if (i_reg0_val != 0xff)
    {
        //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = (i_reg0_val != 0xff) ? i_reg0_val
        l_data64.insertFromRight<0, 6>(i_reg0_val);
        //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = (i_reg0_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));
    }

    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    if (i_reg1_val != 0xff)
    {
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = (i_reg1_val != 0xff) ? i_reg1_val
        l_data64.insertFromRight<0, 6>(i_reg1_val);
        //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = (i_reg1_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
    }

    //Setting HANG_PULSE_2_REG register value (Setting all fields)
    if (i_reg2_val != 0xff)
    {
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = (i_reg2_val != 0xff) ? i_reg2_val
        l_data64.insertFromRight<0, 6>(i_reg2_val);
        //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = (i_reg2_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    //Setting HANG_PULSE_3_REG register value (Setting all fields)
    if (i_reg3_val != 0xff)
    {
        //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = (i_reg3_val != 0xff) ? i_reg3_val
        l_data64.insertFromRight<0, 6>(i_reg3_val);
        //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = (i_reg3_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
    }

    //Setting HANG_PULSE_4_REG register value (Setting all fields)
    if (i_reg4_val != 0xff)
    {
        //HANG_PULSE_4_REG.HANG_PULSE_REG_4 = (i_reg4_val != 0xff) ? i_reg4_val
        l_data64.insertFromRight<0, 6>(i_reg4_val);
        //HANG_PULSE_4_REG.SUPPRESS_HANG_4 = (i_reg4_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_4_REG, l_data64));
    }

    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    if (i_reg5_val != 0xff)
    {
        //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = (i_reg5_val != 0xff) ? i_reg5_val
        l_data64.insertFromRight<0, 6>(i_reg5_val);
        //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = (i_reg5_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_5_REG, l_data64));
    }

    //Setting HANG_PULSE_6_REG register value (Setting all fields)
    if (i_reg6_val != 0xff)
    {
        //HANG_PULSE_6_REG.HANG_PULSE_REG_6 = (i_reg6_val != 0xff) ? i_reg6_val
        l_data64.insertFromRight<0, 6>(i_reg6_val);
        //HANG_PULSE_6_REG.SUPPRESS_HANG_6 = (i_reg6_val != 0xff) ? 0
        l_data64.clearBit<6>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_6_REG, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring NET control registers into Default required value
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("Entering ...");

    //Setting NET_CTRL0 register value
    //NET_CTRL0 = p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL0,
                            p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE));
    //Setting NET_CTRL1 register value
    //NET_CTRL1 = p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL1,
                            p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for Mc chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_MC(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting NET_CTRL1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<3>()
    l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<3>());
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief call all the related mux settings on chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplet)
{
    fapi2::buffer<uint32_t> l_read_attr;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_PLL_MUX, i_target_chiplet,
                           l_read_attr));

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for Mc chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_MC(l_target_cplt, l_read_attr));
    }

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_OBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for OB chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_obus(l_target_cplt, l_read_attr));
    }

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_XBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for XB chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_xbus(l_target_cplt, l_read_attr));
    }

    for (auto l_target_cplt : i_target_chiplet.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_ALL_PCI, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Mux settings for Pcie chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_pcie(l_target_cplt, l_read_attr));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for OB chiplet
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    Clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_obus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    uint8_t l_attr_unit_pos = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                           l_attr_unit_pos));

    if ( l_attr_unit_pos == 0x09 )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<6>()
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<6>());
        l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
        (i_clk_mux_value.getBit<13>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<13>()
        l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
        (i_clk_mux_value.getBit<15>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<15>()
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    if ( l_attr_unit_pos == 0x0A )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>
        (i_clk_mux_value.getBit<16>());  //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<16>()
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    if ( l_attr_unit_pos == 0x0B )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>
        (i_clk_mux_value.getBit<17>());  //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<17>()
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    if ( l_attr_unit_pos == 0x0C )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<7>()
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<7>());
        l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
        (i_clk_mux_value.getBit<9>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<9>()
        l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
        (i_clk_mux_value.getBit<14>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<14>()
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for Pcie chiplet
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_pcie(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    uint8_t l_attr_unit_pos = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                           l_attr_unit_pos));

    if ( l_attr_unit_pos != 0x0E )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>((l_attr_unit_pos == 0x0D) ?
                i_clk_mux_value.getBit<5>() :
                i_clk_mux_value.getBit<4>());  //NET_CTRL1.PLL_CLKIN_SEL = (l_attr_unit_pos == 0x0D)? i_clk_mux_value.getBit<5>() :  i_clk_mux_value.getBit<4>()

        if (l_attr_unit_pos == 0x0D)
        {
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
            (i_clk_mux_value.getBit<10>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = (l_attr_unit_pos == 0x0D)? i_clk_mux_value.getBit<10>()
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<11>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = (l_attr_unit_pos == 0x0D)? i_clk_mux_value.getBit<11>()
        }

        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for XB chiplet
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_xbus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting NET_CTRL1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<8>()
    l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<8>());
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop clk div bypass for Mc chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_div_clk_bypass(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("drop clk_div_bypass_en");
    //Setting NET_CTRL1 register value
    l_data64.flush<1>();
    //NET_CTRL1.CLK_DIV_BYPASS_EN = 0
    l_data64.clearBit<PERV_1_NET_CTRL1_CLK_DIV_BYPASS_EN>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1_WAND, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Enable listen_to_sync mode for all chiplets except MC
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_enable           if TRUE - enable, FALSE - disable
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_listen_to_sync(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_enable)
{
    FAPI_INF("Entering ...");

    //Setting SYNC_CONFIG register value
    //SYNC_CONFIG = i_enable? p9SbeChipletReset::SYNC_CONFIG_DEFAULT : p9SbeChipletReset::SYNC_CONFIG_4TO1
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG,
                            i_enable ? p9SbeChipletReset::SYNC_CONFIG_DEFAULT :
                            p9SbeChipletReset::SYNC_CONFIG_4TO1));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Set Chip-wide HSSPORWREN gate
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_hsspowergate(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;
    FAPI_INF("Entering ...");

    //Getting ROOT_CTRL2 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL2_SCOM,
                            l_read_reg)); //l_read_reg = PIB.ROOT_CTRL2

    l_read_reg.setBit<20>();

    FAPI_DBG("Set Chip-wide HSSPORWREN gate");
    //Setting ROOT_CTRL2 register value
    //PIB.ROOT_CTRL2 = l_read_reg
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL2_SCOM, l_read_reg));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop/ raise MC async reset
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_drop          Raise/drop mc async reset
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_async_reset_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const bool i_drop)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    if ( i_drop )
    {
        FAPI_DBG("Drop mc async reset");
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        //NET_CTRL0.CLK_ASYNC_RESET = 0
        l_data64.clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
    }
    else
    {
        if ( !(i_target_chip.isFunctional()) )
        {
            FAPI_DBG("Raise mc async reset");
            //Setting NET_CTRL0 register value
            l_data64.flush<0>();
            //NET_CTRL0.CLK_ASYNC_RESET = 1
            l_data64.setBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WOR, l_data64));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring multicast registers for nest, cache, core
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_mc_grp1_val      value for MULTICAST_GROUP1 register
/// @param[in]     i_mc_grp2_val      value for MULTICAST_GROUP2 register
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
        const uint64_t i_mc_grp1_val,
        const uint64_t i_mc_grp2_val,
        const uint64_t i_mc_grp3_val)
{
    FAPI_INF("Entering ...");

    //Setting MULTICAST_GROUP_1 register value
    //MULTICAST_GROUP_1 (register) = i_mc_grp1_val
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                            i_mc_grp1_val));

    //Setting MULTICAST_GROUP_2 register value
    if (i_mc_grp2_val != 0x0)
    {
        //MULTICAST_GROUP_2 (register) = (i_mc_grp2_val != 0x0) ? i_mc_grp2_val
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_2,
                                i_mc_grp2_val));
    }

    //Setting MULTICAST_GROUP_3 register value
    if (i_mc_grp3_val != 0x0)
    {
        //MULTICAST_GROUP_REGISTER_3 = (i_mc_grp3_val != 0x0) ? i_mc_grp3_val
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_3,
                                i_mc_grp3_val));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Multicast register setup for Cache chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup_cache(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    uint32_t l_attr_pg = 0;
    FAPI_INF("Entering ...");

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    FAPI_DBG("Setting Multicast register 1&2 for cache chiplet");
    //Setting MULTICAST_GROUP_1 register value
    //MULTICAST_GROUP_1 (register) = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                            p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0));
    //Setting MULTICAST_GROUP_2 register value
    //MULTICAST_GROUP_2 (register) = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP4
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_2,
                            p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP4));

    if ( ( l_attr_pg & 0x1EBA ) == 0x0 ) // Check good EP chiplet clockdomains excluding l31, l21, refr1
    {
        FAPI_DBG("Setting up multicast register 3 for even cache chiplet");
        //Setting MULTICAST_GROUP_3 register value
        //MULTICAST_GROUP_3 (register) = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP5
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_3,
                                p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP5));
    }

    if ( ( l_attr_pg & 0x1D76 ) == 0x0 ) // Check good EP chiplet clockdomains excluding l30, l20, refr0
    {
        FAPI_DBG("Setting up multicast register 4 for odd cache chiplet");
        //Setting MULTICAST_GROUP_4 register value
        //MULTICAST_GROUP_4 (register) = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP6
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_4,
                                p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP6));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setting up hang pulse counter for partial good Nest chiplet
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    // Local variables
    //
    uint8_t   l_attr_chipunit_pos = 0;
    const uint8_t l_n0 = 0x02;
    const uint8_t l_n1 = 0x03;
    const uint8_t l_n2 = 0x04;
    const uint8_t l_n3 = 0x05;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    // Collecting partial good and chiplet unit position attribute
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_cplt,
                           l_attr_chipunit_pos));

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = p9SbeChipletReset::HANG_PULSE_0X06
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X06);
    l_data64.clearBit<6>();  //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_5_REG, l_data64));
    //Setting HANG_PULSE_6_REG register value (Setting all fields)
    //HANG_PULSE_6_REG.HANG_PULSE_REG_6 = p9SbeChipletReset::HANG_PULSE_0X08
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X08);
    l_data64.clearBit<6>();  //HANG_PULSE_6_REG.SUPPRESS_HANG_6 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_6_REG, l_data64));

    if ( l_attr_chipunit_pos == l_n0 )
    {
        //Setting HANG_PULSE_1_REG register value (Setting all fields)
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X18
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X18);
        l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X22
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X22);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
        //Setting HANG_PULSE_3_REG register value (Setting all fields)
        //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X12
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X12);
        l_data64.clearBit<6>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == l_n1 )
    {
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X0F
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X0F);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == l_n2 )
    {
        //Setting HANG_PULSE_3_REG register value (Setting all fields)
        //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X12
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X12);
        l_data64.clearBit<6>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == l_n3 )
    {
        //Setting HANG_PULSE_1_REG register value (Setting all fields)
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X17
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X17);
        l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X13
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X13);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
        //Setting HANG_PULSE_3_REG register value (Setting all fields)
        //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X0F
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X0F);
        l_data64.clearBit<6>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
        //Setting HANG_PULSE_4_REG register value (Setting all fields)
        //HANG_PULSE_4_REG.HANG_PULSE_REG_4 = p9SbeChipletReset::HANG_PULSE_0X1C
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X1C);
        l_data64.clearBit<6>();  //HANG_PULSE_4_REG.SUPPRESS_HANG_4 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_4_REG, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Dropping the net_ctrl0 clock_async_reset
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_ob_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.CLK_ASYNC_RESET = 0
    l_data64.clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop Endpoint reset
/// Drop lvltrans fence
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode
p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Drop lvltrans fence");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.LVLTRANS_FENCE = 0b0
    l_data64.clearBit<PERV_1_NET_CTRL0_LVLTRANS_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("Drop endpoint reset");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.PCB_EP_RESET = 0b0
    l_data64.clearBit<PERV_1_NET_CTRL0_PCB_EP_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief configuring Nest chiplet OPCG registers
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting OPCG_ALIGN register value
    l_data64 =
        p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
    //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X5
    l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X5);
    l_data64.clearBit<PERV_1_OPCG_ALIGN_INOP_WAIT, PERV_1_OPCG_ALIGN_INOP_WAIT_LEN>();  //OPCG_ALIGN.INOP_WAIT = 0
    //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
    l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
    l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
    (p9SbeChipletReset::SCAN_RATIO_0X3);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_ALIGN, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief set scan ratio to 1:1 as long as PLL is in bypass mode
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Set scan ratio to 1:1 as long as PLL is in bypass mode");
    //Setting OPCG_ALIGN register value
    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
    (p9SbeChipletReset::SCAN_RATIO_0X0);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Enable PLL
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_enable           enable/disable pll
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_pll_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_enable)
{
    fapi2::buffer<uint64_t> l_data;;
    FAPI_INF("Entering ...");

    if ( i_enable )
    {
        l_data.flush<0>();
        l_data.setBit<31>();

        FAPI_DBG("Enable pll");
        //Setting NET_CTRL0 register value
        //NET_CTRL0 = l_data
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data));
    }
    else
    {
        if ( !(i_target_chiplet.isFunctional()) )
        {
            l_data.flush<1>();
            l_data.clearBit<31>();

            FAPI_DBG("Disable PLL");
            //Setting NET_CTRL0 register value
            //NET_CTRL0 = l_data
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Scan0 module call
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_scan0_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    FAPI_INF("Entering ...");

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(i_target_chip,
             p9SbeChipletReset::REGIONS_EXCEPT_VITAL, l_regions));

    FAPI_DBG("run scan0 module for region except vital and pll, scan types GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chip, l_regions,
                                          p9SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("run scan0 module for region except vital and pll, scan types except GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chip, l_regions,
                                          p9SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Dorping fence on Partial good chiplet and resetting it.
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    // Local variable and constant definition
    const uint64_t l_error_default_value = 0xFFFFFFFFFFFFFFFFull;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    // EP Reset all chiplet with in multicasting group
    //Setting NET_CTRL0 register value
    l_data64.flush<0>();
    //NET_CTRL0.CHIPLET_ENABLE = 0b1
    l_data64.setBit<PERV_1_NET_CTRL0_CHIPLET_ENABLE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));

    //Setting ERROR_REG register value
    //ERROR_REG = l_error_default_value
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_ERROR_REG,
                            l_error_default_value));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup IOP Logic for PCIe
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_setup_iop_logic(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    //Setting CPLT_CONF1 register value
    l_data64.flush<0>();
    l_data64.setBit<30>();  //CPLT_CONF1.TC_IOP_HSSPORWREN = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF1_OR, l_data64));

    fapi2::delay(p9SbeChipletReset::HW_NS_DELAY,
                 p9SbeChipletReset::SIM_CYCLE_DELAY);

    //Setting CPLT_CONF1 register value
    l_data64.flush<0>();
    l_data64.setBit<28>();  //CPLT_CONF1.TC_IOP_SYS_RESET_PCS = 0b1
    l_data64.setBit<29>();  //CPLT_CONF1.TC_IOP_SYS_RESET_PMA = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF1_OR, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
