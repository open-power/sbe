/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_dpll_bypass.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
/// @file  p10_sbe_tp_dpll_bypass.C
///
/// @brief Stop PAU DPLL clocks and switch to bypass path
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
// EKB-Mirror-To: hw/ppe
//------------------------------------------------------------------------------

#include "p10_sbe_tp_dpll_bypass.H"
#include "p10_scom_proc_4.H"
#include "p10_scom_perv_8.H"
#include "p10_scom_perv_b.H"
#include "p10_scom_perv_f.H"
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>

enum P10_SBE_TP_DPLL_BYPASS_Private_Constants
{
    STOP_CMD = 0x2,
    REGIONS_PAU_DPLL = 0x0040,
    CLOCK_TYPES_ALL = 0x7
};

fapi2::ReturnCode p10_sbe_tp_dpll_bypass(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;

    fapi2::buffer<uint64_t> l_sl_clock_status, l_temp_rc4, l_data64;
    fapi2::buffer<uint16_t> l_attr_spi_bus_div_ref;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_DBG("p10_sbe_tp_dpll_bypass : Entering ...");

    FAPI_DBG("Check that dpllpau clock region is running");
    FAPI_TRY(fapi2::getScom(i_target_chip, proc::TP_TPCHIP_TPC_CLOCK_STAT_SL, l_sl_clock_status));

    if(! l_sl_clock_status.getBit<proc::TP_TPCHIP_TPC_CLOCK_STAT_SL_UNIT8_SL>())
    {
        FAPI_TRY(fapi2::getScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL4_RW, l_temp_rc4));

        FAPI_DBG("Switch glitchless mux2a to PAU DPLL bypass path");
        l_data64.flush<0>().setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL4_TP_MUX2A_CLKIN_SEL_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL4_SET_WO_OR, l_data64));

        FAPI_DBG("PAU DPLL: Put back test_enable and bypass");
        // RC3 bits 25,26
        l_data64.flush<0>()
        .setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_BYPASS_EN_DC>()
        .setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_TEST_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR, l_data64));

        // PAU DPLL stopclocks
        FAPI_DBG("Stopclocks for PAU DPLL regions");
        FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, STOP_CMD, 0, 0,  REGIONS_PAU_DPLL,
                 CLOCK_TYPES_ALL));


        FAPI_DBG("PAU DPLL: Put back into reset");
        // RC3 bit 24
        l_data64.flush<0>()
        .setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_RESET_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR, l_data64));

        FAPI_DBG("PAU DPLL: Switch to external clock");
        // RC3 bit 27
        l_data64.flush<0>()
        .setBit<perv::FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PAU_DPLL_FUNC_CLKSEL_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR, l_data64));

        FAPI_DBG("Restore value of ROOT_CTRL4 from temp variable");
        FAPI_TRY(fapi2::putScom(i_target_chip, perv::FSXCOMP_FSXLOG_ROOT_CTRL4_RW, l_temp_rc4));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SPI_BUS_DIV_REF, i_target_chip, l_attr_spi_bus_div_ref));

        FAPI_DBG("Restore bit rate divisor and delay into all SPI masters");

        for (uint32_t l_addr = 0x000C0003; l_addr <= 0x000C0083; l_addr += 0x20)
        {
            FAPI_TRY(fapi2::getScom(i_target_chip, l_addr, l_data64));
            l_data64.insert< 0, 12, 0 >(l_attr_spi_bus_div_ref);
            l_data64.insertFromRight< 12, 8 >(0x80 >> l_attr_spi_bus_div_ref.getBits< 12, 4>());
            FAPI_TRY(fapi2::putScom(i_target_chip, l_addr, l_data64));
        }
    }


    FAPI_DBG("p10_sbe_tp_dpll_bypass : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
