/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_chiplet_init.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
#include "p10_scom_proc_f.H"
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
    // Values translated from (long link, undo line breaks):
    // https://w3-connections.ibm.com/communities/service/html/communityview?
    //   communityUuid=340f24c2-a7a1-4377-8104-e443d5836678#
    //   fullpageWidgetId=We97b5997bab6_46f0_989d_4c47c6ed3356&
    //   file=361e143b-4808-4e36-a594-9d3d64bd5ad6
    // using chips/p10/procedures/utils/perv_lfir/gen_lfir_settings.sh

    START_CMD = 0x1,
    REGIONS_PERV = 0x4000,
    REGIONS_PERV_PSI = 0x4100,
    CLOCK_TYPES_ALL = 0x7,
    REGIONS_PLL = 0x0010,
    CLOCK_TYPES_SL = 0x4,
    IPOLL_MASK_VALUE = 0xFC00000000000000,
    TOD_ERROR_ROUTING = 0x9FC02000F0004000,
    TOD_ERROR_REG_VAL = 0xFFFFFFFFFFFFFFFF,
    HANGPULSE1_MAIN_DIVIDER = 0x0400000000000000,
    HANGPULSE2_MAIN_DIVIDER = 0x6C00000000000000,
    HANGPULSE3_MAIN_DIVIDER = 0x0400000000000000
};


static fapi2::ReturnCode p10_sbe_tp_chiplet_init_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p10_sbe_tp_chiplet_init(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;
    using namespace scomt::proc;

    fapi2::buffer<uint64_t> l_data64, l_buffer64;
    uint8_t pre_divider;
    uint32_t l_attr_pau_freq_mhz, l_pau_multiplier, l_real_pau_frequency;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type l_attr_contained_ipl_type;
    uint8_t l_cp_refclck_select;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
            (fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_tp_chiplet_init: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
             "Error from FAPI_ATTR_GET (ATTR_CONTAINED_IPL_TYPE)");

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

    if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
    {
        proc::SET_TP_TPCHIP_TPC_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC(l_data64);
    }
    else
    {
        FAPI_DBG("Skip clearing syncclk_muxsel for contained IPL");
    }

    FAPI_TRY(proc::PUT_TP_TPCHIP_TPC_CPLT_CTRL0_WO_CLEAR(i_target_chip, l_data64));

    FAPI_DBG("Start clocks for all regions except Pib, Net and Pll ");

    if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_NONE)
    {
        FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, REGIONS_PERV_PSI,
                 CLOCK_TYPES_ALL));
    }
    else
    {
        FAPI_DBG("Skip setting PSI clocks for contained ipl");
        FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, REGIONS_PERV,
                 CLOCK_TYPES_ALL));
    }

    FAPI_DBG("Clear flush_inhibit to go in to flush mode");
    l_data64.flush<0>()
    .setBit<CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH>();
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL0_WO_CLEAR, l_data64));

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

    // check RCS mode, if redundant, we need to clear the Osc error mask bits
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_REFCLOCK_SELECT, i_target_chip, l_cp_refclck_select),
             "Error from FAPI_ATTR_GET (ATTR_CP_REFCLOCK_SELECT)");

    if(((l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0) ||
        (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1) ||
        (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0_NORED) ||
        (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1_NORED)))
    {
        // if we are in redundant mode, we need to unmask the clock A and clock B OSC errors
        //   which are now going to be masked by default (SW530544)
        l_buffer64.flush<1>();
        l_buffer64.clearBit<TP_TPCHIP_TPC_EPS_FIR_LOCAL_MASK_42>();
        l_buffer64.clearBit<TP_TPCHIP_TPC_EPS_FIR_LOCAL_MASK_43>();
        FAPI_TRY(fapi2::putScom(i_target_chip, TP_TPCHIP_TPC_EPS_FIR_LOCAL_MASK_WO_AND, l_buffer64));

    }

    FAPI_DBG("Unmask RFIR, XFIR Mask");
    // XSTOP_MASK
    FAPI_TRY(fapi2::putScom(l_tpchiplet, XSTOP_MASK_RW, 0));
    // RECOV_MASK
    FAPI_TRY(fapi2::putScom(l_tpchiplet, RECOV_MASK_RW, 0));

    FAPI_DBG("Setup IPOLL mask register");
    FAPI_TRY(fapi2::putScom(i_target_chip, COMP_INTR_HOST_MASK_REG, IPOLL_MASK_VALUE));

    // setup hang pulse freq
    // This calculation seems redundant, but we want to round the attribute value down to
    // an integer multiple of 50/3 since that is the granularity of the PAU DPLL.
    // Add 1 to the attribute value to correct for 2033.333 being represented as 2033 etc.
    l_pau_multiplier = ((l_attr_pau_freq_mhz + 1) * 3) / 50;
    l_real_pau_frequency = (l_pau_multiplier * 50) / 3;

    FAPI_DBG("Set up constant frequency hang pulse 1 pre divider");
    pre_divider =  ((l_real_pau_frequency * 2 + 64) / 125) - 1;
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 8 >(pre_divider);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0072, l_data64));

    FAPI_DBG("Set up constant frequency hang pulse 1 main divider");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0071, HANGPULSE1_MAIN_DIVIDER));

    FAPI_DBG("Set up constant frequency hang pulse 2 pre divider");
    pre_divider =  ((l_real_pau_frequency * 8 + 64) / 125) - 1;
    l_data64.flush<0>();
    l_data64.insertFromRight< 0, 8 >(pre_divider);
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0074, l_data64));

    FAPI_DBG("Set up constant frequency hang pulse 2 main divider");
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x000D0073, HANGPULSE2_MAIN_DIVIDER));

    FAPI_DBG("Set up constant frequency hang pulse 3 pre divider");
    pre_divider =  ((l_real_pau_frequency + 125) / 250) - 1;
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
    l_data64.clearBit<7>(); // Not dropping region fence for OCC
    FAPI_TRY(fapi2::putScom(i_target_chiplet, CPLT_CTRL1_WO_CLEAR, l_data64));

    FAPI_INF("p10_sbe_tp_chiplet_init_region_fence_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
