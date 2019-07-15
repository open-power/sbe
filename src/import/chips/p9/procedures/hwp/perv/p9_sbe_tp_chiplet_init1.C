/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_init1.C $ */
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
/// @file  p9_sbe_tp_chiplet_init1.C
///
/// @brief Initial steps of PIB AND PCB
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_chiplet_init1.H"
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9n2_perv_scom_addresses_fld.H>
#include <p9a_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>

enum P9_SBE_TP_CHIPLET_INIT1_Private_Constants
{
    START_CMD = 0x1,
    CLOCK_TYPES_ALL = 0x7,
    REGIONS_PIB_NET = 0x300,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    REGIONS_EXCEPT_VITAL_PIB_NET = 0x4FF, // Regions excluding VITAL, PIB and NET
    REGIONS_EXCEPT_VITAL_PIB_NET_SBE = 0x4DF, // Regions excluding VITAL, PIB, NET and SBE
    SCAN_TYPES_TIME_GPTR_REPR = 0x230
};

#ifndef __PPE__
/// @brief Switch PCB mux via CFAM
///
/// @param[in]     i_target_chip     Reference to TARGET_TYPE_PROC_CHIP
/// @param[in]     i_new_position    Desired new mux position (bit position in ROOT_CTRL0)
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode switch_pcb_mux_cfam(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    const uint32_t i_new_position)
{
    fapi2::buffer<uint32_t> l_pcb_reset, l_mux_select;

    l_pcb_reset.flush<0>().setBit<PERV_ROOT_CTRL0_PCB_RESET_DC>();

    l_mux_select.flush<0>()
    .setBit<P9A_PERV_ROOT_CTRL0_PIB2PCB_DC>()
    .setBit<P9A_PERV_ROOT_CTRL0_PCB2PCB_DC>();

    FAPI_DBG("Setting PCB RESET bit in  ROOT_CTRL0_REG");
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_SET_FSI, l_pcb_reset));

    // This will unconditionally set both bits (PIB2PCB, PCB2PCB), switching to a safe
    // intermediate state from either of the two settings.
    FAPI_DBG("Setting PCB mux to intermediate state");
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_SET_FSI, l_mux_select));

    // We CLEAR the bit corresponding to the desired new position in l_mux_select
    // and then use the CLEAR address of RC0 to effectively clear the _other_ bit,
    // so that the bit left standing is our desired new position.
    FAPI_DBG("Setting PCB mux to final state");
    l_mux_select.clearBit(i_new_position);
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_CLEAR_FSI, l_mux_select));

    FAPI_DBG("Clearing PCB RESET bit in  ROOT_CTRL0_REG");
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_CLEAR_FSI, l_pcb_reset));

fapi_try_exit:
    return fapi2::current_err;

}
#endif

fapi2::ReturnCode p9_sbe_tp_chiplet_init1(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_data64_perv_ctrl0;
    fapi2::buffer<uint64_t> l_data64_root_ctrl0;
    fapi2::buffer<uint64_t> l_data64_cplt_ctrl1;
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::buffer<uint8_t> l_read_attr;
#if defined(SBE_AXONE_CONFIG) || !defined(__PPE__)
    fapi2::buffer<uint8_t>  l_is_axone;
#endif
    fapi2::buffer<uint64_t> l_clk_regions;
#ifndef __PPE__
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                fapi2::TARGET_STATE_FUNCTIONAL)[0];
#endif
    FAPI_INF("p9_sbe_tp_chiplet_init1: Entering ...");

    FAPI_DBG("Disable local clock gating VITAL");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_VITL_CLOCK_GATING,
                           i_target_chip, l_read_attr));
    FAPI_DBG("l_read_attr is %d", l_read_attr);

#if defined(SBE_AXONE_CONFIG) || !defined(__PPE__)
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_P9A_SBE_REGION, i_target_chip, l_is_axone));
#endif

    //Getting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM,
                            l_data64_perv_ctrl0));
    //Getting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM,
                            l_data64_root_ctrl0));

    if (l_read_attr)
    {
        //PERV_PERV_CTRL0_SET_TP_VITL_ACT_DIS_DC = 1
        l_data64_perv_ctrl0.setBit<PERV_PERV_CTRL0_SET_TP_VITL_ACT_DIS_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM,
                                l_data64_perv_ctrl0));
    }

