/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_switch_gears.C $ */
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
/// @file  p10_sbe_tp_switch_gears.C
///
/// @brief Switch from refclock to PLL AND adjust I2C
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_tp_switch_gears.H"
#include "p9_const_common.H"

#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


enum P10_SBE_TP_SWITCH_GEARS_Private_Constants
{
    BACKUP_SEEPROM_MAGIC_NUM_ADDRESS = 0xD8A9029000000000, // Magic number value from Backup SEEPROM
    BUS_STATUS_BUSY_POLL_COUNT = 256,
    MAGIC_NUMBER = 0x584950205345504D,
    NORMAL_SEEPROM_MAGIC_NUM_ADDRESS = 0xD8A9009000000000, // Magic number value from SEEPROM
    SCAN_RATIO_4TO1 = 0x3
};

fapi2::ReturnCode p10_sbe_tp_switch_gears(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_opcg_align;
    fapi2::buffer<uint32_t> l_attr_freq_pau_mhz;
    fapi2::buffer<uint16_t> sck_clock_divider;
    uint8_t l_attr_dpll_bypass;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    FAPI_INF("p10_sbe_tp_switch_gears: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_dpll_bypass));

    if (! (l_attr_dpll_bypass))
    {
        FAPI_DBG("Program bit rate divisor and delay into multiple SPI masters :1.MEAS 2.BOOT0 3.BOOT1 4.MVPD/Keystore 5.TPM");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_freq_pau_mhz));
        sck_clock_divider = ((l_attr_freq_pau_mhz / 40) - 1 );
        FAPI_DBG("sck clock divider calculation : %#018lX", sck_clock_divider);

        FAPI_TRY(fapi2::getScom(i_target_chip, PU_SPIPSS_ADC_STATUS_REG, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_SPIPSS_ADC_STATUS_REG, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, PU_SPIPSS_ADC_RDATA_REG3, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_SPIPSS_ADC_RDATA_REG3, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, PU_SPIPSS_P2S_STATUS_REG, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, PU_SPIPSS_P2S_STATUS_REG, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x00070063, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x00070063, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x00070083, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x00070083, l_data64));

        FAPI_DBG("Write new clock divider / delay value back to scratch reg");
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_SCRATCH_REGISTER_2_SCOM, l_data64));

        FAPI_DBG("read magic number from seeprom, compare value");
        // TODO - to read and check magic number - might change, calling p9 check as it is
        FAPI_TRY(p10_sbe_tp_switch_gears_check_magicnumber(i_target_chip));

        // adjust scan ratio
        FAPI_DBG("Adjust scan rate to 4:1");
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_OPCG_ALIGN, l_opcg_align),
                 "Error from getScom (PERV_TP_OPCG_ALIGN)");

        l_opcg_align.insertFromRight<PERV_1_OPCG_ALIGN_SCAN_RATIO, PERV_1_OPCG_ALIGN_SCAN_RATIO_LEN>(SCAN_RATIO_4TO1);

        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_OPCG_ALIGN, l_opcg_align),
                 "Error from putScom (PERV_TP_OPCG_ALIGN)");
    }

fapi_try_exit:
    FAPI_INF("p10_sbe_tp_switch_gears: Exiting ...");

    return fapi2::current_err;

}

/// @brief check for magic number
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_tp_switch_gears_check_magicnumber(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::buffer<uint8_t> l_read_attr = 0;
    int l_timeout = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_tp_switch_gears_check_magicnumber: Entering ...");

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
                fapi2::I2C_BUS_STATUS_BUSY()
                .set_STATUS_REGISTER_B(l_data64)
                .set_LOOP_COUNT(BUS_STATUS_BUSY_POLL_COUNT)
                .set_BACKUP_SEEPROM_ATTR(l_read_attr),
                "ERROR:BUS_STSTUS_BUSY_0 NOT SET TO 0");

    FAPI_DBG("Reading the value of DATA0TO7_REGISTER_B");
    //Getting DATA0TO7_REGISTER_B register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PU_DATA0TO7_REGISTER_B,
                            l_read_reg)); //l_read_reg = PIB.DATA0TO7_REGISTER_B

    FAPI_ASSERT(l_read_reg == MAGIC_NUMBER,
                fapi2::MAGIC_NUMBER_NOT_VALID()
                .set_DATA0TO7_REGISTER_B(l_read_reg)
                .set_MAGIC_NUMBER_VALUE(MAGIC_NUMBER)
                .set_BACKUP_SEEPROM_ATTR(l_read_attr),
                "ERROR: Magic number not matching");

    FAPI_INF("p10_sbe_tp_switch_gears_check_magicnumber: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
