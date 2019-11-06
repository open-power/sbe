/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_chiplet_init.C $ */
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
/// @file  p10_sbe_tp_chiplet_init.C
///
/// @brief TP Chiplet Start Clocks
///        Starting clocks for all regions in perv chiplet other than pib and net
///        Clock test to check osc runnong
///        Configures pfet controls, tod errr regs , perv hang counters
///        pervasive LFIRS and set mask in IPOLL mask reg
///        Starts VREF calibration and checks for Calib Done
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_tp_chiplet_init.H"
#include "p10_scom_proc_6.H"
#include "p10_scom_perv_0.H"
#include "p10_scom_perv_6.H"
#include "p10_scom_perv_7.H"
#include "p10_scom_perv_8.H"
#include "p10_scom_perv_a.H"
#include "p10_scom_perv_b.H"
#include "p10_scom_perv_d.H"
#include "p10_scom_perv_e.H"
#include "p10_scom_perv_f.H"
#include <p10_perv_sbe_cmn.H>
#include <p10_hang_pulse_mc_setup_tables.H>
#include <target_filters.H>

enum P10_SBE_TP_CHIPLET_INIT_Private_Constants
{
    START_CMD = 0x1,
    REGIONS_PERV_OCC_PSI = 0x4900,
    CLOCK_TYPES_ALL = 0x7,
    REGIONS_PLL = 0x0010,
    CLOCK_TYPES_SL = 0x4,
    LFIR_ACTION0_VALUE = 0x0000000000000000,
    //LFIR_ACTION1_VALUE = 0xFFFFBC2BFC7FFFFF,
    // Setting FIR_ACTION1 reg to all Fs until we get correct values
    LFIR_ACTION1_VALUE = 0xFFFFFFFFFFFFFFFF,
    FIR_MASK_VALUE = 0x0000000000000000,
    IPOLL_MASK_VALUE = 0xFC00000000000000,
    TOD_ERROR_ROUTING = 0x9FC02000F0004000,
    TOD_ERROR_MASK = 0x0000000003F00002,
    TOD_ERROR_REG_VAL = 0xFFFFFFFFFFFFFFFF,
    HANGPULSE1_MAIN_DIVIDER = 0x0400000000000000,
    HANGPULSE2_MAIN_DIVIDER = 0x7800000000000000,
    HANGPULSE3_MAIN_DIVIDER = 0x0400000000000000
};


