/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/perv/p9_sbe_tp_arrayinit.C $   */
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
//## auto_generated
#include "p9_const_common.H"

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
    SELECT_EDRAM = 0x0,
    PIBMEM_EXCLUDE_ABIST = 0xC000000000000000,
    PIBMEM_INCLUDE_ABIST = 0x8000000000000000
};

static fapi2::ReturnCode p9_sbe_tp_arrayinit_sdisn_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint8_t> i_attr,
    const bool i_set);

fapi2::ReturnCode p9_sbe_tp_arrayinit(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint8_t> l_attr_read;

    FAPI_INF("p9_sbe_tp_arrayinit: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SDISN_SETUP, i_target_chip, l_attr_read));

    FAPI_DBG("Exclude PIBMEM from TP array init");
    //Setting PIBMEM_REPAIR_REGISTER_0 register value
    //PIB.PIBMEM_REPAIR_REGISTER_0 = 0xC000000000000000
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_PIBMEM_REPAIR_REGISTER_0, PIBMEM_EXCLUDE_ABIST ));

    FAPI_DBG("set sdis_n");
    FAPI_TRY(p9_sbe_tp_arrayinit_sdisn_setup(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_attr_read, true));

    FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], REGIONS_EXCEPT_PIB_NET_PLL, l_regions));
    FAPI_DBG("l_regions value: %#018lX", l_regions);

    FAPI_DBG("Call ARRAY INIT Module for Pervasive Chiplet");
    FAPI_TRY(p9_perv_sbe_cmn_array_init_module(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_regions, LOOP_COUNTER, SELECT_SRAM,
                 SELECT_EDRAM, START_ABIST_MATCH_VALUE));

    FAPI_DBG("Call SCAN0 Module for Pervasive Chiplet");
    FAPI_TRY(p9_perv_sbe_cmn_scan0_module(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_regions, SCAN_TYPES));

    FAPI_DBG("clear sdis_n");
    FAPI_TRY(p9_sbe_tp_arrayinit_sdisn_setup(
                 i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP,
                         fapi2::TARGET_STATE_FUNCTIONAL)[0], l_attr_read, false));

    FAPI_DBG("Add PIBMEM back to TP array init");
    //Setting PIBMEM_REPAIR_REGISTER_0 register value
    //PIB.PIBMEM_REPAIR_REGISTER_0 = 0x8000000000000000
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_PIBMEM_REPAIR_REGISTER_0, PIBMEM_INCLUDE_ABIST));

    FAPI_INF("p9_sbe_tp_arrayinit: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Sdis_n set or clear
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_attr          Attribute to decide to sdis_n setup
/// @param[in]     i_set           set or clear the LCBES condition
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_tp_arrayinit_sdisn_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint8_t> i_attr,
    const bool i_set)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_tp_arrayinit_sdisn_setup: Entering ...");

    if ( i_attr )
    {
        if ( i_set )
        {
            //Setting CPLT_CONF0 register value
            l_data64.flush<0>();
            //CPLT_CONF0.CTRL_CC_SDIS_DC_N = 1
            l_data64.setBit<PERV_1_CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF0_OR, l_data64));
        }
        else
        {
            //Setting CPLT_CONF0 register value
            l_data64.flush<0>();
            //CPLT_CONF0.CTRL_CC_SDIS_DC_N = 0
            l_data64.setBit<PERV_1_CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_CPLT_CONF0_CLEAR, l_data64));
        }
    }

    FAPI_INF("p9_sbe_tp_arrayinit_sdisn_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
