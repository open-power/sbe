/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_chiplet_reset.C $ */
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
// *HWP Level           : 3
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
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_MC_XBUS(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value, uint8_t axone_only);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_obus(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value, uint8_t axone_only);

static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_pcie(
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

static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

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

static fapi2::ReturnCode p9_sbe_chiplet_reset_all_obus_scan0(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip);

static fapi2::ReturnCode p9_sbe_chiplet_reset_sectorbuffer_pulsemode_attr_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip);

static fapi2::ReturnCode p9_sbe_chiplet_reset_assert_scan_clk(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_chiplet_reset(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    // Local variable
    uint8_t l_mc_sync_mode = 0;
    uint8_t l_pll_bypass = 0;
    uint8_t l_attr_axone_only;
    fapi2::buffer<uint8_t> l_read_attr;
#ifndef __PPE__
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    uint8_t l_attr_system_ipl_phase;
#endif
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint8_t attr_force_all = 0;
    uint8_t l_use_dmi_buckets = 0;

    // Created Vectors before hand instead of calling getChildren for each usage
    auto l_perv_func_WO_Core_Cache = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                         static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                                 fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                                 fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
                                         fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_perv_func = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                           static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                   fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                   fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                   fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
                           fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_perv_pres = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                           static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_CACHES |
                                   fapi2::TARGET_FILTER_ALL_CORES | fapi2::TARGET_FILTER_ALL_MC |
                                   fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                   fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
                           fapi2::TARGET_STATE_PRESENT);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_mc_sync_mode),
             "Error from FAPI_ATTR_GET (ATTR_MC_SYNC_MODE)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, i_target_chip, l_pll_bypass),
             "Error from FAPI_ATTR_GET (ATTR_NEST_MEM_X_O_PCI_BYPASS)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS, i_target_chip, l_use_dmi_buckets),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS)");

    FAPI_INF("p9_sbe_chiplet_reset: Entering ...");

    FAPI_DBG("Do a scan0 to all obus chiplets independent of PG information");

    for (auto& obus : l_perv_pres)
    {
        uint32_t l_chipletID = obus.getChipletNumber();

        if(l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID)
        {
            FAPI_TRY(p9_sbe_chiplet_reset_all_obus_scan0(obus));
        }
    }

    // Setup Perv into MCG0
    FAPI_DBG("Adding PERV to Multicast group 0");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_MULTICAST_GROUP_1,
                            p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0));

    // Setup cache/cores multicast groups only in FORCE_ALL_CORE mode.
    // If not in FORCE_ALL_CORE mode, cache/core multicast groups will be setup
    // in preparation of p9_sbe_select_ex in preparation of istep 4.

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYS_FORCE_ALL_CORES,
                           FAPI_SYSTEM,
                           attr_force_all));

    if (attr_force_all)
    {
        for (auto& targ : l_perv_func)
        {
            // Cache & Core - Functional
            uint32_t l_chipletID = targ.getChipletNumber();

            if((l_chipletID >= EQ0_CHIPLET_ID && l_chipletID <= EQ5_CHIPLET_ID) ||
               (l_chipletID >= EC0_CHIPLET_ID && l_chipletID <= EC23_CHIPLET_ID))
            {
                // Configuring chiplet multicasting registers..
                FAPI_DBG("Configuring cache/core chiplet multicasting registers");
                FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(targ));
            }
        }
    }

    // do not do other setup if not PPE and cache_contained mode.
