/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_switch_gears.C $ */
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
/// @file  p9_sbe_tp_switch_gears.C
///
/// @brief Switch from refclock to PLL AND adjust I2C
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumarj8@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_switch_gears.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_sbe_gear_switcher.H>


enum P9_SBE_TP_SWITCH_GEARS_Private_Constants
{
    BACKUP_SEEPROM_MAGIC_NUM_ADDRESS = 0xD8A9029000000000, // Magic number value from Backup SEEPROM
    BUS_STATUS_BUSY_POLL_COUNT = 256,
    MAGIC_NUMBER = 0x584950205345504D,
    NORMAL_SEEPROM_MAGIC_NUM_ADDRESS = 0xD8A9009000000000 // Magic number value from SEEPROM
};

fapi2::ReturnCode p9_sbe_tp_switch_gears(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_tp_switch_gears: Entering ...");

#ifdef __PPE__
    uint8_t l_nest_bypass = 0;
    // - if we've just switched from refclock->PLL, we need to adjust the
    // i2c bit rate divisor to account for the new frequency (plus
    // check our ability to access the seeprom with this setting)
    // - if no frequency change has occurred (nest PLL in bypass),
    // skip all of these steps as the current divisor in place is appropriate
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_MEM_X_O_PCI_BYPASS, i_target_chip, l_nest_bypass));

    if (l_nest_bypass == 0)
    {
        FAPI_TRY(p9_sbe_gear_switcher_apply_i2c_bit_rate_divisor_setting(i_target_chip));
        FAPI_TRY(p9_sbe_gear_switcher_i2c_stop_sequence(i_target_chip));
        FAPI_DBG("Checking Magic number");
        FAPI_TRY(p9_sbe_tp_switch_gears_check_magicnumber(i_target_chip));
    }

fapi_try_exit:
#endif

    FAPI_INF("p9_sbe_tp_switch_gears: Exiting ...");

    return fapi2::current_err;

}

/// @brief check for magic number
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_sbe_tp_switch_gears_check_magicnumber(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::buffer<uint8_t> l_read_attr = 0;
    int l_timeout = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_tp_switch_gears_check_magicnumber: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKUP_SEEPROM_SELECT, i_target_chip,
                           l_read_attr));

    if ( l_read_attr.getBit<7>() == 1 )
    {
        FAPI_DBG("Read magic number from Backup SEEPROM");
        //Setting CONTROL_REGISTER_B register value
        //PIB.CONTROL_REGISTER_B = BACKUP_SEEPROM_MAGIC_NUM_ADDRESS
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_CONTROL_REGISTER_B,
                                BACKUP_SEEPROM_MAGIC_NUM_ADDRESS));
    }
    else
    {
        FAPI_DBG("Read magic number from SEEPROM");
        //Setting CONTROL_REGISTER_B register value
        //PIB.CONTROL_REGISTER_B = NORMAL_SEEPROM_MAGIC_NUM_ADDRESS
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_CONTROL_REGISTER_B,
                                NORMAL_SEEPROM_MAGIC_NUM_ADDRESS));
    }

    FAPI_DBG("Poll for stop command completion");
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

    FAPI_DBG("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::BUS_STATUS_BUSY0(),
                "ERROR:BUS_STSTUS_BUSY_0 NOT SET TO 0");

    FAPI_DBG("Reading the value of DATA0TO7_REGISTER_B");
    //Getting DATA0TO7_REGISTER_B register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PU_DATA0TO7_REGISTER_B,
                            l_read_reg)); //l_read_reg = PIB.DATA0TO7_REGISTER_B

    FAPI_ASSERT(l_read_reg == MAGIC_NUMBER,
                fapi2::MAGIC_NUMBER_NOT_VALID(),
                "ERROR: Magic number not matching");

    FAPI_INF("p9_sbe_tp_switch_gears_check_magicnumber: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
