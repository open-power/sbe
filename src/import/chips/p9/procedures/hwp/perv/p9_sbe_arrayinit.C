/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_arrayinit.C $  */
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
/// @file  p9_sbe_arrayinit.C
///
/// @brief array init procedure to be called with any chiplet target except TP,EP,EC
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_arrayinit.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>


enum P9_SBE_ARRAYINIT_Private_Constants
{
    LOOP_COUNTER = 0x0000000000042FFF,
    REGIONS_EXCEPT_VITAL_AND_PLL = 0x7FE,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    SELECT_EDRAM = 0x0,
    SELECT_SRAM = 0x1,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

static fapi2::ReturnCode p9_sbe_arrayinit_scan0_and_arrayinit_module_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint16_t> i_regions);

static fapi2::ReturnCode p9_sbe_arrayinit_sdisn_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
        const bool i_set);

fapi2::ReturnCode p9_sbe_arrayinit(const
                                   fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    FAPI_INF("p9_sbe_arrayinit: Entering ...");

    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("set sdis_n");
        FAPI_TRY(p9_sbe_arrayinit_sdisn_setup(l_chplt_trgt, true));

        FAPI_DBG("Region setup");
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(l_chplt_trgt,
                 REGIONS_EXCEPT_VITAL_AND_PLL, l_regions));
        FAPI_DBG("l_regions value: %#018lX ", l_regions);

        FAPI_DBG("Call proc_sbe_arryinit_scan0_and_arrayinit_module_function");
        FAPI_TRY(p9_sbe_arrayinit_scan0_and_arrayinit_module_function(l_chplt_trgt,
                 l_regions));

        FAPI_DBG("clear sdis_n");
        FAPI_TRY(p9_sbe_arrayinit_sdisn_setup(l_chplt_trgt, false));
    }

    FAPI_INF("p9_sbe_arrayinit: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --Run arrayinit on all enabled chiplets
/// --Scan flush 0 to all rings except GPTR, Time, Repair on all enabled chiplets
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_regions          region value settings
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_arrayinit_scan0_and_arrayinit_module_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const fapi2::buffer<uint16_t> i_regions)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_arrayinit_scan0_and_arrayinit_module_function: Entering ...");

    FAPI_DBG("Check for chiplet enable");
    //Getting NET_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_NET_CTRL0, l_data64));
    //l_read_reg = NET_CTRL0.CHIPLET_ENABLE
    l_read_reg = l_data64.getBit<PERV_1_NET_CTRL0_CHIPLET_ENABLE>();

    if ( l_read_reg )
    {
        FAPI_DBG("run array_init module for all chiplet except TP, EC, EP");
        FAPI_TRY(p9_perv_sbe_cmn_array_init_module(i_target_chiplet, i_regions,
                 LOOP_COUNTER, SELECT_SRAM, SELECT_EDRAM, START_ABIST_MATCH_VALUE));

        FAPI_DBG("run scan0 module for region except vital and pll, scan types except GPTR TIME REPR all chiplets except TP, EC, EP");
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(i_target_chiplet, i_regions,
                                              SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));
    }

    FAPI_INF("p9_sbe_arrayinit_scan0_and_arrayinit_module_function: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Sdis_n setup
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_set           set or clear the LCBES condition
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_arrayinit_sdisn_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
        const bool i_set)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_arrayinit_sdisn_setup: Entering ...");

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

    FAPI_INF("p9_sbe_arrayinit_sdisn_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
