/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_npll_setup.C $ */
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
/// @file  p10_sbe_npll_setup.C
///
/// @brief scan initialize level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_npll_setup.H"
#include "p10_scom_perv_3.H"
#include "p10_scom_perv_4.H"
#include "p10_scom_perv_8.H"
#include "p10_scom_perv_a.H"
#include "p10_scom_perv_e.H"
#include "p10_scom_proc_1.H"
#include "p10_scom_proc_2.H"
#include "p10_scom_proc_d.H"
#include "p10_scom_proc_e.H"
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>

#define SEEPROM_START 0xFF800000

enum P10_SBE_NPLL_SETUP_Private_Constants
{
    MAGIC_NUMBER = 0x584950205345504D,
    START_CMD = 0x1,
    REGIONS_PAU_DPLL = 0x0040,
    REGIONS_NEST_DPLL = 0x0020,
    CLOCK_TYPES_ALL = 0x7,
    PAU_DPLL_INITIALIZE_MODE1 = 0xA000000000000000,
    NEST_DPLL_INITIALIZE_MODE1 = 0xA001010000000000,
    NEST_DPLL_INITIALIZE_MODE2 = 0x8001010000000000,
};

fapi2::ReturnCode p10_sbe_check_magicnumber(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

static fapi2::ReturnCode p10_sbe_npll_setup_sectorbuffer_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_sbe_npll_setup(const
                                     fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;

    fapi2::buffer<uint64_t> l_data64_root_ctrl3, l_read_reg, l_data64_root_ctrl4, l_data64;
    fapi2::buffer<uint8_t> l_attr_plltodflt_bypass, l_attr_pllnestflt_bypass, l_attr_pllioflt_bypass,
          l_attr_plliossflt_bypass,
          l_attr_cp_refclk_select, l_attr_pau_dpll_bypass, l_attr_nest_dpll_bypass;
    fapi2::buffer<uint32_t> l_attr_pau_freq, l_attr_core_boot_freq;
    fapi2::buffer<uint16_t> freq_calculated;
    fapi2::buffer<uint64_t> l_regions;
    fapi2::ReturnCode l_rc;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_npll_setup: Entering ...");

    FAPI_DBG("Sector buffer strength setup");
    FAPI_TRY(p10_sbe_npll_setup_sectorbuffer_settings(i_target_chip));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLTODFLT_BYPASS, i_target_chip, l_attr_plltodflt_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLNESTFLT_BYPASS, i_target_chip, l_attr_pllnestflt_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLIOFLT_BYPASS, i_target_chip, l_attr_pllioflt_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_PLLIOSSFLT_BYPASS, i_target_chip, l_attr_plliossflt_bypass));

    FAPI_DBG("Release PLL test enable for filter PLLs");
    // RC3 bits 10,14,18,22
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLTODFLT_TEST_EN_DC>(!
            (l_attr_plltodflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLNESTFLT_TEST_EN_DC>(!
            (l_attr_pllnestflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOFLT_TEST_EN_DC>(!
            (l_attr_pllioflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOSSFLT_TEST_EN_DC>(!
            (l_attr_plliossflt_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl3));

    FAPI_DBG("Release PLL reset for filter PLLs");
    // RC3 bits 8,12,16,20
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLTODFLT_RESET_DC>(!
            (l_attr_plltodflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLNESTFLT_RESET_DC>(!
            (l_attr_pllnestflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOFLT_RESET_DC>(!
            (l_attr_pllioflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOSSFLT_RESET_DC>(!
            (l_attr_plliossflt_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl3));

    FAPI_DBG("Check filter PLL lock");
    l_data64.flush<0>()
    .writeBit<2>(!l_attr_plltodflt_bypass)
    .writeBit<3>(!l_attr_pllnestflt_bypass)
    .writeBit<4>(!l_attr_pllioflt_bypass)
    .writeBit<5>(!l_attr_plliossflt_bypass);

    l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_tpchiplet, l_data64, l_read_reg);
    FAPI_ASSERT(l_rc != fapi2::FAPI2_RC_FALSE,
                fapi2::FILT_PLL_LOCK_ERR()
                .set_PLL_EXPECT(l_data64)
                .set_PLL_READ(l_read_reg),
                "One or more filter PLLs failed to lock! expected 0x%X, got 0x%X "
                "(bits: PLLTODFLT, PLLNESTFLT, PLLIOFLT, PLLIOSSFLT)",
                (l_data64.getBits<2, 4>()), (l_read_reg.getBits<2, 4>()));
    FAPI_TRY(l_rc, "Failed to poll for PLL lock");

    FAPI_DBG("Release PLL bypass for filter PLLs");
    // RC3 bits 9,13,17,21
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLTODFLT_BYPASS_EN_DC>(!
            (l_attr_plltodflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLNESTFLT_BYPASS_EN_DC>(!
            (l_attr_pllnestflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOFLT_BYPASS_EN_DC>(!
            (l_attr_pllioflt_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLIOSSFLT_BYPASS_EN_DC>(!
            (l_attr_plliossflt_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl3));

    FAPI_DBG("PAU DPLL: Initialize to mode1");
    FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_CTRL_RW, PAU_DPLL_INITIALIZE_MODE1));

    FAPI_DBG("PAU DPLL: Write frequency settings");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_pau_freq));
    freq_calculated = (((l_attr_pau_freq + 1) * 3) / 50);
    FAPI_DBG("PAU_DPLL frequency calculation : %#018lX", freq_calculated);

    l_read_reg.flush<0>();
    l_read_reg. insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMAX,
                proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMAX_LEN > (freq_calculated);
    l_read_reg. insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMULT,
                proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMULT_LEN > (freq_calculated);
    l_read_reg. insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMIN,
                proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ_FMIN_LEN > (freq_calculated);
    FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_DPLL_CNTL_PAU_REGS_FREQ, l_read_reg));

    FAPI_DBG("NEST DPLL: Initialize to mode1");
    FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_CTRL_RW, NEST_DPLL_INITIALIZE_MODE1));

    FAPI_DBG("NEST DPLL: Write frequency settings");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_CORE_BOOT_MHZ, i_target_chip, l_attr_core_boot_freq));
    freq_calculated = (((l_attr_core_boot_freq + 1) * 3) / 50);
    FAPI_DBG("NEST_DPLL frequency calculation : %#018lX", freq_calculated);

    l_read_reg.flush<0>();
    l_read_reg. insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ_FMAX,
                proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ_FMAX_LEN > (freq_calculated);
    l_read_reg. insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ_FMULT,
                proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ_FMULT_LEN > (freq_calculated);
    l_read_reg. insertFromRight < proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ_FMIN,
                proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ_FMIN_LEN > (freq_calculated);
    FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_FREQ, l_read_reg));

    FAPI_DBG("PAU DPLL, NEST DPLL : Switch to internal clocks");
    l_data64_root_ctrl3.flush<0>()
    .setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_FUNC_CLKSEL_DC>()
    .setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_NEST_DPLL_FUNC_CLKSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl3));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_pau_dpll_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_DPLL_BYPASS, i_target_chip, l_attr_nest_dpll_bypass));
    FAPI_DBG("PAU DPLL, NEST DPLL: Release reset");
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_RESET_DC>(!
            (l_attr_pau_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_NEST_DPLL_RESET_DC>(!
            (l_attr_nest_dpll_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl3));

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

    FAPI_DBG("Check for PAU, NEST DPLL lock");
    l_data64.flush<0>()
    .writeBit<6>(!l_attr_pau_dpll_bypass)
    .writeBit<7>(!l_attr_nest_dpll_bypass);

    l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_tpchiplet, l_data64, l_read_reg);
    FAPI_ASSERT(l_rc != fapi2::FAPI2_RC_FALSE,
                fapi2::DPLL_LOCK_ERR()
                .set_PLL_EXPECT(l_data64)
                .set_PLL_READ(l_read_reg),
                "One or more DPLLs failed to lock! exp/got: "
                "PAU DPLL: %d/%d  NEST DPLL: %d/%d",
                l_data64.getBit<6>(), l_read_reg.getBit<6>(),
                l_data64.getBit<7>(), l_read_reg.getBit<7>());
    FAPI_TRY(l_rc, "Failed to poll for PLL lock");

    FAPI_DBG("PAU DPLL + NEST DPLL: Release test_enable ");
    // RC3 bits 25,26,29,30
    l_data64_root_ctrl3.flush<0>();
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_BYPASS_EN_DC>(!
            (l_attr_pau_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_TEST_EN_DC>(!
            (l_attr_pau_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_NEST_DPLL_BYPASS_EN_DC>(!
            (l_attr_nest_dpll_bypass.getBit<7>()));
    l_data64_root_ctrl3.writeBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_NEST_DPLL_TEST_EN_DC>(!
            (l_attr_nest_dpll_bypass.getBit<7>()));
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl3));

    FAPI_DBG("NEST DPLL: Transition from mode1 to mode2 (enable frequency slewing)");
    FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_DPLL_CNTL_NEST_REGS_CTRL_RW, NEST_DPLL_INITIALIZE_MODE2));

    // Clear Perv pcb slave error register
    FAPI_DBG(" Reset PCB error reg");
    l_read_reg.flush<1>();
    FAPI_TRY(fapi2::putScom(l_tpchiplet, perv::ERROR_REG, l_read_reg));

    // Clearing mask bit in pcb slave config register [bit 12] to allow unlock error
    // to propagate to Pervasive Lfir [ bit 21]

    FAPI_DBG(" Unmasking pll unlock error in   Pcb slave config reg");
    FAPI_TRY(fapi2::getScom(l_tpchiplet, perv::SLAVE_CONFIG_REG, l_read_reg));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_REFCLOCK_SELECT, i_target_chip, l_attr_cp_refclk_select));
    {
        const bool l_dual_osc = (l_attr_cp_refclk_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0)
                                || (l_attr_cp_refclk_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1);
        l_read_reg.writeBit<12>(!l_dual_osc);
        l_read_reg.writeBit<13>(!l_dual_osc);
    }

    l_read_reg.writeBit<14>(l_attr_plltodflt_bypass.getBit<7>());
    l_read_reg.writeBit<15>(l_attr_pllnestflt_bypass.getBit<7>());
    l_read_reg.writeBit<16>(l_attr_pllioflt_bypass.getBit<7>());
    l_read_reg.writeBit<17>(l_attr_plliossflt_bypass.getBit<7>());
    l_read_reg.writeBit<18>(l_attr_pau_dpll_bypass.getBit<7>());
    // Temporarily keep Nest DPLL errors masked until we figure out the intermittent unlocks
    //l_read_reg.writeBit<19>(l_attr_nest_dpll_bypass.getBit<7>());

    FAPI_TRY(fapi2::putScom(l_tpchiplet, perv::SLAVE_CONFIG_REG, l_read_reg));

