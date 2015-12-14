/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_perv_sbe_cmn.C $              */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
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
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_perv_sbe_cmn.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include "p9_const_common.H"


enum P9_PERV_SBE_CMN_Private_Constants
{
    P9_OPCG_DONE_POLL_COUNT = 700, // No. of times OPCG read to check OPCG_DONE
    P9_OPCG_DONE_HW_NS_DELAY = 100000, // unit is nano seconds
    P9_OPCG_DONE_SIM_CYCLE_DELAY = 5000 // unit is cycles
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
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    fapi2::buffer<uint64_t> l_data64_clk_region;
    FAPI_DBG("Entering ...");

    FAPI_INF("Start pervasive Clocks");
    l_data64_clk_region.flush<0>();
    //Setting CLK_REGION register value
    l_data64_clk_region.insertFromRight<PERV_1_CLK_REGION_CLOCK_CMD, PERV_1_CLK_REGION_CLOCK_CMD_LEN>
    (0b01);  //CLK_REGION.CLOCK_CMD = 0b01
    l_data64_clk_region.setBit<4>();  //CLK_REGION.CLOCK_REGION_PERV = 1
    //CLK_REGION.SEL_THOLD_SL = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_SL>();
    //CLK_REGION.SEL_THOLD_NSL = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_NSL>();
    //CLK_REGION.SEL_THOLD_ARY = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_ARY>();
    FAPI_INF("Clock start data : %#018lX", l_data64_clk_region);
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION,
                            l_data64_clk_region));
    

    FAPI_INF("Mask all LFIR's in Chiplet Global FIR");
    //Setting FIR_MASK register value
    //FIR_MASK = 0xFFFFFFFFFFFFFFFF
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_FIR_MASK, 0xFFFFFFFFFFFFFFFF));

    FAPI_INF("Mask Special Attention");
    //Setting SPA_MASK register value
    //SPA_MASK = 0xFFFFFFFFFFFFFFFF
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_SPA_MASK, 0xFFFFFFFFFFFFFFFF));

    FAPI_INF("Stop Pervasive clocks");
    l_data64_clk_region.flush<0>();
    //Setting CLK_REGION register value
    l_data64_clk_region.insertFromRight<PERV_1_CLK_REGION_CLOCK_CMD, PERV_1_CLK_REGION_CLOCK_CMD_LEN>
    (0b10);  //CLK_REGION.CLOCK_CMD = 0b10
    l_data64_clk_region.setBit<4>();  //CLK_REGION.CLOCK_REGION_PERV = 1
    //CLK_REGION.SEL_THOLD_SL = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_SL>();
    //CLK_REGION.SEL_THOLD_NSL = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_NSL>();
    //CLK_REGION.SEL_THOLD_ARY = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_ARY>();
    FAPI_INF("Clock stop data : %#018lX", l_data64_clk_region);
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION,
                            l_data64_clk_region));
    

    FAPI_INF("Setup ABISTMUX_SEL");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 1
    l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL0_OR, l_data64));

    FAPI_INF("setup ABIST modes , BIST REGIONS:%#018lX", i_regions);
    //Setting BIST register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_BIST, l_data64));
    l_data64.clearBit<0>();  //BIST.TC_BIST_START_TEST_DC = 0
    //BIST.TC_SRAM_ABIST_MODE_DC = i_select_sram
    l_data64.writeBit<PERV_1_BIST_TC_SRAM_ABIST_MODE_DC>(i_select_sram);
    //BIST.TC_EDRAM_ABIST_MODE_DC = i_select_edram
    l_data64.writeBit<PERV_1_BIST_TC_EDRAM_ABIST_MODE_DC>(i_select_edram);
    //BIST.BIST_PERV = i_regions.getBit<5>()
    l_data64.writeBit<4>(i_regions.getBit<5>());
    //BIST.BIST_UNIT1 = i_regions.getBit<6>()
    l_data64.writeBit<5>(i_regions.getBit<6>());
    //BIST.BIST_UNIT2 = i_regions.getBit<7>()
    l_data64.writeBit<6>(i_regions.getBit<7>());
    //BIST.BIST_UNIT3 = i_regions.getBit<8>()
    l_data64.writeBit<7>(i_regions.getBit<8>());
    //BIST.BIST_UNIT4 = i_regions.getBit<9>()
    l_data64.writeBit<8>(i_regions.getBit<9>());
    //BIST.BIST_UNIT5 = i_regions.getBit<10>()
    l_data64.writeBit<9>(i_regions.getBit<10>());
    //BIST.BIST_UNIT6 = i_regions.getBit<11>()
    l_data64.writeBit<10>(i_regions.getBit<11>());
    //BIST.BIST_UNIT7 = i_regions.getBit<12>()
    l_data64.writeBit<11>(i_regions.getBit<12>());
    //BIST.BIST_UNIT8 = i_regions.getBit<13>()
    l_data64.writeBit<12>(i_regions.getBit<13>());
    //BIST.BIST_UNIT9 = i_regions.getBit<14>()
    l_data64.writeBit<13>(i_regions.getBit<14>());
    //BIST.BIST_UNIT10 = i_regions.getBit<15>()
    l_data64.writeBit<14>(i_regions.getBit<15>());
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_BIST, l_data64));
    FAPI_INF("l_data64 value:%#018lX", l_data64);

    FAPI_INF("Setup all Clock Domains and Clock Types");
    //Setting CLK_REGION register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CLK_REGION,
                            l_data64_clk_region));
    //CLK_REGION.CLOCK_REGION_PERV = i_regions.getBit<5>()
    l_data64_clk_region.writeBit<4>(i_regions.getBit<5>());
    //CLK_REGION.CLOCK_REGION_UNIT1 = i_regions.getBit<6>()
    l_data64_clk_region.writeBit<5>(i_regions.getBit<6>());
    //CLK_REGION.CLOCK_REGION_UNIT2 = i_regions.getBit<7>()
    l_data64_clk_region.writeBit<6>(i_regions.getBit<7>());
    //CLK_REGION.CLOCK_REGION_UNIT3 = i_regions.getBit<8>()
    l_data64_clk_region.writeBit<7>(i_regions.getBit<8>());
    //CLK_REGION.CLOCK_REGION_UNIT4 = i_regions.getBit<9>()
    l_data64_clk_region.writeBit<8>(i_regions.getBit<9>());
    //CLK_REGION.CLOCK_REGION_UNIT5 = i_regions.getBit<10>()
    l_data64_clk_region.writeBit<9>(i_regions.getBit<10>());
    //CLK_REGION.CLOCK_REGION_UNIT6 = i_regions.getBit<11>()
    l_data64_clk_region.writeBit<10>(i_regions.getBit<11>());
    //CLK_REGION.CLOCK_REGION_UNIT7 = i_regions.getBit<12>()
    l_data64_clk_region.writeBit<11>(i_regions.getBit<12>());
    //CLK_REGION.CLOCK_REGION_UNIT8 = i_regions.getBit<13>()
    l_data64_clk_region.writeBit<12>(i_regions.getBit<13>());
    //CLK_REGION.CLOCK_REGION_UNIT9 = i_regions.getBit<14>()
    l_data64_clk_region.writeBit<13>(i_regions.getBit<14>());
    //CLK_REGION.CLOCK_REGION_UNIT10 = i_regions.getBit<15>()
    l_data64_clk_region.writeBit<14>(i_regions.getBit<15>());
    //CLK_REGION.SEL_THOLD_SL = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_SL>();
    //CLK_REGION.SEL_THOLD_NSL = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_NSL>();
    //CLK_REGION.SEL_THOLD_ARY = 1
    l_data64_clk_region.setBit<PERV_1_CLK_REGION_SEL_THOLD_ARY>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION,
                            l_data64_clk_region));

    FAPI_INF("Drop Vital Fence(moved to arrayinit from scan0 module)");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_VITL_REGION_FENCE = 0
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_INF("Drop Region fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_PERV_REGION_FENCE>
    (i_regions.getBit<5>());  //CPLT_CTRL1.TC_PERV_REGION_FENCE = i_regions.getBit<5>()
    //CPLT_CTRL1.TC_REGION1_FENCE = i_regions.getBit<6>()
    l_data64.writeBit<5>(i_regions.getBit<6>());
    //CPLT_CTRL1.TC_REGION2_FENCE = i_regions.getBit<7>()
    l_data64.writeBit<6>(i_regions.getBit<7>());
    //CPLT_CTRL1.TC_REGION3_FENCE = i_regions.getBit<8>()
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_REGION3_FENCE>(i_regions.getBit<8>());
    //CPLT_CTRL1.TC_REGION4_FENCE = i_regions.getBit<9>()
    l_data64.writeBit<8>(i_regions.getBit<9>());
    //CPLT_CTRL1.TC_REGION5_FENCE = i_regions.getBit<10>()
    l_data64.writeBit<9>(i_regions.getBit<10>());
    //CPLT_CTRL1.TC_REGION6_FENCE = i_regions.getBit<11>()
    l_data64.writeBit<10>(i_regions.getBit<11>());
    //CPLT_CTRL1.TC_REGION7_FENCE = i_regions.getBit<12>()
    l_data64.writeBit<11>(i_regions.getBit<12>());
    //CPLT_CTRL1.UNUSED_12B = i_regions.getBit<13>()
    l_data64.writeBit<PERV_1_CPLT_CTRL1_UNUSED_12B>(i_regions.getBit<13>());
    //CPLT_CTRL1.UNUSED_13B = i_regions.getBit<14>()
    l_data64.writeBit<PERV_1_CPLT_CTRL1_UNUSED_13B>(i_regions.getBit<14>());
    //CPLT_CTRL1.UNUSED_14B = i_regions.getBit<15>()
    l_data64.writeBit<PERV_1_CPLT_CTRL1_UNUSED_14B>(i_regions.getBit<15>());
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_INF("Setup: loopcount , OPCG engine start ABIST, run-N mode");
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

    FAPI_INF("Setup IDLE count");
    //Setting OPCG_REG1 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG1, l_data64));
    l_data64.insertFromRight<PERV_1_OPCG_REG1_SCAN_COUNT, PERV_1_OPCG_REG1_SCAN_COUNT_LEN>
    (l_scan_count);  //OPCG_REG1.SCAN_COUNT = l_scan_count
    l_data64.insertFromRight<PERV_1_OPCG_REG1_MISR_A_VAL, PERV_1_OPCG_REG1_MISR_A_VAL_LEN>
    (l_misr_a_value);  //OPCG_REG1.MISR_A_VAL = l_misr_a_value
    l_data64.insertFromRight<PERV_1_OPCG_REG1_MISR_B_VAL, PERV_1_OPCG_REG1_MISR_B_VAL_LEN>
    (l_misr_b_value);  //OPCG_REG1.MISR_B_VAL = l_misr_b_value
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG1, l_data64));

    FAPI_INF("opcg go");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.setBit<1>();  //OPCG_REG0.OPCG_GO = 1
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    FAPI_INF("Poll OPCG done bit to check for run-N completeness");
    l_timeout = P9_OPCG_DONE_POLL_COUNT;

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

        fapi2::delay(P9_OPCG_DONE_HW_NS_DELAY, P9_OPCG_DONE_SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_INF("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::SBE_ARRAYINIT_POLL_THRESHOLD_ERR(),
                "ERROR:OPCG DONE BIT NOT SET");

    //oaim_poll_done
    {
        FAPI_INF("OPCG done, clear Run-N mode");
        //Setting OPCG_REG0 register value
        FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
        l_data64.clearBit<PERV_1_OPCG_REG0_RUNN_MODE>();  //OPCG_REG0.RUNN_MODE = 0
        l_data64.clearBit<14>();  //OPCG_REG0.OPCG_STARTS_BIST = 0
        l_data64.clearBit<PERV_1_OPCG_REG0_LOOP_COUNT, PERV_1_OPCG_REG0_LOOP_COUNT_LEN>();  //OPCG_REG0.LOOP_COUNT = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

        FAPI_INF("clear all clock REGIONS and type");
        //Setting CLK_REGION register value
        //CLK_REGION = 0
        l_data64_clk_region = 0;  //using variable to keep register data
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION,
                                l_data64_clk_region));

        FAPI_INF("clear ABISTCLK_MUXSEL");
        //Setting CPLT_CTRL0 register value
        l_data64.flush<0>();
        //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 0
        l_data64.setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL0_CLEAR, l_data64));

        FAPI_INF("clear BIST REGISTER");
        //Setting BIST register value
        //BIST = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_BIST, 0));
    }

    FAPI_DBG("Exiting ...");

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
    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    FAPI_DBG("Entering ...");

    FAPI_INF("raise Vital clock region fence");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_VITL_REGION_FENCE = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_OR, l_data64));

    FAPI_INF("Raise region fences for scanned regions");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_PERV_REGION_FENCE = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_PERV_REGION_FENCE>();
    l_data64.setBit<5>();  //CPLT_CTRL1.TC_REGION1_FENCE = 1
    l_data64.setBit<6>();  //CPLT_CTRL1.TC_REGION2_FENCE = 1
    //CPLT_CTRL1.TC_REGION3_FENCE = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_TC_REGION3_FENCE>();
    l_data64.setBit<8>();  //CPLT_CTRL1.TC_REGION4_FENCE = 1
    l_data64.setBit<9>();  //CPLT_CTRL1.TC_REGION5_FENCE = 1
    l_data64.setBit<10>();  //CPLT_CTRL1.TC_REGION6_FENCE = 1
    l_data64.setBit<11>();  //CPLT_CTRL1.TC_REGION7_FENCE = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_UNUSED_12B>();  //CPLT_CTRL1.UNUSED_12B = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_UNUSED_13B>();  //CPLT_CTRL1.UNUSED_13B = 1
    l_data64.setBit<PERV_1_CPLT_CTRL1_UNUSED_14B>();  //CPLT_CTRL1.UNUSED_14B = 1
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CPLT_CTRL1_OR, l_data64));

    FAPI_INF("Setup all Clock Domains and Clock Types");
    //Setting CLK_REGION register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_CLK_REGION, l_data64));
    //CLK_REGION.CLOCK_REGION_PERV = i_regions.getBit<5>()
    l_data64.writeBit<4>(i_regions.getBit<5>());
    //CLK_REGION.CLOCK_REGION_UNIT1 = i_regions.getBit<6>()
    l_data64.writeBit<5>(i_regions.getBit<6>());
    //CLK_REGION.CLOCK_REGION_UNIT2 = i_regions.getBit<7>()
    l_data64.writeBit<6>(i_regions.getBit<7>());
    //CLK_REGION.CLOCK_REGION_UNIT3 = i_regions.getBit<8>()
    l_data64.writeBit<7>(i_regions.getBit<8>());
    //CLK_REGION.CLOCK_REGION_UNIT4 = i_regions.getBit<9>()
    l_data64.writeBit<8>(i_regions.getBit<9>());
    //CLK_REGION.CLOCK_REGION_UNIT5 = i_regions.getBit<10>()
    l_data64.writeBit<9>(i_regions.getBit<10>());
    //CLK_REGION.CLOCK_REGION_UNIT6 = i_regions.getBit<11>()
    l_data64.writeBit<10>(i_regions.getBit<11>());
    //CLK_REGION.CLOCK_REGION_UNIT7 = i_regions.getBit<12>()
    l_data64.writeBit<11>(i_regions.getBit<12>());
    //CLK_REGION.CLOCK_REGION_UNIT8 = i_regions.getBit<13>()
    l_data64.writeBit<12>(i_regions.getBit<13>());
    //CLK_REGION.CLOCK_REGION_UNIT9 = i_regions.getBit<14>()
    l_data64.writeBit<13>(i_regions.getBit<14>());
    //CLK_REGION.CLOCK_REGION_UNIT10 = i_regions.getBit<15>()
    l_data64.writeBit<14>(i_regions.getBit<15>());
    //CLK_REGION.SEL_THOLD_SL = 1
    l_data64.setBit<PERV_1_CLK_REGION_SEL_THOLD_SL>();
    //CLK_REGION.SEL_THOLD_NSL = 1
    l_data64.setBit<PERV_1_CLK_REGION_SEL_THOLD_NSL>();
    //CLK_REGION.SEL_THOLD_ARY = 1
    l_data64.setBit<PERV_1_CLK_REGION_SEL_THOLD_ARY>();
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION, l_data64));

    FAPI_INF("Write scan select registers");
    //Setting SCAN_REGION_TYPE register value
    l_data64.flush<0>();  //SCAN_REGION_TYPE = 0
    //SCAN_REGION_TYPE.SCAN_REGION_PERV = i_regions.getBit<5>()
    l_data64.writeBit<4>(i_regions.getBit<5>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT1 = i_regions.getBit<6>()
    l_data64.writeBit<5>(i_regions.getBit<6>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT2 = i_regions.getBit<7>()
    l_data64.writeBit<6>(i_regions.getBit<7>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT3 = i_regions.getBit<8>()
    l_data64.writeBit<7>(i_regions.getBit<8>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT4 = i_regions.getBit<9>()
    l_data64.writeBit<8>(i_regions.getBit<9>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT5 = i_regions.getBit<10>()
    l_data64.writeBit<9>(i_regions.getBit<10>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT6 = i_regions.getBit<11>()
    l_data64.writeBit<10>(i_regions.getBit<11>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT7 = i_regions.getBit<12>()
    l_data64.writeBit<11>(i_regions.getBit<12>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT8 = i_regions.getBit<13>()
    l_data64.writeBit<12>(i_regions.getBit<13>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT9 = i_regions.getBit<14>()
    l_data64.writeBit<13>(i_regions.getBit<14>());
    //SCAN_REGION_TYPE.SCAN_REGION_UNIT10 = i_regions.getBit<15>()
    l_data64.writeBit<14>(i_regions.getBit<15>());
    //SCAN_REGION_TYPE.SCAN_TYPE_FUNC = i_scan_types.getBit<4>()
    l_data64.writeBit<48>(i_scan_types.getBit<4>());
    //SCAN_REGION_TYPE.SCAN_TYPE_CFG = i_scan_types.getBit<5>()
    l_data64.writeBit<49>(i_scan_types.getBit<5>());
    //SCAN_REGION_TYPE.SCAN_TYPE_CCFG_GPTR = i_scan_types.getBit<6>()
    l_data64.writeBit<50>(i_scan_types.getBit<6>());
    //SCAN_REGION_TYPE.SCAN_TYPE_REGF = i_scan_types.getBit<7>()
    l_data64.writeBit<51>(i_scan_types.getBit<7>());
    //SCAN_REGION_TYPE.SCAN_TYPE_LBIST = i_scan_types.getBit<8>()
    l_data64.writeBit<52>(i_scan_types.getBit<8>());
    //SCAN_REGION_TYPE.SCAN_TYPE_ABIST = i_scan_types.getBit<9>()
    l_data64.writeBit<53>(i_scan_types.getBit<9>());
    //SCAN_REGION_TYPE.SCAN_TYPE_REPR = i_scan_types.getBit<10>()
    l_data64.writeBit<54>(i_scan_types.getBit<10>());
    //SCAN_REGION_TYPE.SCAN_TYPE_TIME = i_scan_types.getBit<11>()
    l_data64.writeBit<55>(i_scan_types.getBit<11>());
    //SCAN_REGION_TYPE.SCAN_TYPE_BNDY = i_scan_types.getBit<12>()
    l_data64.writeBit<56>(i_scan_types.getBit<12>());
    //SCAN_REGION_TYPE.SCAN_TYPE_FARR = i_scan_types.getBit<13>()
    l_data64.writeBit<57>(i_scan_types.getBit<13>());
    //SCAN_REGION_TYPE.SCAN_TYPE_CMSK = i_scan_types.getBit<14>()
    l_data64.writeBit<58>(i_scan_types.getBit<14>());
    //SCAN_REGION_TYPE.SCAN_TYPE_INEX = i_scan_types.getBit<15>()
    l_data64.writeBit<59>(i_scan_types.getBit<15>());
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_SCAN_REGION_TYPE, l_data64));

    FAPI_INF("set OPCG_REG0 register bit 0='0'");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.clearBit<PERV_1_OPCG_REG0_RUNN_MODE>();  //OPCG_REG0.RUNN_MODE = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    FAPI_INF("trigger Scan0");
    //Setting OPCG_REG0 register value
    FAPI_TRY(fapi2::getScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));
    l_data64.setBit<PERV_1_OPCG_REG0_RUN_SCAN0>();  //OPCG_REG0.RUN_SCAN0 = 1
    FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_OPCG_REG0, l_data64));

    FAPI_INF("Poll OPCG done bit to check for run-N completeness");
    l_timeout = P9_OPCG_DONE_POLL_COUNT;

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

        --l_timeout;
    }

    FAPI_INF("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::SBE_ARRAYINIT_POLL_THRESHOLD_ERR(),
                "ERROR:OPCG DONE BIT NOT SET");

    //os0m_poll_done
    {
        FAPI_INF("clear all clock REGIONS and type");
        //Setting CLK_REGION register value
        //CLK_REGION = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_CLK_REGION, 0));

        FAPI_INF("Clear Scan Select Register");
        //Setting SCAN_REGION_TYPE register value
        //SCAN_REGION_TYPE = 0
        FAPI_TRY(fapi2::putScom(i_target_chiplets, PERV_SCAN_REGION_TYPE, 0));
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
