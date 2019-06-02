/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_npll_setup.C $ */
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
/// @file  p9_sbe_npll_setup.C
///
/// @brief scan initialize level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_npll_setup.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


enum P9_SBE_NPLL_SETUP_Private_Constants
{
    NS_DELAY = 5000000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000 // unit is sim cycles
};

static fapi2::ReturnCode p9_sbe_npll_setup_sectorbuffer_pulsemode_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

static fapi2::ReturnCode enable_spread_spectrum_via_tod(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p9_sbe_npll_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;
    uint8_t l_read_attr = 0;
    uint8_t l_nest_bypass = 0;
    uint8_t l_attr_ss_filter = 0;
    uint8_t l_attr_cp_filter = 0;
    uint8_t l_attr_io_filter = 0;
    fapi2::buffer<uint64_t> l_data64_root_ctrl8;
    fapi2::buffer<uint64_t> l_data64_perv_ctrl0;
    FAPI_INF("p9_sbe_npll_setup: Entering ...");

    FAPI_DBG("Sector buffer strength and pulse mode setup");
    FAPI_TRY(p9_sbe_npll_setup_sectorbuffer_pulsemode_settings(i_target_chip));

    FAPI_DBG("Reading ROOT_CTRL8 register value");
    //Getting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8)); //l_data64_root_ctrl8 = PIB.ROOT_CTRL8

    FAPI_DBG("Reading ATTR_SS_FILTER_BYPASS, ATTR_CP_FILTER_BYPASS, ATTR_IO_FILTER_BYPASS");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SS_FILTER_BYPASS, i_target_chip,
                           l_attr_ss_filter));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_FILTER_BYPASS, i_target_chip,
                           l_attr_cp_filter));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IO_FILTER_BYPASS, i_target_chip,
                           l_attr_io_filter));
    FAPI_DBG("SS,CP and IO filter configuration 1.ATTR_SS_FILTER_BYPASS: %#018lX 2.ATTR_CP_FILTER_BYPASS: %#018lX 3.ATTR_IO_FILTER_BYPASS: %#018lX",
             l_attr_ss_filter, l_attr_cp_filter, l_attr_io_filter);


    if (l_attr_ss_filter == 0x0 )
    {
        /*
         * If we enable spread spectrum, do it before we drop the PLL reset
         * to avoid glitches due to the asynchronous crossing of the control
         * signal into the PLL core.
         */
        FAPI_TRY(enable_spread_spectrum_via_tod(i_target_chip));

        FAPI_DBG("Drop PLL test enable for Spread Spectrum PLL");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_SS0_PLL_TEST_EN = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_SS0_PLL_TEST_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));

        FAPI_DBG("Release SS PLL reset");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_SS0_PLL_RESET = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_SS0_PLL_RESET>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));

        fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

        FAPI_DBG("check SS PLL lock");
        //Getting PLL_LOCK_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                                l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

        FAPI_ASSERT(l_read_reg.getBit<0>(),
                    fapi2::SS_PLL_LOCK_ERR()
                    .set_MASTER_CHIP(i_target_chip)
                    .set_SS_PLL_READ(l_read_reg)
                    .set_AFTER_SPREAD_ENABLE(false),
                    "ERROR:SS PLL LOCK NOT SET");

        FAPI_DBG("Release SS PLL Bypass");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_SS0_PLL_BYPASS = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_SS0_PLL_BYPASS>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
    }

    if ( l_attr_cp_filter == 0x0 )
    {
        FAPI_DBG("Drop PLL test enable for CP Filter PLL");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_FILT1_PLL_TEST_EN = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT1_PLL_TEST_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));

        FAPI_DBG("Release CP Filter PLL reset");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_FILT1_PLL_RESET = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT1_PLL_RESET>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));

        fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

        FAPI_DBG("check  PLL lock for CP Filter PLL , Check PLL lock fir IO Filter PLL");
        //Getting PLL_LOCK_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                                l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

        FAPI_ASSERT(l_read_reg.getBit<1>(),
                    fapi2::CP_FILTER_PLL_LOCK_ERR()
                    .set_MASTER_CHIP(i_target_chip)
                    .set_CP_FILTER_PLL_READ(l_read_reg),
                    "ERROR:CP FILTER PLL LOCK NOT SET");

        FAPI_DBG("Release CP filter PLL Bypass Signal");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_FILT1_PLL_BYPASS = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT1_PLL_BYPASS>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
    }

    if ( l_attr_io_filter == 0x0 )
    {
        FAPI_DBG("Drop PLL test enable for IO Filter PLL");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_FILT0_PLL_TEST_EN = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT0_PLL_TEST_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));

        FAPI_DBG("Release IO Filter PLL reset");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_FILT0_PLL_RESET = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT0_PLL_RESET>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));

        fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

        FAPI_DBG("check  PLL lock for CP Filter PLL , Check PLL lock fir IO Filter PLL");
        //Getting PLL_LOCK_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                                l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

        FAPI_ASSERT(l_read_reg.getBit<2>(),
                    fapi2::IO_FILTER_PLL_LOCK_ERR()
                    .set_MASTER_CHIP(i_target_chip)
                    .set_IO_FILTER_PLL_READ(l_read_reg),
                    "ERROR:IO FILTER PLL LOCK NOT SET");

        FAPI_DBG("Release IO filter PLL Bypass Signal");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_FILT0_PLL_BYPASS = 0
        l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT0_PLL_BYPASS>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
    }

    FAPI_DBG("Reading ATTR_NEST_MEM_X_O_PCI_BYPASS");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, i_target_chip, l_nest_bypass),
             "Error from FAPI_ATTR_GET (ATTR_NEST_MEM_X_O_PCI_BYPASS)");

    if ( l_nest_bypass == 0x0 )
    {
        FAPI_DBG("Drop PLL test enable for Nest PLL");
        //Setting PERV_CTRL0 register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM,
                                l_data64_perv_ctrl0));
        //PIB.PERV_CTRL0.TP_PLL_TEST_EN_DC = 0
        l_data64_perv_ctrl0.clearBit<PERV_PERV_CTRL0_SET_TP_PLL_TEST_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM,
                                l_data64_perv_ctrl0));
    }

    FAPI_DBG("Reading ATTR_MC_SYNC_MODE");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    if ( l_read_attr == 1 )
    {
        FAPI_DBG("Set MUX to Nest Clock input");
        //Setting ROOT_CTRL8 register value
        //PIB.ROOT_CTRL8.TP_PLL_CLKIN_SEL4_DC = 1
        l_data64_root_ctrl8.setBit<PERV_ROOT_CTRL8_SET_TP_PLL_CLKIN_SEL4_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
    }

    if ( l_nest_bypass == 0x0 )
    {
        FAPI_DBG("Release Nest PLL  reset");
        //Setting PERV_CTRL0 register value
        //PIB.PERV_CTRL0.TP_PLLRST_DC = 0
        l_data64_perv_ctrl0.clearBit<PERV_PERV_CTRL0_SET_TP_PLLRST_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM,
                                l_data64_perv_ctrl0));

        fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

        FAPI_DBG("check  NEST PLL lock");
        //Getting PLL_LOCK_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                                l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

        FAPI_ASSERT(l_read_reg.getBit<3>(),
                    fapi2::NEST_PLL_ERR()
                    .set_MASTER_CHIP(i_target_chip)
                    .set_NEST_PLL_READ(l_read_reg)
                    .set_SS_FILTER_BYPASS_STATUS(l_attr_ss_filter)
                    .set_CP_FILTER_BYPASS_STATUS(l_attr_cp_filter)
                    .set_IO_FILTER_BYPASS_STATUS(l_attr_io_filter),
                    "ERROR:NEST PLL LOCK NOT SET");

        FAPI_DBG("Release PLL bypass2");
        //Setting PERV_CTRL0 register value
        //PIB.PERV_CTRL0.TP_PLLBYP_DC = 0
        l_data64_perv_ctrl0.clearBit<PERV_PERV_CTRL0_SET_TP_PLLBYP_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM,
                                l_data64_perv_ctrl0));
    }

    // Clear Perv pcb slave error register
    FAPI_DBG(" Reset PCB error reg");
    l_read_reg.flush<1>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_ERROR_REG, l_read_reg));

    // Clearing mask bit in pcb slave config register [bit 12] to allow unlock error
    // to propagate to Pervasive Lfir [ bit 21]
    if (  l_nest_bypass == 0x0 &&  l_attr_cp_filter == 0x0 && l_attr_ss_filter == 0x0 && l_attr_io_filter == 0x0 )
    {
        FAPI_DBG(" Unmasking pll unlock error in   Pcb slave config reg");
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_SLAVE_CONFIG_REG, l_read_reg));
        l_read_reg.clearBit<12>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_SLAVE_CONFIG_REG, l_read_reg));
    }

    FAPI_INF("p9_sbe_npll_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup sector buffer strength and pulse mode
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_npll_setup_sectorbuffer_pulsemode_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    fapi2::buffer<uint64_t> l_data64_perv_ctrl1;
    fapi2::buffer<uint8_t> l_attr_buffer_strength = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_enable = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_value = 0;

    FAPI_INF("p9_sbe_npll_setup_sectorbuffer_pulsemode_settings:Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECTOR_BUFFER_STRENGTH, l_sys,
                           l_attr_buffer_strength));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PULSE_MODE_ENABLE, l_sys,
                           l_attr_pulse_mode_enable));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PULSE_MODE_VALUE, l_sys,
                           l_attr_pulse_mode_value));

    FAPI_TRY(fapi2::getScom(i_target_chip , PERV_PERV_CTRL1_SCOM, l_data64_perv_ctrl1));

    FAPI_DBG("Sector buffer strength");
    l_data64_perv_ctrl1.insertFromRight< PERV_PERV_CTRL1_TP_SEC_BUF_DRV_STRENGTH_DC,
                                         PERV_PERV_CTRL1_TP_SEC_BUF_DRV_STRENGTH_DC_LEN >(l_attr_buffer_strength);
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL1_SCOM, l_data64_perv_ctrl1));

    FAPI_DBG("Pulse mode enable & pulse mode");

    if (l_attr_pulse_mode_enable.getBit<7>())
    {
        l_data64_perv_ctrl1.setBit<PERV_PERV_CTRL1_TP_CLK_PULSE_ENABLE_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL1_SCOM, l_data64_perv_ctrl1));

        l_data64_perv_ctrl1.insertFromRight< PERV_PERV_CTRL1_TP_CLK_PULSE_MODE_DC,
                                             PERV_PERV_CTRL1_TP_CLK_PULSE_MODE_DC_LEN  >(l_attr_pulse_mode_value);
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL1_SCOM, l_data64_perv_ctrl1));
    }

    FAPI_INF("p9_sbe_npll_setup_sectorbuffer_pulsemode_settings:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Enable the TOD's spread spectrum enable output
///
/// Set the TOD timer to a nonzero value to arm it and briefly start the
/// TOD so that is passes the set timer value. The TOD will stop in ERROR
/// state due to missing SYNC pulses but we don't care about that and
/// clear out the error afterwards. The spread spectrum enable will stay set.
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode enable_spread_spectrum_via_tod(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data;
    uint8_t l_sync_spread = 0;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FORCE_SYNC_SS_PLL_SPREAD,
                           fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                           l_sync_spread),
             "Error from FAPI_ATTR_GET (ATTR_FORCE_SYNC_SS_PLL_SPREAD)");

    if (l_sync_spread)
    {
        goto fapi_try_exit;
    }

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SYNC_SS_PLL_SPREAD,
                           i_target_chip,
                           l_sync_spread),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_SYNC_SS_PLL_SPREAD)");

    if (l_sync_spread)
    {
        goto fapi_try_exit;
    }

    FAPI_DBG("Enable Spread Spectrum via TOD");

    // Set up the TOD timer unit to trigger on a TOD value of one
    l_data.flush<0>().insertFromRight<PERV_TOD_TIMER_REG_VALUE, PERV_TOD_TIMER_REG_VALUE_LEN>(1ULL);
    l_data.setBit<60>().setBit<61>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TOD_TIMER_REG, l_data));

    // Reset the TOD and set it to a value of one to trigger the timer
    l_data.flush<0>().setBit<PERV_TOD_LOAD_TOD_MOD_REG_FSM_TRIGGER>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TOD_LOAD_TOD_MOD_REG, l_data));
    l_data.flush<0>()
    .insertFromRight<PERV_TOD_LOAD_TOD_REG_VALUE, PERV_TOD_LOAD_TOD_REG_VALUE_LEN>(1ULL)
    .setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TOD_LOAD_TOD_REG, l_data));

    // Check that the TOD timer turned its SSCGEN output on
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TOD_TIMER_REG, l_data));

    // If the Assertion fails, don't exit; we can keep trucking without Spread Spectrum
    FAPI_ASSERT_NOEXIT(l_data.getBit<PERV_TOD_TIMER_REG_STATUS>(),
                       fapi2::SPREAD_SPECTRUM_ENABLE_ERR()
                       .set_MASTER_CHIP(i_target_chip)
                       .set_TOD_TIMER_REG(l_data),
                       "Spread Spectrum enable signal not set");

fapi_try_exit:
    return fapi2::current_err;
}
