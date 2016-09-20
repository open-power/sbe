/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_init3.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file  p9_sbe_tp_chiplet_init3.C
///
/// @brief TP Chiplet Start Clocks
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_chiplet_init3.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>


enum P9_SBE_TP_CHIPLET_INIT3_Private_Constants
{
    START_CMD = 0x1,
    REGIONS_ALL_EXCEPT_PIB_NET_PLL = 0x4FE,
    CLOCK_TYPES = 0x7,
    HW_NS_DELAY = 100000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000, // unit is sim cycles
    POLL_COUNT = 300, // Observed Number of times CBS read for CBS_INTERNAL_STATE_VECTOR
    OSC_ERROR_MASK = 0xF700000000000000, // Mask OSC errors
    LFIR_ACTION0_VALUE = 0x0000000000000000,
    LFIR_ACTION1_VALUE = 0x8000000000000000,
    FIR_MASK_VALUE = 0xFFFFFFFFFFC00000
};

static fapi2::ReturnCode p9_sbe_tp_chiplet_init3_clock_test2(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

static fapi2::ReturnCode p9_sbe_tp_chiplet_init3_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_tp_chiplet_init3(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint32_t> l_pfet_value;
    fapi2::buffer<uint32_t> l_attr_pfet;
    fapi2::buffer<uint64_t> l_regions;
    fapi2::buffer<uint64_t> l_kvref_reg;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                fapi2::TARGET_STATE_FUNCTIONAL)[0];
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    FAPI_INF("p9_sbe_tp_chiplet_init3: Entering ...");

    FAPI_DBG("Reading ATTR_PFET_OFF_CONTROLS");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PFET_OFF_CONTROLS, i_target_chip,
                           l_pfet_value));

    FAPI_DBG("Switch pervasive chiplet OOB mux");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();  //PIB.ROOT_CTRL0.OOB_MUX = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    FAPI_DBG("Reset PCB Master Interrupt Register");
    //Setting INTERRUPT_TYPE_REG register value
    //PIB.INTERRUPT_TYPE_REG = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, 0));

    FAPI_DBG("Clear pervasive chiplet region fence");
    FAPI_TRY(p9_sbe_tp_chiplet_init3_region_fence_setup(l_tpchiplet));

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_tpchiplet,
             REGIONS_ALL_EXCEPT_PIB_NET_PLL, l_regions));
    FAPI_DBG("l_regions value: %#018lX", l_regions);

    FAPI_TRY(p9_sbe_common_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, l_regions,
                                            CLOCK_TYPES));

    FAPI_DBG("Calling clock_test2");
    FAPI_TRY(p9_sbe_tp_chiplet_init3_clock_test2(i_target_chip));

    FAPI_DBG("Drop FSI fence 5");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    //PIB.ROOT_CTRL0.FENCE5_DC = 0
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_FENCE5_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    l_pfet_value.extractToRight<0, 30>(l_attr_pfet);

    FAPI_DBG("Set pfet off controls");
    //Setting DISABLE_FORCE_PFET_OFF register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PU_DISABLE_FORCE_PFET_OFF, l_data64));
    //PIB.DISABLE_FORCE_PFET_OFF.DISABLE_FORCE_PFET_OFF_REG = l_attr_pfet
    l_data64.insertFromRight<0, 30>(l_attr_pfet);
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_DISABLE_FORCE_PFET_OFF, l_data64));

    FAPI_DBG("Drop EDRAM control gate and pfet_force_off");
    //Setting ROOT_CTRL2 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL2_SCOM, l_data64));
    l_data64.clearBit<16>();  //PIB.ROOT_CTRL2.ROOT_CTRL2_16_FREE_USAGE = 0
    //PIB.ROOT_CTRL2.TPFSI_TP_PFET_FORCE_OFF_DC = 0
    l_data64.clearBit<PERV_ROOT_CTRL2_SET_TPFSI_TP_PFET_FORCE_OFF_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL2_SCOM, l_data64));

    //TOD error reg;
    //config TOD error mask reg;
    //clear TOD error reg;

    FAPI_DBG("Clear pervasive LFIR");
    //Setting LOCAL_FIR register value
    //PERV.LOCAL_FIR = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_AND, 0));

    FAPI_DBG("Configure pervasive LFIR" );
    //Setting LOCAL_FIR_ACTION0 register value
    //PERV.LOCAL_FIR_ACTION0 = LFIR_ACTION0_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_ACTION0,
                            LFIR_ACTION0_VALUE));
    //Setting LOCAL_FIR_ACTION1 register value
    //PERV.LOCAL_FIR_ACTION1 = LFIR_ACTION1_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_ACTION1,
                            LFIR_ACTION1_VALUE));
    //Setting LOCAL_FIR_MASK register value
    //PERV.LOCAL_FIR_MASK = FIR_MASK_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_MASK, FIR_MASK_VALUE));

    // Enables any checkstop if set, to propogate to FSP and get notified
    //
    FAPI_DBG("p9_sbe_tp_chiplet_init3: Unmask CFIR Mask");
    //Setting FIR_MASK register value
    //PERV.FIR_MASK = FIR_MASK_VALUE
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_FIR_MASK, FIR_MASK_VALUE));

    FAPI_DBG("Setup Pervasive Hangcounter 0:Thermal, 1:OCC/SBE, 2:PBA hang, 3:Nest freq for TOD hang, 5:malefunction alert");
    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //PERV.HANG_PULSE_0_REG.HANG_PULSE_REG_0 = 0b010000
    l_data64.insertFromRight<0, 6>(0b010000);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    //PERV.HANG_PULSE_1_REG.HANG_PULSE_REG_1 = 0b000100
    l_data64.insertFromRight<0, 6>(0b000100);
    l_data64.setBit<6>();  //PERV.HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_1_REG, l_data64));
    //Setting HANG_PULSE_2_REG register value (Setting all fields)
    //PERV.HANG_PULSE_2_REG.HANG_PULSE_REG_2 = 0b010010
    l_data64.insertFromRight<0, 6>(0b010010);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_2_REG, l_data64));
    //Setting HANG_PULSE_3_REG register value (Setting all fields)
    //PERV.HANG_PULSE_3_REG.HANG_PULSE_REG_3 = 0b000001
    l_data64.insertFromRight<0, 6>(0b000001);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_3_REG.SUPPRESS_HANG_3 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_3_REG, l_data64));
    //Setting HANG_PULSE_5_REG register value (Setting all fields)
    //PERV.HANG_PULSE_5_REG.HANG_PULSE_REG_5 = 0b000110
    l_data64.insertFromRight<0, 6>(0b000110);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_5_REG.SUPPRESS_HANG_5 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_5_REG, l_data64));

    FAPI_DBG("Start  calibration");
    //Setting KVREF_AND_VMEAS_MODE_STATUS_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));
    l_data64.setBit<0>();  //KVREF_AND_VMEAS_MODE_STATUS_REG.KVREF_START_CAL = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));

    FAPI_DBG("Check for calibration done");
    l_timeout = POLL_COUNT;

    //UNTIL KVREF_AND_VMEAS_MODE_STATUS_REG.KVREF_CAL_DONE == 1
    while (l_timeout != 0)
    {
        //Getting KVREF_AND_VMEAS_MODE_STATUS_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));
        //bool l_poll_data = KVREF_AND_VMEAS_MODE_STATUS_REG.KVREF_CAL_DONE
        bool l_poll_data = l_data64.getBit<16>();

        if (l_poll_data == 1)
        {
            break;
        }

        fapi2::delay(HW_NS_DELAY, SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_DBG("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::CALIBRATION_NOT_DONE(),
                "Calibration not done, bit16 not set");

    FAPI_INF("p9_sbe_tp_chiplet_init3: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief clock test
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_tp_chiplet_init3_clock_test2(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read ;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_tp_chiplet_init3_clock_test2: Entering ...");

    FAPI_DBG("unfence 281D");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    l_data64.clearBit<0>();  //PIB.ROOT_CTRL0.TPFSI_SBE_FENCE_VTLIO_DC_UNUSED = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    //Getting ROOT_CTRL3 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL3_SCOM,
                            l_read)); //l_read = PIB.ROOT_CTRL3

    l_read.setBit<27>();

    FAPI_DBG("Set osc_ok latch active");
    //Setting ROOT_CTRL3 register value
    //PIB.ROOT_CTRL3 = l_read
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_SCOM, l_read));

    FAPI_DBG("Turn on oscilate pgood");
    //Setting ROOT_CTRL6 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL6_SCOM, l_data64));
    //PIB.ROOT_CTRL6.TPFSI_OSCSW1_PGOOD = 1
    l_data64.setBit<PERV_ROOT_CTRL6_SET_TPFSI_OSCSW1_PGOOD>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL6_SCOM, l_data64));

    //Getting ROOT_CTRL3 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL3_SCOM,
                            l_read)); //l_read = PIB.ROOT_CTRL3

    l_read.clearBit<17>();

    FAPI_DBG("turn off use_osc_1_0");
    //Setting ROOT_CTRL3 register value
    //PIB.ROOT_CTRL3 = l_read
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL3_SCOM, l_read));

    FAPI_DBG("Mask OSC err");
    //Setting OSCERR_MASK register value
    //PIB.OSCERR_MASK = OSC_ERROR_MASK
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_OSCERR_MASK, OSC_ERROR_MASK));

    FAPI_DBG("reset osc-error_reg");
    //Setting OSCERR_HOLD register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_OSCERR_HOLD, l_data64));
    l_data64.clearBit<4, 4>();  //PERV.OSCERR_HOLD.OSCERR_MEM = 0000
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_OSCERR_HOLD, l_data64));

    FAPI_DBG("Resets FIR");
    //Setting LOCAL_FIR register value
    l_data64.flush<1>();
    l_data64.clearBit<36>();
    l_data64.clearBit<37>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_AND, l_data64));