#ifndef DFT

    if (fapi2::is_platform<fapi2::PLAT_SBE>())
    {
        FAPI_DBG("read magic number from seeprom, compare value");
        // To read and check magic number
        FAPI_TRY(p10_sbe_check_magicnumber(i_target_chip));
    }

#endif /*DFT*/

    FAPI_INF("p10_sbe_npll_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Setup sector buffer strength
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_npll_setup_sectorbuffer_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt::perv;

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    fapi2::buffer<uint64_t> l_data64_perv_ctrl1;
    fapi2::buffer<uint8_t> l_attr_buffer_strength = 0;

    FAPI_INF("p10_sbe_npll_setup_sectorbuffer_settings:Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECTOR_BUFFER_STRENGTH, l_sys,
                           l_attr_buffer_strength));

    FAPI_TRY(fapi2::getScom(i_target_chip , FSXCOMP_FSXLOG_PERV_CTRL1_RW, l_data64_perv_ctrl1));

    FAPI_DBG("Sector buffer strength");
    l_data64_perv_ctrl1.insertFromRight< FSXCOMP_FSXLOG_PERV_CTRL1_TP_SEC_BUF_DRV_STRENGTH_DC,
                                         FSXCOMP_FSXLOG_PERV_CTRL1_TP_SEC_BUF_DRV_STRENGTH_DC_LEN >(l_attr_buffer_strength);
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL1_RW, l_data64_perv_ctrl1));

    FAPI_INF("p10_sbe_npll_setup_sectorbuffer_settings:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief check for magic number
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_check_magicnumber(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;

    FAPI_INF("p10_sbe_check_magicnumber: Entering ...");

    // Read SEEPROM start address FF800000 for magic number
    l_read_reg = *reinterpret_cast<volatile uint64_t*>(SEEPROM_START);

    FAPI_ASSERT(l_read_reg == MAGIC_NUMBER,
                fapi2::MAGIC_NUMBER_NOT_VALID()
                .set_SEEPROM_START_ADDR(l_read_reg)
                .set_MAGIC_NUMBER_VALUE(MAGIC_NUMBER),
                "ERROR: Magic number not matching");

    FAPI_INF("p10_sbe_check_magicnumber: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
