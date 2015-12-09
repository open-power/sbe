/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_nest_startclocks.C $      */
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
/// @file  p9_sbe_nest_startclocks.C
///
/// @brief start PB and Nest clocks
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_nest_startclocks.H"
#include "p9_const_common.H"
#include "p9_misc_scom_addresses_fld.H"
#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include "p9_quad_scom_addresses_fld.H"
#include "p9_sbe_common.H"


enum P9_SBE_NEST_STARTCLOCKS_Private_Constants
{
    CLOCK_CMD = 0x1,
    STARTSLAVE = 0x1,
    STARTMASTER = 0x1,
    REGIONS_ALL_EXCEPT_VITAL_NESTPLL = 0x7FE,
    CLOCK_TYPES = 0x7,
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0
};

static fapi2::ReturnCode p9_sbe_nest_startclocks_check_cc_status_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    const fapi2::buffer<uint8_t> i_clock_cmd,
    const fapi2::buffer<uint16_t> i_regions,
    const fapi2::buffer<uint8_t> i_clock_types);

static fapi2::ReturnCode p9_sbe_nest_startclocks_check_checkstop_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_nest_startclocks_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

static fapi2::ReturnCode p9_sbe_nest_startclocks_flushmode(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_nest_startclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    FAPI_INF("Switch MC meshs to Nest mesh");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    if ( l_read_attr )
    {
        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x07
                   || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
                  (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */)))
            {
                continue;
            }

            FAPI_INF("Call p9_sbe_nest_startclocks_cplt_ctrl_action_function for Nest and Mc chiplets");
            FAPI_TRY(p9_sbe_nest_startclocks_cplt_ctrl_action_function(l_trgt_chplt));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x07
                   || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
                  (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
                  (l_attr_chip_unit_pos == 0x01/* TPChiplet */)))
            {
                continue;
            }

            FAPI_INF("Call module align chiplets for Nest and Mc chiplets");
            FAPI_TRY(p9_sbe_common_align_chiplets(l_trgt_chplt));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02/* N0Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x03/* N1Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x04/* N2Chiplet */)))
            {
                continue;
            }

            FAPI_INF("Call module clock start stop for N0, N1, N2");
            FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD, STARTSLAVE,
                                                    DONT_STARTMASTER, REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
        }

        FAPI_INF("Call module clock start stop for N3");

        // Get the N3Chiplet target
        for (auto it : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, it, l_attr_chip_unit_pos));

            if ((l_attr_chip_unit_pos == 0x05))/* N3Chiplet */
            {
                FAPI_TRY(p9_sbe_common_clock_start_stop(it, CLOCK_CMD, DONT_STARTSLAVE,
                                                        STARTMASTER, REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
                break;
            }
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02/* N0Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x03/* N1Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x04/* N2Chiplet */)))
            {
                continue;
            }

            FAPI_INF("Call clockstatus check function for N0,N1,N2");
            FAPI_TRY(p9_sbe_nest_startclocks_check_cc_status_function(l_trgt_chplt,
                     CLOCK_CMD, REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
        }

        FAPI_INF("Call clockstatus check function for N3");

        // Get the N3Chiplet target
        for (auto it : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, it, l_attr_chip_unit_pos));

            if ((l_attr_chip_unit_pos == 0x05))/* N3Chiplet */
            {
                FAPI_TRY(p9_sbe_nest_startclocks_check_cc_status_function(it, CLOCK_CMD,
                         REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
                break;
            }
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!(l_attr_chip_unit_pos == 0x07
                  || l_attr_chip_unit_pos == 0x08))/* McChiplet */
            {
                continue;
            }

            FAPI_INF("Call module clock start stop for MC01, MC23.");
            FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD,
                                                    DONT_STARTSLAVE, DONT_STARTMASTER, REGIONS_ALL_EXCEPT_VITAL_NESTPLL,
                                                    CLOCK_TYPES));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x07
                   || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
                  (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */)))
            {
                continue;
            }

            FAPI_INF("Call sbe_nest_startclocks_check_checkstop_function for Nest and Mc chiplets ");
            FAPI_TRY(p9_sbe_nest_startclocks_check_checkstop_function(l_trgt_chplt));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x07
                   || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
                  (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
                  (l_attr_chip_unit_pos == 0x01/* TPChiplet */)))
            {
                continue;
            }

            FAPI_TRY(p9_sbe_nest_startclocks_flushmode(l_trgt_chplt));
        }
    }
    else
    {
        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!(l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                  || l_attr_chip_unit_pos == 0x04
                  || l_attr_chip_unit_pos == 0x05))/* NestChiplet */
            {
                continue;
            }

            FAPI_INF("Call p9_sbe_nest_startclocks_cplt_ctrl_action_function for nest chiplets");
            FAPI_TRY(p9_sbe_nest_startclocks_cplt_ctrl_action_function(l_trgt_chplt));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
                  (l_attr_chip_unit_pos == 0x01/* TPChiplet */)))
            {
                continue;
            }

            FAPI_INF("call module align chiplets for nest chiplets");
            FAPI_TRY(p9_sbe_common_align_chiplets(l_trgt_chplt));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02/* N0Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x03/* N1Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x04/* N2Chiplet */)))
            {
                continue;
            }

            FAPI_INF("Call module clock start stop for N0, N1, N2");
            FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD, STARTSLAVE,
                                                    DONT_STARTMASTER, REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
        }

        FAPI_INF("Call module clock start stop for N3");

        // Get the N3Chiplet target
        for (auto it : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, it, l_attr_chip_unit_pos));

            if ((l_attr_chip_unit_pos == 0x05))/* N3Chiplet */
            {
                FAPI_TRY(p9_sbe_common_clock_start_stop(it, CLOCK_CMD, DONT_STARTSLAVE,
                                                        STARTMASTER, REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
                break;
            }
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02/* N0Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x03/* N1Chiplet */) ||
                  (l_attr_chip_unit_pos == 0x04/* N2Chiplet */)))
            {
                continue;
            }

            FAPI_INF("Call clockstatus check function for N0,N1,N2");
            FAPI_TRY(p9_sbe_nest_startclocks_check_cc_status_function(l_trgt_chplt,
                     CLOCK_CMD, REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
        }

        FAPI_INF("Call clockstatus check function for N3");

        // Get the N3Chiplet target
        for (auto it : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, it, l_attr_chip_unit_pos));

            if ((l_attr_chip_unit_pos == 0x05))/* N3Chiplet */
            {
                FAPI_TRY(p9_sbe_nest_startclocks_check_cc_status_function(it, CLOCK_CMD,
                         REGIONS_ALL_EXCEPT_VITAL_NESTPLL, CLOCK_TYPES));
                break;
            }
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!(l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                  || l_attr_chip_unit_pos == 0x04
                  || l_attr_chip_unit_pos == 0x05))/* NestChiplet */
            {
                continue;
            }

            FAPI_INF("call sbe_nest_startclocks_check_checkstop_function for nest chiplets");
            FAPI_TRY(p9_sbe_nest_startclocks_check_checkstop_function(l_trgt_chplt));
        }

        for (auto l_trgt_chplt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_trgt_chplt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
                  (l_attr_chip_unit_pos == 0x01/* TPChiplet */)))
            {
                continue;
            }

            FAPI_TRY(p9_sbe_nest_startclocks_flushmode(l_trgt_chplt));
        }
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief To do check on Clock controller status for Nest chiplets
///
/// @param[in]     i_target        Reference to TARGET_TYPE_PERV target Reference to TARGET_TYPE_PERV target
/// @param[in]     i_clock_cmd     Issue clock controller command (START/STOP)
/// @param[in]     i_regions       Enable required REGIONS
/// @param[in]     i_clock_types   Clock Types to be selected (SL/NSL/ARY)
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_check_cc_status_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    const fapi2::buffer<uint8_t> i_clock_cmd,
    const fapi2::buffer<uint16_t> i_regions,
    const fapi2::buffer<uint8_t> i_clock_types)
{
    bool l_reg_sl = false;
    bool l_reg_nsl = false;
    bool l_reg_ary = false;
    fapi2::buffer<uint64_t> l_sl_clock_status;
    fapi2::buffer<uint64_t> l_nsl_clock_status;
    fapi2::buffer<uint64_t> l_ary_clock_status;
    fapi2::buffer<uint16_t> l_sl_clkregion_status;
    fapi2::buffer<uint16_t> l_nsl_clkregion_status;
    fapi2::buffer<uint16_t> l_ary_clkregion_status;
    fapi2::buffer<uint16_t> l_regions;
    FAPI_DBG("Entering ...");

    l_reg_sl = i_clock_types.getBit<5>();
    l_reg_nsl = i_clock_types.getBit<6>();
    l_reg_ary = i_clock_types.getBit<7>();
    i_regions.extractToRight<5, 11>(l_regions);

    if ( l_reg_sl )
    {
        FAPI_INF("Check for Clocks running SL");
        //Getting CLOCK_STAT_SL register value
        FAPI_TRY(fapi2::getScom(i_target, PERV_CLOCK_STAT_SL,
                                l_sl_clock_status)); //l_sl_clock_status = CLOCK_STAT_SL
        FAPI_INF("SL Clock status register is %#018lX", l_sl_clock_status);

        if ( i_clock_cmd == 0b01 )
        {
            FAPI_INF("Checking for clock start command");
            l_sl_clkregion_status.flush<1>();
            l_sl_clock_status.extractToRight<4, 11>(l_sl_clkregion_status);
            l_sl_clkregion_status.invert();
            l_sl_clkregion_status &= l_regions;

            FAPI_ASSERT(l_sl_clkregion_status == l_regions,
                        fapi2::NEST_SL_ERR()
                        .set_READ_CLK_SL(l_sl_clock_status),
                        "Clock running for sl type not matching with expected values");
        }

        if ( i_clock_cmd == 0b10 )
        {
            FAPI_INF("Checking for clock stop command");
            l_sl_clkregion_status.flush<0>();
            l_sl_clock_status.extractToRight<4, 11>(l_sl_clkregion_status);
            l_sl_clkregion_status &= l_regions;

            FAPI_ASSERT(l_sl_clkregion_status == l_regions,
                        fapi2::NEST_SL_ERR()
                        .set_READ_CLK_SL(l_sl_clock_status),
                        "Clock running for sl type not matching with expected values");
        }
    }

    if ( l_reg_nsl )
    {
        FAPI_INF("Check for clocks running NSL");
        //Getting CLOCK_STAT_NSL register value
        FAPI_TRY(fapi2::getScom(i_target, PERV_CLOCK_STAT_NSL,
                                l_nsl_clock_status)); //l_nsl_clock_status = CLOCK_STAT_NSL
        FAPI_INF("NSL Clock status register is %#018lX", l_nsl_clock_status);

        if ( i_clock_cmd == 0b01 )
        {
            FAPI_INF("Checking for clock start command");
            l_nsl_clkregion_status.flush<1>();
            l_nsl_clock_status.extractToRight<4, 11>(l_nsl_clkregion_status);
            l_nsl_clkregion_status.invert();
            l_nsl_clkregion_status &= l_regions;

            FAPI_ASSERT(l_nsl_clkregion_status == l_regions,
                        fapi2::NEST_NSL_ERR()
                        .set_READ_CLK_NSL(l_nsl_clock_status),
                        "Clock running for nsl type not matching with expected values");
        }

        if ( i_clock_cmd == 0b10 )
        {
            FAPI_INF("Checking for clock stop command");
            l_nsl_clkregion_status.flush<0>();
            l_nsl_clock_status.extractToRight<4, 11>(l_nsl_clkregion_status);
            l_nsl_clkregion_status &= l_regions;

            FAPI_ASSERT(l_nsl_clkregion_status == l_regions,
                        fapi2::NEST_NSL_ERR()
                        .set_READ_CLK_NSL(l_nsl_clock_status),
                        "Clock running for nsl type not matching with expected values");
        }
    }

    if ( l_reg_ary )
    {
        FAPI_INF("Check for clocks running ARY");
        //Getting CLOCK_STAT_ARY register value
        FAPI_TRY(fapi2::getScom(i_target, PERV_CLOCK_STAT_ARY,
                                l_ary_clock_status)); //l_ary_clock_status = CLOCK_STAT_ARY
        FAPI_INF("ARY Clock status register is %#018lX", l_ary_clock_status);

        if ( i_clock_cmd == 0b01 )
        {
            FAPI_INF("Checking for clock start command");
            l_ary_clkregion_status.flush<1>();
            l_ary_clock_status.extractToRight<4, 11>(l_ary_clkregion_status);
            l_ary_clkregion_status.invert();
            l_ary_clkregion_status &= l_regions;

            FAPI_ASSERT(l_ary_clkregion_status == l_regions,
                        fapi2::NEST_ARY_ERR()
                        .set_READ_CLK_ARY(l_ary_clock_status),
                        "Clock running for ary type not matching with expected values");
        }

        if ( i_clock_cmd == 0b10 )
        {
            FAPI_INF("Checking for clock stop command");
            l_ary_clkregion_status.flush<0>();
            l_ary_clock_status.extractToRight<4, 11>(l_ary_clkregion_status);
            l_ary_clkregion_status &= l_regions;

            FAPI_ASSERT(l_ary_clkregion_status == l_regions,
                        fapi2::NEST_ARY_ERR()
                        .set_READ_CLK_ARY(l_ary_clock_status),
                        "Clock running for ary type not matching with expected values");
        }
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --drop chiplet fence
/// --check checkstop register
/// --clear flush inhibit to go into flush mode
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_check_checkstop_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_read_reg;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    FAPI_INF("Drop chiplet fence");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<PEC_STACK0_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("Check checkstop register");
    //Getting XFIR register value
    FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_XFIR,
                            l_read_reg)); //l_read_reg = XFIR

    FAPI_ASSERT(l_read_reg == 0,
                fapi2::READ_ALL_CHECKSTOP_ERR()
                .set_READ_ALL_CHECKSTOP(l_read_reg),
                "ERROR: COMBINE ALL CHECKSTOP ERROR");

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief --drop vital fence
/// --reset abstclk muxsel and syncclk muxsel
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    // Local variable and constant definition
    fapi2::buffer <uint32_t> l_attr_pg;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();

    FAPI_INF("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    //CPLT_CTRL1.TC_VITL_REGION_FENCE = l_attr_pg.getBit<19>()
    l_data64.writeBit<PEC_CPLT_CTRL1_TC_VITL_REGION_FENCE>(l_attr_pg.getBit<19>());
    //CPLT_CTRL1.TC_PERV_REGION_FENCE = l_attr_pg.getBit<20>()
    l_data64.writeBit<PEC_CPLT_CTRL1_TC_PERV_REGION_FENCE>(l_attr_pg.getBit<20>());
    //CPLT_CTRL1.TC_REGION1_FENCE = l_attr_pg.getBit<21>()
    l_data64.writeBit<PEC_CPLT_CTRL1_TC_REGION1_FENCE>(l_attr_pg.getBit<21>());
    //CPLT_CTRL1.TC_REGION2_FENCE = l_attr_pg.getBit<22>()
    l_data64.writeBit<PEC_CPLT_CTRL1_TC_REGION2_FENCE>(l_attr_pg.getBit<22>());
    //CPLT_CTRL1.TC_REGION3_FENCE = l_attr_pg.getBit<23>()
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_REGION3_FENCE>(l_attr_pg.getBit<23>());
    //CPLT_CTRL1.TC_REGION4_FENCE = l_attr_pg.getBit<24>()
    l_data64.writeBit<EQ_CPLT_CTRL1_TC_REGION4_FENCE>(l_attr_pg.getBit<24>());
    //CPLT_CTRL1.TC_REGION5_FENCE = l_attr_pg.getBit<25>()
    l_data64.writeBit<EQ_CPLT_CTRL1_TC_REGION5_FENCE>(l_attr_pg.getBit<25>());
    //CPLT_CTRL1.TC_REGION6_FENCE = l_attr_pg.getBit<26>()
    l_data64.writeBit<EQ_CPLT_CTRL1_TC_REGION6_FENCE>(l_attr_pg.getBit<26>());
    //CPLT_CTRL1.TC_REGION7_FENCE = l_attr_pg.getBit<27>()
    l_data64.writeBit<EQ_CPLT_CTRL1_TC_REGION7_FENCE>(l_attr_pg.getBit<27>());
    //CPLT_CTRL1.UNUSED_12B = l_attr_pg.getBit<28>()
    l_data64.writeBit<PEC_CPLT_CTRL1_UNUSED_12B>(l_attr_pg.getBit<28>());
    //CPLT_CTRL1.UNUSED_13B = l_attr_pg.getBit<29>()
    l_data64.writeBit<PEC_CPLT_CTRL1_UNUSED_13B>(l_attr_pg.getBit<29>());
    //CPLT_CTRL1.UNUSED_14B = l_attr_pg.getBit<30>()
    l_data64.writeBit<PEC_CPLT_CTRL1_UNUSED_14B>(l_attr_pg.getBit<30>());
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_INF("reset abistclk_muxsel and syncclk_muxsel");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_ABSTCLK_MUXSEL_DC = 1
    l_data64.writeBit<PEC_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>(1);
    //CPLT_CTRL0.TC_UNIT_SYNCCLK_MUXSEL_DC = 1
    l_data64.writeBit<PEC_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC>(1);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief will force all chiplets out of flush
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_flushmode(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_DBG("Entering ...");

    FAPI_INF("Clear flush_inhibit to go in to flush mode");
    //Setting CPLT_CTRL0 register value
    l_data64.flush<0>();
    //CPLT_CTRL0.CTRL_CC_FLUSHMODE_INH_DC = 0
    l_data64.setBit<PEC_CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