#ifndef SIM_ONLY_OSC_SWC_CHK

    FAPI_DBG("check for OSC ok");
    //Getting SNS1LTH register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SNS1LTH_SCOM,
                            l_read)); //l_read = PIB.SNS1LTH

    FAPI_ASSERT(l_read.getBit<21>() == 0 && l_read.getBit<28>() == 1,
                fapi2::MF_OSC_NOT_TOGGLE()
                .set_READ_SNS1LTH(l_read),
                "MF oscillator not toggling");

    FAPI_DBG("Osc error active");
    //Getting OSCERR_HOLD register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_OSCERR_HOLD,
                            l_read)); //l_read = PERV.OSCERR_HOLD

    FAPI_ASSERT(l_read.getBit<4>() == 0,
                fapi2::MF_OSC_ERR()
                .set_READ_OSCERR_HOLD(l_read),
                "MF oscillator error active");

#endif

    FAPI_INF("p9_sbe_tp_chiplet_init3_clock_test2: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief region fence setup
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_tp_chiplet_init3_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    // Local variable and constant definition
    fapi2::buffer <uint16_t> l_attr_pg;
    fapi2::buffer <uint16_t> l_attr_pg_data;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_tp_chiplet_init3_region_fence_setup: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<4, 11>(l_attr_pg_data);

    FAPI_DBG("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>
    (l_attr_pg.getBit<3>());
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_attr_pg_data
    l_data64.insertFromRight<4, 11>(l_attr_pg_data);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_INF("p9_sbe_tp_chiplet_init3_region_fence_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
