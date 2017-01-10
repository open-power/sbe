/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_init1.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_chiplet_init1.H"
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>


enum P9_SBE_TP_CHIPLET_INIT1_Private_Constants
{
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCE,
    REGIONS_EXCEPT_VITAL_PIB_NET = 0x4FF, // Regions excluding VITAL, PIB and NET
    SCAN_TYPES_TIME_GPTR_REPR = 0x230
};

fapi2::ReturnCode p9_sbe_tp_chiplet_init1(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_data64_perv_ctrl0;
    fapi2::buffer<uint64_t> l_data64_root_ctrl0;
    fapi2::buffer<uint8_t> l_read_attr;
    FAPI_INF("p9_sbe_tp_chiplet_init1: Entering ...");

    FAPI_DBG("Disable local clock gating VITAL");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_VITL_CLOCK_GATING,
                           i_target_chip, l_read_attr));
    FAPI_DBG("l_read_attr is %d", l_read_attr);

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

    FAPI_DBG("Drop Global Endpoint reset");
    //PIB.ROOT_CTRL0.GLOBAL_EP_RESET_DC = 0
    l_data64_root_ctrl0.clearBit<PERV_ROOT_CTRL0_SET_GLOBAL_EP_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64_root_ctrl0));
    FAPI_DBG("Switching PIB trace bus to SBE tracing");

    FAPI_DBG("Drop OOB Mux");
    l_data64_root_ctrl0.clearBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();  //PIB.ROOT_CTRL0.OOB_MUX = 0
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64_root_ctrl0));

    FAPI_DBG("Region setup call");
    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], REGIONS_EXCEPT_VITAL_PIB_NET, l_regions));
    FAPI_DBG("l_regions value : %#018lX", l_regions);

    FAPI_DBG("run scan0 module for region except vital,PIB,net, scan types GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_regions, SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("run scan0 module for region except vital,PIB,net, scan types except GPTR, TIME, REPR");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_regions,
                 SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_INF("p9_sbe_tp_chiplet_init1: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
