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

enum P9_common_stopclocks_Private_Constants
{
    CBS_ACK_POLL = 20,
    P9_WAIT_CBS_ACK_HW_NS_DELAY = 16000,
    P9_WAIT_CBS_ACK_SIM_CYCLE_DELAY = 800000,
    CBS_CMD_COMPLETE_POLL = 20,
    P9_WAIT_CBS_CMD_COMPLETE_HW_NS_DELAY = 16000,
    P9_WAIT_CBS_CMD_COMPLETE_SIM_CYCLE_DELAY = 800000

};

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
    fapi2::buffer<uint16_t> l_attr_pg;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering p9_common_stopclocks_cplt_ctrl_action_function...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<4, 11>(l_cplt_ctrl_init);

    FAPI_DBG("Raise partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>
    (l_attr_pg.getBit<3>());
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_cplt_ctrl_init
    l_data64.insertFromRight<4, 11>(l_cplt_ctrl_init);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_OR, l_data64));

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
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
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
#ifdef __PPE__
/// @brief p9_common_stopclock_is_scommable - Read Clock STAT SL,NSL,ARY
///             Read ATTR_PG
///             Compare Region Anding(SL,NSL,ARY) with ATTR_PG
///@param[in]   i_target_cplt  - chiplet target
///@param[out]  o_isScommable  - is chiplet scommable
fapi2::ReturnCode p9_common_stopclock_is_scommable(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    uint8_t& o_isScommable)
{
    fapi2::buffer<uint64_t> l_sl_clock_status_act;
    fapi2::buffer<uint64_t> l_nsl_clock_status_act;
    fapi2::buffer<uint64_t> l_ary_clock_status_act;
    fapi2::buffer<uint16_t> l_sl_regions_act;
    fapi2::buffer<uint16_t> l_nsl_regions_act;
    fapi2::buffer<uint16_t> l_ary_regions_act;
    fapi2::buffer<uint16_t> l_read_attr;
    fapi2::buffer<uint16_t> l_attr_regions;
    FAPI_DBG("p9_stopclock_is_scommable: Entering ...");

    FAPI_DBG("Read Clock Stat SL");
    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_CLOCK_STAT_SL,
                            l_sl_clock_status_act));
    FAPI_DBG("CLOCK_STAT_SL Value : %#018lX", l_sl_clock_status_act);

    FAPI_DBG("Read Clock Stat NSL");
    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_CLOCK_STAT_NSL,
                            l_nsl_clock_status_act));
    FAPI_DBG("CLOCK_STAT_NSL Value : %#018lX", l_nsl_clock_status_act);

    FAPI_DBG("Read Clock Stat ARY");
    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_CLOCK_STAT_ARY,
                            l_ary_clock_status_act));
    FAPI_DBG("CLOCK_STAT_ARY Value : %#018lX", l_ary_clock_status_act);

    FAPI_DBG("Extract Regions bits excluding PLL from Clock Stat SL/NSL/ARY");
    l_sl_clock_status_act.extractToRight<4, 10>(l_sl_regions_act);
    l_nsl_clock_status_act.extractToRight<4, 10>(l_nsl_regions_act);
    l_ary_clock_status_act.extractToRight<4, 10>(l_ary_regions_act);
    FAPI_DBG("Region bits from Clock status registers\n\t SL = %#06lX, "
             "NSL = %#06lX, ARY = %#06lX", l_sl_regions_act,
             l_nsl_regions_act, l_ary_regions_act);

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_cplt, l_read_attr));
    FAPI_DBG("ATTR_PG Value : %#010lX", l_read_attr);

    FAPI_DBG("Extract Regions bits excluding PLL from ATTR_PG");
    l_read_attr.extractToRight<4, 10>(l_attr_regions);
    FAPI_DBG("Regions bits from ATTR_PG Value : %#06lX", l_attr_regions);

    FAPI_DBG("Compare region(ANDing Regions bits from SL/NSL/ARY register)"
             " with ATTR_PG");

    if ((l_sl_regions_act & l_nsl_regions_act & l_ary_regions_act)
        == l_attr_regions)
    {
        FAPI_DBG("o_isScommable is True");
        o_isScommable = true;
    }
    else
    {
        FAPI_DBG("o_isScommable is False");
        o_isScommable = false;
    }

    FAPI_DBG("p9_stopclock_is_scommable: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/*
 * @brief - Check if the chiplet is accesible for scom,
 *          valid for all chiplets except TP
 *
 * @param[in]  - i_target        - Pervasive target
 * @param[out] - o_o_isAccesible - If the the chiplet is accesible
 * */
fapi2::ReturnCode p9_common_stopclock_chiplet_accesible(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    uint8_t& o_isAccesible)
{
    fapi2::buffer<uint64_t> l_data64;
    //Reading NET_CTRL0 for chiplets except TP/EQ/EC and call function
    //target_is_scommable
    FAPI_TRY(fapi2::getScom(i_target, PERV_NET_CTRL0, l_data64));
    FAPI_DBG("Read NET_CTRL0 Reg Value and Observe bit0(cplt_en) set,"
             "bit1(ep_rst) clear, bit16(vtl_thld) clear : %#018lX",
             l_data64);

    if ((l_data64.getBit(PERV_1_NET_CTRL0_CHIPLET_ENABLE))
        && !(l_data64.getBit(PERV_1_NET_CTRL0_PCB_EP_RESET))
        && !(l_data64.getBit(PERV_1_NET_CTRL0_VITAL_THOLD)))
    {
        FAPI_DBG("Read NET_CTRL0 Reg Value and required bits met -- "
                 "cplt_en is set, ep_rst is clear, vtl_thld is clear :"
                 "Target Chiplet is acessible");
        o_isAccesible = 1;
    }
    else
    {
        FAPI_DBG("Read NET_CTRL0 Reg Value and min one of them required bits"
                 "is not met -- cplt_en is not set, ep_rst is not clear, "
                 "vtl_thld is not clear : Target Chiplet is not acessible");
        o_isAccesible = 0;
    }

fapi_try_exit:
    return fapi2::current_err;
}
#endif
#ifndef __PPE__
/// @brief Stopping PIB & NET domain clocks in PERV chiplet using CBS
///
/// @param[in]     i_target_chip   Reference to TARGET_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_common_stopclocks_pib_net_clkstop(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint32_t> l_data32_root_ctrl0;

    FAPI_INF("Entering p9_common_stopclocks_pib_net_clkstop ...");

    FAPI_DBG("Clear FSI Fence1 to open CBS-CC interface");
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));

    if(l_data32_root_ctrl0.getBit<PERV_ROOT_CTRL0_FENCE1_DC>())
    {
        l_data32_root_ctrl0.clearBit<PERV_ROOT_CTRL0_FENCE1_DC>();
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));
    }

    FAPI_DBG("Check that state of CBS_REQ");

    if(l_data32_root_ctrl0.getBit<PERV_ROOT_CTRL0_FSI_CC_VSB_CBS_REQ>())
    {
        FAPI_DBG("CBS_REQ is high - Calling p9_common_stopclocks_poll_cbs_cmd_complete function");
        FAPI_TRY(p9_common_stopclocks_poll_cbs_cmd_complete(i_target_chip));
    }
    else
    {
        FAPI_DBG("CBS_REQ is Low - Hence CBS is idle");
    }

    FAPI_DBG("Set CBS_CMD to stop PIB,NET clocks");
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));
    l_data32_root_ctrl0.insertFromRight<PERV_ROOT_CTRL0_FSI_CC_VSB_CBS_CMD, PERV_ROOT_CTRL0_FSI_CC_VSB_CBS_CMD_LEN>(0x4);
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));

    FAPI_DBG("Set CBS_REQ to 1 to start command");
    l_data32_root_ctrl0.setBit<PERV_ROOT_CTRL0_FSI_CC_VSB_CBS_REQ>();
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));

    FAPI_DBG("Calling p9_common_stopclocks_poll_cbs_cmd_complete function");
    FAPI_TRY(p9_common_stopclocks_poll_cbs_cmd_complete(i_target_chip));

    FAPI_INF("Exiting p9_common_stopclocks_pib_net_clkstop ...");

