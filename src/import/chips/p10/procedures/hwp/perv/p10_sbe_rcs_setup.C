/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_rcs_setup.C $ */
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
/// @file  p10_sbe_rcs_setup.C
///
/// @brief
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#include "p10_sbe_rcs_setup.H"

#include "p10_scom_perv_0.H"
#include "p10_scom_perv_2.H"
#include "p10_scom_perv_3.H"
#include "p10_scom_perv_8.H"
#include "p10_scom_perv_a.H"
#include "p10_scom_perv_d.H"
#include "p10_scom_perv_f.H"
#include "p10_scom_proc_5.H"
#include "p10_scom_proc_e.H"
#include <target_filters.H>
#include "p10_perv_sbe_cmn.H"
#include "p10_clock_test_cmn.H"
#include "hw540133.H"

enum P10_SBE_RCS_SETUP_Private_Constants
{
    RCS_CONTRL5_DC_CFAM_RESET_VAL = 0x0010000000000000,
    HW_NS_DELAY = 20, // unit is nano seconds
    SIM_CYCLE_DELAY = 100000, // unit is sim cycles
    POLL_COUNT = 10,
    RCS_BYPASS_NS_DELAY = 100000, // unit is nano seconds (100us)
    RCS_BYPASS_SIM_CYCLE_DELAY = 100, // unit is sim cycles
    RCS_RESET_NS_DELAY = 100000, // unit is nano seconds (100us)
    RCS_RESSEL_NS_DELAY = 15000000, // unit is nano seconds (15ms)
    RCS_RESET_SIM_CYCLE_DELAY = 100  // unit is sim cycles
};

static const int INVALID_DESKEW = -1;

static fapi2::ReturnCode p10_sbe_rcs_setup_test_latches(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    uint8_t attr_cp_refclock_select,
    bool set_rcs_clock_test_in);

//---------------------------------------------------------------------------------
// Function to make sure RCS exits on the correct side
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_sbe_rcs_check_side(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    uint8_t i_cp_refclck_select)
{
    using namespace scomt;
    using namespace scomt::proc;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64_rcsns;
    fapi2::buffer<uint64_t> l_data64_rc5;
    bool     l_aside = 0, l_bside = 0;

    FAPI_DBG("Entering RCS check side...");

    //read back the RCS sens register
    GET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_SNS1LTH_RO(i_target_chip, l_data64_rcsns);

    FAPI_DBG("rcs sens register %#018lX", l_data64_rcsns);

    //Check if the A-side or B-side is selected
    l_aside = l_data64_rcsns.getBit<12>();
    l_bside = l_data64_rcsns.getBit<13>();

    if(((i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0) ||
        (i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0_NORED)) &&
       (l_bside))
    {
        l_data64_rc5.flush<0>();
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
        SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_TPFSI_RCS_FORCE_BYPASS_CLKSEL_DC(1, l_data64_rc5);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

        // force a stuck 0 error on the B-side
        l_data64_rc5.flush<0>();
        SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_SWO_FORCE_LOW(1, l_data64_rc5);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

    }
    else if(((i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1) ||
             (i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1_NORED)) &&
            (l_aside))
    {
        l_data64_rc5.flush<0>();
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
        SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_TPFSI_RCS_FORCE_BYPASS_CLKSEL_DC(1, l_data64_rc5);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

        // force a stuck 0 error on the A-side
        l_data64_rc5.flush<0>();
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
        SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_SWO_FORCE_LOW(1, l_data64_rc5);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

    }

    // clear the FORCE_LOW before we clear the errors
    l_data64_rc5.flush<0>();
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_SWO_FORCE_LOW(1, l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

    FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

    if((i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0) ||
       (i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0_NORED))
    {
        l_data64_rc5.flush<0>();
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
        SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_TPFSI_RCS_FORCE_BYPASS_CLKSEL_DC(1, l_data64_rc5);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));
    }
    else if((i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1) ||
            (i_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1_NORED))
    {
        l_data64_rc5.flush<0>();
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
        SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_TPFSI_RCS_FORCE_BYPASS_CLKSEL_DC(1, l_data64_rc5);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

    }

    FAPI_DBG("Clear RCS errors");
    l_data64_rc5.flush<0>().setBit<6, 2>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR, l_data64_rc5));
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

    FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));


