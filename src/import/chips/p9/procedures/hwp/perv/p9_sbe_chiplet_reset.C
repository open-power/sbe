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
///
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

#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"


static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep);

static fapi2::ReturnCode p9_sbe_chiplet_reset_core_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec);

static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_pll(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_net_ctrl_clk_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
        const bool i_iscache = false,
        const bool i_iscore = false);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_net_ctrl_pcb_ep_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint8_t i_attr_mc_sync_mode);

static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_reset_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_chiplet_reset(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    // Local variable
    uint8_t l_mc_sync_mode = 0;
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    // Collecting MC sync mode attribute...
    FAPI_INF("Collecting ATTR_MC_SYNC_MODE attribute value");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip,
                           l_mc_sync_mode));

    // Configuring chiplet multicasting registers..
    FAPI_TRY(p9_sbe_chiplet_reset_nest_mc_call(i_target_chip));

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos >= 0x10
              && l_attr_chip_unit_pos <= 0x15))/* CacheChiplet */
        {
            continue;
        }

        // Configuring chiplet multicasting registers..
        FAPI_INF("Configuring cache chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt, true, false));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos >= 0x20
              && l_attr_chip_unit_pos <= 0x37))/* CoreChiplet */
        {
            continue;
        }

        // Configuring chiplet multicasting registers..
        FAPI_INF("Configuring core chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt, false, true));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos >= 0x10
               && l_attr_chip_unit_pos <= 0x15/* CacheChiplet */) ||
              (l_attr_chip_unit_pos >= 0x20 && l_attr_chip_unit_pos <= 0x37/* CoreChiplet */)
              ||
              (l_attr_chip_unit_pos == 0x07 || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        // Configuring NET control registers into Default required value
        FAPI_INF("Restore NET_CTRL0&1 init value - for all chiplets except TP");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        // Setting up hang pulse counter for register 0 and register 6
        FAPI_INF("Setup hang pulse counter for Mc,Xbus,Obus,Pcie");
        FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
              || l_attr_chip_unit_pos == 0x04
              || l_attr_chip_unit_pos == 0x05))/* NestChiplet */
        {
            continue;
        }

        // Setting up hang pulse counter for register 5
        FAPI_INF("Setup hang pulse counter for nest chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_nest_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos >= 0x20
              && l_attr_chip_unit_pos <= 0x37))/* CoreChiplet */
        {
            continue;
        }

        // Setting up hang pulse counter for register 5
        FAPI_INF("Setup hang pulse counter for core chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_core_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos >= 0x10
              && l_attr_chip_unit_pos <= 0x15))/* CacheChiplet */
        {
            continue;
        }

        // Setting up hang pulse counter for register 5
        FAPI_INF("Setup hang pulse counter for cache chiplet");
        FAPI_TRY(p9_sbe_chiplet_reset_cache_hang_cnt_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos >= 0x10
               && l_attr_chip_unit_pos <= 0x15/* CacheChiplet */) ||
              (l_attr_chip_unit_pos >= 0x20 && l_attr_chip_unit_pos <= 0x37/* CoreChiplet */)
              ||
              (l_attr_chip_unit_pos == 0x07 || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        // Setting up partial good fence drop and resetting chiplet.
        FAPI_INF("Enable chiplet and reset error register");
        FAPI_TRY(p9_sbe_chiplet_reset_setup(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        FAPI_INF("enable PLL");
        FAPI_TRY(p9_sbe_chiplet_reset_enable_pll(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos == 0x07
              || l_attr_chip_unit_pos == 0x08))/* McChiplet */
        {
            continue;
        }

        FAPI_INF("Drop clk async reset");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_net_ctrl_clk_async_reset(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        FAPI_INF("Drop endpoint reset");
        FAPI_TRY(p9_sbe_chiplet_reset_net_ctrl_pcb_ep_reset(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
              || l_attr_chip_unit_pos == 0x04
              || l_attr_chip_unit_pos == 0x05))/* NestChiplet */
        {
            continue;
        }

        FAPI_INF("Initialize OPCG registers for Nest");
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(l_target_cplt));
    }

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!(l_attr_chip_unit_pos == 0x07
              || l_attr_chip_unit_pos == 0x08))/* McChiplet */
        {
            continue;
        }

        FAPI_INF("Initialize OPCG registers for Mc");
        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(l_target_cplt, l_mc_sync_mode));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setting up hang pulse counter for all parital good chiplet except for Tp,nest, core and cache
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));

    FAPI_DBG("Exiting ...");

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
    FAPI_DBG("Entering ...");

    //Setting NET_CTRL0 register value
    //NET_CTRL0 = p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL0,
                            p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE));
    //Setting NET_CTRL1 register value
    //NET_CTRL1 = p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL1,
                            p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup Cache hang pulse counter configuration...
