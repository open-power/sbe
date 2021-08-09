/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_switch_gears.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
#include "p10_perv_sbe_cmn.H"
#include "p10_scom_proc.H"
#include "p10_scom_perv.H"

const uint64_t SECURITY_SWITCH_REG_ADDR = 0x00010005; // Security Switch Register Address
#define TPM_DECONFIG_BIT 12

enum P10_SBE_TP_SWITCH_GEARS_Private_Constants
{
    SCAN_RATIO_4TO1           = 0x3,
    SPI_RECEIVE_DELAY_ENCODED = 1,  // Might need to be adapted once we get some lab experience
    SPI_RECEIVE_DELAY_DECODED = 0x80 >> SPI_RECEIVE_DELAY_ENCODED,
    SBE_LFR                   = 0x000C0002040,
    GLSMUX_DELAY_NS           = 100,
    GLSMUX_DELAY_CYCLES       = 1000,
};

static fapi2::ReturnCode switch_nest_to_2to1(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64, l_dpll_bypass, l_mux345_reset, l_mux3_select, l_mux4a_select;
    bool l_mux3_prev_state;

    l_dpll_bypass .flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_NEST_DPLL_BYPASS_EN_DC>();
    l_mux345_reset.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL4_TP_AN_CLKGLM_NEST_ASYNC_RESET_DC>().setBit<31>();
    l_mux3_select .flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL4_TP_MUX3_CLKIN_SEL_DC>();
    l_mux4a_select.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL4_TP_MUX4A_CLKIN_SEL_DC>();

    // Some comments to explain how this workaround works:
    //
    // Switching MUX4A from 1:1 to 2:1 might dealign the PCB because there are multiple copies of
    // that mux, and the select signal is untimed, so individual copies might switch at different times,
    // causing different sections of the PCB to receive different amounts of clock cycles, losing alignment.
    //
    // The solution is to turn off the nest mesh before we flip MUX4A. We have two options on the signal
    // path before it fans out into a clock tree: We can put MUX3 into reset, or we can take the Nest DPLL
    // (which is still turned off at this point) out of bypass. Unfortunately MUX3 will glitch when we
    // put it into reset, while the DPLL will glitch when we put it back into bypass, so we have to use
    // both and interleave the steps:

    // First make sure that mux3 is set to the nest DPLL (but save the original setting)
    FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_RW, l_data64));
    l_mux3_prev_state = l_data64.getBit<FSXCOMP_FSXLOG_ROOT_CTRL4_TP_MUX3_CLKIN_SEL_DC>();

    if (l_mux3_prev_state)
    {
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_CLEAR_WO_CLEAR, l_mux3_select));
        fapi2::delay(GLSMUX_DELAY_NS, GLSMUX_DELAY_CYCLES); // Give the mux some time to switch
    }

    // Then bring the DPLL out of bypass without turning it on. This will glitchlessly disable its output.
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR, l_dpll_bypass));

    // Then put MUX3 into reset. This would glitch, but its input clock is already inactive so we're safe.
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_SET_WO_OR, l_mux345_reset));

    // Now put the DPLL back into bypass, turning its output back on. This will glitch, but since we
    // disabled MUX3 the glitch will not reach the mesh.
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR, l_dpll_bypass));

    // While MUX3 is reset, flip it back to its original setting.
    if (l_mux3_prev_state)
    {
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_SET_WO_OR, l_mux3_select));
        fapi2::delay(GLSMUX_DELAY_NS, GLSMUX_DELAY_CYCLES); // Give the mux some time to switch
    }

    // By this time the mesh is inactive, and we can proceed to flipping MUX4A without danger of
    // breaking PCB alignment.
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_CLEAR_WO_CLEAR, l_mux4a_select));

    // And finally, release MUX3 reset to (glitchlessly) turn the mesh back on, now at half speed.
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_CLEAR_WO_CLEAR, l_mux345_reset));

fapi_try_exit:
    return fapi2::current_err;
}

