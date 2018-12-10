/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_nest_startclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
// *HWP Level           : 3
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
#include <target_filters.H>

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

static fapi2::ReturnCode p9_sbe_nest_startclocks_nest_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p9_sbe_nest_startclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    uint8_t l_use_dmi_buckets = 0;
    fapi2::buffer<uint8_t> l_read_flush_attr;
    fapi2::buffer<uint16_t> l_attr_pg;
    fapi2::buffer<uint64_t> l_pg_vector;
    fapi2::buffer<uint64_t> l_clock_regions;
    fapi2::buffer<uint64_t> l_n3_clock_regions;
    fapi2::buffer<uint16_t> l_ccstatus_regions;
    fapi2::buffer<uint16_t> l_n3_ccstatus_regions;
    FAPI_INF("p9_sbe_nest_startclocks: Entering ...");

    auto l_perv_nest_mc_func = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                   static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                           fapi2::TARGET_FILTER_ALL_NEST | fapi2::TARGET_FILTER_TP),
                                   fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_N3_FLUSH_MODE, i_target_chip,
                           l_read_flush_attr));

    FAPI_TRY(p9_sbe_common_get_pg_vector(i_target_chip, l_pg_vector));
    FAPI_DBG("pg targets vector: %#018lX", l_pg_vector);

    FAPI_INF("Reading ATTR_MC_SYNC_MODE");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    FAPI_INF("Read ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS ");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_DMI_MC_PLL_SCAN_BUCKETS, i_target_chip, l_use_dmi_buckets));

    l_read_attr |= l_use_dmi_buckets;

    // NEST WEST
    for (auto& nest : l_perv_nest_mc_func)
    {
        if(nest.getChipletNumber() == N3_CHIPLET_ID)
        {
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(nest,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_n3_clock_regions));

            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(nest,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_n3_ccstatus_regions));
            break;
        }
    }

    // All NEST & MC (Check PG bit respectively)
    for (auto& perv : l_perv_nest_mc_func)
    {
        uint32_t l_chipletID = perv.getChipletNumber();

        if( ((l_chipletID == N3_CHIPLET_ID) && (l_pg_vector.getBit<1>() == 1)) ||
            ((l_chipletID == N0_CHIPLET_ID || l_chipletID == N1_CHIPLET_ID || l_chipletID == N2_CHIPLET_ID)
             && (l_pg_vector.getBit<5>() == 1)) ||
            (l_read_attr && ((l_chipletID == MC01_CHIPLET_ID) && (l_pg_vector.getBit<5>() == 1))) ||
            (l_read_attr && (l_chipletID == MC23_CHIPLET_ID) && (l_pg_vector.getBit<3>() == 1)) )
        {
            FAPI_DBG("Drop chiplet fence for N3 // N0,N1,N2 // MC");
            FAPI_TRY(p9_sbe_nest_startclocks_nest_fence_drop(perv));
        }
    }

    for (auto& perv : l_perv_nest_mc_func)
    {
        uint32_t l_chipletID = perv.getChipletNumber();

        if(!l_read_attr && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID))
        {
            continue;
        }

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, perv, l_attr_pg));

        FAPI_DBG("Call common_cplt_ctrl_action_function for Nest and Mc chiplets");
        FAPI_TRY(p9_sbe_common_cplt_ctrl_action_function(perv, l_attr_pg));

        FAPI_DBG("Call module align chiplets for Nest and Mc chiplets");
        FAPI_TRY(p9_sbe_common_align_chiplets(perv));
    }

    for (auto& perv : l_perv_nest_mc_func)
    {
        uint32_t l_chipletID = perv.getChipletNumber();

        if(l_chipletID == N0_CHIPLET_ID || l_chipletID == N1_CHIPLET_ID || l_chipletID == N2_CHIPLET_ID)
        {
            FAPI_DBG("Regions value: %#018lX", l_clock_regions);
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(perv,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_clock_regions));

            FAPI_DBG("Call module clock start stop for N0, N1, N2");
            FAPI_TRY(p9_sbe_common_clock_start_stop(perv, CLOCK_CMD, STARTSLAVE,
                                                    DONT_STARTMASTER, l_clock_regions, CLOCK_TYPES));
        }

        if(l_chipletID == N3_CHIPLET_ID)
        {
            FAPI_TRY(p9_sbe_common_clock_start_stop(perv, CLOCK_CMD,
                                                    DONT_STARTSLAVE, STARTMASTER, l_n3_clock_regions, CLOCK_TYPES));
        }
    }

    for (auto& perv : l_perv_nest_mc_func)
    {
        uint32_t l_chipletID = perv.getChipletNumber();

        if(l_chipletID == N0_CHIPLET_ID || l_chipletID == N1_CHIPLET_ID || l_chipletID == N2_CHIPLET_ID)
        {
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(perv,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_ccstatus_regions));
            FAPI_DBG("Regions value: %#018lX", l_ccstatus_regions);

            FAPI_DBG("Call clockstatus check function for N0,N1,N2");
            FAPI_TRY(p9_sbe_common_check_cc_status_function(perv, CLOCK_CMD,
                     l_ccstatus_regions, CLOCK_TYPES));
        }

        if(l_chipletID == N3_CHIPLET_ID)
        {
            FAPI_DBG("Call clockstatus check function for N3");
            FAPI_TRY(p9_sbe_common_check_cc_status_function(perv, CLOCK_CMD,
                     l_n3_ccstatus_regions, CLOCK_TYPES));
        }
    }

    for (auto& perv : l_perv_nest_mc_func)
    {
        // MC
        uint32_t l_chipletID = perv.getChipletNumber();

        if( l_read_attr && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID) )
        {
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(perv,
                     REGIONS_ALL_EXCEPT_VITAL_NESTPLL, l_clock_regions));
            FAPI_DBG("Regions value: %#018lX", l_clock_regions);

            FAPI_DBG("Call module clock start stop for MC01, MC23.");
            FAPI_TRY(p9_sbe_common_clock_start_stop(perv, CLOCK_CMD,
                                                    DONT_STARTSLAVE, DONT_STARTMASTER, l_clock_regions, CLOCK_TYPES));
        }
    }

    for (auto& perv : l_perv_nest_mc_func)
    {
        uint32_t l_chipletID = perv.getChipletNumber();

        if(l_chipletID == N3_CHIPLET_ID && l_read_flush_attr)
        {
            continue;
        }

        if(!l_read_attr && (l_chipletID == MC01_CHIPLET_ID || l_chipletID == MC23_CHIPLET_ID))
        {
            continue;
        }

        FAPI_DBG("clear  flush_inhibit to go into flush mode");
        FAPI_TRY(p9_sbe_common_flushmode(perv));
    }

    FAPI_INF("p9_sbe_nest_startclocks: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Drop chiplet fence for perv chiplet (MC, NEST)
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p9_sbe_nest_startclocks_nest_fence_drop(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_nest_startclocks_nest_fence_drop: Entering ...");

    FAPI_DBG("Drop chiplet fence");
    //Setting NET_CTRL0 register value
    l_data64.flush<1>();
    l_data64.clearBit<PERV_1_NET_CTRL0_FENCE_EN>();  //NET_CTRL0.FENCE_EN = 0
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_INF("p9_sbe_nest_startclocks_nest_fence_drop: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