#ifndef __PPE__

    if (l_is_axone)
    {
        /*
         * In some lab configurations, the SBE might have done these exact steps
         * before us already, but it won't hurt to do them all again.
         */
        FAPI_DBG("Switch PCB mux to PIB2PCB position");
        FAPI_TRY(switch_pcb_mux_cfam(i_target_chip, P9A_PERV_ROOT_CTRL0_PIB2PCB_DC));

        FAPI_DBG("Set Chiplet Enable");
        //PIB.PERV_CTRL0.TP_CHIPLET_EN_DC = 1
        l_data64_perv_ctrl0.setBit<PERV_PERV_CTRL0_SET_TP_CHIPLET_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64_perv_ctrl0));

        FAPI_DBG("Drop TP Chiplet Fence Enable");
        //PIB.PERV_CTRL0.TP_FENCE_EN_DC = 0
        l_data64_perv_ctrl0.clearBit<PERV_PERV_CTRL0_SET_TP_FENCE_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64_perv_ctrl0));

        FAPI_DBG("Drop NET and PIB region fence");
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_CPLT_CTRL1, l_data64_cplt_ctrl1));
        l_data64_cplt_ctrl1.clearBit<PERV_1_CPLT_CTRL1_UNUSED_5B>();
        l_data64_cplt_ctrl1.clearBit<PERV_1_CPLT_CTRL1_UNUSED_6B>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_CPLT_CTRL1, l_data64_cplt_ctrl1));

        FAPI_DBG("Starting clock for PIB and NET");
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_tpchiplet, REGIONS_PIB_NET, l_clk_regions));
        FAPI_DBG("l_clk_regions value: %#018lX", l_clk_regions);
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, l_clk_regions, CLOCK_TYPES_ALL));

        FAPI_DBG("Switch PCB mux to PCB2PCB position");
        FAPI_TRY(switch_pcb_mux_cfam(i_target_chip, P9A_PERV_ROOT_CTRL0_PCB2PCB_DC));

        //Getting ROOT_CTRL0 register value again as it is changed in above function
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM,
                                l_data64_root_ctrl0));
    }
    else
#endif
    {
        FAPI_DBG("Release PCB Reset");
        //PIB.ROOT_CTRL0.PCB_RESET_DC = 0
        l_data64_root_ctrl0.clearBit<PERV_ROOT_CTRL0_SET_PCB_RESET_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64_root_ctrl0));

        FAPI_DBG("Enable PCB auto-reset");
        l_data64.flush<0>().setBit<PERV_RESET_REG_TIMEOUT_EN>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_RESET_REG, l_data64));

        FAPI_DBG("Set Chiplet Enable");
        //PIB.PERV_CTRL0.TP_CHIPLET_EN_DC = 1
        l_data64_perv_ctrl0.setBit<PERV_PERV_CTRL0_SET_TP_CHIPLET_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64_perv_ctrl0));

        FAPI_DBG("Drop TP Chiplet Fence Enable");
        //PIB.PERV_CTRL0.TP_FENCE_EN_DC = 0
        l_data64_perv_ctrl0.clearBit<PERV_PERV_CTRL0_SET_TP_FENCE_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64_perv_ctrl0));
    }

    FAPI_DBG("Drop Global Endpoint reset");
    //PIB.ROOT_CTRL0.GLOBAL_EP_RESET_DC = 0
    l_data64_root_ctrl0.clearBit<PERV_ROOT_CTRL0_SET_GLOBAL_EP_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64_root_ctrl0));
    FAPI_DBG("Switching PIB trace bus to SBE tracing");

    FAPI_DBG("Drop OOB Mux");
    l_data64_root_ctrl0.clearBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();  //PIB.ROOT_CTRL0.OOB_MUX = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64_root_ctrl0));

#if defined(SBE_AXONE_CONFIG) || !defined(__PPE__)

    if(l_is_axone)
    {

        FAPI_DBG("Region setup call");
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(
                     i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                             fapi2::TARGET_STATE_FUNCTIONAL)[0], REGIONS_EXCEPT_VITAL_PIB_NET_SBE, l_regions));
        FAPI_DBG("l_regions value : %#018lX", l_regions);
    }
    else
#endif
    {
        FAPI_DBG("Region setup call");
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(
                     i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                             fapi2::TARGET_STATE_FUNCTIONAL)[0], REGIONS_EXCEPT_VITAL_PIB_NET, l_regions));
        FAPI_DBG("l_regions value : %#018lX", l_regions);
    }

    FAPI_DBG("Run scan0 module for region except vital,PIB,NET,SBE(SBE only for axone), scan types GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_regions, SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("Run scan0 module for region except vital,PIB,NET,SBE(SBE only for axone), scan types except GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_regions,
                 SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_DBG("Set TP_TCPERV_SRAM_ENABLE_DC");
    l_data64_perv_ctrl0.setBit<P9N2_PERV_PERV_CTRL0_TP_TCPERV_SRAM_ENABLE_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64_perv_ctrl0));

    FAPI_INF("p9_sbe_tp_chiplet_init1: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
