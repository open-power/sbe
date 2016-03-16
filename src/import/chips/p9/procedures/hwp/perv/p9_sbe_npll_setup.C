/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_npll_setup.C $ */
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
/// @file  p9_sbe_npll_setup.C
///
/// @brief scan initialize level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_npll_setup.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


enum P9_SBE_NPLL_SETUP_Private_Constants
{
    NS_DELAY = 100000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000 // unit is sim cycles
};

fapi2::ReturnCode p9_sbe_npll_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;
    uint8_t l_read_attr = 0;
    fapi2::buffer<uint64_t> l_data64_root_ctrl8;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("Entering ...");

    FAPI_DBG("Drop PLL test enable for Spread Spectrum PLL");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    //PIB.ROOT_CTRL8.TP_SS0_PLL_TEST_EN = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_SS0_PLL_TEST_EN>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_DBG("Drop PLL test enable for CP Filter PLL");
    //Setting ROOT_CTRL8 register value
    //PIB.ROOT_CTRL8.TP_FILT1_PLL_TEST_EN = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT1_PLL_TEST_EN>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_DBG("Drop PLL test enable for IO Filter PLL");
    //Setting ROOT_CTRL8 register value
    //PIB.ROOT_CTRL8.TP_FILT0_PLL_TEST_EN = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT0_PLL_TEST_EN>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_DBG("Drop PLL test enable for Nest PLL");
    //Setting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    //PIB.PERV_CTRL0.TP_PLL_TEST_EN_DC = 0
    l_data64.clearBit<PERV_PERV_CTRL0_SET_TP_PLL_TEST_EN_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));

    FAPI_DBG("Release SS PLL reset");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    //PIB.ROOT_CTRL8.TP_SS0_PLL_RESET = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_SS0_PLL_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_DBG("check SS PLL lock");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

    FAPI_ASSERT(l_read_reg.getBit<0>(),
                fapi2::SS_PLL_LOCK_ERR()
                .set_SS_PLL_READ(l_read_reg),
                "ERROR:SS PLL LOCK NOT SET");

    FAPI_DBG("Release SS PLL Bypass");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    //PIB.ROOT_CTRL8.TP_SS0_PLL_BYPASS = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_SS0_PLL_BYPASS>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_DBG("Release CP Filter PLL reset");
    //Setting ROOT_CTRL8 register value
    //PIB.ROOT_CTRL8.TP_FILT1_PLL_RESET = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT1_PLL_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_DBG("Release IO Filter PLL reset");
    //Setting ROOT_CTRL8 register value
    //PIB.ROOT_CTRL8.TP_FILT0_PLL_RESET = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT0_PLL_RESET>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_DBG("check  PLL lock for CP Filter PLL , Check PLL lock fir IO Filter PLL");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

    FAPI_ASSERT(l_read_reg.getBit<1>() && l_read_reg.getBit<2>(),
                fapi2::FILTER_PLL_LOCK_ERR()
                .set_FILTER_PLL_READ(l_read_reg),
                "ERROR:CP or IO FILTER PLL LOCK NOT SET");

    FAPI_DBG("Release CP filetr and IO filter PLL Bypass Signals");
    //Setting ROOT_CTRL8 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));
    //PIB.ROOT_CTRL8.TP_FILT1_PLL_BYPASS = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT1_PLL_BYPASS>();
    //PIB.ROOT_CTRL8.TP_FILT0_PLL_BYPASS = 0
    l_data64_root_ctrl8.clearBit<PERV_ROOT_CTRL8_SET_TP_FILT0_PLL_BYPASS>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                            l_data64_root_ctrl8));

    FAPI_DBG("Switch MC meshs to Nest mesh");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    if ( l_read_attr == 1 )
    {
        FAPI_DBG("Set MUX to Nest Clock input");
        //Setting ROOT_CTRL8 register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
        //PIB.ROOT_CTRL8.TP_PLL_CLKIN_SEL4_DC = 1
        l_data64_root_ctrl8.setBit<PERV_ROOT_CTRL8_SET_TP_PLL_CLKIN_SEL4_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL8_SCOM,
                                l_data64_root_ctrl8));
    }

    FAPI_DBG("Release Nest PLL  reset");
    //Setting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    //PIB.PERV_CTRL0.TP_PLLRST_DC = 0
    l_data64.clearBit<PERV_PERV_CTRL0_SET_TP_PLLRST_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));

    fapi2::delay(NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_DBG("check  NEST PLL lock");
    //Getting PLL_LOCK_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_PLL_LOCK_REG,
                            l_read_reg)); //l_read_reg = PERV.PLL_LOCK_REG

    FAPI_ASSERT(l_read_reg.getBit<3>(),
                fapi2::NEST_PLL_ERR()
                .set_NEST_PLL_READ(l_read_reg),
                "ERROR:NEST PLL LOCK NOT SET");

    FAPI_DBG("Release PLL bypass2");
    //Setting PERV_CTRL0 register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    //PIB.PERV_CTRL0.TP_PLLBYP_DC = 0
    l_data64.clearBit<PERV_PERV_CTRL0_SET_TP_PLLBYP_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