fapi_try_exit:
    return fapi2::current_err;
}



/// @brief Checking for CBS request complete
///
/// @param[in]     i_target_chip   Reference to TARGET_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_common_stopclocks_poll_cbs_cmd_complete(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&
        i_target_chip)
{
    fapi2::buffer<uint32_t> l_data32_cbs_cc_stat;
    fapi2::buffer<uint32_t> l_data32_root_ctrl0;
    int l_timeout = 0;

    FAPI_INF("Entering p9_common_stopclocks_poll_cbs_cmd_complete ...");
    FAPI_DBG("Wait for CBS_ACK to go to 1");
    l_timeout = CBS_ACK_POLL;

    while (l_timeout != 0)
    {
        FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_CBS_STAT_FSI, l_data32_cbs_cc_stat));
        bool l_poll_data = l_data32_cbs_cc_stat.getBit<PERV_CBS_STAT_DBG_TP_TPFSI_ACK>();

        if(l_poll_data == 1)
        {
            break;
        }

        fapi2::delay(P9_WAIT_CBS_ACK_HW_NS_DELAY, P9_WAIT_CBS_ACK_SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_ASSERT(l_timeout > 0, fapi2::CBS_ACK_NOT_HIGH_ERR(), "CBS_ACK is not HIGH with in expected time");

    FAPI_DBG("Lower CBS_REQ");
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));
    l_data32_root_ctrl0.clearBit<PERV_ROOT_CTRL0_FSI_CC_VSB_CBS_REQ>();
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_data32_root_ctrl0));

    FAPI_DBG("Poll for CBS_ACK to go to 0");
    l_timeout = CBS_CMD_COMPLETE_POLL;

    while (l_timeout != 0)
    {
        FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_CBS_STAT_FSI, l_data32_cbs_cc_stat));
        bool l_poll_data = l_data32_cbs_cc_stat.getBit<PERV_CBS_STAT_DBG_TP_TPFSI_ACK>();

        if(l_poll_data == 0)
        {
            break;
        }

        fapi2::delay(P9_WAIT_CBS_CMD_COMPLETE_HW_NS_DELAY, P9_WAIT_CBS_CMD_COMPLETE_SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_ASSERT(l_timeout > 0, fapi2::CBS_ACK_NOT_LOW_ERR(), "CBS_ACK is not LOW with in expected time");

    FAPI_INF("Exiting p9_common_stopclocks_poll_cbs_cmd_complete ...");

fapi_try_exit:
    return fapi2::current_err;
}
#endif
