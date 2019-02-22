/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_pll_setup.C $ */
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
/// @file  p10_sbe_chiplet_pll_setup.C
///
/// @brief Setup PLL for PCI, MC, IOHS
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------


#include "p10_sbe_chiplet_pll_setup.H"
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_CHIPLET_PLL_SETUP_Private_Constants
{
    NS_DELAY = 5000000, // unit is nano seconds
    SIM_CYCLE_DELAY = 100000, // unit is sim cycles
    OPCG_ALIGN_INOP_ALIGN = 0x5, // INOP phase alignment - 8:1
    OPCG_ALIGN_INOP_WAIT = 0x0,
    OPCG_ALIGN_SCANRATIO_4to1 = 0x03,
    OPCG_ALIGN_WAIT_CYCLES = 0x020,
    ERROR_REG_VALUE = 0xFFFFFFFFFFFFFFFF,
};


static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_check_pll_lock(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_pll_reset(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_pll_test_enable(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_pll_bypass(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

fapi2::ReturnCode p10_sbe_chiplet_pll_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    uint8_t l_bypass;
    fapi2::buffer<uint64_t> l_data64;

    FAPI_INF("p10_sbe_chiplet_pll_setup: Entering ...");

    FAPI_DBG("Reading bypass attribute");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IO_TANK_PLL_BYPASS, i_target_chip, l_bypass));

    if (!l_bypass)
    {
        auto l_mc_iohs = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_IOHS);
        auto l_mc_pci = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PCI);
        auto l_mc_mc = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_MC);

        auto l_iohs_pci_mc = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                 static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                         fapi2::TARGET_FILTER_ALL_IOHS | fapi2::TARGET_FILTER_ALL_PCI),
                                 fapi2::TARGET_STATE_FUNCTIONAL);


        FAPI_DBG("Release PLL test enable");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_test_enable(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_test_enable(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_test_enable(l_mc_mc));

        FAPI_DBG("Release PLL reset");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_reset(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_reset(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_reset(l_mc_mc));

        fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

        FAPI_DBG("Check PLL lock for IOHS, MC, PCI chiplets");

        for (auto& targ : l_iohs_pci_mc)
        {
            FAPI_TRY(p10_sbe_chiplet_pll_setup_check_pll_lock(targ));
        }

        FAPI_DBG("Release PLL bypass");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_bypass(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_bypass(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_bypass(l_mc_mc));

        FAPI_DBG("set scan ratio to 4:1 as soon as PLLs are out of bypass mode");
        l_data64.flush<0>();
        l_data64.insertFromRight< PERV_1_OPCG_ALIGN_INOP, PERV_1_OPCG_ALIGN_INOP_LEN >(OPCG_ALIGN_INOP_ALIGN);
        l_data64.insertFromRight< PERV_1_OPCG_ALIGN_INOP_WAIT, PERV_1_OPCG_ALIGN_INOP_WAIT_LEN >(OPCG_ALIGN_INOP_WAIT);
        l_data64.insertFromRight< PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN >(OPCG_ALIGN_SCANRATIO_4to1);
        l_data64.insertFromRight< PERV_1_OPCG_ALIGN_WAIT_CYCLES, PERV_1_OPCG_ALIGN_WAIT_CYCLES_LEN >(OPCG_ALIGN_WAIT_CYCLES);
        FAPI_TRY(fapi2::putScom(l_mc_iohs, PERV_OPCG_ALIGN, l_data64));
        FAPI_TRY(fapi2::putScom(l_mc_pci, PERV_OPCG_ALIGN, l_data64));
        FAPI_TRY(fapi2::putScom(l_mc_mc, PERV_OPCG_ALIGN, l_data64));

        FAPI_DBG("Reset PCB Slave error register");
        FAPI_TRY(fapi2::putScom(l_mc_iohs, PERV_ERROR_REG, ERROR_REG_VALUE));
        FAPI_TRY(fapi2::putScom(l_mc_pci, PERV_ERROR_REG, ERROR_REG_VALUE));
        FAPI_TRY(fapi2::putScom(l_mc_mc, PERV_ERROR_REG, ERROR_REG_VALUE));

        FAPI_DBG(" Unmasking pll unlock error in   Pcb slave config reg");

        for (auto& targ : l_iohs_pci_mc)
        {
            FAPI_TRY(fapi2::getScom(targ, PERV_TP_SLAVE_CONFIG_REG, l_data64));
            l_data64.clearBit<12>();
            FAPI_TRY(fapi2::putScom(targ, PERV_TP_SLAVE_CONFIG_REG, l_data64));
        }

    }

    FAPI_INF("p10_sbe_chiplet_pll_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief check pll lock for IOHS,MC,PCI chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_check_pll_lock(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_read_reg;
    FAPI_INF("p10_sbe_chiplet_pll_setup_check_pll_lock: Entering ...");

    FAPI_DBG("Check  PLL lock");
    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_PLL_LOCK_REG, l_read_reg));

    FAPI_ASSERT(l_read_reg.getBit<0>() == 1 ,
                fapi2::PLL_LOCK_ERR()
                .set_TARGET_CHIPLET(i_target_cplt)
                .set_PLL_READ(l_read_reg),
                "ERROR:PLL LOCK NOT SET");

    FAPI_INF("p10_sbe_chiplet_pll_setup_check_pll_lock: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief release pll bypass
///
/// @param[in]     i_mcast_target   Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_pll_bypass(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_bypass: Entering ...");

    FAPI_DBG("Drop PLL Reset");
    l_data64.flush<1>().clearBit<PERV_1_NET_CTRL0_PLL_BYPASS>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_bypass: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


/// @brief release pll reset
///
/// @param[in]     i_mcast_target  Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_pll_reset(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_reset: Entering ...");

    FAPI_DBG("Drop PLL Reset");
    l_data64.flush<1>().clearBit<PERV_1_NET_CTRL0_PLL_RESET>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Release pll test enable
///
/// @param[in]     i_mcast_target   Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_pll_test_enable(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_test_enable: Entering ...");

    FAPI_DBG("Release PLL test enable");
    l_data64.flush<1>().clearBit<PERV_1_NET_CTRL0_PLL_TEST_EN>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_test_enable: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