#ifndef __PPE__
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, l_sys,
                           l_attr_system_ipl_phase));

    if (l_attr_system_ipl_phase !=
        fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_CACHE_CONTAINED) //Skip for cache contained.
    {
#endif
        uint8_t l_is_p9c;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_P9C_LOGIC_ONLY,
                               i_target_chip,
                               l_is_p9c));

        // NEST OBUS XBUS PCI MC - Functional
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {
            FAPI_DBG("Configuring multicasting registers for Mc,Nest,Xb,Obus,pcie chiplets");

            // if in ASYNC mode DO NOT add to multicast groups because the chiplet is non
            // responsive.  Wait until clocks are started up in hostboot
            uint32_t l_chipletID = targ.getChipletNumber();

            if((l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID) &&
               (!l_mc_sync_mode && !l_use_dmi_buckets))
            {
                continue;
            }

            FAPI_TRY(p9_sbe_chiplet_reset_mc_setup(targ));
        }

        // NEST OBUS XBUS PCI MC CACHE CORE - Functional
        for (auto& targ : l_perv_func)
        {
            // Configuring NET control registers into Default required value
            FAPI_DBG("Restore NET_CTRL0&1 init value - for all chiplets except TP");
            FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_net_cntl_setup(targ));

            FAPI_DBG("Setup hang pulse counter for all perv chiplet except TP");
            FAPI_TRY(p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(targ));
        }

        FAPI_DBG("Clock mux settings");

        for (auto& targ : l_perv_pres)
        {
            fapi2::buffer<uint32_t> l_read_attr;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCK_PLL_MUX, i_target_chip, l_read_attr));
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_P9A_LOGIC_ONLY, i_target_chip, l_attr_axone_only));
            uint32_t l_chipletID = targ.getChipletNumber();

            // MC & XBUS
            if((l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID) || (l_chipletID == XB_CHIPLET_ID ))
            {
                FAPI_DBG("Mux settings for Mc/Xbus chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_MC_XBUS(targ, l_read_attr, l_attr_axone_only));
            }
            // OBUS
            else if(l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID)
            {
                FAPI_DBG("Mux settings for OB chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_obus(targ, l_read_attr, l_attr_axone_only));
            }
            // PCI
            else if(l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID)
            {
                FAPI_DBG("Mux settings for Pcie chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_clk_mux_pcie(targ, l_read_attr));
            }
        }

        FAPI_DBG("Sector buffer strength and pulse mode setup");

        // MC XBUS OBUS PCI
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID)
            {
                continue;
            }

            FAPI_TRY(p9_sbe_chiplet_reset_sectorbuffer_pulsemode_attr_setup(targ));
        }


        //Perv
        for (auto& targ : l_perv_pres)
        {
            uint32_t l_chipletID = targ.getChipletNumber();
            // Setting up partial good fence drop and resetting chiplet.
            FAPI_DBG("PLL Setup : Enable pll");
            FAPI_TRY(p9_sbe_chiplet_reset_pll_setup(targ, true));

            if(l_chipletID == N3_CHIPLET_ID)
            {
                FAPI_DBG("Drop clk async reset for N3 chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_nest_ob_async_reset(targ));
            }

            if(l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID)
            {
                FAPI_DBG("Drop clk async reset for Mc chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_mc_async_reset_setup(targ, true));
            }
        }

        fapi2::delay(10000, (40 * 400));

        // Perv
        for (auto& targ : l_perv_pres)
        {
            // Setting up partial good fence drop and resetting chiplet.
            FAPI_DBG("PLL setup : Disable pll");
            FAPI_TRY(p9_sbe_chiplet_reset_pll_setup(targ, false));
        }

        for (auto& targ : l_perv_pres)
        {
            // MC
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID)
            {
                FAPI_DBG("Raise clk async reset for Mc chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_mc_async_reset_setup(targ, false));
            }
        }

        for (auto& targ : l_perv_func)
        {
            //MC
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID)
            {
                FAPI_DBG("Drop clk_div_bypass for Mc chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_div_clk_bypass(targ));

                //Setting VITL_AL config bit to disable listening to cross-chiplet DDR sync signal
                FAPI_DBG("Set VITL_AL for MC chiplet");
                FAPI_TRY(fapi2::putScom(targ, PERV_NET_CTRL0_WOR,
                                        p9SbeChipletReset::NET_CNTL0_SET_VITL_AL));
            }
        }

        // Perv
        for (auto& targ : l_perv_func)
        {
            FAPI_DBG("Enable chiplet and reset error register");
            FAPI_TRY(p9_sbe_chiplet_reset_setup(targ));
        }

        // Perv without Core/Cache
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {
            FAPI_DBG("Drop lvltrans fence and endpoint reset");
            FAPI_TRY(p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset(
                         targ));
        }

        // Perv without Core/Cache
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {
            FAPI_DBG("Initialize OPCG registers for Nest,MC,XB,OB,PCIe");
            FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg(targ));
        }

        // NEST OBUS PCI XBUS
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID)
            {
                continue;
            }

            FAPI_DBG("Enable listen to sync for NEST,OB,XB,PCIe");
            FAPI_TRY(p9_sbe_chiplet_reset_enable_listen_to_sync(targ, true));
        }

        // MC OBUS PCI XBUS
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID)
            {
                continue;
            }

            FAPI_DBG("Disable listen_to_sync for MC,XB,OB,PCIe");
            FAPI_TRY(p9_sbe_chiplet_reset_enable_listen_to_sync(targ, false));
        }

        FAPI_DBG("Set Chip-wide HSSPORWREN gate");
        FAPI_TRY(p9_sbe_chiplet_reset_hsspowergate(i_target_chip));

        // PCI
        for (auto& targ : l_perv_func)
        {
            //PCI
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID)
            {
                FAPI_DBG("Setup IOP Logic for PCIe");
                FAPI_TRY(p9_sbe_chiplet_reset_setup_iop_logic(targ));
            }
        }

        // lower scan ratio for chiplets operating at PLL speed
        // Nest: on pll, if not in bypass
        // MC:   on pll, if not in bypsas, and if in sync mode
        if (!l_pll_bypass)
        {
            if (l_mc_sync_mode || l_use_dmi_buckets)
            {
                for (auto& targ : l_perv_func)
                {
                    // NEST MC
                    uint32_t l_chipletID = targ.getChipletNumber();

                    if((l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID) ||
                       (l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID))
                    {
                        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(targ));
                    }
                }
            }
            else
            {
                for (auto& targ : l_perv_func)
                {
                    // NEST
                    uint32_t l_chipletID = targ.getChipletNumber();

                    if(l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID)
                    {
                        FAPI_TRY(p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(targ));
                    }
                }
            }
        }

        FAPI_DBG("reading ec_level attr HW404176_ASSERT_SCAN_CLK");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW404176_ASSERT_SCAN_CLK,
                               i_target_chip, l_read_attr));

        // Perv without Core/Cache
        for (auto& targ : l_perv_func_WO_Core_Cache)
        {

            FAPI_TRY(p9_sbe_chiplet_reset_scan0_call(targ));

            uint32_t l_chipletID = targ.getChipletNumber();

            if((l_read_attr) && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID)) // cumulus chip & MC chiplet
            {
                FAPI_DBG("assert SCAN_CLK_USE_EVEN=1 in OPCG_REG1 for cumulus chip Mc chiplet");
                FAPI_TRY(p9_sbe_chiplet_reset_assert_scan_clk(targ));
            }
        }

        for (auto& targ : l_perv_func)
        {
            //MC
            uint32_t l_chipletID = targ.getChipletNumber();

            if (l_is_p9c && (l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID))
            {
                FAPI_DBG("Set TC_IOM_FASTX2_RATIO_DC");
                FAPI_TRY(fapi2::putScom(targ,
                                        PERV_CPLT_CONF1_OR,
                                        p9SbeChipletReset::MC_CPLT_CONF1_FASTX2_RATIO_MASK));
            }
        }

