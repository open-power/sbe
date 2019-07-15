/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_perv_sbe_cmn.C $   */
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
/// @file  p9_perv_sbe_cmn.C
///
/// @brief Modules for scan 0 and array init
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_perv_sbe_cmn.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_quad_scom_addresses_fld.H>
#include <p9_const_common.H>


enum P9_PERV_SBE_CMN_Private_Constants
{
    P9_OPCG_DONE_SCAN0_POLL_COUNT = 200, // Scan0 Poll count
    P9_OPCG_DONE_SCAN0_HW_NS_DELAY = 16000, // unit is nano seconds [min : 8k cycles x 4 = 8000/2 x 4 = 16000 x 10(-9) = 16 us
    //                       max : 8k cycles  =  (8000/25) x 10 (-6) = 320 us]
    P9_OPCG_DONE_SCAN0_SIM_CYCLE_DELAY = 800000, // unit is cycles, to match the poll count change ( 10000 * 8 )
    P9_OPCG_DONE_ARRAYINIT_HW_NS_DELAY = 200000, // unit is nano seconds [min : 400k/2 = 200k ns = 200 us
    //                       max : 200k /25 = 8000 us = 8 ms]
    P9_OPCG_DONE_ARRAYINIT_POLL_COUNT = 400, // Arrayinit Poll count
    P9_OPCG_DONE_ARRAYINIT_SIM_CYCLE_DELAY = 1120000 // unit is cycles,to match the poll count change ( 280000 * 4 )
};