fapi_try_exit:
    return fapi2::current_err;

}
//---------------------------------------------------------------------------------
// Function to find the deskew val from the input string of error bits
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_get_deskew_dd2(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    const uint8_t i_clockPos,
    const uint32_t  i_shiftedErrVals,
    const int32_t i_loop,
    const bool i_side,
    int&  o_deskewVal
)
{
    const int32_t C_MIN_WINDOW = 5;
    uint32_t l_deskew = i_shiftedErrVals;
    int32_t l_step = 0;
    int32_t l_firstError = 16;
    int32_t l_lastError = 16;
    o_deskewVal = -1;

    // Case 1: If no errors, we will pass through and exit
    while (l_deskew)
    {
        if (l_deskew & 0x8000)
        {
            // Set the first error, this will only get set once
            if (l_firstError > l_step)
            {
                l_firstError = l_step;
            }

            // Case 2: Look for a window condition
            // - This is where 2 sets of errors are found
            if ( (l_step - l_lastError) > C_MIN_WINDOW)
            {
                o_deskewVal = (l_step + l_lastError) / 2;
                break;
            }

            l_lastError = l_step;
        }

        l_deskew = (l_deskew << 1) & 0xFFFE;
        l_step++;
    }

    // Case 3: single set of errors
    if (i_shiftedErrVals && o_deskewVal == -1)
    {
        o_deskewVal = (l_lastError < 10) ? l_lastError + 6 : l_firstError - 6;
    }

#ifndef DFT
    FAPI_DBG("RCS Calc Deskew shifted(0x%08X) step(%d) firstError(%d), lastError(%d) deskew(%d)",
             i_shiftedErrVals, l_step, l_firstError, l_lastError, o_deskewVal)
#endif //DFT

    // Verify Selected Deskew is Valid
    if (o_deskewVal != -1)
    {
        // Verify we are in the correct range
        FAPI_ASSERT(((o_deskewVal >= 0) && (o_deskewVal <= 15)),
                    fapi2::RCS_FPLL_DESKEW_CAL_ERROR()
                    .set_MASTER_CHIP(i_target_chip)
                    .set_SHIFTED_ERR_VAL(i_shiftedErrVals)
                    .set_SELECTED_DESKEW_VAL(o_deskewVal)
                    .set_STEP(l_step)
                    .set_FIRST_ERROR(l_firstError)
                    .set_LAST_ERROR(l_lastError)
                    .set_LOOP(i_loop)
                    .set_SIDE_A_NOT_B(i_side)
                    .set_CLOCK_POS(i_clockPos),
                    "RCS Deskew not in valid range");
        //"[%d]RCS Deskew %d (%d, 0x%04X) not in valid range : [%d,%d,%d]",
        //i_loop, i_side, o_deskewVal, i_shiftedErrVals, l_step, l_firstError, l_lastError);

        for (int32_t l_offset = -2; l_offset <= 2; l_offset++)
        {
            // Verify we have margin at the selected value
            if (((o_deskewVal + l_offset) >= 0) && ((o_deskewVal + l_offset) <= 15))
            {
                FAPI_ASSERT(!((0x8000 >> (o_deskewVal + l_offset)) & i_shiftedErrVals),
                            fapi2::RCS_FPLL_DESKEW_CAL_ERROR()
                            .set_MASTER_CHIP(i_target_chip)
                            .set_SHIFTED_ERR_VAL(i_shiftedErrVals)
                            .set_SELECTED_DESKEW_VAL(o_deskewVal)
                            .set_STEP(l_step)
                            .set_FIRST_ERROR(l_firstError)
                            .set_LAST_ERROR(l_lastError)
                            .set_LOOP(i_loop)
                            .set_SIDE_A_NOT_B(i_side)
                            .set_CLOCK_POS(i_clockPos),
                            "RCS Deskew error at offset");
            }
        }
    }


fapi_try_exit:
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------
// Function to find the deskew val from the input string of error bits
//---------------------------------------------------------------------------------
void p10_get_deskew(
    const uint32_t  i_shiftedErrVals,
    int&  o_deskewVal
)
{

    uint32_t l_streak = 0;
    uint32_t l_streakMax = 0;
    uint32_t l_lastGoodPos = 0;
    o_deskewVal = -1;


    FAPI_DBG("Input shiftedErrVal = 0x%08X", i_shiftedErrVals);

    //From the input error values i_shiftedErrVal, the algorithm will find the optimum
    //deskew to set on the PLL.
    //For finding the optimum deskew there must be a sequence of three or more deskew
    //that did not result in an error.
    //The algorithm will interate over the 16 bits in the input i_shiftedErrVal that represent
    //output of the experiments
    for ( int l_step = 0 ; l_step < 16; ++l_step)
    {
        //Check if the current bit position in the input error string respresents an error
        if ((i_shiftedErrVals >> (15 - l_step)) & 0x1)
        {
            if (l_streak > l_streakMax)     //found longest good streak
            {
                l_streakMax = l_streak;
                l_lastGoodPos = l_step - 1; //last good position
            }

            l_streak = 0; //restart the counter
        }
        else //The current position do not represent error
        {
            ++l_streak;
        }
    }

    if ( l_streak == 16)  //This represents  a case where no error was found, trivially assign the
        //o_deskewVal to zero
    {
        o_deskewVal = 0;
    }
    else if ( l_streakMax >= 3 ) //If three consecutive non-error bits were found
    {
        //set deskew to center of streak
        o_deskewVal = (l_lastGoodPos - (l_streakMax / 2)) % 8;
    }


    return;
}

//---------------------------------------------------------------------------------
// Run the deskew algorithm to find optimum deskew for the RCS FPLL
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_sbe_rcs_dd2_deskew_calibrate(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("Entering p10_sbe_rcs_dd2_deskew_calibrate");
    using namespace scomt;
    using namespace scomt::proc;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64_rc6;
    fapi2::buffer<uint64_t> l_data64_rc5;
    fapi2::buffer<uint64_t> l_data64_rc3;
    fapi2::buffer<uint64_t> l_data64_rcsns;
    fapi2::buffer<uint64_t> l_deskew_buf;

    bool     l_clkErrA = false, l_clkErrB = false;
    uint32_t l_clkAErrVals = 0, l_shiftedAErrVals = 0;
    uint32_t l_clkBErrVals = 0, l_shiftedBErrVals = 0;
    const uint16_t l_max_deskews = 16;
    const uint16_t l_ressel_count = 4;
    uint64_t l_deskew_array[l_max_deskews] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    int      l_max_loop = 10;                  //The number of times PLL reset will be tried
    int      l_goodDeskewA = INVALID_DESKEW;   //Initialize to a deskew non-valid deskew
    int      l_goodDeskewB = INVALID_DESKEW;   //Initialize to a deskew non-valid deskew
    // variable for creating clock pos ffdc
    uint8_t l_callout_clock;

    for(int l_index = 0; l_index < l_max_loop ; l_index++)
    {
        l_clkAErrVals = 0;
        l_clkBErrVals = 0;
        l_shiftedAErrVals = 0;
        l_shiftedBErrVals = 0;

        FAPI_DBG("Deskew attempt #%d", l_index);

        for( int l_deskewIndex = 0 ; l_deskewIndex < l_max_deskews; l_deskewIndex++)
        {
            l_clkErrA = false, l_clkErrB = false;

            //Initialize the RC5 to ASSERT RESET and BYPASS (Add:2925)
            l_data64_rc5 = 0x0406000200000000ull;
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

            //Set deskew to 0
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip));
            l_deskew_buf.flush<0>();
            SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_DESKEW_SEL_A(0xF, l_deskew_buf);
            SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_DESKEW_SEL_B(0xF, l_deskew_buf);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip, l_deskew_buf));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

            //Initialize the DESKEW value
            l_data64_rc6.flush<0>();
            l_deskew_buf = l_deskew_array[l_deskewIndex];
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_WO_OR(i_target_chip));
            SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_DESKEW_SEL_A(l_deskew_buf, l_data64_rc6);
            SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_DESKEW_SEL_B(l_deskew_buf, l_data64_rc6);
            SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_RCS_CONTROL_7_0(0x0B,
                    l_data64_rc6);       //set SEL_RES and pulse_width chicken switch
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_WO_OR(i_target_chip, l_data64_rc6));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

            // loop through the RES_SEL at each deskew to fine tune the metastability search
            for( int l_ressel = 0 ; l_ressel < l_ressel_count; l_ressel++)
            {
                l_data64_rc6.flush<0>();
                FAPI_DBG("Clear RES_SEL values");       //bits <2:1> of the 8 bit field
                FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip));
                SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_RCS_CONTROL_7_0(3 << 1, l_data64_rc6);
                FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc6));

                l_data64_rc6.flush<0>();
                FAPI_DBG("Set RES_SEL to %d", l_ressel);       //bits <2:1> of the 8 bit field
                FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_WO_OR(i_target_chip));
                SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_RCS_CONTROL_7_0(l_ressel, l_data64_rc6);
                FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_WO_OR(i_target_chip, l_data64_rc6));

                FAPI_DBG("Clear RCS errors");
                l_data64_rc5.flush<0>();
                FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
                SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_CLEAR_CLK_ERROR_A(l_data64_rc5);
                SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_CLEAR_CLK_ERROR_B(l_data64_rc5);
                FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

                FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

                //Do not clear l_data64_rc5 here
                FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
                FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

                FAPI_TRY(fapi2::delay(RCS_RESSEL_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

                //read back the RCS sense register
                GET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_SNS1LTH_RO(i_target_chip, l_data64_rcsns);

                FAPI_DBG("deskew loop 0x%02X, RES_SEL 0x%X, rcs sens register %#018lX", l_deskewIndex, l_ressel, l_data64_rcsns);

                //Check if there was a clock error
                l_clkErrA |= l_data64_rcsns.getBit<6>() | l_data64_rcsns.getBit<7>();
                l_clkErrB |= l_data64_rcsns.getBit<8>() | l_data64_rcsns.getBit<9>();
            }

            //The clock errors encountered for each deskew will be put into a bit string with
            //0th deskew forming the leftmost bit, the 1st deskew forming the 2nd bit from left and
            //so on.
            l_clkAErrVals += l_clkErrA << (15 - l_deskew_buf);
            l_clkBErrVals += l_clkErrB << (15 - l_deskew_buf);
            FAPI_DBG("deskew loop 0x%02X, l_clkErrA 0x%02X, l_clkErrB 0x%02X",
                     l_deskewIndex, l_clkErrA, l_clkErrB);
            FAPI_DBG("l_clkAErrVals 0x%04X, l_clkBErrVals 0x%04X", l_clkAErrVals, l_clkBErrVals);

        }//end of deskew range for

        //Arrange the bit string values one after the other in-order at arrive at the
        //pattern from which three consecutive deskew that did not produce an error will be searched.
        l_shiftedAErrVals = l_clkAErrVals & 0xFFFF;
        l_shiftedBErrVals = l_clkBErrVals & 0xFFFF;

        FAPI_TRY(p10_get_deskew_dd2(
                     i_target_chip,
                     fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0,
                     l_shiftedAErrVals, l_index, true, l_goodDeskewA));
        FAPI_TRY(p10_get_deskew_dd2(
                     i_target_chip,
                     fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1,
                     l_shiftedBErrVals, l_index, false, l_goodDeskewB));

