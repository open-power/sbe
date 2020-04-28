/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_switch_gears.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
#include "p10_scom_perv_4.H"
#include "p10_scom_perv_7.H"
#include "p10_scom_perv_b.H"

enum P10_SBE_TP_SWITCH_GEARS_Private_Constants
{
    SCAN_RATIO_4TO1           = 0x3,
    SPI_RECEIVE_DELAY_ENCODED = 1,  // Might need to be adapted once we get some lab experience
    SPI_RECEIVE_DELAY_DECODED = 0x80 >> SPI_RECEIVE_DELAY_ENCODED,
    SBE_LFR                   = 0x000C0002040,
};

fapi2::ReturnCode p10_sbe_tp_switch_gears(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;

    fapi2::buffer<uint64_t> l_data64, l_read_reg;
    fapi2::buffer<uint64_t> l_opcg_align;
    fapi2::buffer<uint32_t> l_attr_freq_pau_mhz;
    unsigned int sck_clock_divider;
    uint8_t l_attr_dpll_bypass;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    FAPI_INF("p10_sbe_tp_switch_gears: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_dpll_bypass));

    FAPI_DBG("Switch Nest/Cache mesh from 1:1 to 2:1");
    l_data64.flush<0>().setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL4_TP_MUX4A_CLKIN_SEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL4_CLEAR_WO_CLEAR, l_data64));

    if (! (l_attr_dpll_bypass))
    {
        FAPI_DBG("Program bit rate divisor and delay into multiple SPI masters :1.MEAS 2.BOOT0 3.BOOT1 4.MVPD/Keystore 5.TPM");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_freq_pau_mhz));
        sck_clock_divider = ((l_attr_freq_pau_mhz / 40) - 1 );
        FAPI_DBG("sck clock divider calculation : %#018lX", sck_clock_divider);

        if (fapi2::is_platform<fapi2::PLAT_SBE>())
        {
            // Need to store the Clock Divider and receive delay in LFR 0xc0002040 Reg (bit0-11, 20-23),
            // so that it can be retrieved and used in case of HReset and Mpipl flow
            // (In HReset & Mpipl, we won't execute this procedure and will be
            // resetting the SPIms)
            FAPI_DBG("Update clock divider and receive delay in SBE LFR register");
            FAPI_TRY(fapi2::getScom(i_target_chip, SBE_LFR, l_data64));
            l_data64.insertFromRight< 0, 12 >(sck_clock_divider);
            l_data64.insertFromRight< 20, 4 >(SPI_RECEIVE_DELAY_ENCODED);
            FAPI_TRY(fapi2::putScom(i_target_chip, SBE_LFR, l_data64));
        }

        FAPI_DBG("Update clock divider and receive delay in SPI masters");

        for (uint32_t l_addr = 0x000C0003; l_addr <= 0x000C0083; l_addr += 0x20)
        {
            FAPI_TRY(fapi2::getScom(i_target_chip, l_addr, l_data64));
            l_data64.insertFromRight< 0, 12 >(sck_clock_divider);
            l_data64.insertFromRight< 12, 8 >(SPI_RECEIVE_DELAY_DECODED);
            FAPI_TRY(fapi2::putScom(i_target_chip, l_addr, l_data64));
        }

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
