/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_init3.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>


enum P9_SBE_TP_CHIPLET_INIT3_Private_Constants
{
    START_CMD = 0x1,
    REGIONS_ALL_EXCEPT_PIB_NET = 0x4FF,
    CLOCK_TYPES = 0x7
};

static fapi2::ReturnCode p9_sbe_tp_chiplet_init3_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_tp_chiplet_init3(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    bool l_read_reg = 0;
    fapi2::buffer<uint64_t> l_regions;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                fapi2::TARGET_STATE_FUNCTIONAL)[0];
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Switch pervasive chiplet OOB mux");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();  //PIB.ROOT_CTRL0.OOB_MUX = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    FAPI_DBG("Reset PCB Master Interrupt Register");
    //Setting INTERRUPT_TYPE_REG register value
    //PIB.INTERRUPT_TYPE_REG = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, 0));

    FAPI_DBG("Clear Pervasive Chiplet region fence");
    FAPI_TRY(p9_sbe_tp_chiplet_init3_region_fence_setup(l_tpchiplet));

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_tpchiplet,
             REGIONS_ALL_EXCEPT_PIB_NET, l_regions));
    FAPI_DBG("l_regions value: %#018lX", l_regions);

    FAPI_TRY(p9_sbe_common_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, l_regions,
                                            CLOCK_TYPES));

    FAPI_DBG("Drop FSI fence 5");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    //PIB.ROOT_CTRL0.FENCE5_DC = 0
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_FENCE5_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    FAPI_DBG("Drop EDRAM control gate");
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
    //PERV.LOCAL_FIR_ACTION0=0;
    //PERV.LOCAL_FIR_ACTION1=0;
    //PERV.LOCAL_FIR_MASK=0;

    FAPI_DBG("Add Pervasive chiplet to Multicast Group 0");
    //Setting MULTICAST_GROUP_1 register value
    //PERV.MULTICAST_GROUP_1 = 0xE0001c0000000000
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_MULTICAST_GROUP_1,
                            0xE0001c0000000000));

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

    FAPI_DBG("CHECK FOR XSTOP");
    //Getting INTERRUPT_TYPE_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, l_data64));
    //l_read_reg = PIB.INTERRUPT_TYPE_REG.CHECKSTOP
    l_read_reg = l_data64.getBit<PERV_INTERRUPT_TYPE_REG_CHECKSTOP>();

    FAPI_ASSERT(l_read_reg == 0,
                fapi2::XSTOP_ERR()
                .set_READ_XSTOP(l_read_reg),
                "XSTOP BIT GET SET");

    FAPI_INF("Exiting ...");

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
    fapi2::buffer <uint32_t> l_attr_pg;
    fapi2::buffer <uint16_t> l_attr_pg_data;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<20, 11>(l_attr_pg_data);

    FAPI_INF("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>
    (l_attr_pg.getBit<19>());  //CPLT_CTRL1.TC_VITL_REGION_FENCE = l_attr_pg.getBit<19>()
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_attr_pg_data
    l_data64.insertFromRight<4, 11>(l_attr_pg_data);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