#ifndef DFT
        FAPI_DBG("RCS Deskew Loop[%d]: DeskewA(0x%08X)[%d] DeskewB(0x%08X)[%d]",
                 l_index, l_clkAErrVals, l_goodDeskewA, l_clkBErrVals, l_goodDeskewB);
#endif //DFT

        if((l_goodDeskewA == INVALID_DESKEW) || (l_goodDeskewB == INVALID_DESKEW))
        {
#ifndef DFT
            FAPI_DBG("Valid deskew not found, loop count 0x%02x  DeskewA(0x%08X)[%d] DeskewB(0x%08X)[%d]",
                     l_index, l_clkAErrVals, l_goodDeskewA, l_clkBErrVals, l_goodDeskewB);
#endif //DFT

            //reset the RCS PLL before entering the new loop
            l_data64_rc3.flush<0>();
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_FSI(i_target_chip));

            if(l_goodDeskewA == INVALID_DESKEW)
            {
                SET_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_TP_PLLCLKSW1_RESET_DC(l_data64_rc3);
            }

            if(l_goodDeskewB == INVALID_DESKEW)
            {
                SET_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_TP_PLLCLKSW2_RESET_DC(l_data64_rc3);
            }

            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR(i_target_chip, l_data64_rc3));

            fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY);
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_FSI(i_target_chip));
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc3));
            fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY);
        }
        else
        {
            FAPI_INF("Found good A-side deskew 0x%02x, loopcount 0x%02X", l_goodDeskewA, l_index);
            FAPI_INF("Found good B-side deskew 0x%02x, loopcount 0x%02X", l_goodDeskewB, l_index);
            break;
        }


    }// End of loop to find the deskew

    GET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_SNS1LTH_RO(i_target_chip, l_data64_rcsns);
    l_callout_clock = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0;
    FAPI_ASSERT((l_goodDeskewA != INVALID_DESKEW),
                fapi2::RCS_FPLL_DESKEW_ERR()
                .set_MASTER_CHIP(i_target_chip)
                .set_READ_SNS1LTH(l_data64_rcsns)
                .set_DESKEW_A(l_goodDeskewA)
                .set_DESKEW_B(l_goodDeskewB)
                .set_SHIFTED_A_ERR_VAL(l_shiftedAErrVals)
                .set_SHIFTED_B_ERR_VAL(l_shiftedBErrVals)
                .set_CLOCK_POS(l_callout_clock),
                "Deskew calibration failed on A-side");
    l_callout_clock = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1;
    FAPI_ASSERT((l_goodDeskewB != INVALID_DESKEW),
                fapi2::RCS_FPLL_DESKEW_ERR()
                .set_MASTER_CHIP(i_target_chip)
                .set_READ_SNS1LTH(l_data64_rcsns)
                .set_DESKEW_A(l_goodDeskewA)
                .set_DESKEW_B(l_goodDeskewB)
                .set_SHIFTED_A_ERR_VAL(l_shiftedAErrVals)
                .set_SHIFTED_B_ERR_VAL(l_shiftedBErrVals)
                .set_CLOCK_POS(l_callout_clock),
                "Deskew calibration failed on B-side");

    l_data64_rc6.flush<0>();
    FAPI_DBG("Set RES_SEL from 3 to 1");       //bits <2:1> of the 8 bit field
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip));
    SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_RCS_CONTROL_7_0(1 << 2, l_data64_rc6);  // clear only MSB bit
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc6));

    //Set deskew to 0
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip));
    l_deskew_buf.flush<0>();
    SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_DESKEW_SEL_A(0xF, l_deskew_buf);
    SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_DESKEW_SEL_B(0xF, l_deskew_buf);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_CLEAR_WO_CLEAR(i_target_chip, l_deskew_buf));

    //set the newly determined deskew val
    l_data64_rc5.flush<0>();
    l_deskew_buf = l_goodDeskewA;
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_WO_OR(i_target_chip));
    SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_DESKEW_SEL_A(l_deskew_buf, l_data64_rc5);
    l_deskew_buf = l_goodDeskewB;
    SET_P10_20_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_DESKEW_SEL_B(l_deskew_buf, l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL6_SET_WO_OR(i_target_chip, l_data64_rc5));

    FAPI_DBG("Clear RCS errors");
    l_data64_rc5.flush<0>().setBit<6, 2>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR, l_data64_rc5));
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

    // //set the sel_del val
    // l_data64_rc5.flush<0>();
    // FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
    // SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_SEL_DEL(l_data64_rc5);
    // FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

    FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

    //clear the BLOCK_SWO bit
    l_data64_rc5.flush<0>();
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_BLOCK_SWO(l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

    FAPI_TRY(fapi2::delay(RCS_RESSEL_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

    //read back the RCS sense register
    GET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_SNS1LTH_RO(i_target_chip, l_data64_rcsns);

    l_callout_clock = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0;
    FAPI_ASSERT(!(l_data64_rcsns.getBit<6>() | l_data64_rcsns.getBit<7>()),
                fapi2::RCS_FPLL_DESKEW_ERR()
                .set_MASTER_CHIP(i_target_chip)
                .set_READ_SNS1LTH(l_data64_rcsns)
                .set_DESKEW_A(l_goodDeskewA)
                .set_DESKEW_B(l_goodDeskewB)
                .set_SHIFTED_A_ERR_VAL(l_shiftedAErrVals)
                .set_SHIFTED_B_ERR_VAL(l_shiftedBErrVals)
                .set_CLOCK_POS(l_callout_clock),
                "Deskew Calibration Final Check Failed on A-side");

    l_callout_clock = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1;
    FAPI_ASSERT(!(l_data64_rcsns.getBit<8>() | l_data64_rcsns.getBit<9>()),
                fapi2::RCS_FPLL_DESKEW_ERR()
                .set_MASTER_CHIP(i_target_chip)
                .set_READ_SNS1LTH(l_data64_rcsns)
                .set_DESKEW_A(l_goodDeskewA)
                .set_DESKEW_B(l_goodDeskewB)
                .set_SHIFTED_A_ERR_VAL(l_shiftedAErrVals)
                .set_SHIFTED_B_ERR_VAL(l_shiftedBErrVals)
                .set_CLOCK_POS(l_callout_clock),
                "Deskew Calibration Final Check Failed on B-side");


    FAPI_INF("p10_sbe_rcs_dd2_deskew_calibrate: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}


//---------------------------------------------------------------------------------
// Run the deskew algorithm to find optimum deskew for the RCS FPLL
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_sbe_rcs_deskew_calibrate(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("Entering p10_sbe_rcs_deskew_calibrate");
    using namespace scomt;
    using namespace scomt::proc;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64_rc5;
    fapi2::buffer<uint64_t> l_data64_rc3;
    fapi2::buffer<uint64_t> l_data64_rcsns;
    fapi2::buffer<uint64_t> l_deskew_buf;

    bool     l_clkErrA = false, l_clkErrB = false;
    uint32_t l_clkErrVals = 0, l_shiftedErrVals = 0;
    const uint8_t l_max_deskews = 8;
    uint64_t l_deskew_array[l_max_deskews] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
    int      l_max_loop = 5;                   //The number of times PLL reset will be tried
    int      l_goodDeskew = INVALID_DESKEW;   //Initialize to a deskew non-valid deskew

    for(int l_index = 0; l_index < l_max_loop ; l_index++)
    {
        l_clkErrVals = 0;

        for( int l_deskewIndex = 0 ; l_deskewIndex < l_max_deskews; l_deskewIndex++)
        {
            //Initialize the RC5 to ASSERT RESET and BYPASS (Add:2925)
            l_data64_rc5 = 0xC000000200000000ull;
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

            //Set deskew to 0
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
            l_deskew_buf.flush<0>();
            SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_DESKEW(0x7, l_deskew_buf);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_deskew_buf));

            //Initialize the DESKEW value
            l_data64_rc5.flush<0>();
            l_deskew_buf = l_deskew_array[l_deskewIndex];
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
            SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_DESKEW(l_deskew_buf, l_data64_rc5);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

            //take RCS out of RESET
            l_data64_rc5.flush<0>();
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
            SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_TPFSI_RCS_RESET_DC(l_data64_rc5);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

            //clear BYPASS
            l_data64_rc5.flush<0>();
            SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_TPFSI_RCS_BYPASS_DC(l_data64_rc5);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

            FAPI_DBG("Clear RCS errors");
            l_data64_rc5.flush<0>();
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
            SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_CLEAR_CLK_ERROR_A(l_data64_rc5);
            SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_CLEAR_CLK_ERROR_B(l_data64_rc5);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

            //Do not clear l_data64_rc5 here
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

            //read back the RCS sens register
            GET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_SNS1LTH_RO(i_target_chip, l_data64_rcsns);

            FAPI_DBG("deskew loop 0x%02X, rcs sens register %#018lX", l_deskewIndex, l_data64_rcsns);

            //Check if there was a clock error
            l_clkErrA = l_data64_rcsns.getBit<6>();
            l_clkErrB = l_data64_rcsns.getBit<8>();


            //The clock errors encountered for each deskew will be put into a bit string with
            //0th deskew forming the leftmost bit, the 1st deskew froming the 2nd bit from left and
            //son on.
            l_clkErrVals += (l_clkErrA | l_clkErrB) << (7 - l_deskew_buf);
            FAPI_DBG("deskew loop 0x%02X, l_clkErrA 0x%02X, l_clkErrB 0x%02X, l_clkErrVals 0x%02X",
                     l_deskewIndex, l_clkErrA, l_clkErrB, l_clkErrVals);

        }//end of deskew range for

        //Arrange the bit string values one after the other in-order at arrive at the
        //pattern from which three consecutive deskew that did not produce an error will be searched.
        l_shiftedErrVals = (l_clkErrVals & 0xFF) | ((l_clkErrVals << 8) & 0xFF00);

        p10_get_deskew(l_shiftedErrVals, l_goodDeskew);

        if(l_goodDeskew != INVALID_DESKEW)
        {
            FAPI_INF("Found  good deskew 0x%02x, loopcount 0x%02X", l_goodDeskew, l_index);
            break;
        }
        else
        {
            FAPI_INF("Valid deskew not found, loop count 0x%02x", l_index);
        }

        //reset the RCS PLL before entering the new loop
        l_data64_rc3.flush<0>();
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_FSI(i_target_chip));
        SET_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_TP_PLLCLKSW1_RESET_DC(l_data64_rc3);
        SET_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_TP_PLLCLKSW2_RESET_DC(l_data64_rc3);
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR(i_target_chip, l_data64_rc3));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));
        FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_FSI(i_target_chip));
        FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc3));
    }// End of loop to find the deskew

    // As per new error xml, this assert need clock pos and shifted error value for both
    // clock A and clock B
    // We cannot put here, because DD1 doesnt have seperate deskew setting for clock-A and
    // clock-B.
    //
    // Ignoring this problem, since this part of code will never execute, since redundacny
    // mode will not be requested for DD1 chip.
    GET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_SNS1LTH_RO(i_target_chip, l_data64_rcsns);
    FAPI_ASSERT((l_goodDeskew != INVALID_DESKEW),
                fapi2::RCS_FPLL_DESKEW_ERR()
                .set_READ_SNS1LTH(l_data64_rcsns)
                .set_SHIFTED_A_ERR_VAL(l_shiftedErrVals),
                "Deskew caliberation failed");

    //Set deskew to 0
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
    l_deskew_buf.flush<0>();
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_DESKEW(0x7, l_deskew_buf);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_deskew_buf));

    //set the newly determined deskew val
    l_deskew_buf = l_goodDeskew;
    l_data64_rc5.flush<0>();
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
    SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_DESKEW(l_deskew_buf, l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

    //Put RCS into RESET
    l_data64_rc5.flush<0>();
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_TPFSI_RCS_RESET_DC(l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

    //set RCS BYPASS
    l_data64_rc5.flush<0>();
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_TPFSI_RCS_BYPASS_DC(l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

    //take RCS out of RESET
    l_data64_rc5.flush<0>();
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip));
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_TPFSI_RCS_RESET_DC(l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

    //clear BYPASS
    l_data64_rc5.flush<0>();
    SET_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_TPFSI_RCS_BYPASS_DC(l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR(i_target_chip, l_data64_rc5));

    //set the sel_del val
    l_data64_rc5.flush<0>();
    FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
    SET_TP_TPVSB_FSI_W_MAILBOX_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_SEL_DEL(l_data64_rc5);
    FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

    FAPI_INF("p10_sbe_rcs_deskew_calibrate: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------
// RCS Setup Routine
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_sbe_rcs_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;
    using namespace scomt::proc;

    fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133_Type l_filter_pll_hw540133;
    fapi2::ATTR_CHIP_EC_FEATURE_HW545231_Type l_rcs_hw545231;
    fapi2::buffer<uint64_t> l_data64_rc5, l_data64_rc3, l_pll_expect, l_pll_status, l_rcs_status, buffer64;
    fapi2::ReturnCode l_rc;
    uint8_t l_cp_refclck_select;
    bool skipClkCheck = false;
    // variable for creating clock pos ffdc
    uint8_t l_callout_clock;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];


    FAPI_INF("p10_sbe_rcs_setup: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133,
                           i_target_chip, l_filter_pll_hw540133));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CP_REFCLOCK_SELECT, i_target_chip, l_cp_refclck_select),
             "Error from FAPI_ATTR_GET (ATTR_CP_REFCLOCK_SELECT)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW545231,
                           i_target_chip, l_rcs_hw545231));

    FAPI_DBG("Set up RCS filter PLL altrefclk selects");
    FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_RW, l_data64_rc3));
    l_data64_rc3.writeBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_ALTREFCLK_SEL_DC>((l_cp_refclck_select ==
            fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1));
    l_data64_rc3.writeBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_ALTREFCLK_SEL_DC>((l_cp_refclck_select ==
            fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0));
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_RW, l_data64_rc3));

    if(((l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0) ||
        (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1) ||
        (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0_NORED) ||
        (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1_NORED)))
    {

        FAPI_DBG("Set up RCS configuration for sync mode, preserve bypass select bits");
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR, RCS_CONTRL5_DC_CFAM_RESET_VAL));

        for(int i = 0; i < POLL_COUNT; i++)
        {
            FAPI_DBG("Set input valuse to clock test latches - RCS_CLOCK_TEST_IN = 1");
            FAPI_TRY(p10_sbe_rcs_setup_test_latches(i_target_chip, l_cp_refclck_select, true));

            FAPI_DBG("Set input valuse to clock test latches - RCS_CLOCK_TEST_IN = 0");
            FAPI_TRY(p10_sbe_rcs_setup_test_latches(i_target_chip, l_cp_refclck_select, false));
        }

        FAPI_DBG("Release RCS PLLs test enable");

        l_data64_rc3.flush<0>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_TEST_EN_DC>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_TEST_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR, l_data64_rc3));

        FAPI_DBG("Release RCS PLLs reset & bypass ");

        l_data64_rc3.flush<0>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_RESET_DC>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_BYPASS_EN_DC>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_RESET_DC>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_BYPASS_EN_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR, l_data64_rc3));

        if (l_filter_pll_hw540133 && l_cp_refclck_select != fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1)
        {
            FAPI_TRY(hw540133::apply_workaround(l_tpchiplet, hw540133::rcs_pll0));
        }

        if (l_filter_pll_hw540133 && l_cp_refclck_select != fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0)
        {
            FAPI_TRY(hw540133::apply_workaround(l_tpchiplet, hw540133::rcs_pll1));
        }

        FAPI_DBG("Check for RCS PLL lock");
        l_pll_expect.flush<0>()
        .writeBit<0>(l_cp_refclck_select != fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1_NORED)
        .writeBit<1>(l_cp_refclck_select != fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0_NORED);

        l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_tpchiplet, l_pll_expect, (l_filter_pll_hw540133 == 0)
                                              && (l_pll_expect != 0), l_pll_status);
        FAPI_ASSERT(l_rc != fapi2::FAPI2_RC_FALSE,
                    fapi2::RCS_PLL_LOCK_ERR()
                    .set_TP_PLL_LOCK_REG(l_pll_status)
                    .set_ATTR_CP_REFCLOCK_SELECT_VALUE(l_cp_refclck_select),
                    "One or both RCS PLLs failed to lock! exp/got: "
                    "PLL_A: %d/%d  PLL_B: %d/%d",
                    l_pll_expect.getBit<0>(), l_pll_status.getBit<0>(),
                    l_pll_expect.getBit<1>(), l_pll_status.getBit<1>());
        FAPI_TRY(l_rc, "Failed to poll for PLL lock");

        FAPI_DBG("Put any failing RCS PLL back into reset+bypass");
        l_data64_rc3.flush<0>();

        if (!l_pll_status.getBit<0>())
        {
            l_data64_rc3
            .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_RESET_DC>()
            .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_BYPASS_EN_DC>();
        }

        if (!l_pll_status.getBit<1>())
        {
            l_data64_rc3
            .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_RESET_DC>()
            .setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_BYPASS_EN_DC>();
        }

        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_SET_WO_OR, l_data64_rc3));

        FAPI_DBG("Check what side RCS is on");
        FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SNS1LTH_RO, l_rcs_status));

        if(!l_rcs_hw545231)
        {
            //set the BLOCK_SWO bit for dd2 BEFORE coming out of RESET and BYPASS
            FAPI_DBG("Setting the BLOCK_SWO bit before exiting RESET and BYPASS");
            l_data64_rc5.flush<0>();
            FAPI_TRY(PREP_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip));
            SET_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_BLOCK_SWO(l_data64_rc5);
            FAPI_TRY(PUT_FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR(i_target_chip, l_data64_rc5));

            FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));
        }

        FAPI_DBG("Drop RCS reset");
        l_data64_rc5.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL5_TPFSI_RCS_RESET_DC>(); // rc5 bit0 = 0
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

        FAPI_DBG("RCS out of bypass");
        l_data64_rc5.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL5_TPFSI_RCS_BYPASS_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

        FAPI_DBG("Check what side RCS is on");
        FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SNS1LTH_RO, l_rcs_status));

        FAPI_TRY(p10_perv_sbe_cmn_is_simulation_check(skipClkCheck));

