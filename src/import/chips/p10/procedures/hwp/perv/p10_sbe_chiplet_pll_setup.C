/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_pll_setup.C $ */
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
/// @file  p10_sbe_chiplet_pll_setup.C
///
/// @brief Setup PLL for PCI, MC, IOHS
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------


#include "p10_sbe_chiplet_pll_setup.H"
#include "p10_scom_perv_0.H"
#include "p10_scom_perv_3.H"
#include "p10_scom_perv_6.H"
#include "p10_scom_perv_8.H"
#include "p10_scom_perv_e.H"
#include "p10_perv_sbe_cmn.H"
#include <target_filters.H>
#include <multicast_group_defs.H>
#include "hw540133.H"


enum P10_SBE_CHIPLET_PLL_SETUP_Private_Constants
{
    OPCG_ALIGN_INOP_ALIGN_VAL = 0x7, // INOP phase alignment - 16:1
    OPCG_ALIGN_INOP_WAIT_VAL = 0x0,
    OPCG_ALIGN_SCANRATIO_4to1 = 0x03,
    OPCG_ALIGN_WAIT_CYCLES_VAL = 0x020,
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

static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_drop_pll_region_fence(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

fapi2::ReturnCode p10_sbe_chiplet_pll_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    uint8_t l_bypass;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::ReturnCode l_rc;

    FAPI_INF("p10_sbe_chiplet_pll_setup: Entering ...");

    FAPI_DBG("Reading bypass attribute");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IO_TANK_PLL_BYPASS, i_target_chip, l_bypass));

    if (!l_bypass)
    {
        fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133_Type l_filter_pll_hw540133;
        fapi2::ATTR_CHIP_EC_FEATURE_TANK_PLL_HW540133_Type l_tank_pll_hw540133;

        auto l_mc_iohs = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_IOHS);
        auto l_mc_pci = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PCI);
        auto l_mc_mc = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_MC);

        auto l_iohs_pci_mc = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                 static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                         fapi2::TARGET_FILTER_ALL_IOHS | fapi2::TARGET_FILTER_ALL_PCI),
                                 fapi2::TARGET_STATE_FUNCTIONAL);

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133,
                               i_target_chip,
                               l_filter_pll_hw540133));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_TANK_PLL_HW540133,
                               i_target_chip,
                               l_tank_pll_hw540133));

        FAPI_DBG("Drop Pll fence");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_drop_pll_region_fence(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_drop_pll_region_fence(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_drop_pll_region_fence(l_mc_mc));


        FAPI_DBG("Release PLL test enable");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_test_enable(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_test_enable(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_test_enable(l_mc_mc));

        FAPI_DBG("Release PLL reset");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_reset(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_reset(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_reset(l_mc_mc));

        if (l_tank_pll_hw540133)
        {
            FAPI_TRY(hw540133::apply_workaround(l_mc_iohs, hw540133::iohs_plls));
            FAPI_TRY(hw540133::apply_workaround(l_mc_mc, hw540133::mc_plls));
        }

        if (l_filter_pll_hw540133)
        {
            FAPI_TRY(hw540133::apply_workaround(l_mc_pci, hw540133::pci_plls));
        }

        FAPI_DBG("Check PLL lock for IOHS, MC, PCI chiplets");
        // This smells like three separate timeouts, and it kinda is, but in the good case
        // the second and third poll call will exit very quickly since their PLLs had ample
        // time to lock.
        l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_mc_iohs, 0x8000000000000000ULL, l_data64);

        if (l_rc == fapi2::FAPI2_RC_SUCCESS)
        {
            l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_mc_pci, 0x8000000000000000ULL, l_data64);
        }

        if (l_rc == fapi2::FAPI2_RC_SUCCESS)
        {
            l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_mc_mc, 0x8000000000000000ULL, l_data64);
        }

        if (l_rc == fapi2::FAPI2_RC_FALSE)
        {
            // One or more PLLs failed to lock, fall back to unicast to figure out which
            for (auto& targ : l_iohs_pci_mc)
            {
                FAPI_TRY(p10_sbe_chiplet_pll_setup_check_pll_lock(targ));
            }
        }

        FAPI_TRY(l_rc, "Failed to poll for PLL lock");

        FAPI_DBG("Release PLL bypass");
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_bypass(l_mc_iohs));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_bypass(l_mc_pci));
        FAPI_TRY(p10_sbe_chiplet_pll_setup_pll_bypass(l_mc_mc));

        FAPI_DBG("set scan ratio to 4:1 as soon as PLLs are out of bypass mode");
        l_data64.flush<0>();
        l_data64.insertFromRight< OPCG_ALIGN_INOP_ALIGN, OPCG_ALIGN_INOP_ALIGN_LEN >(OPCG_ALIGN_INOP_ALIGN_VAL);
        l_data64.insertFromRight< OPCG_ALIGN_INOP_WAIT, OPCG_ALIGN_INOP_WAIT_LEN >(OPCG_ALIGN_INOP_WAIT_VAL);
        l_data64.insertFromRight< OPCG_ALIGN_SCAN_RATIO, OPCG_ALIGN_SCAN_RATIO_LEN >(OPCG_ALIGN_SCANRATIO_4to1);
        l_data64.insertFromRight< OPCG_ALIGN_OPCG_WAIT_CYCLES, OPCG_ALIGN_OPCG_WAIT_CYCLES_LEN >(OPCG_ALIGN_WAIT_CYCLES_VAL);
        FAPI_TRY(fapi2::putScom(l_mc_iohs, OPCG_ALIGN, l_data64));
        FAPI_TRY(fapi2::putScom(l_mc_pci, OPCG_ALIGN, l_data64));
        FAPI_TRY(fapi2::putScom(l_mc_mc, OPCG_ALIGN, l_data64));

        FAPI_DBG("Reset PCB Slave error register");
        FAPI_TRY(fapi2::putScom(l_mc_iohs, ERROR_REG, ERROR_REG_VALUE));
        FAPI_TRY(fapi2::putScom(l_mc_pci, ERROR_REG, ERROR_REG_VALUE));
        FAPI_TRY(fapi2::putScom(l_mc_mc, ERROR_REG, ERROR_REG_VALUE));

        FAPI_DBG(" Unmasking pll unlock error in   Pcb slave config reg");

        for (auto& targ : l_iohs_pci_mc)
        {
            FAPI_TRY(fapi2::getScom(targ, SLAVE_CONFIG_REG, l_data64));
            l_data64.clearBit<12>();
            FAPI_TRY(fapi2::putScom(targ, SLAVE_CONFIG_REG, l_data64));
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
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_read_reg;
    FAPI_INF("p10_sbe_chiplet_pll_setup_check_pll_lock: Entering ...");

    FAPI_DBG("Check  PLL lock");
    FAPI_TRY(fapi2::getScom(i_target_cplt, PLL_LOCK_REG, l_read_reg));

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
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_bypass: Entering ...");

    FAPI_DBG("Drop PLL Reset");
    l_data64.flush<1>().clearBit<NET_CTRL0_PLL_BYPASS>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, NET_CTRL0_RW_WAND, l_data64));

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
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_reset: Entering ...");

    FAPI_DBG("Drop PLL Reset");
    l_data64.flush<1>().clearBit<NET_CTRL0_PLL_RESET>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, NET_CTRL0_RW_WAND, l_data64));

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
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_test_enable: Entering ...");

    FAPI_DBG("Release PLL test enable");
    l_data64.flush<1>().clearBit<NET_CTRL0_PLL_TEST_EN>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, NET_CTRL0_RW_WAND, l_data64));

    FAPI_INF("p10_sbe_chiplet_pll_setup_pll_test_enable: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop Pll region fence for Axon, Pci, Mc chiplets
///
/// @param[in]     i_mcast_target   Reference to TARGET_TYPE_PERV target or Multicast target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_chiplet_pll_setup_drop_pll_region_fence(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_chiplet_pll_setup_drop_pll_region_fence: Entering ...");

    FAPI_DBG("Drop Pll region fence for Axon, Pci, Mc chiplets");
    l_data64.flush<0>().setBit<CPLT_CTRL1_REGION10_FENCE_DC>();
    FAPI_TRY(fapi2::putScom(i_mcast_target, CPLT_CTRL1_WO_CLEAR, l_data64));

    FAPI_INF("p10_sbe_chiplet_pll_setup_drop_pll_region_fence: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