fapi2::ReturnCode p10_sbe_tp_switch_gears(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;

    fapi2::buffer<uint64_t> l_data64, l_read_reg, l_tpmDeconfigData;
    fapi2::buffer<uint64_t> l_opcg_align;
    fapi2::buffer<uint32_t> l_attr_freq_pau_mhz;
    unsigned int sck_clock_divider, tpm_sck_clock_divider;
    uint8_t l_attr_dpll_bypass;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint32_t tpm_spi_clock_delay = 7; //Hard-coded TPM SPI Delay for cronus

    FAPI_INF("p10_sbe_tp_switch_gears: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_dpll_bypass));

    FAPI_DBG("Switch Nest/Cache mesh from 1:1 to 2:1");
    FAPI_TRY(switch_nest_to_2to1(i_target_chip));

    if (! (l_attr_dpll_bypass))
    {
        FAPI_DBG("Program bit rate divisor and delay into multiple SPI masters :1.MEAS 2.BOOT0 3.BOOT1 4.MVPD/Keystore 5.TPM");

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_freq_pau_mhz));
        sck_clock_divider = ((l_attr_freq_pau_mhz / 40) - 1 ); // freq = pau_freq/4*2*(divider + 1)
        FAPI_DBG("sck clock divider calculation for seeprom SPIs: %#018lX", sck_clock_divider);

        // Hardcoded for Cronus Use-Case
        tpm_sck_clock_divider = ((l_attr_freq_pau_mhz / 192) - 1); // 24MHz = ((PAU/4)/2(N+1))
        FAPI_DBG("sck clock divider calculation for TPM SPI: %#018lX", tpm_sck_clock_divider);

        if (fapi2::is_platform<fapi2::PLAT_SBE>())
        {
            // Need to store the Clock Divider and receive delay in LFR 0xc0002040 Reg (bit0-11, 20-23),
            // so that it can be retrieved and used in case of HReset and Mpipl flow
            // (In HReset & Mpipl, we won't execute this procedure and will be
            // resetting the SPIms)
            // Not storing the TPM SPI clock divider above since we don't have free bits in LFR
            // We will have to calculate the TPM SPI clock divider everytime if we need it from
            // ((PAU/freq_in_mhz * 8) - 1)
            uint32_t tpm_spi_freq_mhz = 0;
            tpm_spi_clock_delay = 0;
            FAPI_DBG("Update clock divider and receive delay in SBE LFR register");
            FAPI_TRY(fapi2::getScom(i_target_chip, SBE_LFR, l_data64));
            // Fetch TPM Frequency from LFR from the dial
            l_data64.extractToRight< 24, 8 >(tpm_spi_freq_mhz);
            l_data64.extractToRight< 32, 4 >(tpm_spi_clock_delay);
            tpm_sck_clock_divider =  ((l_attr_freq_pau_mhz / (tpm_spi_freq_mhz * 8)) - 1);
            FAPI_INF("TPM SPI Clock Freq [0x%04X] TPM SPI Divider [0x%04X] TPM SPI Delay[0x%04X]",
                     tpm_spi_freq_mhz, tpm_sck_clock_divider, tpm_spi_clock_delay);
            l_data64.insertFromRight< 0, 12 >(sck_clock_divider);
            l_data64.insertFromRight< 20, 4 >(SPI_RECEIVE_DELAY_ENCODED);
            l_data64.insertFromRight< 48, 16 >(l_attr_freq_pau_mhz); //Save this to be used in HReset/Mpipl Reset
            FAPI_TRY(fapi2::putScom(i_target_chip, SBE_LFR, l_data64));
        }

        FAPI_DBG("Update clock divider and receive delay in SPI masters");

        // Update the Seeprom SPIs separately since they have different clock divider
        // setting, which is to get 5MHz Spi clock.
        for (uint32_t l_addr = 0x000C0003; l_addr <= 0x000C0063; l_addr += 0x20)
        {
            FAPI_TRY(fapi2::getScom(i_target_chip, l_addr, l_data64));
            l_data64.insertFromRight< 0, 12 >(sck_clock_divider);
            l_data64.insertFromRight< 12, 8 >(SPI_RECEIVE_DELAY_DECODED);
            FAPI_TRY(fapi2::putScom(i_target_chip, l_addr, l_data64));
        }

        // Update to TPM SPI separately since it needs a different clock divider
        // To be calculated on the fly from PAU and update into c0083, this is a
        // common clock divider to be used across the system to keep the TPM SPI
        // clock at 24MHz
        FAPI_DBG("Read the TPM deconfig Bit");
        FAPI_TRY(fapi2::getScom(i_target_chip, SECURITY_SWITCH_REG_ADDR, l_tpmDeconfigData));

        if(!l_tpmDeconfigData.getBit<TPM_DECONFIG_BIT>())
        {
            uint32_t l_addr = 0x000C0083;
            FAPI_TRY(fapi2::getScom(i_target_chip, l_addr, l_data64));
            l_data64.insertFromRight< 0, 12 >(tpm_sck_clock_divider);
            l_data64.insertFromRight< 12, 8 >((0x80 >> tpm_spi_clock_delay));
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
