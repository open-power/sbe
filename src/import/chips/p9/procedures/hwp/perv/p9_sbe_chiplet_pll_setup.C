/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_chiplet_pll_setup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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
/// @file  p9_sbe_chiplet_pll_setup.C
///
/// @brief Setup PLL for Obus, Xbus, PCIe, DMI
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_pll_setup.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_sbe_common.H>


enum P9_SBE_CHIPLET_PLL_SETUP_Private_Constants
{
    NS_DELAY = 5000000, // unit is nano seconds
    SIM_CYCLE_DELAY = 100000, // unit is sim cycles
    CLOCK_CMD = 0x1,
    CLOCK_TYPES = 0x2,
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    REGIONS = 0x001
};


static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_check_pll_lock(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    bool is_pcie_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_bypass,
    const bool i_unmask);

static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_mc_pdly_dcc_bypass(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    bool mc_pdly_bypass, bool mc_dcc_bypass);

static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_pll_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_pll_test_enable(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_strt_pci_nsl_drp_synclk_mux(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_chiplet_pll_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    uint8_t l_bypass = 0;
    uint8_t l_use_dmi_buckets = 0;
    uint8_t l_hw415692 = 0;

    FAPI_INF("p9_sbe_chiplet_pll_setup: Entering ...");

    auto l_mc_io_func = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                            static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                    fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_XBUS |
                                    fapi2::TARGET_FILTER_ALL_PCI),
                            fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_DBG("Read ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS ");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS, i_target_chip, l_use_dmi_buckets));

    FAPI_DBG("Reading bypass attribute");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, i_target_chip, l_bypass));

    FAPI_DBG("Reading ATTR_mc_sync_mode");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW415692, i_target_chip, l_hw415692));

    for (auto& mc : l_mc_io_func)
    {
        uint32_t l_chipletID = mc.getChipletNumber();

        if( l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID )
        {
            FAPI_DBG("Drop PDLY bypass");
            FAPI_TRY(p9_sbe_chiplet_pll_setup_mc_pdly_dcc_bypass(mc, true, false));

            if ( !(l_read_attr) || l_use_dmi_buckets )
            {
                FAPI_DBG("Drop DCC  bypass");
                FAPI_TRY(p9_sbe_chiplet_pll_setup_mc_pdly_dcc_bypass(mc, false, true));
            }
        }
    }

    for (auto& pcie : l_mc_io_func)
    {
        // PCIe
        uint32_t l_chipletID = pcie.getChipletNumber();

        if( l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID )
        {
            FAPI_DBG("call clock start stop module and drop syncclk muxsel");
            FAPI_TRY(p9_sbe_chiplet_pll_setup_strt_pci_nsl_drp_synclk_mux( pcie ));
        }
    }

    if (l_bypass == 0)
    {
        for (auto& targ : l_mc_io_func)
        {
            // OBUS, XBUS, MC
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID == XB_CHIPLET_ID || (l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID) ||
               ((!l_read_attr || l_use_dmi_buckets) && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID))  )
            {
                FAPI_DBG("release pll test enable for except pcie");
                FAPI_TRY(p9_sbe_chiplet_pll_setup_pll_test_enable(targ));
            }
        }

        for (auto& targ : l_mc_io_func)
        {
            // OBUS, XBUS, PCIe, MC
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID == XB_CHIPLET_ID || (l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID) ||
               (l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID) ||
               ((!l_read_attr || l_use_dmi_buckets) && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID))  )
            {
                FAPI_DBG("Release PLL reset");
                FAPI_TRY(p9_sbe_chiplet_pll_setup_pll_reset(targ));
            }
        }

        for (auto& pcie : l_mc_io_func)
        {
            // PCIe
            uint32_t l_chipletID = pcie.getChipletNumber();

            if( l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID )
            {
                FAPI_DBG("Check pll lock for pcie");
                FAPI_TRY(p9_sbe_chiplet_pll_setup_check_pll_lock(pcie, true));
            }
        }

        for (auto& targ : l_mc_io_func)
        {
            // OBUS, XBUS, MC
            uint32_t l_chipletID = targ.getChipletNumber();

            if(l_chipletID == XB_CHIPLET_ID || (l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID) ||
               ((!l_read_attr || l_use_dmi_buckets) && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID))  )
            {
                FAPI_DBG("check pll lock for Mc,Xb,Ob");
                FAPI_TRY(p9_sbe_chiplet_pll_setup_check_pll_lock(targ, false));
            }
        }
    }

    for (auto& targ : l_mc_io_func)
    {
        // OBUS, XBUS, PCIe, MC
        uint32_t l_chipletID = targ.getChipletNumber();
        bool l_unmask = !(l_hw415692 && (l_chipletID == XB_CHIPLET_ID));

        if(l_chipletID == XB_CHIPLET_ID || (l_chipletID >= OB0_CHIPLET_ID && l_chipletID <= OB3_CHIPLET_ID) ||
           (l_chipletID >= PCI0_CHIPLET_ID && l_chipletID <= PCI2_CHIPLET_ID) ||
           ((!l_read_attr || l_use_dmi_buckets) && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID))  )
        {
            FAPI_TRY(p9_sbe_chiplet_pll_setup_function(targ, l_bypass, l_unmask));
        }
    }

    FAPI_INF("p9_sbe_chiplet_pll_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief check pll lock for OB,XB,MC,PCIe chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_check_pll_lock(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet, bool is_pcie_chiplet)
{
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_target_chiplet.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    FAPI_INF("p9_sbe_chiplet_pll_setup_check_pll_lock: Entering ...");

    FAPI_DBG("Check  PLL lock");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PLL_LOCK_REG

    if ( is_pcie_chiplet )
    {
        FAPI_ASSERT(l_read_reg.getBit<0>() == 1 && l_read_reg.getBit<1>() == 1,
                    fapi2::PLL_LOCK_ERR()
                    .set_TARGET_CHIPLET(i_target_chiplet)
                    .set_TARGET_CHIP(l_chip)
                    .set_PLL_READ(l_read_reg),
                    "ERROR:PCIe PLL LOCK NOT SET");
    }
    else
    {
        FAPI_ASSERT(l_read_reg.getBit<0>() == 1 ,
                    fapi2::PLL_LOCK_ERR()
                    .set_TARGET_CHIPLET(i_target_chiplet)
                    .set_TARGET_CHIP(l_chip)
                    .set_PLL_READ(l_read_reg),
                    "ERROR:PLL LOCK NOT SET");
    }

    FAPI_INF("p9_sbe_chiplet_pll_setup_check_pll_lock: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


/// @brief Setup PLL for XBus, OBus, PCIe, (MC) chiplets
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_bypass           Leave PLL in bypass?
/// @param[in]     i_unmask           Unmask PLL error reporting?
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_bypass,
    const bool i_unmask)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_pll_setup_function: Entering ...");

    if (i_bypass == 0)
    {
        FAPI_DBG("Drop PLL Bypass");
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        l_data64.clearBit<PERV_1_NET_CTRL0_PLL_BYPASS>();  //NET_CTRL0.PLL_BYPASS = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

        FAPI_DBG("Set scan ratio to 4:1 as soon as PLL is out of bypass mode");
        //Setting OPCG_ALIGN register value
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_OPCG_ALIGN, l_data64));
        l_data64.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>
        (0x3);  //OPCG_ALIGN.SCAN_RATIO = 0x3
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_ALIGN, l_data64));
    }

    FAPI_DBG("Reset PCB Slave error register");
    //Setting ERROR_REG register value
    //ERROR_REG = 0xFFFFFFFFFFFFFFFF
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_ERROR_REG, 0xFFFFFFFFFFFFFFFF));

    if ((i_bypass == 0) && i_unmask)
    {
        FAPI_DBG(" Unmasking pll unlock error in   Pcb slave config reg");
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_SLAVE_CONFIG_REG, l_data64));
        l_data64.clearBit<12>();
        FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_SLAVE_CONFIG_REG, l_data64));
    }

    FAPI_INF("p9_sbe_chiplet_pll_setup_function: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop Mc DCC bypass, Drop Mc PDLY bypass
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_mc_pdly_dcc_bypass(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    bool mc_pdly_bypass, bool mc_dcc_bypass)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_pll_setup_mc_pdly_dcc_bypass: Entering ...");

    l_data64.flush<1>();

    if ( mc_pdly_bypass )
    {
        FAPI_DBG("Drop PDLY bypass");
        l_data64.clearBit<PERV_1_NET_CTRL1_CLK_PDLY_BYPASS_EN>();
    }

    if ( mc_dcc_bypass )
    {
        FAPI_DBG("Drop DCC bypass");
        l_data64.clearBit<PERV_1_NET_CTRL1_CLK_DCC_BYPASS_EN>();
    }

    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL1_WAND, l_data64));

    FAPI_INF("p9_sbe_chiplet_pll_setup_mc_pdly_dcc_bypass: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


/// @brief release pll reset and wait
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_pll_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_pll_setup_pll_reset: Entering ...");

    FAPI_DBG("Drop PLL Reset");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<PERV_1_NET_CTRL0_PLL_RESET>();  //NET_CTRL0.PLL_RESET = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_INF("p9_sbe_chiplet_pll_setup_pll_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Release pll test enable except for pcie
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_pll_test_enable(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_pll_setup_pll_test_enable: Entering ...");

    FAPI_DBG("Release PLL test enable for except pcie");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<PERV_1_NET_CTRL0_PLL_TEST_EN>();  //NET_CTRL0.PLL_TEST_EN = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p9_sbe_chiplet_pll_setup_pll_test_enable: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief start PLL clock region, NSL latches only , call module clock_start_stop
/// Drop syncclk_muxsel
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_chiplet_pll_setup_strt_pci_nsl_drp_synclk_mux(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_pll_setup_strt_pci_nsl_drp_synclk_mux: Entering ...");

    FAPI_DBG("call module clock start stop");
    FAPI_TRY(p9_sbe_common_clock_start_stop(i_target_chiplet, CLOCK_CMD,
                                            DONT_STARTSLAVE, DONT_STARTMASTER, REGIONS, CLOCK_TYPES));

    FAPI_DBG("Drop syncclk muxsel for pcie chiplet");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.TC_UNIT_SYNCCLK_MUXSEL_DC = 0
    l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_INF("p9_sbe_chiplet_pll_setup_strt_pci_nsl_drp_synclk_mux: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