///
/// @param[in]     i_target_ep   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_cache_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ep)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X01
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X01);
    l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_1_REG, l_data64));
    //Setting HANG_PULSE_2_REG register value (Setting all fields)
    //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X01
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X01);
    l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_2_REG, l_data64));
    //Setting HANG_PULSE_3_REG register value (Setting all fields)
    //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X04
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X04);
    l_data64.clearBit<6>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_3_REG, l_data64));
    //Setting HANG_PULSE_4_REG register value (Setting all fields)
    //HANG_PULSE_4_REG.HANG_PULSE_REG_4 = p9SbeChipletReset::HANG_PULSE_0X00
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X00);
    l_data64.clearBit<6>();  //HANG_PULSE_4_REG.SUPPRESS_HANG_4 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_4_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = p9SbeChipletReset::HANG_PULSE_0X06
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X06);
    l_data64.clearBit<6>();  //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0
    FAPI_TRY(fapi2::putScom(i_target_ep, PERV_HANG_PULSE_5_REG, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup core hang pulse counter configuration...
///
/// @param[in]     i_target_ec   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_core_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_ec)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = p9SbeChipletReset::HANG_PULSE_0X10
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
    l_data64.clearBit<6>();  //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X1A
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X1A);
    l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_HANG_PULSE_1_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = p9SbeChipletReset::HANG_PULSE_0X06
    l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X06);
    l_data64.clearBit<6>();  //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0
    FAPI_TRY(fapi2::putScom(i_target_ec, PERV_HANG_PULSE_5_REG, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Enable PLL
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_enable_pll(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data;
    FAPI_DBG("Entering ...");

    l_data.flush<0>();
    l_data.setBit<31>();

    //Setting NET_CTRL0 register value
    //NET_CTRL0 = l_data
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Dropping the net_ctrl0 clock_async_reset
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_net_ctrl_clk_async_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.CLK_ASYNC_RESET = 0
    l_data64.clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring multicast registers for nest, cache, core
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_iscache          cache chiplet mc setup
/// @param[in]     i_iscore           core chiplet Mc setup
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
        const bool i_iscache,
        const bool i_iscore)
{
    FAPI_DBG("Entering ...");

    //Setting MULTICAST_GROUP_1 register value
    //MULTICAST_GROUP_1 = p9SbeChipletReset::MCGR0_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_1,
                            p9SbeChipletReset::MCGR0_CNFG_SETTINGS));
    //Setting MULTICAST_GROUP_2 register value
    //MULTICAST_GROUP_2 = p9SbeChipletReset::MCGR3_CNFG_SETTINGS
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_2,
                            p9SbeChipletReset::MCGR3_CNFG_SETTINGS));

    //Setting MULTICAST_GROUP_3 register value
    if (i_iscache | i_iscore)
    {
        //MULTICAST_GROUP_3 = (i_iscache | i_iscore)?p9SbeChipletReset::MCGR1_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_3,
                                p9SbeChipletReset::MCGR1_CNFG_SETTINGS));
    }

    //Setting MULTICAST_GROUP_4 register value
    if (i_iscore)
    {
        //MULTICAST_GROUP_4 = i_iscore ?p9SbeChipletReset::MCGR2_CNFG_SETTINGS
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_MULTICAST_GROUP_4,
                                p9SbeChipletReset::MCGR2_CNFG_SETTINGS));
    }

    FAPI_DBG("Exiting ...");

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
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // Local variables
    //
    uint8_t   l_attr_chipunit_pos;
    const uint8_t N0 = 0x02;
    const uint8_t N1 = 0x03;
    const uint8_t N3 = 0x05;

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

    if ( l_attr_chipunit_pos == N0 )
    {
        //Setting HANG_PULSE_1_REG register value (Setting all fields)
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X10
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X10);
        l_data64.clearBit<6>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X22
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X22);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == N1 )
    {
        //Setting HANG_PULSE_2_REG register value (Setting all fields)
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X0F
        l_data64.insertFromRight<0, 6>(p9SbeChipletReset::HANG_PULSE_0X0F);
        l_data64.clearBit<6>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    if ( l_attr_chipunit_pos == N3 )
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
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Calling nest/mc/other bus multicasting set up
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_nest_mc_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_cplt)
{
    auto l_perv_functional_vector =
        i_target_cplt.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    for (auto l_target_cplt : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt,
                               l_attr_chip_unit_pos));

        if (!((l_attr_chip_unit_pos == 0x07
               || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
              (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
               || l_attr_chip_unit_pos == 0x04
               || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
              (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
               || l_attr_chip_unit_pos == 0x0B
               || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
              (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
               || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
              (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
        {
            continue;
        }

        // Configuring chiplet multicasting registers.
        FAPI_INF("Configuring nest chiplet multicasting registers");
        FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(l_target_cplt));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop Endpoint reset
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_net_ctrl_pcb_ep_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.PCB_EP_RESET = 0b0
    l_data64.clearBit<PERV_1_NET_CTRL0_PCB_EP_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief configuring MC chiplet OPCG registers
///
/// @param[in]     i_target_cplt         Reference to TARGET_TYPE_PERV target
/// @param[in]     i_attr_mc_sync_mode   Collecting MC sync mode attribute
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    const uint8_t i_attr_mc_sync_mode)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    if ( i_attr_mc_sync_mode  )
    {
        // INOP_ALIGN is 8:1 ratio
        // INOP_WAIT is zero
        // OPCG_WAIT is 0x020
        // SCAN_RATIO is 4:1
        //Setting OPCG_ALIGN register value
        l_data64 =
            p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
        //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X9
        l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X9);
        l_data64.clearBit<PERV_1_OPCG_ALIGN_INOP_WAIT, PERV_1_OPCG_ALIGN_INOP_WAIT_LEN>();  //OPCG_ALIGN.INOP_WAIT = 0
        //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
        l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
        l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
        (p9SbeChipletReset::SCAN_RATIO_0X3);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));

        // Enable listen_to_sync mode
        //Setting SYNC_CONFIG register value
        //SYNC_CONFIG = p9SbeChipletReset::SYNC_CONFIG_DEFAULT
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_SYNC_CONFIG,
                                p9SbeChipletReset::SYNC_CONFIG_DEFAULT));
    }

    FAPI_DBG("Exiting ...");

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
    FAPI_DBG("Entering ...");

    //Setting OPCG_ALIGN register value
    l_data64 =
        p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
    //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X9
    l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X9);
    l_data64.clearBit<PERV_1_OPCG_ALIGN_INOP_WAIT, PERV_1_OPCG_ALIGN_INOP_WAIT_LEN>();  //OPCG_ALIGN.INOP_WAIT = 0
    //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
    l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
    l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
    (p9SbeChipletReset::SCAN_RATIO_0X3);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_ALIGN, l_data64));

    //Setting SYNC_CONFIG register value
    //SYNC_CONFIG = p9SbeChipletReset::SYNC_CONFIG_DEFAULT
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG,
                            p9SbeChipletReset::SYNC_CONFIG_DEFAULT));

    FAPI_DBG("Exiting ...");

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
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // Local variable and constant definition
    const uint64_t ERROR_DEFAULT_VALUE = 0xFFFFFFFFFFFFFFFFull;

    // EP Reset all chiplet with in multicasting group
    //Setting NET_CTRL0 register value
    l_data64.flush<0>();
    //NET_CTRL0.CHIPLET_ENABLE = 0b1
    l_data64.setBit<PERV_1_NET_CTRL0_CHIPLET_ENABLE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));

    //Setting ERROR_REG register value
    //ERROR_REG = ERROR_DEFAULT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_ERROR_REG, ERROR_DEFAULT_VALUE));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
