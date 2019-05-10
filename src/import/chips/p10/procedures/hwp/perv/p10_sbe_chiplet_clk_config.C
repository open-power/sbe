/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_clk_config.C $ */
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
/// @file  p10_sbe_chiplet_clk_config.C
///
/// @brief
///
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_clk_config.H"
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <target_filters.H>
#include <multicast_group_defs.H>

static fapi2::ReturnCode p10_sbe_chiplet_clk_config_sectorbuffer_pulsemode_attr_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

static fapi2::ReturnCode p10_sbe_chiplet_clk_config_force_pll_out_enable(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

fapi2::ReturnCode p10_sbe_chiplet_clk_config(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64_nc1, l_data64_nc0;
    fapi2::buffer<uint8_t> l_attr_mux_iohs, l_attr_mux_pci;

    FAPI_INF("p10_sbe_chiplet_clk_config: Exiting ...");

    auto l_perv_iohs = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                           static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_IOHS),
                           fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_perv_pci = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                          static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
                          fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_perv_nest_pau_pci = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                   static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_ALL_PAU | fapi2::TARGET_FILTER_ALL_PCI),
                                   fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_mc_all    = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    auto l_mc_iohs = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_IOHS);
    auto l_mc_pci = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PCI);
    auto l_mc_mc = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_MC);

    FAPI_DBG("Setup Axon clock muxing");

    for (auto& targ : l_perv_iohs)
    {
        uint32_t l_chipletID = targ.getChipletNumber();

        FAPI_TRY(fapi2::getScom(targ, PERV_NET_CTRL1, l_data64_nc1));

        if (l_chipletID == 0x18) // IOHS0 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX15_IOHS0_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x19) // IOHS1 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX16_IOHS1_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x1A) // IOHS2 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX17_IOHS2_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x1B) // IOHS3 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX18_IOHS3_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x1C) // IOHS4 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX19_IOHS4_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x1D) // IOHS5 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX20_IOHS5_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x1E) // IOHS6 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX21_IOHS6_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }
        else if (l_chipletID == 0x1F) // IOHS7 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX22_IOHS7_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_iohs);
        }

        FAPI_TRY(fapi2::putScom(targ, PERV_NET_CTRL1, l_data64_nc1));
    }


    FAPI_DBG("Setup PCI clock muxing");

    for (auto& targ : l_perv_pci)
    {
        uint32_t l_chipletID = targ.getChipletNumber();

        FAPI_TRY(fapi2::getScom(targ, PERV_NET_CTRL1, l_data64_nc1));

        if (l_chipletID == 0x8) // PCI0 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX24_PCI0_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_pci);
        }
        else if (l_chipletID == 0x9) // PCI1 cplt
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_MUX25_PCI1_LCPLL_INPUT, i_target_chip, l_attr_mux_iohs));
            l_data64_nc1.insertFromRight< PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL, 2 >(l_attr_mux_pci);
        }

        FAPI_TRY(fapi2::putScom(targ, PERV_NET_CTRL1, l_data64_nc1));
    }

    FAPI_DBG("Setup Sector buffer strength, Pulse mode enable and pulse mode");
    FAPI_TRY(p10_sbe_chiplet_clk_config_sectorbuffer_pulsemode_attr_setup(l_mc_all));

    FAPI_DBG("Drop clk_async_reset for Nest, PCI and PAU chiplets");

    for (auto& targ : l_perv_nest_pau_pci)
    {
        uint32_t l_chipletID = targ.getChipletNumber();

        l_data64_nc0.flush<1>().clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
        FAPI_TRY(fapi2::putScom(targ, PERV_NET_CTRL0_WAND, l_data64_nc0));

        if ((l_chipletID >= 0x10) && (l_chipletID <= 0x13) ) // PAU cplts
        {
            fapi2::buffer<uint32_t> l_attr_pg;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, targ, l_attr_pg));

            l_data64_nc1.flush<1>().writeBit<2>(l_attr_pg.getBit<13>()).writeBit<3>(l_attr_pg.getBit<14>());
            FAPI_TRY(fapi2::putScom(targ, PERV_NET_CTRL1_WAND, l_data64_nc1));
        }
    }

    FAPI_DBG("Force PLL out enable for IO PLLs(IOHS, PCIE, MC)");
    FAPI_TRY(p10_sbe_chiplet_clk_config_force_pll_out_enable(l_mc_iohs));
    FAPI_TRY(p10_sbe_chiplet_clk_config_force_pll_out_enable(l_mc_pci));
    FAPI_TRY(p10_sbe_chiplet_clk_config_force_pll_out_enable(l_mc_mc));

    FAPI_INF("p10_sbe_chiplet_clk_config: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Force PLL out enable for IO PLLs(PCIE, MC, AXON)
///
/// @param[in]     i_mcast_target   Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_clk_config_force_pll_out_enable(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{
    fapi2::buffer<uint64_t> l_data64_net_ctrl0;
    FAPI_INF("p10_sbe_chiplet_clk_config_force_pll_out_enable: Entering ...");

    l_data64_net_ctrl0.flush<0>().setBit<PERV_1_NET_CTRL0_PLLFORCE_OUT_EN>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL0_WOR, l_data64_net_ctrl0));

    FAPI_INF("p10_sbe_chiplet_clk_config_force_pll_out_enable: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}



/// @brief Setup sector buffer strength and pulse mode for all good cplts except TP
///
/// @param[in]     i_mcast_target   Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_clk_config_sectorbuffer_pulsemode_attr_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{

    fapi2::buffer<uint64_t> l_data64_net_ctrl1;
    fapi2::buffer<uint8_t> l_attr_buffer_strength = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_enable = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_value = 0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;

    FAPI_INF("p10_sbe_chiplet_clk_config_sectorbuffer_pulsemode_attr_setup:Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECTOR_BUFFER_STRENGTH, l_sys,
                           l_attr_buffer_strength));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PULSE_MODE_ENABLE, l_sys,
                           l_attr_pulse_mode_enable));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PULSE_MODE_VALUE, l_sys,
                           l_attr_pulse_mode_value));

    l_data64_net_ctrl1
    .flush<1>()
    .clearBit<PERV_1_NET_CTRL1_SB_STRENGTH, PERV_1_NET_CTRL1_SB_STRENGTH_LEN>()
    .clearBit<PERV_1_NET_CTRL1_CLK_PULSE_EN>()
    .clearBit<PERV_1_NET_CTRL1_CLK_PULSE_MODE, PERV_1_NET_CTRL1_CLK_PULSE_MODE_LEN>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL1_WAND,
                            l_data64_net_ctrl1));

    FAPI_DBG("Sector buffer strength");
    l_data64_net_ctrl1
    .flush<0>()
    .insertFromRight< PERV_1_NET_CTRL1_SB_STRENGTH,
                      PERV_1_NET_CTRL1_SB_STRENGTH_LEN >(l_attr_buffer_strength);

    FAPI_DBG("Pulse mode enable & pulse mode");

    if (l_attr_pulse_mode_enable.getBit<7>())
    {
        FAPI_DBG("setting pulse mode enable");
        l_data64_net_ctrl1
        .setBit<PERV_1_NET_CTRL1_CLK_PULSE_EN>()
        .insertFromRight< PERV_1_NET_CTRL1_CLK_PULSE_MODE,
                          PERV_1_NET_CTRL1_CLK_PULSE_MODE_LEN >(l_attr_pulse_mode_value);
    }

    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL1_WOR, l_data64_net_ctrl1));

    FAPI_INF("p10_sbe_chiplet_clk_config_sectorbuffer_pulsemode_attr_setup:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
