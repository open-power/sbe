/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_chiplet_reset.C $ */
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
/// @file  p10_sbe_tp_chiplet_reset.C
///
/// @brief Initial steps of PIB AND PCB
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------
//
#include "p10_sbe_tp_chiplet_reset.H"
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9n2_perv_scom_addresses_fld.H>
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <p10_hang_pulse_mc_setup_tables.H>

enum P10_SBE_TP_CHIPLET_RESET_Private_Constants
{
    PGOOD_REGIONS_STARTBIT = 4,
    PGOOD_REGIONS_LENGTH = 15,
    PGOOD_REGIONS_OFFSET = 12,
};

fapi2::ReturnCode p10_sbe_tp_chiplet_reset(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    const uint32_t BASE_ADDRESS = 0x000F0020;
    const uint8_t PRE_DIVIDER = 0x1;
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint32_t> l_read_attr_pg;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_tp_chiplet_reset: Entering ...");

    FAPI_DBG("Drop Global Endpoint reset");
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_GLOBAL_EP_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    FAPI_DBG("Drop OOB Mux");
    l_data64.clearBit<PERV_ROOT_CTRL0_SET_OOB_MUX>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));

    FAPI_DBG("Transfer Perv pgood attribute into region good register(cplt_ctrl2 reg)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, l_tpchiplet, l_read_attr_pg));
    l_data64.flush<0>();
    l_read_attr_pg.invert();
    l_data64.insert< PGOOD_REGIONS_STARTBIT, PGOOD_REGIONS_LENGTH, PGOOD_REGIONS_OFFSET >(l_read_attr_pg);
    FAPI_TRY(fapi2::putScom(l_tpchiplet, 0x00000002, l_data64));

    FAPI_DBG("Assert sram_enable");
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    l_data64.setBit<P9N2_PERV_PERV_CTRL0_TP_TCPERV_SRAM_ENABLE_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));

    FAPI_DBG("Setup hang counters for Perv chiplet");
    FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_tpchiplet, false, BASE_ADDRESS, PRE_DIVIDER,
             SETUP_HANG_COUNTERS_PERV));

    FAPI_INF("p10_sbe_tp_chiplet_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
