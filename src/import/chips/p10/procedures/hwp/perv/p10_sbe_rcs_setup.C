/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_rcs_setup.C $ */
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
#include <target_filters.H>
#include "p10_perv_sbe_cmn.H"
#include "hw540133.H"

enum P10_SBE_RCS_SETUP_Private_Constants
{
    RCS_CONTRL_DC_CFAM_RESET_VAL = 0x0010000000000000,
    HW_NS_DELAY = 20, // unit is nano seconds
    SIM_CYCLE_DELAY = 100000, // unit is sim cycles
    POLL_COUNT = 10,
    RCS_BYPASS_NS_DELAY = 10000, // unit is nano seconds
    RCS_BYPASS_SIM_CYCLE_DELAY = 100, // unit is sim cycles
    RCS_RESET_NS_DELAY = 1000, // unit is nano seconds
    RCS_RESET_SIM_CYCLE_DELAY = 100  // unit is sim cycles
};

static fapi2::ReturnCode p10_sbe_rcs_setup_test_latches(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
    bool set_rcs_clock_test_in,
    uint8_t attr_cp_refclock_select);

fapi2::ReturnCode p10_sbe_rcs_setup(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::ATTR_CHIP_EC_FEATURE_FILTER_PLL_HW540133_Type l_filter_pll_hw540133;
    fapi2::ATTR_CHIP_EC_FEATURE_HW545231_Type l_rcs_hw545231;
    fapi2::buffer<uint64_t> l_data64_rc5, l_data64_rc3, l_pll_expect, l_read_reg;
    fapi2::ReturnCode l_rc;
    uint8_t l_cp_refclck_select;
    bool skipClkCheck = false;

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

    if((l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC0) ||
       (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_BOTH_OSC1))
    {

        FAPI_DBG("Set up RCS configuration for sync mode, preserve bypass select bits");
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR, RCS_CONTRL_DC_CFAM_RESET_VAL));

        for(int i = 0; i < POLL_COUNT; i++)
        {
            FAPI_DBG("Set input valuse to clock test latches - RCS_CLOCK_TEST_IN = 1");
            FAPI_TRY(p10_sbe_rcs_setup_test_latches(i_target_chip, true, l_cp_refclck_select));

            FAPI_DBG("Set input valuse to clock test latches - RCS_CLOCK_TEST_IN = 0");
            FAPI_TRY(p10_sbe_rcs_setup_test_latches(i_target_chip, false, l_cp_refclck_select));
        }

        FAPI_DBG("Release RCS PLLs test enable");

        l_data64_rc3.flush<0>();

        if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1))
        {
            l_data64_rc3.setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_TEST_EN_DC>(); // RC3 bit2 = 0
        }

        if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0))
        {
            l_data64_rc3.setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_TEST_EN_DC>(); // RC3 bit6 = 0
        }

        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL3_CLEAR_WO_CLEAR, l_data64_rc3));

        FAPI_DBG("Release RCS PLLs reset & bypass ");

        l_data64_rc3.flush<0>();

        if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1))
        {
            l_data64_rc3.setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_RESET_DC>();     // RC3 bit0 = 0
            l_data64_rc3.setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW1_BYPASS_EN_DC>(); // RC3 bit1 = 0
        }

        if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0))
        {
            l_data64_rc3.setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_RESET_DC>();     // RC3 bit4 = 0
            l_data64_rc3.setBit<FSXCOMP_FSXLOG_ROOT_CTRL3_TP_PLLCLKSW2_BYPASS_EN_DC>(); // RC3 bit5 = 0
        }

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
        .writeBit<0>(! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1))
        .writeBit<1>(! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0));

        l_rc = p10_perv_sbe_cmn_poll_pll_lock(l_tpchiplet, l_pll_expect, l_read_reg);
        FAPI_ASSERT(l_rc != fapi2::FAPI2_RC_FALSE,
                    fapi2::RCS_PLL_LOCK_ERR()
                    .set_TP_PLL_LOCK_REG(l_read_reg)
                    .set_ATTR_CP_REFCLOCK_SELECT_VALUE(l_cp_refclck_select),
                    "One or both RCS PLLs failed to lock! exp/got: "
                    "PLL_A: %d/%d  PLL_B: %d/%d",
                    l_pll_expect.getBit<0>(), l_read_reg.getBit<0>(),
                    l_pll_expect.getBit<1>(), l_read_reg.getBit<1>());
        FAPI_TRY(l_rc, "Failed to poll for PLL lock");

        FAPI_DBG("Drop RCS reset");
        l_data64_rc5.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL5_TPFSI_RCS_RESET_DC>(); // rc5 bit0 = 0
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

        fapi2::delay(RCS_RESET_NS_DELAY, RCS_RESET_SIM_CYCLE_DELAY);

        FAPI_DBG("RCS out of bypass");
        l_data64_rc5.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL5_TPFSI_RCS_BYPASS_DC>();
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

        fapi2::delay(RCS_BYPASS_NS_DELAY, RCS_BYPASS_SIM_CYCLE_DELAY);

        FAPI_DBG("Clear RCS errors");
        l_data64_rc5.flush<0>().setBit<6, 2>();
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR, l_data64_rc5));
        FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR, l_data64_rc5));

        if(!l_rcs_hw545231)
        {
            FAPI_TRY(p10_perv_sbe_cmn_is_simulation_check(skipClkCheck));


            if(!skipClkCheck)
            {

                FAPI_DBG("Check for clock errors");
                FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SNS1LTH_RO, l_read_reg));

                if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1))
                {
                    FAPI_ASSERT(((l_read_reg.getBit<0>() == 0) && (l_read_reg.getBit<2>() == 0)),
                                fapi2::RCS_CLOCK_ERR()
                                .set_READ_SNS1LTH(l_read_reg)
                                .set_ATTR_CP_REFCLOCK_SELECT_VALUE(l_cp_refclck_select),
                                "RCS_CLOCK error : Clock A is bad");
                }

                if (! (l_cp_refclck_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0))
                {
                    FAPI_ASSERT(((l_read_reg.getBit<1>() == 0) && (l_read_reg.getBit<3>() == 0)),
                                fapi2::RCS_CLOCK_ERR()
                                .set_READ_SNS1LTH(l_read_reg)
                                .set_ATTR_CP_REFCLOCK_SELECT_VALUE(l_cp_refclck_select),
                                "RCS_CLOCK error : Clock B is bad");
                }
            }
        }
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
    bool set_rcs_clock_test_in,
    uint8_t i_attr_cp_refclock_select)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    bool check_clockA;
    bool check_clockB;

    FAPI_INF("p10_sbe_rcs_setup_test_latches: Entering ...");

    l_data64.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL5_TPFSI_RCS_CLK_TEST_IN_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip,
                            set_rcs_clock_test_in ? FSXCOMP_FSXLOG_ROOT_CTRL5_SET_WO_OR :
                            FSXCOMP_FSXLOG_ROOT_CTRL5_CLEAR_WO_CLEAR,
                            l_data64));

    fapi2::delay(HW_NS_DELAY, SIM_CYCLE_DELAY);

    FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_SNS1LTH_RO, l_data64));

    check_clockA = set_rcs_clock_test_in ? (l_data64.getBit<4>() == 1) : (l_data64.getBit<4>() == 0) ;
    check_clockB = set_rcs_clock_test_in ? (l_data64.getBit<5>() == 1) : (l_data64.getBit<5>() == 0) ;

    if (! (i_attr_cp_refclock_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC1))
    {

        FAPI_ASSERT(check_clockA,
                    fapi2::RCS_CLOCK_TEST_OUT_ERR()
                    .set_READ_SNS1LTH(l_data64)
                    .set_ATTR_CP_REFCLOCK_SELECT_VALUE(i_attr_cp_refclock_select)
                    .set_RCS_CLOCK_TEST_IN(set_rcs_clock_test_in),
                    "RCS_CLOCK_TEST_IN_OUT error : Clock A is bad");
    }

    if (! (i_attr_cp_refclock_select == fapi2::ENUM_ATTR_CP_REFCLOCK_SELECT_OSC0))
    {

        FAPI_ASSERT(check_clockB,
                    fapi2::RCS_CLOCK_TEST_OUT_ERR()
                    .set_READ_SNS1LTH(l_data64)
                    .set_ATTR_CP_REFCLOCK_SELECT_VALUE(i_attr_cp_refclock_select)
                    .set_RCS_CLOCK_TEST_IN(set_rcs_clock_test_in),
                    "RCS_CLOCK_TEST_IN_OUT error : Clock B is bad");
    }


    FAPI_INF("p10_sbe_rcs_setup_test_latches: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
