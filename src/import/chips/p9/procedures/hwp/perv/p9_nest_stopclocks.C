/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_nest_stopclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/// @file  p9_nest_stopclocks.C
///
/// @brief stopclocks for nest,mc chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha R Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_nest_stopclocks.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>
#include <p9_common_stopclocks.H>


enum P9_NEST_STOPCLOCKS_Private_Constants
{
    CLOCK_CMD = 0x2,
    CLOCK_TYPES = 0x7,
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    REGIONS_ALL_EXCEPT_PLL = 0x7FE,
    STARTMASTER = 0x1,
    STARTSLAVE = 0x1
};

fapi2::ReturnCode p9_nest_stopclocks(const
                                     fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    fapi2::buffer<uint32_t> l_attr_pg;
    fapi2::buffer<uint64_t> l_pg_vector;
    fapi2::buffer<uint64_t> l_clock_regions;
    fapi2::buffer<uint64_t> l_n3_clock_regions;
    fapi2::buffer<uint16_t> l_ccstatus_regions;
    fapi2::buffer<uint16_t> l_n3_ccstatus_regions;
    FAPI_INF("Entering ...");

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_target_cplt, REGIONS_ALL_EXCEPT_PLL,
                 l_n3_clock_regions));
        FAPI_DBG("n3_clock_regions: %#018lX", l_n3_clock_regions);

        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(l_target_cplt, REGIONS_ALL_EXCEPT_PLL,
                 l_n3_ccstatus_regions));
        FAPI_DBG("n3_ccstatus_regions: %#018lX", l_n3_ccstatus_regions);
    }

    FAPI_DBG("Reading ATTR_MC_SYNC_MODE");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    fapi2::TargetFilter l_nest_filter;

    if (l_read_attr)
    {
        l_nest_filter = static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                        fapi2::TARGET_FILTER_ALL_NEST);
    }
    else
    {
        l_nest_filter = fapi2::TARGET_FILTER_ALL_NEST;
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_nest_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Raise chiplet fence");
        FAPI_TRY(p9_common_stopclocks_raise_fence(l_trgt_chplt));
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_NEST_NORTH |
                                           fapi2::TARGET_FILTER_NEST_SOUTH | fapi2::TARGET_FILTER_NEST_EAST),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Regions value: %#018lX", l_clock_regions);
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_trgt_chplt, REGIONS_ALL_EXCEPT_PLL,
                 l_clock_regions));

        FAPI_DBG("Call module clock start stop for N0, N1, N2");
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD, STARTSLAVE,
                                                DONT_STARTMASTER, l_clock_regions, CLOCK_TYPES));
    }

    for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (fapi2::TARGET_FILTER_NEST_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Call module clock start stop for N3");
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_target_cplt, CLOCK_CMD,
                                                DONT_STARTSLAVE, STARTMASTER, l_n3_clock_regions, CLOCK_TYPES));
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_NEST_NORTH |
                                           fapi2::TARGET_FILTER_NEST_SOUTH | fapi2::TARGET_FILTER_NEST_EAST),
          fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_16(l_trgt_chplt, REGIONS_ALL_EXCEPT_PLL,
                 l_ccstatus_regions));
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
    }

    if ( l_read_attr )
    {
        for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
        {
            FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_trgt_chplt, REGIONS_ALL_EXCEPT_PLL,
                     l_clock_regions));
            FAPI_DBG("Regions value: %#018lX", l_clock_regions);

            FAPI_DBG("Call module clock start stop for MC01, MC23.");
            FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD,
                                                    DONT_STARTSLAVE, DONT_STARTMASTER, l_clock_regions, CLOCK_TYPES));
        }
    }

    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_nest_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Assert vital fence and flush mode for Nest and Mc chiplets");
        FAPI_TRY(p9_common_stopclocks_set_vitalfence_flushmode(l_trgt_chplt));

        FAPI_DBG("Call p9_common_stopclocks_cplt_ctrl_action_function for Nest and Mc chiplets");
        FAPI_TRY(p9_common_stopclocks_cplt_ctrl_action_function(l_trgt_chplt));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
