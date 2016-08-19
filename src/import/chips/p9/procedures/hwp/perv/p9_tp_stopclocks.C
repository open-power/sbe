/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_tp_stopclocks.C $  */
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
/// @file  p9_tp_stopclocks.C
///
/// @brief Stop clocks for tp chiplet
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_tp_stopclocks.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>
#include <p9_common_stopclocks.H>


enum P9_TP_STOPCLOCKS_Private_Constants
{
    CLOCK_CMD = 0x2,
    CLOCK_TYPES = 0x7,
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    REGIONS_ALL_EXCEPT_PLL = 0x7FE,
    STARTMASTER = 0x1,
    STARTSLAVE = 0x1
};

fapi2::ReturnCode p9_tp_stopclocks(const
                                   fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_clock_regions;
    fapi2::buffer<uint32_t> l_data32;
    fapi2::buffer<uint32_t> l_data32_root_ctrl0;
    FAPI_INF("p9_tp_stopclocks: Entering ...");

    FAPI_DBG("p9_tp_stopclocks: Raise chiplet fence");
    //Setting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_data32));
    //CFAM.PERV_CTRL0.TP_FENCE_EN_DC = 1
    l_data32.setBit<PERV_PERV_CTRL0_SET_TP_FENCE_EN_DC>();
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_data32));

    FAPI_DBG("p9_tp_stopclocks: Raise pib2pcb mux");
    //Setting ROOT_CTRL0 register value
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI,
                                    l_data32_root_ctrl0));
    //CFAM.ROOT_CTRL0.PIB2PCB_DC = 1
    l_data32_root_ctrl0.setBit<PERV_ROOT_CTRL0_SET_PIB2PCB_DC>();
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI,
                                    l_data32_root_ctrl0));

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], REGIONS_ALL_EXCEPT_PLL, l_clock_regions));
    FAPI_DBG("p9_tp_stopclocks: Regions value: %#018lX", l_clock_regions);

    FAPI_DBG("p9_tp_stopclocks: Call module clock start stop for Tp chiplet");
    FAPI_TRY(p9_sbe_common_clock_start_stop(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], CLOCK_CMD, DONT_STARTSLAVE, DONT_STARTMASTER,
                 l_clock_regions, CLOCK_TYPES));

    FAPI_DBG("p9_tp_stopclocks: Assert vital fence and set flush_inhibit");
    FAPI_TRY(p9_common_stopclocks_set_vitalfence_flushmode(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0]));

    FAPI_DBG("p9_tp_stopclocks: Raise partial good fences and set abist_muxsel, syncclk_muxsel");
    FAPI_TRY(p9_common_stopclocks_cplt_ctrl_action_function(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0]));

    FAPI_INF("p9_tp_stopclocks: Raise OOB Mux");
    //Setting ROOT_CTRL0 register value
    //CFAM.ROOT_CTRL0.OOB_MUX = 1
    l_data32_root_ctrl0.setBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI,
                                    l_data32_root_ctrl0));

    FAPI_INF("p9_tp_stopclocks: Raise Global Endpoint reset");
    //Setting ROOT_CTRL0 register value
    //CFAM.ROOT_CTRL0.GLOBAL_EP_RESET_DC = 1
    l_data32_root_ctrl0.setBit<PERV_ROOT_CTRL0_SET_GLOBAL_EP_RESET_DC>();
    FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI,
                                    l_data32_root_ctrl0));

    FAPI_INF("p9_tp_stopclocks: Exiting ...");
fapi_try_exit:
    return fapi2::current_err;

}
