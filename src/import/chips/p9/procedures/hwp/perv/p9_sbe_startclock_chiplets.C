/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_startclock_chiplets.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_sbe_startclock_chiplets.C
///
/// @brief Start clock procedure for XBUS, OBUS, PCIe
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_startclock_chiplets.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>


enum P9_SBE_STARTCLOCK_CHIPLETS_Private_Constants
{
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    CLOCK_CMD_START = 0x1,
    CLOCK_CMD_STOP = 0x2,
    CLOCK_TYPES = 0x7,
    REGIONS_ALL_EXCEPT_VITAL_NESTPLL = 0x7FE,
    REGION1_PBIOOA = 0x200
};


static fapi2::ReturnCode p9_sbe_startclock_chiplets_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_meshctrl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool value);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_ob_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_set_ob_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint8_t i_attr);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_sync_config(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint16_t> i_attr_pg);

static fapi2::ReturnCode p9_sbe_startclock_chiplets_reset_syncclk_muxsel(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_startclock_chiplets(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_pg_vector;
    fapi2::buffer<uint64_t> l_regions;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_data_cplt_fence;
    fapi2::buffer<uint8_t> l_ndl_meshctrl_setup;
    fapi2::buffer<uint8_t> l_attr_obus_ratio;
    fapi2::buffer<uint16_t> l_attr_pg;
    bool l_obus_chiplets = false;
    uint8_t l_hw404391_scom = false;

    FAPI_DBG("p9_sbe_startclock_chiplets: Entering ...");

    auto l_io_func = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                         static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_OBUS |
                                 fapi2::TARGET_FILTER_XBUS | fapi2::TARGET_FILTER_ALL_PCI),
                         fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NDL_MESHCTRL_SETUP, i_target_chip,
                           l_ndl_meshctrl_setup));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_OBUS_RATIO_VALUE, i_target_chip,
                           l_attr_obus_ratio));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW404391_SCOM, i_target_chip,
                           l_hw404391_scom));

    FAPI_TRY(p9_sbe_common_get_pg_vector(i_target_chip, l_pg_vector));
    FAPI_DBG("partial good targets vector: %#018lX", l_pg_vector);

    for (auto& targ : l_io_func)
    {
        // OBUS
        uint32_t l_chipletID = targ.getChipletNumber();
        l_obus_chiplets = false;

        if(l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID)
        {
            l_obus_chiplets = true;
            FAPI_TRY(p9_sbe_startclock_chiplets_set_ob_ratio(targ, l_attr_obus_ratio));
        }

        // XBUS, OBUS, PCIe
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, targ, l_attr_pg));

        FAPI_DBG("Call p9_sbe_startclock_chiplets_cplt_ctrl_action_function for xbus, obus, pcie chiplets");
        FAPI_TRY(p9_sbe_startclock_chiplets_cplt_ctrl_action_function(targ, l_attr_pg));

        // XBUS, PCIe
        if(!(l_obus_chiplets))
        {
            FAPI_DBG("Reset syncclk muxsel for xbus,pcie chiplets");
            FAPI_TRY(p9_sbe_startclock_chiplets_reset_syncclk_muxsel(targ));
        }

        // XBUS, OBUS, PCIe
        FAPI_DBG("Disable listen to sync for all non-master/slave chiplets");
        FAPI_TRY(p9_sbe_startclock_chiplets_sync_config(targ));

        FAPI_DBG("call module align chiplets for xbus obus pcie chiplets");
        FAPI_TRY(p9_sbe_common_align_chiplets(targ));

        // XBUS, PCIe
        if(!(l_obus_chiplets))
        {
            FAPI_DBG("Region setup ");
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(targ,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_regions));
            FAPI_DBG("Regions value: %#018lX", l_regions);

            FAPI_DBG("Call module clock start stop for xbus, pcie chiplets");
            FAPI_TRY(p9_sbe_common_clock_start_stop(targ, CLOCK_CMD_START,
                                                    DONT_STARTSLAVE, DONT_STARTMASTER, l_regions, CLOCK_TYPES));
        }

        // OBUS
        if(l_obus_chiplets)
        {
            if (l_hw404391_scom)
            {
                FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(targ, REGION1_PBIOOA, l_regions));
                FAPI_DBG("Regions value: %#018lX", l_regions);

                FAPI_DBG("Set nv2 iovalid");
                l_data64.flush<0>();
                l_data64.setBit<PERV_1_CPLT_CONF1_IOVALID_6D>();
                l_data64.setBit<PERV_1_CPLT_CONF1_IOVALID_7D>();
                l_data64.setBit<PERV_1_CPLT_CONF1_IOVALID_8D>();
                FAPI_TRY(fapi2::putScom(targ, PERV_CPLT_CONF1_OR, l_data64));

                FAPI_DBG("drop chiplet fence for OB");
                FAPI_TRY(p9_sbe_startclock_chiplets_fence_drop(targ));

                FAPI_DBG("Clock start : region 1 pbiooa to propagate the clock select for nv logic");
                FAPI_TRY(p9_sbe_common_clock_start_stop(targ, CLOCK_CMD_START,
                                                        DONT_STARTSLAVE, DONT_STARTMASTER, l_regions, CLOCK_TYPES));

                FAPI_DBG("Clock stop : region 1 pbiooa ");
                FAPI_TRY(p9_sbe_common_clock_start_stop(targ, CLOCK_CMD_STOP,
                                                        DONT_STARTSLAVE, DONT_STARTMASTER, l_regions, CLOCK_TYPES));

                FAPI_DBG("raise chiplet fence for OB");
                l_data_cplt_fence.flush<0>();
                l_data_cplt_fence.setBit<PERV_1_NET_CTRL0_FENCE_EN>();
                FAPI_TRY(fapi2::putScom(targ, PERV_NET_CTRL0_WOR, l_data_cplt_fence));

                FAPI_DBG("Clear nv2 iovalid");
                FAPI_TRY(fapi2::putScom(targ, PERV_CPLT_CONF1_CLEAR, l_data64));
            }

            FAPI_DBG("Meshctrl setup");
            FAPI_TRY(p9_sbe_startclock_chiplets_meshctrl_setup(targ, ((l_ndl_meshctrl_setup >> (12 - l_chipletID)) & 1)));

            FAPI_DBG("Drop clk async reset for Obus chiplets");
            FAPI_TRY(p9_sbe_startclock_chiplets_ob_async_reset(targ));

            FAPI_DBG("Reset syncclk muxsel for obus chiplets");
            FAPI_TRY(p9_sbe_startclock_chiplets_reset_syncclk_muxsel(targ));

            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(targ,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_regions));
            FAPI_DBG("Regions value: %#018lX", l_regions);

            FAPI_DBG("Call module clock start stop for obus chiplet for all regions except PLL");
            FAPI_TRY(p9_sbe_common_clock_start_stop(targ, CLOCK_CMD_START,
                                                    DONT_STARTSLAVE, DONT_STARTMASTER, l_regions, CLOCK_TYPES));
        }

    }

    // Fences are dropped after clock_start is done for all chiplets.
    // That's the reason this goes into separate loop
    for (auto& targ : l_io_func)
    {
        // XBUS, OBUS, PCIe
        uint32_t l_chipletID = targ.getChipletNumber();

        if(((l_chipletID == XB_CHIPLET_ID) && (l_pg_vector.getBit<2>() == 1)) ||
           ((l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID) && (l_pg_vector.getBit<3>() == 1)) ||
           ((l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID) && (l_pg_vector.getBit<4>() == 1)))
        {
            FAPI_DBG("Drop chiplet fence for XB // OBUS // PCIe");
            FAPI_TRY(p9_sbe_startclock_chiplets_fence_drop(targ));
        }

        // skip dropping flushmode inhbit if PCIE chiplet
        if (!(l_chipletID >= PCI0_CHIPLET_ID) && (l_chipletID <= PCI2_CHIPLET_ID))
        {
            FAPI_DBG("call  sbe_common_flushmode for xbus, obus chiplets");
            FAPI_TRY(p9_sbe_common_flushmode(targ));
        }
    }

    FAPI_DBG("p9_sbe_startclock_chiplets: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for OB,XB,PCIe chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     Pg vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p9_sbe_startclock_chiplets_fence_drop: Entering ...");

    FAPI_DBG("Drop chiplet fence");
    l_data64.flush<1>();
    l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("p9_sbe_startclock_chiplets_fence_drop: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief meshctrl setup
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     value              0 or 1 to be written into perv_cplt_ctrl1 reg
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_meshctrl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet, bool value)
{
    fapi2::buffer<uint64_t> l_data = 0;
    l_data.setBit<21>();
    FAPI_DBG("p9_sbe_startclock_chiplets_meshctrl_setup: Entering ...");

    if ( value )
    {
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_OR, l_data));
    }
    else
    {
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data));
    }

    FAPI_DBG("p9_sbe_startclock_chiplets_meshctrl_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Dropping the net_ctrl0 clock_async_reset
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_ob_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_startclock_chiplets_ob_async_reset: Entering ...");

    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.CLK_ASYNC_RESET = 0
    l_data64.clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p9_sbe_startclock_chiplets_ob_async_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief set obus ratio
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_attr          Attribute that holds the OBUS ratio value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_set_ob_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint8_t i_attr)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p9_sbe_startclock_chiplets_set_ob_ratio: Entering ...");

    //Setting CPLT_CONF1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_CPLT_CONF1, l_data64));
    l_data64.insertFromRight<16, 2>(i_attr);  //CPLT_CONF1.TC_OB_RATIO_DC = i_attr
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CONF1, l_data64));

    FAPI_DBG("p9_sbe_startclock_chiplets_set_ob_ratio: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Disable listen to sync for all non-master / slave chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_sync_config(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p9_sbe_startclock_chiplets_sync_config: Entering ...");

    //Setting SYNC_CONFIG register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_SYNC_CONFIG, l_data64));
    l_data64.setBit<4>();  //SYNC_CONFIG.LISTEN_TO_SYNC_PULSE_DIS = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG, l_data64));

    FAPI_DBG("p9_sbe_startclock_chiplets_sync_config: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --drop vital fence
///        --reset abstclk muxsel
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_attr_pg          ATTR_PG for the corresponding chiplet
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint16_t> i_attr_pg)
{
    // Local variable and constant definition
    fapi2::buffer <uint16_t> l_cplt_ctrl_init;
    fapi2::buffer<uint16_t> l_attr_pg;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p9_sbe_startclock_chiplets_cplt_ctrl_action_function: Entering ...");

    l_attr_pg = i_attr_pg;
    l_attr_pg.invert();
    l_attr_pg.extractToRight<4, 11>(l_cplt_ctrl_init);

    // Not needed as have only nest chiplet (no dual clock controller) Bit 62 ->0
    //
    FAPI_DBG("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>
    (l_attr_pg.getBit<3>());
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_cplt_ctrl_init
    l_data64.insertFromRight<4, 11>(l_cplt_ctrl_init);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("reset abistclk_muxsel");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_DBG("p9_sbe_startclock_chiplets_cplt_ctrl_action_function: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Reset syncclk_muxsel
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_startclock_chiplets_reset_syncclk_muxsel(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("p9_sbe_startclock_chiplets_reset_syncclk_muxsel: Entering ...");

    FAPI_DBG("reset syncclk_muxsel");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.TC_UNIT_SYNCCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_DBG("p9_sbe_startclock_chiplets_reset_syncclk_muxsel: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