/// @brief Seeprom array Init Module
/// --ABISTCLK_MUXSEL
/// --ABIST modes
/// --Setup BIST regions
/// --Setup all Clock Regions and Types
/// --Setup:
///    - loopcount
///    - OPCG engine start ABIST
///    - run-N mode
/// --Setup IDLE count
/// --OPCG go
/// --Poll OPCG done bit to check for completeness
/// --Clear:
///    - loopcount
///    - OPCG engine start ABIST
///    - run-N mode
/// --Clear all Clock Regions and Types
/// --Clear ABISTCLK_MUXSEL
/// --Clear BIST register
///
///
///
/// @param[in]     i_target_chiplets           Reference to TARGET_TYPE_PERV target Targets all chiplets
/// @param[in]     i_regions                   select clk regions
/// @param[in]     i_loop_counter              loop count value to set opcg run-N mode
/// @param[in]     i_select_sram               select sram abist mode
/// @param[in]     i_select_edram              Set edram abist mode
/// @param[in]     i_start_abist_match_value   match setup idle count value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_perv_sbe_cmn_array_init_module(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplets,
        const fapi2::buffer<uint16_t> i_regions,
        const fapi2::buffer<uint64_t> i_loop_counter,
        const bool i_select_sram,
        const bool i_select_edram,
        const fapi2::buffer<uint64_t> i_start_abist_match_value)
{
    fapi2::buffer<uint16_t> l_scan_count;
    fapi2::buffer<uint16_t> l_misr_a_value;
    fapi2::buffer<uint16_t> l_misr_b_value;
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_read_reg;
    bool l_abist_check = false;
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    fapi2::buffer<uint64_t> l_data64_clk_region;
    FAPI_INF("p9_perv_sbe_cmn_array_init_module: Entering ...");

    i_regions.extractToRight<5, 11>(l_regions);

    FAPI_DBG("Drop vital fence (moved to arrayinit from sacn0 module)");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_VITL_REGION_FENCE = 0
    l_data64.setBit<C_CPLT_CTRL1_TC_VITL_REGION_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("Setup ABISTMUX_SEL");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL0_OR, l_data64));

    FAPI_DBG("setup ABIST modes , BIST REGIONS:%#018lX", i_regions);
    //Setting BIST register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_BIST, l_data64));
    l_data64.clearBit<0>();  //BIST.TC_BIST_START_TEST_DC = 0
    //BIST.TC_SRAM_ABIST_MODE_DC = i_select_sram
    l_data64.writeBit<PERV_1_BIST_TC_SRAM_ABIST_MODE_DC>(i_select_sram);
    //BIST.TC_EDRAM_ABIST_MODE_DC = i_select_edram
    l_data64.writeBit<PERV_1_BIST_TC_EDRAM_ABIST_MODE_DC>(i_select_edram);
    l_data64.insertFromRight<4, 11>(l_regions);  //BIST.BIST_ALL_UNITS = l_regions
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_BIST, l_data64));
    FAPI_DBG("l_data64 value:%#018lX", l_data64);

    FAPI_DBG("Setup all Clock Domains and Clock Types");
    //Setting CLK_REGION register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CLK_REGION,
                            l_data64_clk_region));
    //CLK_REGION.CLOCK_REGION_ALL_UNITS = l_regions
    l_data64_clk_region.insertFromRight<4, 11>(l_regions);
    l_data64_clk_region.setBit<48, 3>();  //CLK_REGION.SEL_THOLD_ALL = 0b111
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION,
                            l_data64_clk_region));

    FAPI_DBG("Drop Region fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_regions
    l_data64.insertFromRight<4, 11>(l_regions);
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("Setup: loopcount , OPCG engine start ABIST, run-N mode");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.setBit<PERV_1_OPCG_REG0_RUNN_MODE>();  //OPCG_REG0.RUNN_MODE = 1
    l_data64.setBit<14>();  //OPCG_REG0.OPCG_STARTS_BIST = 1
    l_data64.insertFromRight<PERV_1_OPCG_REG0_LOOP_COUNT, PERV_1_OPCG_REG0_LOOP_COUNT_LEN>((
                uint64_t)(i_loop_counter));  //OPCG_REG0.LOOP_COUNT = i_loop_counter
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    i_start_abist_match_value.extractToRight<0, 12>(l_scan_count);
    i_start_abist_match_value.extractToRight<12, 12>(l_misr_a_value);
    i_start_abist_match_value.extractToRight<24, 12>(l_misr_b_value);

    FAPI_DBG("Setup IDLE count");
    //Setting OPCG_REG1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG1, l_data64));
    l_data64.insertFromRight<PERV_1_OPCG_REG1_SCAN_COUNT, PERV_1_OPCG_REG1_SCAN_COUNT_LEN>
    (l_scan_count);  //OPCG_REG1.SCAN_COUNT = l_scan_count
    l_data64.insertFromRight<PERV_1_OPCG_REG1_MISR_A_VAL, PERV_1_OPCG_REG1_MISR_A_VAL_LEN>
    (l_misr_a_value);  //OPCG_REG1.MISR_A_VAL = l_misr_a_value
    l_data64.insertFromRight<PERV_1_OPCG_REG1_MISR_B_VAL, PERV_1_OPCG_REG1_MISR_B_VAL_LEN>
    (l_misr_b_value);  //OPCG_REG1.MISR_B_VAL = l_misr_b_value
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG1, l_data64));

    FAPI_DBG("opcg go");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.setBit<1>();  //OPCG_REG0.OPCG_GO = 1
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    FAPI_DBG("Poll OPCG done bit to check for run-N completeness");
    l_timeout = P9_OPCG_DONE_ARRAYINIT_POLL_COUNT;

    //UNTIL CPLT_STAT0.CC_CTRL_OPCG_DONE_DC == 1
    while (l_timeout != 0)
    {
        //Getting CPLT_STAT0 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CPLT_STAT0, l_data64));
        bool l_poll_data =
            l_data64.getBit<PERV_1_CPLT_STAT0_CC_CTRL_OPCG_DONE_DC>();  //bool l_poll_data = CPLT_STAT0.CC_CTRL_OPCG_DONE_DC

        if (l_poll_data == 1)
        {
            break;
        }

        fapi2::delay(P9_OPCG_DONE_ARRAYINIT_HW_NS_DELAY,
                     P9_OPCG_DONE_ARRAYINIT_SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_DBG("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::SBE_ARRAYINIT_POLL_THRESHOLD_ERR()
                .set_TARGET_CHIPLET(i_target_chiplets)
                .set_PERV_CPLT_STAT0(l_data64)
                .set_LOOP_COUNT(l_timeout)
                .set_HW_DELAY(P9_OPCG_DONE_ARRAYINIT_HW_NS_DELAY),
                "ERROR:OPCG DONE BIT NOT SET");

    //Getting CPLT_STAT0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CPLT_STAT0,
                            l_read_reg)); //l_read_reg = CPLT_STAT0

    if ( i_select_sram )
    {
        FAPI_DBG("Checking sram abist done");
        FAPI_ASSERT(l_read_reg.getBit<0>() == 1,
                    fapi2::SRAM_ABIST_DONE_BIT_ERR()
                    .set_TARGET_CHIPLET(i_target_chiplets)
                    .set_PERV_CPLT_STAT(l_read_reg)
                    .set_SELECT_SRAM(i_select_sram)
                    .set_READ_ABIST_DONE(l_abist_check),
                    "ERROR:SRAM_ABIST_DONE_BIT_NOT_SET");
    }

    if ( i_select_edram )
    {
        FAPI_DBG("Checking edram abist done");
        FAPI_ASSERT(l_read_reg.getBit<1>() == 1,
                    fapi2::EDRAM_ABIST_DONE_BIT_ERR()
                    .set_TARGET_CHIPLET(i_target_chiplets)
                    .set_PERV_CPLT_STAT(l_read_reg)
                    .set_SELECT_EDRAM(i_select_edram)
                    .set_READ_ABIST_DONE(l_abist_check),
                    "ERROR:EDRAM_ABIST_DONE_BIT_NOT_SET");
    }

    //oaim_poll_done
    {
        FAPI_DBG("OPCG done, clear Run-N mode");
        //Setting OPCG_REG0 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
        l_data64.clearBit<PERV_1_OPCG_REG0_RUNN_MODE>();  //OPCG_REG0.RUNN_MODE = 0
        l_data64.clearBit<14>();  //OPCG_REG0.OPCG_STARTS_BIST = 0
        l_data64.clearBit<PERV_1_OPCG_REG0_LOOP_COUNT, PERV_1_OPCG_REG0_LOOP_COUNT_LEN>();  //OPCG_REG0.LOOP_COUNT = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

        FAPI_DBG("clear all clock REGIONS and type");
        //Setting CLK_REGION register value
        //CLK_REGION = 0
        l_data64_clk_region = 0;  //using variable to keep register data
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION,
                                l_data64_clk_region));

        FAPI_DBG("clear ABISTCLK_MUXSEL");
        //Setting CPLT_CTRL0 register value
        l_data64.flush<0>();
        //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 0
        l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL0_CLEAR, l_data64));

        FAPI_DBG("clear BIST REGISTER");
        //Setting BIST register value
        //BIST = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_BIST, 0));
    }

    FAPI_INF("p9_perv_sbe_cmn_array_init_module: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Region value settings
///
/// @param[in]     i_target_chip     Reference to TARGET_TYPE_PERV target ATTR_PG of the corresponding chiplet
/// @param[in]     i_regions_value   regions except vital and pll
/// @param[out]    o_regions_value   regions value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_perv_sbe_cmn_regions_setup_16(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
        const fapi2::buffer<uint16_t> i_regions_value,
        fapi2::buffer<uint16_t>& o_regions_value)
{
    fapi2::buffer<uint16_t> l_read_attr = 0;
    fapi2::buffer<uint16_t> l_read_attr_invert = 0;
    fapi2::buffer<uint16_t> l_read_attr_shift1_right = 0;
    FAPI_INF("p9_perv_sbe_cmn_regions_setup_16: Entering ...");

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chip, l_read_attr));
    FAPI_DBG("ATTR_PG Value : %#018lX", l_read_attr);

    FAPI_DBG("i_regions_value input from calling function: %#018lX",
             i_regions_value);

    if ( l_read_attr == 0x0 )
    {
        o_regions_value = i_regions_value;
    }
    else
    {
        l_read_attr_invert = l_read_attr.invert();
        FAPI_DBG("ATTR_PG inverted: %#018lX", l_read_attr_invert);
        l_read_attr_shift1_right = (l_read_attr_invert >> 1);
        FAPI_DBG("ATTR_PG inverted and shifted right by 1 %#018lX",
                 l_read_attr_shift1_right);

        o_regions_value = (i_regions_value & l_read_attr_shift1_right);
    }

    FAPI_INF("p9_perv_sbe_cmn_regions_setup_16: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Region value settings
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_regions_value    regions except vital and pll
/// @param[out]    o_regions_value    Regions value
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_perv_sbe_cmn_regions_setup_64(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
        const fapi2::buffer<uint16_t> i_regions_value,
        fapi2::buffer<uint64_t>& o_regions_value)
{
    fapi2::buffer<uint16_t> l_read_attr = 0;
    fapi2::buffer<uint16_t> l_read_attr_invert = 0;
    fapi2::buffer<uint16_t> l_read_attr_shift1_right = 0;
    fapi2::buffer<uint64_t> l_temp = 0;
    FAPI_INF("p9_perv_sbe_cmn_regions_setup_64: Entering ...");

    FAPI_DBG("Reading ATTR_PG");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_read_attr));
    FAPI_DBG("ATTR_PG Value : %#018lX", l_read_attr);

    FAPI_DBG("i_regions_value input from calling function: %#018lX",
             i_regions_value);

    if ( l_read_attr == 0x0 )
    {
        o_regions_value = (i_regions_value | l_temp);
    }
    else
    {
        l_read_attr_invert = l_read_attr.invert();
        FAPI_DBG("ATTR_PG inverted: %#018lX", l_read_attr_invert);
        l_read_attr_shift1_right = (l_read_attr_invert >> 1);
        FAPI_DBG("ATTR_PG inverted and shifted right by 1 %#018lX",
                 l_read_attr_shift1_right);

        o_regions_value = (i_regions_value & l_read_attr_shift1_right);
    }

    FAPI_INF("p9_perv_sbe_cmn_regions_setup_64: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Seeprom scan0 module
/// --Raise VITAL clock region fence
/// --Write Clock Region Register
/// --Write Scan Select Register
/// --set OPCG_REG0 register bit 0='0'
/// --set OPCG_REG0 register bit 2 = '1'
/// --Poll OPCG done bit to check for scan0 completeness
/// --clear clock region register
/// --clear scan select register
/// --Drop VITAL fence
///
///
/// @param[in]     i_target_chiplets   Reference to TARGET_TYPE_PERV target Targets for all chiplets
/// @param[in]     i_regions           set the clk region
/// @param[in]     i_scan_types        set scan types region
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p9_perv_sbe_cmn_scan0_module(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplets,
        const fapi2::buffer<uint16_t> i_regions,
        const fapi2::buffer<uint16_t> i_scan_types)
{
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint16_t> l_scan_types;
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
#if defined(SBE_AXONE_CONFIG) || !defined(__PPE__)
    uint32_t l_chipletID = i_target_chiplets.getChipletNumber();
    fapi2::buffer<uint8_t>  l_is_axone;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_taget_chip = i_target_chiplets.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
#endif

    FAPI_INF("p9_perv_sbe_cmn_scan0_module: Entering ...");

#if defined(SBE_AXONE_CONFIG) || !defined(__PPE__)
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_P9A_SBE_REGION, l_taget_chip, l_is_axone));
#endif

    i_regions.extractToRight<5, 11>(l_regions);
    i_scan_types.extractToRight<4, 12>(l_scan_types);

    FAPI_DBG("raise Vital clock region fence");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_VITL_REGION_FENCE = 1
    l_data64.setBit<C_CPLT_CTRL1_TC_VITL_REGION_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_OR, l_data64));

    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();

