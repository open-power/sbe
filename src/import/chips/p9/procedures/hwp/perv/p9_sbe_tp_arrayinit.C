/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_arrayinit.C $ */
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
/// @file  p9_sbe_tp_arrayinit.C
///
/// @brief SBE PRV Array Init Procedure
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_arrayinit.H"

#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>


enum P9_SBE_TP_ARRAYINIT_Private_Constants
{
    REGIONS_EXCEPT_PIB_NET_PLL = 0x4FE,
    SCAN_TYPES = 0xDCF,
    LOOP_COUNTER = 0x0000000000042FFF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000,
    SELECT_SRAM = 0x1,
    SELECT_EDRAM = 0x0
};

fapi2::ReturnCode p9_sbe_tp_arrayinit(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    bool l_sram_abist_check = false;
    fapi2::buffer<uint16_t> l_regions;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                fapi2::TARGET_STATE_FUNCTIONAL)[0];
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Exclude PIBMEM from TP array init");
    //Setting PIBMEM_REPAIR_REGISTER_0 register value
    //PIB.PIBMEM_REPAIR_REGISTER_0 = 0x0000000000000001
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_PIBMEM_REPAIR_REGISTER_0,
                            0x0000000000000001));

    // Step 1: Array Init for PRV Cplt
    // ===============================

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(l_tpchiplet,
             REGIONS_EXCEPT_PIB_NET_PLL, l_regions));
    FAPI_DBG("l_regions value: %#018lX", l_regions);

    FAPI_DBG("Call ARRAY INIT Module for Pervasive Chiplet");
    FAPI_TRY(p9_perv_sbe_cmn_array_init_module(l_tpchiplet, l_regions, LOOP_COUNTER,
             SELECT_SRAM, SELECT_EDRAM, START_ABIST_MATCH_VALUE));

    FAPI_DBG("Check SRAM Abist Done");
    //Getting CPLT_STAT0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_CPLT_STAT0, l_data64));
    //l_sram_abist_check = PERV.CPLT_STAT0.SRAM_ABIST_DONE_DC
    l_sram_abist_check = l_data64.getBit<PERV_1_CPLT_STAT0_SRAM_ABIST_DONE_DC>();

    FAPI_ASSERT(l_sram_abist_check ,
                fapi2::SRAM_ABIST_DONE_BIT_ERR()
                .set_READ_ABIST_DONE(l_sram_abist_check),
                "ERROR:ABIST_DONE_BIT_NOT_SET");

    // Step 2: Scan0 for PRV Cplt
    // ==========================

    FAPI_DBG("Call SCAN0 Module for Pervasive Chiplet");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_tpchiplet, l_regions, SCAN_TYPES));

    FAPI_DBG("Add PIBMEM back to TP array init");
    //Setting PIBMEM_REPAIR_REGISTER_0 register value
    //PIB.PIBMEM_REPAIR_REGISTER_0 = 0x0
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_PIBMEM_REPAIR_REGISTER_0, 0x0));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
