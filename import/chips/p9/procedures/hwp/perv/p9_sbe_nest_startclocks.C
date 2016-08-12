/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/perv/p9_sbe_nest_startclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>


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

static fapi2::ReturnCode p9_sbe_nest_startclocks_N3_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector);

static fapi2::ReturnCode p9_sbe_nest_startclocks_get_attr_pg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    fapi2::buffer<uint32_t>& o_attr_pg);

static fapi2::ReturnCode p9_sbe_nest_startclocks_mc_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector);

static fapi2::ReturnCode p9_sbe_nest_startclocks_nest_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector);

fapi2::ReturnCode p9_sbe_nest_startclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    fapi2::buffer<uint8_t> l_read_flush_attr;
    fapi2::buffer<uint32_t> l_attr_pg;
    fapi2::buffer<uint64_t> l_pg_vector;
    fapi2::buffer<uint64_t> l_clock_regions;
    fapi2::buffer<uint64_t> l_n3_clock_regions;
    fapi2::buffer<uint16_t> l_ccstatus_regions;
    fapi2::buffer<uint16_t> l_n3_ccstatus_regions;
    FAPI_INF("p9_sbe_nest_startclocks: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_N3_FLUSH_MODE, i_target_chip,
                           l_read_flush_attr));

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                                           fapi2::TARGET_FILTER_TP), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_sbe_common_get_pg_vector(l_target_cplt, l_pg_vector));
        FAPI_DBG("pg targets vector: %#018lX", l_pg_vector);
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_target_cplt,
                 REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_n3_clock_regions));
        FAPI_DBG("pg targets vector: %#018lX", l_pg_vector);

        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(l_target_cplt,
                 REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_n3_ccstatus_regions));
        FAPI_DBG("pg targets vector: %#018lX", l_pg_vector);
    }

    FAPI_INF("Reading ATTR_MC_SYNC_MODE");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    fapi2::TargetFilter l_nest_filter, l_nest_tp_filter, l_dd1_filter_without_N3;

    if (l_read_attr)
    {
        l_nest_filter = static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                        fapi2::TARGET_FILTER_ALL_NEST);
        l_nest_tp_filter = static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC
                           | fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_TP);
        l_dd1_filter_without_N3 = static_cast<fapi2::TargetFilter>
                                  (fapi2::TARGET_FILTER_ALL_MC | fapi2::TARGET_FILTER_NEST_NORTH |
                                   fapi2::TARGET_FILTER_NEST_SOUTH | fapi2::TARGET_FILTER_NEST_EAST |
                                   fapi2::TARGET_FILTER_TP);
    }
    else
    {
        l_nest_filter = fapi2::TARGET_FILTER_ALL_NEST;
        l_nest_tp_filter = static_cast<fapi2::TargetFilter>
                           (fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_TP);
        l_dd1_filter_without_N3 = static_cast<fapi2::TargetFilter>
                                  (fapi2::TARGET_FILTER_NEST_NORTH | fapi2::TARGET_FILTER_NEST_SOUTH |
                                   fapi2::TARGET_FILTER_NEST_EAST | fapi2::TARGET_FILTER_TP);
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop chiplet fence for N3");
        FAPI_TRY(p9_sbe_nest_startclocks_N3_fence_drop(l_trgt_chplt, l_pg_vector));
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_NEST_NORTH |
                                           fapi2::TARGET_FILTER_NEST_SOUTH | fapi2::TARGET_FILTER_NEST_EAST),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Drop chiplet fence for N0,N1,N2");
        FAPI_TRY(p9_sbe_nest_startclocks_nest_fence_drop(l_trgt_chplt, l_pg_vector));
    }

    if ( l_read_attr )
    {
        for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
        {
            FAPI_DBG("Drop chiplet fence for MC");
            FAPI_TRY(p9_sbe_nest_startclocks_mc_fence_drop(l_trgt_chplt, l_pg_vector));
        }
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_nest_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_sbe_nest_startclocks_get_attr_pg(l_trgt_chplt, l_attr_pg));

        FAPI_DBG("Call common_cplt_ctrl_action_function for Nest and Mc chiplets");
        FAPI_TRY(p9_sbe_common_cplt_ctrl_action_function(l_trgt_chplt, l_attr_pg));
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_nest_tp_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Call module align chiplets for Nest and Mc chiplets");
        FAPI_TRY(p9_sbe_common_align_chiplets(l_trgt_chplt));
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_NEST_NORTH |
                                           fapi2::TARGET_FILTER_NEST_SOUTH | fapi2::TARGET_FILTER_NEST_EAST),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Regions value: %#018lX", l_clock_regions);
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_trgt_chplt,
                 REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_clock_regions));

        FAPI_DBG("Call module clock start stop for N0, N1, N2");
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD, STARTSLAVE,
                                                DONT_STARTMASTER, l_clock_regions, CLOCK_TYPES));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_target_cplt, CLOCK_CMD,
                                                DONT_STARTSLAVE, STARTMASTER, l_n3_clock_regions, CLOCK_TYPES));
        FAPI_DBG("pg targets vector: %#018lX", l_pg_vector);
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_NEST_NORTH |
                                           fapi2::TARGET_FILTER_NEST_SOUTH | fapi2::TARGET_FILTER_NEST_EAST),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(l_trgt_chplt,
                 REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_ccstatus_regions));
        FAPI_DBG("Regions value: %#018lX", l_ccstatus_regions);

        FAPI_DBG("Call clockstatus check function for N0,N1,N2");
        FAPI_TRY(p9_sbe_common_check_cc_status_function(l_trgt_chplt, CLOCK_CMD,
                 l_ccstatus_regions, CLOCK_TYPES));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Call clockstatus check function for N3");
        FAPI_TRY(p9_sbe_common_check_cc_status_function(l_target_cplt, CLOCK_CMD,
                 l_n3_ccstatus_regions, CLOCK_TYPES));
        FAPI_DBG("pg targets vector: %#018lX", l_pg_vector);
    }

    if ( l_read_attr )
    {
        for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
        {
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_trgt_chplt,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_clock_regions));
            FAPI_DBG("Regions value: %#018lX", l_clock_regions);

            FAPI_DBG("Call module clock start stop for MC01, MC23.");
            FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD,
                                                    DONT_STARTSLAVE, DONT_STARTMASTER, l_clock_regions, CLOCK_TYPES));
        }
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_nest_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Call common_check_checkstop_function for Nest and Mc chiplets ");
        FAPI_TRY(p9_sbe_common_check_checkstop_function(l_trgt_chplt));
    }

    if ( l_read_flush_attr )
    {
        for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (l_dd1_filter_without_N3, fapi2::TARGET_STATE_FUNCTIONAL))
        {
            FAPI_DBG("clear  flush_inhibit to go into flush mode");
            FAPI_TRY(p9_sbe_common_flushmode(l_trgt_chplt));
        }
    }
    else
    {
        for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (l_nest_tp_filter, fapi2::TARGET_STATE_FUNCTIONAL))
        {
            FAPI_DBG("clear  flush_inhibit to go into flush mode");
            FAPI_TRY(p9_sbe_common_flushmode(l_trgt_chplt));
        }
    }

    FAPI_INF("p9_sbe_nest_startclocks: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for OB chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     Pg vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_N3_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_nest_startclocks_N3_fence_drop: Entering ...");

    if ( i_pg_vector.getBit<0>() == 1 )
    {
        FAPI_DBG("Drop chiplet fence");
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 0
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
    }

    FAPI_INF("p9_sbe_nest_startclocks_N3_fence_drop: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief get attr_pg for the chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[out]    o_attr_pg       ATTR_PG for the chiplet
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_get_attr_pg(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    fapi2::buffer<uint32_t>& o_attr_pg)
{
    FAPI_INF("p9_sbe_nest_startclocks_get_attr_pg: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chip, o_attr_pg));

    FAPI_INF("p9_sbe_nest_startclocks_get_attr_pg: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for MC
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     Pg vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_mc_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector)
{
    uint8_t l_read_attrunitpos = 0;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_nest_startclocks_mc_fence_drop: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_target_chip,
                           l_read_attrunitpos));

    if ( l_read_attrunitpos == 0x07 )
    {
        if ( i_pg_vector.getBit<4>() == 1 )
        {
            FAPI_DBG("Drop chiplet fence");
            //Setting NET_CTRL0 register value
            l_data64.flush<1>();
            l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 0
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
        }
    }

    if ( l_read_attrunitpos == 0x08 )
    {
        if ( i_pg_vector.getBit<2>() == 1 )
        {
            FAPI_DBG("Drop chiplet fence");
            //Setting NET_CTRL0 register value
            l_data64.flush<1>();
            l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 0
            FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
        }
    }

    FAPI_INF("p9_sbe_nest_startclocks_mc_fence_drop: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Drop chiplet fence for pcie chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_pg_vector     Pg vector of targets
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_nest_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const fapi2::buffer<uint64_t> i_pg_vector)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_nest_startclocks_nest_fence_drop: Entering ...");

    if ( i_pg_vector.getBit<4>() == 1 )
    {
        FAPI_DBG("Drop chiplet fence");
        //Setting NET_CTRL0 register value
        l_data64.flush<1>();
        l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 0
        FAPI_TRY(fapi2::putScom(i_target_chip, PERV_NET_CTRL0_WAND, l_data64));
    }

    FAPI_INF("p9_sbe_nest_startclocks_nest_fence_drop: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