#if defined(SBE_AXONE_CONFIG) || !defined(__PPE__)

    if ((l_is_axone) && (l_chipletID == PERV_CHIPLET_ID))
    {
        FAPI_DBG("Raise region fences for scanned regions only");
        //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = l_regions
        l_data64.insertFromRight<4, 11>(l_regions);
    }
    else
#endif
    {
        FAPI_DBG("Raise region fences for all regions");
        l_data64.setBit<4, 11>();  //CPLT_CTRL1.TC_ALL_REGIONS_FENCE = 0b11111111111
    }

    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_OR, l_data64));

    FAPI_DBG("Setup all Clock Domains and Clock Types");
    //Setting CLK_REGION register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CLK_REGION, l_data64));
    //CLK_REGION.CLOCK_REGION_ALL_UNITS = l_regions
    l_data64.insertFromRight<4, 11>(l_regions);
    l_data64.setBit<48, 3>();  //CLK_REGION.SEL_THOLD_ALL = 0b111
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION, l_data64));

    FAPI_DBG("Write scan select register");
    //Setting SCAN_REGION_TYPE register value
    l_data64.flush<0>();  //SCAN_REGION_TYPE = 0
    //SCAN_REGION_TYPE.SCAN_REGION_ALL_UNITS = l_regions
    l_data64.insertFromRight<4, 11>(l_regions);
    //SCAN_REGION_TYPE.SCAN_ALL_TYPES = l_scan_types
    l_data64.insertFromRight<48, 12>(l_scan_types);
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_SCAN_REGION_TYPE, l_data64));

    FAPI_DBG("set OPCG_REG0 register bit 0='0'");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.clearBit<PERV_1_OPCG_REG0_RUNN_MODE>();  //OPCG_REG0.RUNN_MODE = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    FAPI_DBG("trigger Scan0");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.setBit<PERV_1_OPCG_REG0_RUN_SCAN0>();  //OPCG_REG0.RUN_SCAN0 = 1
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    FAPI_DBG("Poll OPCG done bit to check for run-N completeness");
    l_timeout = P9_OPCG_DONE_SCAN0_POLL_COUNT;

    //UNTIL CPLT_STAT0.CC_CTRL_OPCG_DONE_DC == 1
    while (l_timeout != 0)
    {
        //Getting CPLT_STAT0 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CPLT_STAT0, l_data64));
        bool l_poll_data =
            l_data64.getBit<PERV_1_CPLT_STAT0_CC_CTRL_OPCG_DONE_DC>();  //bool l_poll_data = CPLT_STAT0.CC_CTRL_OPCG_DONE_DC

        if (l_poll_data == 1)
        {
            break;
        }

        fapi2::delay(P9_OPCG_DONE_SCAN0_HW_NS_DELAY,
                     P9_OPCG_DONE_SCAN0_SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_DBG("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::SBE_SCAN0_DONE_POLL_THRESHOLD_ERR()
                .set_TARGET_CHIPLET(i_target_chiplets)
                .set_PERV_CPLT_STAT0(l_data64)
                .set_LOOP_COUNT(l_timeout)
                .set_HW_DELAY(P9_OPCG_DONE_SCAN0_HW_NS_DELAY),
                "ERROR:OPCG DONE BIT NOT SET");

    //os0m_poll_done
    {
        FAPI_DBG("clear all clock REGIONS and type");
        //Setting CLK_REGION register value
        //CLK_REGION = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION, 0));

        FAPI_DBG("Clear Scan Select Register");
        //Setting SCAN_REGION_TYPE register value
        //SCAN_REGION_TYPE = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_SCAN_REGION_TYPE, 0));
    }

    FAPI_INF("p9_perv_sbe_cmn_scan0_module: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