static fapi2::ReturnCode p10_sbe_tp_chiplet_init_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p10_sbe_tp_chiplet_init(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    uint8_t pre_divider;
    uint32_t l_attr_pau_freq_mhz;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
            (fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_tp_chiplet_init: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_pau_freq_mhz));

    // This step is redundant since tp_chiplet_reset already drops the OOB mux

    FAPI_DBG("Reset PCB Master Interrupt Register");
    FAPI_TRY(fapi2::putScom(i_target_chip, INTERRUPT_TYPE_REG, 0x0));

    FAPI_DBG("Clear pervasive chiplet region fence");
    FAPI_TRY(p10_sbe_tp_chiplet_init_region_fence_setup(l_tpchiplet));

    FAPI_DBG("Reset abistclk_muxsel and syncclk_muxsel");
    l_data64.flush<0>();
    FAPI_TRY(proc::PREP_TP_TPCHIP_TPC_CPLT_CTRL0_WO_CLEAR(i_target_chip));
    proc::SET_TP_TPCHIP_TPC_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC(l_data64);
    proc::SET_TP_TPCHIP_TPC_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC(l_data64);
    FAPI_TRY(proc::PUT_TP_TPCHIP_TPC_CPLT_CTRL0_WO_CLEAR(i_target_chip, l_data64));

    FAPI_DBG("Start clocks for all regions except Pib, Net and Pll ");
    FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, REGIONS_PERV_OCC_PSI,
             CLOCK_TYPES_ALL));

    // startclocks for pll  - This is no longer necessary for P10

    FAPI_DBG("Set TOD error routing register");
    FAPI_TRY(fapi2::putScom(i_target_chip, TOD_ERROR_ROUTING_REG, TOD_ERROR_ROUTING));
    //config TOD error mask reg;
    FAPI_DBG("Configure TOD error mask register");
    FAPI_TRY(fapi2::putScom(i_target_chip, TOD_ERROR_MASK_REG, TOD_ERROR_MASK));
    //clear TOD error reg;
    FAPI_DBG("Clear TOD error register");
    FAPI_TRY(fapi2::putScom(i_target_chip, TOD_ERROR_REG, TOD_ERROR_REG_VAL));

    FAPI_DBG("Clear pervasive LFIR");
    FAPI_TRY(fapi2::putScom(l_tpchiplet, LOCAL_FIR_RW, 0));

    FAPI_DBG("Configure pervasive LFIR" );
    //Setting LOCAL_FIR_ACTION0 register value
    FAPI_TRY(fapi2::putScom(l_tpchiplet, EPS_FIR_LOCAL_ACTION0, LFIR_ACTION0_VALUE));
    //Setting LOCAL_FIR_ACTION1 register value
    FAPI_TRY(fapi2::putScom(l_tpchiplet, EPS_FIR_LOCAL_ACTION1, LFIR_ACTION1_VALUE));
    //Setting LOCAL_FIR_MASK register value
    FAPI_TRY(fapi2::putScom(l_tpchiplet, EPS_FIR_LOCAL_MASK_RW, FIR_MASK_VALUE));

    FAPI_DBG("Unmask RFIR, XFIR Mask");
    // XSTOP_MASK
    FAPI_TRY(fapi2::putScom(l_tpchiplet, XSTOP_MASK_RW, 0));
    // RECOV_MASK
    FAPI_TRY(fapi2::putScom(l_tpchiplet, RECOV_MASK_RW, 0));

    FAPI_DBG("Setup IPOLL mask register");
    FAPI_TRY(fapi2::putScom(i_target_chip, COMP_INTR_HOST_MASK_REG, IPOLL_MASK_VALUE));

    // setup hang pulse freq
    FAPI_DBG("Set up constant frequency hang pulse 1 pre divider");
    pre_divider =  ((l_attr_pau_freq_mhz * 4 + 64) / 125);
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 8 >(pre_divider);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0072, l_data64));

    FAPI_DBG("Set up constant frequency hang pulse 1 main divider");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0071, HANGPULSE1_MAIN_DIVIDER));

    FAPI_DBG("Set up constant frequency hang pulse 2 pre divider");
    pre_divider =  ((l_attr_pau_freq_mhz * 10 + 79) / 158);
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 8 >(pre_divider);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0074, l_data64));

    FAPI_DBG("Set up constant frequency hang pulse 2 main divider");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0073, HANGPULSE2_MAIN_DIVIDER));

    FAPI_DBG("Set up constant frequency hang pulse 3 pre divider");
    pre_divider =  ((l_attr_pau_freq_mhz + 125) / 250);
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 8 >(pre_divider);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0076, l_data64));

    FAPI_DBG("Set up constant frequency hang pulse 3 main divider");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0075, HANGPULSE3_MAIN_DIVIDER));

    FAPI_INF("p10_sbe_tp_chiplet_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}


/// @brief region fence setup
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_tp_chiplet_init_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    using namespace scomt::perv;

    // Local variable and constant definition
    fapi2::buffer <uint32_t> l_attr_pg;
    fapi2::buffer <uint16_t> l_attr_pg_data;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_tp_chiplet_init_region_fence_setup: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<12, 15>(l_attr_pg_data);

    FAPI_DBG("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    // No need to drop Vital fence
    l_data64.flush<0>();
    l_data64.insertFromRight<4, 15>(l_attr_pg_data);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CPLT_CTRL1_WO_CLEAR, l_data64));

    FAPI_INF("p10_sbe_tp_chiplet_init_region_fence_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