#ifdef __PPE__

        if (SBE::isSimicsRunning())
        {
            FAPI_INF("Skipping workaround because Simics does not support scanning");
            skipClkCheck = true;
        }

#endif

        if(!skipClkCheck)   // don't run deskew in simics because it will never find a metastability region
        {
            //Run the deskew algorithm to calibrate the RCS FPLL
            if(l_rcs_hw545231)
            {
                FAPI_TRY(p10_sbe_rcs_deskew_calibrate(i_target_chip));
            }
            else
            {
                FAPI_DBG("Run DD2 Deskew algo in RCS sync mode");
                FAPI_TRY(p10_sbe_rcs_dd2_deskew_calibrate(i_target_chip));

            }
        }

        FAPI_TRY(fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY));

        FAPI_DBG("Clear RCS errors");
        l_data64_rc5.flush<0>().setBit<6, 2>();
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR, l_data64_rc5));
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));


        if(!skipClkCheck)
        {
            FAPI_DBG("Check for clock errors");
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SNS1LTH_RO, l_rcs_status));


            if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1))
            {
                l_callout_clock = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0;
                FAPI_ASSERT(((l_rcs_status.getBit<0>() == 0)),
                            fapi2::RCS_CLOCK_ERR()
                            .set_MASTER_CHIP(i_target_chip)
                            .set_READ_SNS1LTH(l_rcs_status)
                            .set_CLOCK_POS(l_callout_clock)
                            .set_ATTR_CP_REFCLOCK_SELECT_VALUE(l_cp_refclck_select),
                            "RCS_CLOCK error : Clock A is bad");
            }

            if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0))
            {
                l_callout_clock = fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0;
                FAPI_ASSERT(((l_rcs_status.getBit<1>() == 0)),
                            fapi2::RCS_CLOCK_ERR()
                            .set_MASTER_CHIP(i_target_chip)
                            .set_READ_SNS1LTH(l_rcs_status)
                            .set_CLOCK_POS(l_callout_clock)
                            .set_ATTR_CP_REFCLOCK_SELECT_VALUE(l_cp_refclck_select),
                            "RCS_CLOCK error : Clock B is bad");
            }

        }

        // check to make sure we come out of RCS setup on the correct side
        FAPI_TRY(p10_sbe_rcs_check_side(i_target_chip, l_cp_refclck_select));


    }

    FAPI_INF("p10_sbe_rcs_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Verify that latches clocked by input clocks transported input value to output
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @param[in]     bool        RCS_CLOCK_TEST_IN
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_rcs_setup_test_latches(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    uint8_t i_cp_refclock_select,
    bool set_rcs_clock_test_in)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;

    l_data64.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL5_TPFSI_RCS_CLK_TEST_IN_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip,
                            set_rcs_clock_test_in ? FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR :
                            FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR,
                            l_data64));


    fapi2::delay(HW_NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SNS1LTH_RO, l_data64));
    FAPI_TRY(p10_clock_test_check_error(i_target_chip, i_cp_refclock_select, set_rcs_clock_test_in, l_data64 >> 32));

fapi_try_exit:
    return fapi2::current_err;

}
