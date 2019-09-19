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
#include "p10_scom_proc_6.H"
#include "p10_scom_perv_7.H"

#define SEEPROM_START 0xFF800000

enum P10_SBE_TP_SWITCH_GEARS_Private_Constants
{
    MAGIC_NUMBER = 0x584950205345504D,
    SCAN_RATIO_4TO1 = 0x3
};

fapi2::ReturnCode p10_sbe_tp_switch_gears(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;

    fapi2::buffer<uint64_t> l_data64, l_read_reg;
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

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x000C0003, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x000C0003, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x000C0023, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x000C0023, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x000C0043, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x000C0043, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x000C0063, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x000C0063, l_data64));

        FAPI_TRY(fapi2::getScom(i_target_chip, 0x000C0083, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, 0x000C0083, l_data64));

        FAPI_DBG("Write new clock divider / delay value back to scratch reg");
        FAPI_TRY(fapi2::getScom(i_target_chip, perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_2_RW, l_data64));
        l_data64.insert< 0, 12, 4 >(sck_clock_divider);
        l_data64.insertFromRight< 12, 4 >(0x4);
        FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_SCRATCH_REGISTER_2_RW, l_data64));

#ifdef __PPE__
        FAPI_DBG("read magic number from seeprom, compare value");
        // To read and check magic number
        FAPI_TRY(p10_sbe_tp_switch_gears_check_magicnumber(i_target_chip));
#endif

        // adjust scan ratio
        FAPI_DBG("Adjust scan rate to 4:1");
        FAPI_TRY(fapi2::getScom(i_target_chip, proc::TP_TPCHIP_TPC_OPCG_ALIGN, l_opcg_align),
                 "Error from getScom (PERV_TP_OPCG_ALIGN)");

        l_opcg_align.insertFromRight<proc::TP_TPCHIP_TPC_OPCG_ALIGN_SCAN_RATIO, proc::TP_TPCHIP_TPC_OPCG_ALIGN_SCAN_RATIO_LEN>
        (SCAN_RATIO_4TO1);

        FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_OPCG_ALIGN, l_opcg_align),
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

    FAPI_INF("p10_sbe_tp_switch_gears_check_magicnumber: Entering ...");

    // Read SEEPROM start address FF800000 for magic number
    l_read_reg = *reinterpret_cast<volatile uint64_t*>(SEEPROM_START);

    FAPI_ASSERT(l_read_reg == MAGIC_NUMBER,
                fapi2::MAGIC_NUMBER_NOT_VALID()
                .set_SEEPROM_START_ADDR(l_read_reg)
                .set_MAGIC_NUMBER_VALUE(MAGIC_NUMBER),
                "ERROR: Magic number not matching");

    FAPI_INF("p10_sbe_tp_switch_gears_check_magicnumber: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
