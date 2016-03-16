/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_init3.C $      */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
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

fapi2::ReturnCode p9_sbe_tp_chiplet_init3(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    bool l_read_reg = 0;
    fapi2::buffer<uint64_t> l_regions;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                fapi2::TARGET_STATE_FUNCTIONAL)[0];
    FAPI_DBG("Entering ...");

    FAPI_DBG("Switch pervasive chiplet OOB mux");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();  //PIB.ROOT_CTRL0.OOB_MUX = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    FAPI_DBG("Reset PCB Master Interrupt Register");
    //Setting INTERRUPT_TYPE_REG register value
    //PIB.INTERRUPT_TYPE_REG = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, 0));

    FAPI_DBG("Clear PIB,NET,OCC-PIB and PRV region fence");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //PERV.CPLT_CTRL1.TC_PERV_REGION_FENCE = 0b0
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_PERV_REGION_FENCE>();
    l_data64.setBit<5>();  //PERV.CPLT_CTRL1.TC_REGION1_FENCE = 0
    l_data64.setBit<6>();  //PERV.CPLT_CTRL1.TC_REGION2_FENCE = 0
    //PERV.CPLT_CTRL1.TC_REGION3_FENCE = 0
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_REGION3_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("Drop vital fence for TP chiplet");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //PERV.CPLT_CTRL1.TC_VITL_REGION_FENCE = 0
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_CPLT_CTRL1_CLEAR, l_data64));

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

    FAPI_DBG("Setup Pervasive Hangcounter 0:PBA, 1:ADU, 2:OCC/SBE, 3:PB, 4:malefunction alert");
    //Setting HANG_PULSE_0_REG register value (Setting all fields)
    //PERV.HANG_PULSE_0_REG.HANG_PULSE_REG_0 = 0b010010
    l_data64.insertFromRight<0, 6>(0b010010);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_0_REG.SUPPRESS_HANG_0 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_0_REG, l_data64));
    //Setting HANG_PULSE_1_REG register value (Setting all fields)
    //PERV.HANG_PULSE_1_REG.HANG_PULSE_REG_1 = 0b011100
    l_data64.insertFromRight<0, 6>(0b011100);
    l_data64.setBit<6>();  //PERV.HANG_PULSE_1_REG.SUPPRESS_HANG_1 = 0b1
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_1_REG, l_data64));
    //Setting HANG_PULSE_2_REG register value (Setting all fields)
    //PERV.HANG_PULSE_2_REG.HANG_PULSE_REG_2 = 0b000100
    l_data64.insertFromRight<0, 6>(0b000100);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_2_REG.SUPPRESS_HANG_2 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_2_REG, l_data64));
    //Setting HANG_PULSE_4_REG register value (Setting all fields)
    //PERV.HANG_PULSE_4_REG.HANG_PULSE_REG_4 = 0b000001
    l_data64.insertFromRight<0, 6>(0b000001);
    l_data64.clearBit<6>();  //PERV.HANG_PULSE_4_REG.SUPPRESS_HANG_4 = 0b0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_HANG_PULSE_4_REG, l_data64));
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

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
