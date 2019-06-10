/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_gear_switcher.C $ */
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
/// @file  p9_sbe_gear_switcher.C
///
/// @brief Modules for I2C Bit rate divisor setting
/// And stop sequence on I2C
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#include "p9_sbe_gear_switcher.H"
#include "p9_frequency_buckets.H"
#include "p9_misc_scom_addresses.H"
#include "p9_perv_scom_addresses.H"


enum P9_SBE_GEAR_SWITCHER_Private_Constants
{
    DEFAULT_MB_BIT_RATE_DIVISOR = 0x00000000,
    BUS_STATUS_BUSY_POLL_COUNT = 64
};

/// @brief --adjust I2C bit rate divisor setting in I2CM B mode reg
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_sbe_gear_switcher_apply_i2c_bit_rate_divisor_setting(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_attr_nest_pll_bucket = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_CHIP_EC_FEATURE_I2CM_INTERNAL_CLK_DIV2_Type l_i2cm_internal_clk_div2 = 0;
    fapi2::buffer<uint16_t> l_mb_bit_rate_divisor;
    fapi2::buffer<uint64_t> l_data64;

    FAPI_DBG("Entering ...");

    FAPI_DBG("Reading ATTR_NEST_PLL_BUCKET");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM,
                           l_attr_nest_pll_bucket));
    FAPI_DBG("ATTR_NEST_PLL_BUCKET value: %d", l_attr_nest_pll_bucket);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_I2CM_INTERNAL_CLK_DIV2, i_target_chip, l_i2cm_internal_clk_div2));

    l_mb_bit_rate_divisor = NEST_PLL_FREQ_I2CDIV_LIST[l_attr_nest_pll_bucket - 1] *
                            ((l_i2cm_internal_clk_div2) ? (2) : (1));

    FAPI_DBG("bit_rate_divisor value: %d", l_mb_bit_rate_divisor);

    FAPI_DBG("Adjust I2C bit rate divisor setting in I2CM B Mode Reg");
    FAPI_TRY(fapi2::getScom(i_target_chip, PU_MODE_REGISTER_B, l_data64));
    l_data64.insertFromRight< 0, 16 >(l_mb_bit_rate_divisor);
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_MODE_REGISTER_B, l_data64));

    FAPI_DBG("Writing I2C bit rate divisor into mailbox_reg_2");
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM, l_data64));
    l_data64.insertFromRight< 0, 16 >(l_mb_bit_rate_divisor);
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --send a stop sequence on I2C
/// --poll for stop command completion
/// --check for magic number
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_sbe_gear_switcher_i2c_stop_sequence(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    FAPI_DBG("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKUP_SEEPROM_SELECT, i_target_chip,
                           l_read_attr));

    // WRITE Control register
    // enable enhance mode
    // Point to port_0 where the Primary SEEPROM Sits
    FAPI_INF("Send a STOP sequence on I2C");
    l_data64.flush<0>();
    l_data64.setBit<3>();  //PIB.CONTROL_REGISTER_B.PIB_CNTR_REG_BIT_WITHSTOP_0 = 1
    //PIB.CONTROL_REGISTER_B.PIB_CNTR_REG_PORT_NUMBER_0 = l_read_attr
    l_data64.insertFromRight<18, 5>(l_read_attr);
    l_data64.setBit<26>();  //PIB.CONTROL_REGISTER_B.ENH_MODE_0 = 1
    FAPI_TRY(fapi2::putScom(i_target_chip, PU_CONTROL_REGISTER_B, l_data64));

    FAPI_INF("Poll for stop command completion");
    l_timeout = BUS_STATUS_BUSY_POLL_COUNT;

    //UNTIL STATUS_REGISTER_B.BUS_STATUS_BUSY_0 == 0
    while (l_timeout != 0)
    {
        //Getting STATUS_REGISTER_B register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PU_STATUS_REGISTER_B, l_data64));
        //bool l_poll_data = PIB.STATUS_REGISTER_B.BUS_STATUS_BUSY_0
        bool l_poll_data = l_data64.getBit<44>();

        if (l_poll_data == 0)
        {
            break;
        }

        --l_timeout;
    }

    FAPI_INF("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::BUS_STATUS_BUSY_0()
                .set_TARGET_CHIP(i_target_chip)
                .set_STATUS_REGISTER_B(l_data64)
                .set_LOOP_COUNT(BUS_STATUS_BUSY_POLL_COUNT)
                .set_BACKUP_SEEPROM_ATTR(l_read_attr),
                "ERROR:BUS_STATUS_BUSY_0 NOT SET TO 0");

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
