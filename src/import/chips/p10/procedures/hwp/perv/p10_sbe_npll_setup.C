/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_npll_setup.C $ */
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
/// @file  p10_sbe_npll_setup.C
///
/// @brief scan initialize level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_npll_setup.H"
#include <p10_perv_sbe_cmn.H>
#include "p9_const_common.H"
#include <target_filters.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


enum P10_SBE_NPLL_SETUP_Private_Constants
{
    START_CMD = 0x1,
    REGIONS_PAU_DPLL = 0x0040,
    REGIONS_NEST_DPLL = 0x0020,
    CLOCK_TYPES_ALL = 0x7,
    NS_DELAY = 5000000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000, // unit is sim cycles
    PAU_DPLL_INITIALIZE_MODE = 0xA000000000000000,
    NEST_DPLL_INITIALIZE_MODE = 0xA000000000000000
};

static fapi2::ReturnCode p10_sbe_npll_setup_sectorbuffer_pulsemode_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_sbe_npll_setup(const
                                     fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64_root_ctrl3, l_read_reg;
    fapi2::buffer<uint8_t> l_attr_filter1_bypass, l_attr_filter2_bypass, l_attr_filter3_bypass, l_attr_filter4_bypass,
          l_attr_cp_refclk_select, l_attr_pau_dpll_bypass, l_attr_nest_dpll_bypass;
    fapi2::buffer<uint32_t> l_attr_pau_freq, l_attr_core_boot_freq;
    fapi2::buffer<uint16_t> freq_calculated;
    fapi2::buffer<uint64_t> l_regions;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_npll_setup: Entering ...");

    FAPI_DBG("Sector buffer strength and pulse mode setup");
    FAPI_TRY(p10_sbe_npll_setup_sectorbuffer_pulsemode_settings(i_target_chip));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_FILTER_1_BYPASS, i_target_chip, l_attr_filter1_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_FILTER_2_BYPASS, i_target_chip, l_attr_filter2_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_FILTER_3_BYPASS, i_target_chip, l_attr_filter3_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_FILTER_4_BYPASS, i_target_chip, l_attr_filter4_bypass));

    FAPI_DBG("Release PLL test enable for filter PLLs");
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<10>(!(l_attr_filter1_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<14>(!(l_attr_filter2_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<18>(!(l_attr_filter3_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<22>(!(l_attr_filter4_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_CLEAR_SCOM, l_data64_root_ctrl3));

    FAPI_DBG("Release PLL reset for filter PLLs");
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<8>(!(l_attr_filter1_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<12>(!(l_attr_filter2_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<16>(!(l_attr_filter3_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<20>(!(l_attr_filter4_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_CLEAR_SCOM, l_data64_root_ctrl3));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_DBG("Check filter PLL lock");
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG, l_read_reg));

    if(!l_attr_filter1_bypass)
    {
        FAPI_ASSERT(l_read_reg.getBit<2>(),
                    fapi2::FILT_PLL_LOCK_ERR()
                    .set_PLL_READ(l_read_reg),
                    "ERROR:FILTER0 PLL LOCK NOT SET");
    }

    if(!l_attr_filter2_bypass)
    {
        FAPI_ASSERT(l_read_reg.getBit<3>(),
                    fapi2::FILT_PLL_LOCK_ERR()
                    .set_PLL_READ(l_read_reg),
                    "ERROR:FILTER1 PLL LOCK NOT SET");
    }

    if(!l_attr_filter3_bypass)
    {
        FAPI_ASSERT(l_read_reg.getBit<4>(),
                    fapi2::FILT_PLL_LOCK_ERR()
                    .set_PLL_READ(l_read_reg),
                    "ERROR:FILTER2 PLL LOCK NOT SET");
    }

    if(!l_attr_filter4_bypass)
    {
        FAPI_ASSERT(l_read_reg.getBit<5>(),
                    fapi2::FILT_PLL_LOCK_ERR()
                    .set_PLL_READ(l_read_reg),
                    "ERROR:FILTER3 PLL LOCK NOT SET");
    }

    FAPI_DBG("Release PLL bypass for filter PLLs");
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<9>(!(l_attr_filter1_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<13>(!(l_attr_filter2_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<17>(!(l_attr_filter3_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<21>(!(l_attr_filter4_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_CLEAR_SCOM, l_data64_root_ctrl3));

    FAPI_DBG("PAU DPLL: Initialize to mode1");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x01060052, PAU_DPLL_INITIALIZE_MODE));

    FAPI_DBG("PAU DPLL: Write frequency settings");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_pau_freq));
    freq_calculated = ((l_attr_pau_freq * 96) / 25);
    FAPI_DBG("PAU_DPLL frequency calculation : %#018lX", freq_calculated);

    l_read_reg.flush<0>();
    l_read_reg. insertFromRight< 1, 16 > (freq_calculated);
    l_read_reg. insertFromRight< 17, 16 > (freq_calculated);
    l_read_reg. insertFromRight< 33, 16 > (freq_calculated);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x01060051, l_read_reg));

    FAPI_DBG("NEST DPLL: Initialize to mode1");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x01060152, NEST_DPLL_INITIALIZE_MODE));

    FAPI_DBG("NEST DPLL: Write frequency settings");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_CORE_BOOT_MHZ, i_target_chip, l_attr_core_boot_freq));
    freq_calculated = ((l_attr_core_boot_freq * 96) / 25);
    FAPI_DBG("NEST_DPLL frequency calculation : %#018lX", freq_calculated);

    l_read_reg.flush<0>();
    l_read_reg. insertFromRight< 1, 16 > (freq_calculated);
    l_read_reg. insertFromRight< 17, 16 > (freq_calculated);
    l_read_reg. insertFromRight< 33, 16 > (freq_calculated);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x01060151, l_read_reg));


    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_pau_dpll_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_DPLL_BYPASS, i_target_chip, l_attr_nest_dpll_bypass));
    FAPI_DBG("PAU DPLL, NEST DPLL: Release reset");
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<24>(!(l_attr_pau_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<28>(!(l_attr_nest_dpll_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_CLEAR_SCOM, l_data64_root_ctrl3));

    //PAU DPLL, NEST DPLL - startclocks
    if (!l_attr_pau_dpll_bypass)
    {
        FAPI_DBG("Startclocks for PAU DPLL regions");
        FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0,  REGIONS_PAU_DPLL,
                 CLOCK_TYPES_ALL));
    }

    if (!l_attr_nest_dpll_bypass)
    {
        FAPI_DBG("Startclocks for NEST DPLL regions");
        FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0,  REGIONS_NEST_DPLL,
                 CLOCK_TYPES_ALL));
    }

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_DBG("Check for PAU, NEST DPLL lock");
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG, l_read_reg));

    if(!l_attr_pau_dpll_bypass)
    {
        FAPI_ASSERT(l_read_reg.getBit<6>(),
                    fapi2::DPLL_LOCK_ERR()
                    .set_DPLL_READ(l_read_reg),
                    "ERROR:PAU DPLL LOCK NOT SET");
    }


    if(!l_attr_nest_dpll_bypass)
    {
        FAPI_ASSERT(l_read_reg.getBit<7>(),
                    fapi2::DPLL_LOCK_ERR()
                    .set_DPLL_READ(l_read_reg),
                    "ERROR:NEST DPLL LOCK NOT SET");
    }

    FAPI_DBG("PAU DPLL + NEST DPLL: Release test_enable, PAU DPLL + NEST DPLL: Release bypass ");
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<25>(!(l_attr_pau_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<26>(!(l_attr_nest_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<29>(!(l_attr_pau_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<30>(!(l_attr_nest_dpll_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_CLEAR_SCOM, l_data64_root_ctrl3));

    // Clear Perv pcb slave error register
    FAPI_DBG(" Reset PCB error reg");
    l_read_reg.flush<1>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_ERROR_REG, l_read_reg));

    // Clearing mask bit in pcb slave config register [bit 12] to allow unlock error
    // to propagate to Pervasive Lfir [ bit 21]

    FAPI_DBG(" Unmasking pll unlock error in   Pcb slave config reg");
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_SLAVE_CONFIG_REG, l_read_reg));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_REFCLOCK_SELECT, i_target_chip, l_attr_cp_refclk_select));
    l_read_reg.writeBit<12>(!(l_attr_cp_refclk_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1));
    l_read_reg.writeBit<13>(!(l_attr_cp_refclk_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0));

    l_read_reg.writeBit<14>(l_attr_filter1_bypass.getBit<7>());
    l_read_reg.writeBit<15>(l_attr_filter2_bypass.getBit<7>());
    l_read_reg.writeBit<16>(l_attr_filter3_bypass.getBit<7>());
    l_read_reg.writeBit<17>(l_attr_filter4_bypass.getBit<7>());
    l_read_reg.writeBit<18>(l_attr_pau_dpll_bypass.getBit<7>());
    l_read_reg.writeBit<19>(l_attr_nest_dpll_bypass.getBit<7>());

    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_SLAVE_CONFIG_REG, l_read_reg));

    FAPI_INF("p10_sbe_npll_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup sector buffer strength and pulse mode
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_npll_setup_sectorbuffer_pulsemode_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    fapi2::buffer<uint64_t> l_data64_perv_ctrl1;
    fapi2::buffer<uint8_t> l_attr_buffer_strength = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_enable = 0;
    fapi2::buffer<uint8_t> l_attr_pulse_mode_value = 0;

    FAPI_INF("p10_sbe_npll_setup_sectorbuffer_pulsemode_settings:Entering ...");

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
        l_data64_perv_ctrl1.setBit<PERV_PERV_CTRL1_TP_CLK_PULSE_ENABLE_DC>()
        .insertFromRight< PERV_PERV_CTRL1_TP_CLK_PULSE_MODE_DC,
                          PERV_PERV_CTRL1_TP_CLK_PULSE_MODE_DC_LEN  >(l_attr_pulse_mode_value);
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL1_SCOM, l_data64_perv_ctrl1));
    }

    FAPI_INF("p10_sbe_npll_setup_sectorbuffer_pulsemode_settings:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