#ifndef __PPE__
    }

#endif

    FAPI_INF("p9_sbe_chiplet_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


/// @brief assert SCAN_CLK_USE_EVEN=1 in OPCG_REG1 for cumulus chip Mc chiplet
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_assert_scan_clk(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("p9_sbe_chiplet_reset_assert_scan_clk: Entering ...");
    fapi2::buffer<uint64_t> l_data64;

    FAPI_DBG("assert SCAN_CLK_USE_EVEN=1 in OPCG_REG1 for cumulus chip Mc chiplet");
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_OPCG_REG1, l_data64));
    l_data64.setBit<PERV_1_OPCG_REG1_SCAN_CLK_USE_EVEN>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_REG1, l_data64));


    FAPI_INF("p9_sbe_chiplet_reset_assert_scan_clk: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
/// @brief Setting up hang pulse counter for all parital good chiplet except for Tp
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup: Entering ...");
    fapi2::buffer<uint64_t> l_data64;
    uint8_t   l_attr_chipunit_pos = 0;

    // Local variables
    uint8_t i_reg0_val = 0xff;
    uint8_t i_reg1_val = 0xff;
    uint8_t i_reg2_val = 0xff;
    uint8_t i_reg3_val = 0xff;
    uint8_t i_reg4_val = 0xff;
    uint8_t i_reg5_val = 0xff;
    uint8_t i_reg6_val = 0xff;

    uint32_t l_chipletID = i_target_cplt.getChipletNumber();

    // MC Perv Targets || PCI Perv Targets
    if((l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID) || (l_chipletID >= PCI0_CHIPLET_ID
            && l_chipletID <= PCI2_CHIPLET_ID))
    {
        i_reg0_val = p9SbeChipletReset::HANG_PULSE_0X10;
        i_reg6_val = p9SbeChipletReset::HANG_PULSE_0X10;
    }
    // OBUS & XBUS Perv Targets
    else if((l_chipletID == XB_CHIPLET_ID) || (l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID))
    {
        i_reg0_val = p9SbeChipletReset::HANG_PULSE_0X10;
        i_reg1_val = p9SbeChipletReset::HANG_PULSE_0X04;
        i_reg6_val = p9SbeChipletReset::HANG_PULSE_0X10;
    }
    // Core Perv Target
    else  if(l_chipletID >= EC0_CHIPLET_ID && l_chipletID <= EC23_CHIPLET_ID)
    {
        i_reg0_val = p9SbeChipletReset::HANG_PULSE_0X10;
        i_reg5_val = p9SbeChipletReset::HANG_PULSE_0X06;
        i_reg6_val = p9SbeChipletReset::HANG_PULSE_0X10;
    }
    // Cache Perv Target
    else if(l_chipletID >= EQ0_CHIPLET_ID && l_chipletID <= EQ5_CHIPLET_ID)
    {
        i_reg0_val = p9SbeChipletReset::HANG_PULSE_0X10;
        i_reg1_val = p9SbeChipletReset::HANG_PULSE_0X01;
        i_reg2_val = p9SbeChipletReset::HANG_PULSE_0X01;
        i_reg3_val = p9SbeChipletReset::HANG_PULSE_0X04;
        i_reg4_val = p9SbeChipletReset::HANG_PULSE_0X00;
        i_reg5_val = p9SbeChipletReset::HANG_PULSE_0X06;
        i_reg6_val = p9SbeChipletReset::HANG_PULSE_0X10;
    }

    // NEST Perv Target
    else if(l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID)
    {
        i_reg0_val = p9SbeChipletReset::HANG_PULSE_0X10;
        i_reg5_val = p9SbeChipletReset::HANG_PULSE_0X06;
        i_reg6_val = p9SbeChipletReset::HANG_PULSE_0X10;
    }

    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    if (i_reg0_val != 0xff)
    {
        //HANG_PULSE_0_REG.HANG_PULSE_REG_0 = (i_reg0_val != 0xff) ? i_reg0_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_0_REG_0, PERV_1_HANG_PULSE_0_REG_0_LEN>(i_reg0_val);
        //HANG_PULSE_0_REG.SUPPRESS_HANG_0 = (i_reg0_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_0_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_0_REG, l_data64));
    }

    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    if (i_reg1_val != 0xff)
    {
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = (i_reg1_val != 0xff) ? i_reg1_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_1_REG_1, PERV_1_HANG_PULSE_1_REG_1_LEN>(i_reg1_val);
        //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = (i_reg1_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_1_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
    }

    //Setting HANG_PULSE_2_REG register value (Setting all fields)
    if (i_reg2_val != 0xff)
    {
        //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = (i_reg2_val != 0xff) ? i_reg2_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_2_REG_2, PERV_1_HANG_PULSE_2_REG_2_LEN>(i_reg2_val);
        //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = (i_reg2_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_2_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
    }

    //Setting HANG_PULSE_3_REG register value (Setting all fields)
    if (i_reg3_val != 0xff)
    {
        //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = (i_reg3_val != 0xff) ? i_reg3_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_3_REG_3, PERV_1_HANG_PULSE_3_REG_3_LEN>(i_reg3_val);
        //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = (i_reg3_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_3_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
    }

    //Setting HANG_PULSE_4_REG register value (Setting all fields)
    if (i_reg4_val != 0xff)
    {
        //HANG_PULSE_4_REG.HANG_PULSE_REG_4 = (i_reg4_val != 0xff) ? i_reg4_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_4_REG_4, PERV_1_HANG_PULSE_4_REG_4_LEN>(i_reg4_val);
        //HANG_PULSE_4_REG.SUPPRESS_HANG_4 = (i_reg4_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_4_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_4_REG, l_data64));
    }

    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    if (i_reg5_val != 0xff)
    {
        //HANG_PULSE_5_REG.HANG_PULSE_REG_5 = (i_reg5_val != 0xff) ? i_reg5_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_5_REG_5, PERV_1_HANG_PULSE_5_REG_5_LEN>(i_reg5_val);
        //HANG_PULSE_5_REG.SUPPRESS_HANG_5 = (i_reg5_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_5_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_5_REG, l_data64));
    }

    //Setting HANG_PULSE_6_REG register value (Setting all fields)
    if (i_reg6_val != 0xff)
    {
        //HANG_PULSE_6_REG.HANG_PULSE_REG_6 = (i_reg6_val != 0xff) ? i_reg6_val
        l_data64.insertFromRight<PERV_1_HANG_PULSE_6_REG_6, PERV_1_HANG_PULSE_6_REG_6_LEN>(i_reg6_val);
        //HANG_PULSE_6_REG.SUPPRESS_HANG_6 = (i_reg6_val != 0xff) ? 0
        l_data64.clearBit<PERV_1_HANG_PULSE_6_REG_SUPPRESS>();
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_6_REG, l_data64));
    }

    // Core Perv Target
    if(l_chipletID >= EC0_CHIPLET_ID && l_chipletID <= EC23_CHIPLET_ID)
    {
        //Setting HANG_PULSE_1_REG register value (Setting all fields)
        //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X1A
        l_data64.insertFromRight<PERV_1_HANG_PULSE_1_REG_1, PERV_1_HANG_PULSE_1_REG_1_LEN>(p9SbeChipletReset::HANG_PULSE_0X1A);
        l_data64.setBit<PERV_1_HANG_PULSE_1_REG_SUPPRESS>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 1
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
    }

    // NEST Perv Target
    if(l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID)
    {
        // Collecting partial good and chiplet unit position attribute
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_cplt,
                               l_attr_chipunit_pos));

        if ( l_attr_chipunit_pos == N0_CHIPLET_ID )
        {
            //Setting HANG_PULSE_1_REG register value (Setting all fields)
            //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X18
            l_data64.insertFromRight<PERV_1_HANG_PULSE_1_REG_1, PERV_1_HANG_PULSE_1_REG_1_LEN>(p9SbeChipletReset::HANG_PULSE_0X18);
            l_data64.setBit<PERV_1_HANG_PULSE_1_REG_SUPPRESS>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
            //Setting HANG_PULSE_2_REG register value (Setting all fields)
            //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X22
            l_data64.insertFromRight<PERV_1_HANG_PULSE_2_REG_2, PERV_1_HANG_PULSE_2_REG_2_LEN>(p9SbeChipletReset::HANG_PULSE_0X22);
            l_data64.setBit<PERV_1_HANG_PULSE_2_REG_SUPPRESS>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
            //Setting HANG_PULSE_3_REG register value (Setting all fields)
            //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X12
            l_data64.insertFromRight<PERV_1_HANG_PULSE_3_REG_3, PERV_1_HANG_PULSE_3_REG_3_LEN>(p9SbeChipletReset::HANG_PULSE_0X12);
            l_data64.setBit<PERV_1_HANG_PULSE_3_REG_SUPPRESS>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
        }

        else if ( l_attr_chipunit_pos == N1_CHIPLET_ID )
        {
            //Setting HANG_PULSE_2_REG register value (Setting all fields)
            //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X0F
            l_data64.insertFromRight<PERV_1_HANG_PULSE_2_REG_2, PERV_1_HANG_PULSE_2_REG_2_LEN>(p9SbeChipletReset::HANG_PULSE_0X0F);
            l_data64.setBit<PERV_1_HANG_PULSE_2_REG_SUPPRESS>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
        }

        else if ( l_attr_chipunit_pos == N2_CHIPLET_ID )
        {
            //Setting HANG_PULSE_3_REG register value (Setting all fields)
            //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X12
            l_data64.insertFromRight<PERV_1_HANG_PULSE_3_REG_3, PERV_1_HANG_PULSE_3_REG_3_LEN>(p9SbeChipletReset::HANG_PULSE_0X12);
            l_data64.setBit<PERV_1_HANG_PULSE_3_REG_SUPPRESS>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
        }

        else if ( l_attr_chipunit_pos == N3_CHIPLET_ID )
        {
            //Setting HANG_PULSE_1_REG register value (Setting all fields)
            //HANG_PULSE_1_REG.HANG_PULSE_REG_1 = p9SbeChipletReset::HANG_PULSE_0X17
            l_data64.insertFromRight<PERV_1_HANG_PULSE_1_REG_1, PERV_1_HANG_PULSE_1_REG_1_LEN>(p9SbeChipletReset::HANG_PULSE_0X17);
            l_data64.setBit<PERV_1_HANG_PULSE_1_REG_SUPPRESS>();  //HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_1_REG, l_data64));
            //Setting HANG_PULSE_2_REG register value (Setting all fields)
            //HANG_PULSE_2_REG.HANG_PULSE_REG_2 = p9SbeChipletReset::HANG_PULSE_0X0F
            l_data64.insertFromRight<PERV_1_HANG_PULSE_2_REG_2, PERV_1_HANG_PULSE_2_REG_2_LEN>(p9SbeChipletReset::HANG_PULSE_0X0F);
            l_data64.setBit<PERV_1_HANG_PULSE_2_REG_SUPPRESS>();  //HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_2_REG, l_data64));
            //Setting HANG_PULSE_3_REG register value (Setting all fields)
            //HANG_PULSE_3_REG.HANG_PULSE_REG_3 = p9SbeChipletReset::HANG_PULSE_0X13
            l_data64.insertFromRight<PERV_1_HANG_PULSE_3_REG_3, PERV_1_HANG_PULSE_3_REG_3_LEN>(p9SbeChipletReset::HANG_PULSE_0X13);
            l_data64.setBit<PERV_1_HANG_PULSE_3_REG_SUPPRESS>();  //HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_3_REG, l_data64));
            //Setting HANG_PULSE_4_REG register value (Setting all fields)
            //HANG_PULSE_4_REG.HANG_PULSE_REG_4 = p9SbeChipletReset::HANG_PULSE_0X1C
            l_data64.insertFromRight<PERV_1_HANG_PULSE_4_REG_4, PERV_1_HANG_PULSE_4_REG_4_LEN>(p9SbeChipletReset::HANG_PULSE_0X1C);
            l_data64.setBit<PERV_1_HANG_PULSE_4_REG_SUPPRESS>();  //HANG_PULSE_4_REG.SUPPRESS_HANG_4 = 1
            FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_HANG_PULSE_4_REG, l_data64));
        }
    }

    FAPI_INF("p9_sbe_chiplet_reset_all_cplt_hang_cnt_setup: Exiting ...");

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
    fapi2::buffer<uint8_t> l_read_attr;
    FAPI_INF("p9_sbe_chiplet_reset_all_cplt_net_cntl_setup: Entering ...");

    //Setting NET_CTRL0 register value
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target_cplt.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    FAPI_DBG("Disable local clock gating VITAL");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_VITL_CLOCK_GATING,
                           l_chip, l_read_attr));

    if (l_read_attr)
    {
        //NET_CTRL0 = p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE_FOR_DD1
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL0,
                                p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE_FOR_DD1));
    }
    else
    {
        //NET_CTRL0 = p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE
        FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL0,
                                p9SbeChipletReset::NET_CNTL0_HW_INIT_VALUE));
    }

    //Setting NET_CTRL1 register value
    //NET_CTRL1 = p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_NET_CTRL1,
                            p9SbeChipletReset::NET_CNTL1_HW_INIT_VALUE));

    FAPI_INF("p9_sbe_chiplet_reset_all_cplt_net_cntl_setup:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock mux settings for Mc/Xbus chiplet
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clk_mux_value    clock mux value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_clk_mux_MC_XBUS(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint32_t> i_clk_mux_value, uint8_t axone_only)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_reset_clk_mux_MC_XBUS: Entering ...");
    uint32_t l_chipletID = i_target_chiplet.getChipletNumber();

    //Setting NET_CTRL1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));

    //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<3>()
    if(l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID) //MC
    {
        if (axone_only)
        {
            FAPI_DBG("Mux setings n/a for Axone for MC chiplet");
        }
        else // Nimbus, Cumulus
        {
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<3>());
        }
    }
    else // XBUS  // Nimbus, Cumulus, Axone
    {
        l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<8>());
    }

    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_clk_mux_MC_XBUS: Exiting ...");

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
    const fapi2::buffer<uint32_t> i_clk_mux_value, uint8_t axone_only)
{
    uint8_t l_attr_unit_pos = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_reset_clk_mux_obus: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                           l_attr_unit_pos));
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));

    if ( l_attr_unit_pos == OB0_CHIPLET_ID )
    {
        if (axone_only)
        {
            FAPI_DBG("mux settings for OB0 - Axone");
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<22>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>(i_clk_mux_value.getBit<28>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>(i_clk_mux_value.getBit<29>());
        }
        else  // Nimbus, Cumulus
        {
            //Setting NET_CTRL1 register value
            //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<6>()
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<6>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
            (i_clk_mux_value.getBit<13>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<13>()
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<15>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<15>()
        }
    }

    else if ( l_attr_unit_pos == OB1_CHIPLET_ID )
    {
        if (axone_only)
        {
            FAPI_DBG("Mux settings n/a for OB1 - Axone");
        }
        else
        {
            //Setting NET_CTRL1 register value
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>
            (i_clk_mux_value.getBit<16>());  //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<16>()
        }
    }

    else if ( l_attr_unit_pos == OB2_CHIPLET_ID )
    {
        if (axone_only)
        {
            FAPI_DBG("Mux settings n/a for OB2 - Axone");
        }
        else
        {
            //Setting NET_CTRL1 register value
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>
            (i_clk_mux_value.getBit<17>());  //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<17>()
        }
    }

    else if ( l_attr_unit_pos == OB3_CHIPLET_ID )
    {
        if (axone_only)
        {
            FAPI_DBG("mux settings for OB3 - Axone");
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<23>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>(i_clk_mux_value.getBit<27>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>(i_clk_mux_value.getBit<30>());
        }
        else
        {
            //Setting NET_CTRL1 register value
            //NET_CTRL1.PLL_CLKIN_SEL = i_clk_mux_value.getBit<7>()
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>(i_clk_mux_value.getBit<7>());
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
            (i_clk_mux_value.getBit<9>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = i_clk_mux_value.getBit<9>()
            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<14>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = i_clk_mux_value.getBit<14>()
        }
    }

    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    FAPI_INF("p9_sbe_chiplet_reset_clk_mux_obus: Exiting ...");

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
    uint8_t l_use_ss_pll = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_reset_clk_mux_pcie: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chiplet,
                           l_attr_unit_pos),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

    // leverage SS filter PLL to feed PCI PLLs, instead of IO filter PLL
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SLOW_PCI_REF_CLOCK_ENABLE,
                           i_target_chiplet.getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                           l_use_ss_pll),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_SLOW_PCI_REF_CLOCK_ENABLE)");

    if (l_use_ss_pll)
    {
        fapi2::ATTR_DD1_SLOW_PCI_REF_CLOCK_Type l_slow_ref_clock;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DD1_SLOW_PCI_REF_CLOCK,
                               fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                               l_slow_ref_clock),
                 "Error from FAPI_ATTR_GET (ATTR_DD1_SLOW_PCI_REF_CLOCK)");

        if (l_slow_ref_clock != fapi2::ENUM_ATTR_DD1_SLOW_PCI_REF_CLOCK_SLOW)
        {
            l_use_ss_pll = 0;
        }
    }

    if ( l_attr_unit_pos != 0x0E )
    {
        //Setting NET_CTRL1 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));

        if (l_use_ss_pll)
        {
            l_data64.clearBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>();
        }
        else
        {
            l_data64.writeBit<PERV_1_NET_CTRL1_PLL_CLKIN_SEL>((l_attr_unit_pos == 0x0D) ?
                    i_clk_mux_value.getBit<5>() :
                    i_clk_mux_value.getBit<4>());  //NET_CTRL1.PLL_CLKIN_SEL = (l_attr_unit_pos == 0x0D)? i_clk_mux_value.getBit<5>() :  i_clk_mux_value.getBit<4>()
        }

        if (l_attr_unit_pos == 0x0D)
        {
            if (l_use_ss_pll)
            {
                l_data64.setBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>();
            }
            else
            {
                l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX0_SEL>
                (i_clk_mux_value.getBit<10>());  //NET_CTRL1.REFCLK_CLKMUX0_SEL = (l_attr_unit_pos == 0x0D)? i_clk_mux_value.getBit<10>()
            }

            l_data64.writeBit<PERV_1_NET_CTRL1_REFCLK_CLKMUX1_SEL>
            (i_clk_mux_value.getBit<11>());  //NET_CTRL1.REFCLK_CLKMUX1_SEL = (l_attr_unit_pos == 0x0D)? i_clk_mux_value.getBit<11>()
        }

        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64));
    }

    FAPI_INF("p9_sbe_chiplet_reset_clk_mux_pcie: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_div_clk_bypass: Entering ...");

    FAPI_DBG("drop clk_div_bypass_en");
    //Setting NET_CTRL1 register value
    l_data64.flush<1>();
    //NET_CTRL1.CLK_DIV_BYPASS_EN = 0
    l_data64.clearBit<PERV_1_NET_CTRL1_CLK_DIV_BYPASS_EN>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1_WAND, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_div_clk_bypass: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_enable_listen_to_sync: Entering ...");

    //Setting SYNC_CONFIG register value
    //SYNC_CONFIG = i_enable? p9SbeChipletReset::SYNC_CONFIG_DEFAULT : p9SbeChipletReset::SYNC_CONFIG_4TO1
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SYNC_CONFIG,
                            i_enable ? p9SbeChipletReset::SYNC_CONFIG_DEFAULT :
                            p9SbeChipletReset::SYNC_CONFIG_4TO1));

    FAPI_INF("p9_sbe_chiplet_reset_enable_listen_to_sync: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_hsspowergate: Entering ...");

    //Getting ROOT_CTRL2 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL2_SCOM,
                            l_read_reg)); //l_read_reg = PIB.ROOT_CTRL2

    l_read_reg.setBit<PERV_ROOT_CTRL2_TPFSI_TC_HSSPORWREN_ALLOW>();

    FAPI_DBG("Set Chip-wide HSSPORWREN gate");
    //Setting ROOT_CTRL2 register value
    //PIB.ROOT_CTRL2 = l_read_reg
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL2_SCOM, l_read_reg));

    FAPI_INF("p9_sbe_chiplet_reset_hsspowergate: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_mc_async_reset_setup: Entering ...");

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

    FAPI_INF("p9_sbe_chiplet_reset_mc_async_reset_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring multicast registers for nest, cache, core, obus,
///        pci, xbus, mc
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_mc_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    FAPI_INF("p9_sbe_chiplet_reset_mc_setup: Entering ...");
    uint64_t i_mc_grp1_val = 0;
    uint64_t i_mc_grp2_val = 0;
    uint64_t i_mc_grp3_val = 0;

    uint32_t l_chipletID = i_target_chiplet.getChipletNumber();

    // Core
    if(l_chipletID >= EC0_CHIPLET_ID && l_chipletID <= EC23_CHIPLET_ID)
    {
        i_mc_grp1_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0;
        i_mc_grp2_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP1;
        i_mc_grp3_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP3;
    }
    // Nest/Obus/PCI/Xbus
    else if((l_chipletID == XB_CHIPLET_ID) || (l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID) ||
            (l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID) ||
            (l_chipletID >= N0_CHIPLET_ID && l_chipletID <= N3_CHIPLET_ID))
    {
        i_mc_grp1_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0;
    }
    // MC
    else if(l_chipletID >= MC01_CHIPLET_ID && l_chipletID <= MC23_CHIPLET_ID)
    {
        i_mc_grp1_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0;
        i_mc_grp2_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP2;
    }
    // Cache
    else if(l_chipletID >= EQ0_CHIPLET_ID && l_chipletID <= EQ5_CHIPLET_ID)
    {
        i_mc_grp1_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP0;
        i_mc_grp2_val = p9SbeChipletReset::MCGR_CNFG_SETTING_GROUP4;
    }

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

    // Only For Cache
    if(l_chipletID >= EQ0_CHIPLET_ID && l_chipletID <= EQ5_CHIPLET_ID)
    {
        uint16_t l_attr_pg = 0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

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
    }

    FAPI_INF("p9_sbe_chiplet_reset_mc_setup: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_nest_ob_async_reset: Entering ...");

    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    //NET_CTRL0.CLK_ASYNC_RESET = 0
    l_data64.clearBit<PERV_1_NET_CTRL0_CLK_ASYNC_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_nest_ob_async_reset: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset: Entering ...");

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

    FAPI_DBG("Assert sram enable");
    //Setting NET_CTRL0 register value
    l_data64.flush<0>();
    l_data64.setBit<23>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_net_ctrl_lvltrans_fence_pcb_ep_reset: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_opcg_cnfg: Entering ...");

    //Setting OPCG_ALIGN register value
    l_data64 =
        p9SbeChipletReset::OPCG_ALIGN_SETTING;  //OPCG_ALIGN = p9SbeChipletReset::OPCG_ALIGN_SETTING
    //OPCG_ALIGN.INOP_ALIGN = p9SbeChipletReset::INOP_ALIGN_SETTING_0X5
    l_data64.insertFromRight<0, 4>(p9SbeChipletReset::INOP_ALIGN_SETTING_0X5);
    l_data64.clearBit<PERV_1_OPCG_ALIGN_INOP_WAIT, PERV_1_OPCG_ALIGN_INOP_WAIT_LEN>();  //OPCG_ALIGN.INOP_WAIT = 0
    //OPCG_ALIGN.OPCG_WAIT_CYCLES = p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020
    l_data64.insertFromRight<52, 12>(p9SbeChipletReset::OPCG_WAIT_CYCLE_0X020);
    l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
    (p9SbeChipletReset::SCAN_RATIO_0X0);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_ALIGN, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_opcg_cnfg: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief set scan ratio to 4:1 for chiplets running at PLL speed
///
/// @param[in]     i_target_cplt   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio: Entering ...");

    FAPI_DBG("Set scan ratio to 4:1 for at speed PLL");
    //Setting OPCG_ALIGN register value
    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
    (p9SbeChipletReset::SCAN_RATIO_0X3);  //OPCG_ALIGN.SCAN_RATIO = p9SbeChipletReset::SCAN_RATIO_0X3
    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_OPCG_ALIGN, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_opcg_cnfg_scan_ratio: Exiting ...");

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
    fapi2::buffer<uint64_t> l_data;
    FAPI_INF("p9_sbe_chiplet_reset_pll_setup: Entering ...");

    if ( i_enable )
    {
        l_data.flush<0>();
        l_data.setBit<PERV_1_NET_CTRL0_PLLFORCE_OUT_EN>();

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
            l_data.clearBit<PERV_1_NET_CTRL0_PLLFORCE_OUT_EN>();

            FAPI_DBG("Disable PLL");
            //Setting NET_CTRL0 register value
            //NET_CTRL0 = l_data
            FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data));
        }
    }

    FAPI_INF("p9_sbe_chiplet_reset_pll_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Scan0 module call
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_scan0_call(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint16_t> l_regions;
    FAPI_INF("p9_sbe_chiplet_reset_scan0_call: Entering ...");

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(i_target_chiplet,
             p9SbeChipletReset::REGIONS_EXCEPT_VITAL, l_regions));

    FAPI_DBG("run scan0 module for region except vital and pll, scan types GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, l_regions,
                                          p9SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("run scan0 module for region except vital and pll, scan types except GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, l_regions,
                                          p9SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_INF("p9_sbe_chiplet_reset_scan0_call: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_setup: Entering ...");

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

    FAPI_INF("p9_sbe_chiplet_reset_setup: Exiting ...");

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
    FAPI_INF("p9_sbe_chiplet_reset_setup_iop_logic: Entering ...");

    //Setting CPLT_CONF1 register value
    l_data64.flush<0>();
    l_data64.setBit<PERV_1_CPLT_CONF1_FREE_USAGE_30D>();  //CPLT_CONF1.TC_IOP_HSSPORWREN = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF1_OR, l_data64));

    fapi2::delay(p9SbeChipletReset::HW_NS_DELAY,
                 p9SbeChipletReset::SIM_CYCLE_DELAY);

    //Setting CPLT_CONF1 register value
    l_data64.flush<0>();
    l_data64.setBit<PERV_1_CPLT_CONF1_FREE_USAGE_28D>();  //CPLT_CONF1.TC_IOP_SYS_RESET_PCS = 0b1
    l_data64.setBit<PERV_1_CPLT_CONF1_FREE_USAGE_29D>();  //CPLT_CONF1.TC_IOP_SYS_RESET_PMA = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF1_OR, l_data64));

    FAPI_INF("p9_sbe_chiplet_reset_setup_iop_logic:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Do a scan0 to all obus chiplets independent of PG information
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_all_obus_scan0(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_data;

    FAPI_INF("p9_sbe_chiplet_reset_all_obus_scan0: Entering ...");

    l_data.flush<0>();
    l_data.setBit<PERV_1_NET_CTRL0_PLLFORCE_OUT_EN>();
    FAPI_DBG("Force PLL out enable for PLLs");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data));

    fapi2::delay(10000, (40 * 400));

    l_data.flush<1>();
    l_data.clearBit<PERV_1_NET_CTRL0_PCB_EP_RESET>();
    FAPI_DBG("Release endpoint reset");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data));

    l_data.flush<0>();
    l_data.setBit<PERV_1_NET_CTRL0_CHIPLET_ENABLE>();
    FAPI_DBG("Set partial good enable");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data));

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(i_target_chiplet,
             p9SbeChipletReset::REGIONS_EXCEPT_VITAL, l_regions));

    FAPI_DBG("run scan0 module for region except vital and pll, scan types GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, l_regions,
                                          p9SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("run scan0 module for region except vital and pll, scan types except GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, l_regions,
                                          p9SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    l_data.flush<1>();
    l_data.clearBit<PERV_1_NET_CTRL0_CHIPLET_ENABLE>();
    FAPI_DBG("Reset partial good enable");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data));

    l_data.flush<0>();
    l_data.setBit<PERV_1_NET_CTRL0_PCB_EP_RESET>();
    FAPI_DBG("Assert endpoint reset");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WOR, l_data));

    l_data.flush<1>();
    l_data.clearBit<PERV_1_NET_CTRL0_PLLFORCE_OUT_EN>();
    FAPI_DBG("Reset Force PLL out enable for PLLs");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data));

    FAPI_INF("p9_sbe_chiplet_reset_all_obus_scan0:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Setup sector buffer strength and pulse mode for MC,OB,XB,PCIE
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_reset_sectorbuffer_pulsemode_attr_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{

    fapi2::buffer<uint64_t> l_data64_net_ctrl1;
    fapi2::buffer<uint8_t> l_attr_buffer_strength = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_enable = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_value = 0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;

    FAPI_INF("p9_sbe_chiplet_reset_sectorbuffer_pulsemode_attr_setup:Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECTOR_BUFFER_STRENGTH, l_sys,
                           l_attr_buffer_strength));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PULSE_MODE_ENABLE, l_sys,
                           l_attr_pulse_mode_enable));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PULSE_MODE_VALUE, l_sys,
                           l_attr_pulse_mode_value));

    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL1, l_data64_net_ctrl1));

    FAPI_DBG("Sector buffer strength");
    l_data64_net_ctrl1.insertFromRight< PERV_1_NET_CTRL1_SB_STRENGTH,
                                        PERV_1_NET_CTRL1_SB_STRENGTH_LEN >(l_attr_buffer_strength);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64_net_ctrl1));

    FAPI_DBG("Pulse mode enable & pulse mode");

    if (l_attr_pulse_mode_enable.getBit<7>())
    {
        FAPI_DBG("setting pulse mode enable");
        l_data64_net_ctrl1.setBit<PERV_1_NET_CTRL1_CLK_PULSE_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64_net_ctrl1));

        l_data64_net_ctrl1.insertFromRight< PERV_1_NET_CTRL1_CLK_PULSE_MODE,
                                            PERV_1_NET_CTRL1_CLK_PULSE_MODE_LEN >(l_attr_pulse_mode_value);
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1, l_data64_net_ctrl1));
    }

    FAPI_INF("p9_sbe_chiplet_reset_sectorbuffer_pulsemode_attr_setup:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
