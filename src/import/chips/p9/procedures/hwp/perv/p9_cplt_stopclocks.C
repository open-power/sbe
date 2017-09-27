/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_cplt_stopclocks.C $ */
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
/// @file  p9_cplt_stopclocks.C
///
/// @brief stop clocks for Xb,Ob,Pcie
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_cplt_stopclocks.H"
//## auto_generated
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_sbe_common.H>
#include <p9_common_stopclocks.H>


enum P9_CPLT_STOPCLOCKS_Private_Constants
{
    CLOCK_CMD = 0x2,
    CLOCK_TYPES = 0x7,
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    REGIONS_ALL_INCLUDING_PLL = 0x7FF,
    STARTMASTER = 0x1,
    STARTSLAVE = 0x1
};

fapi2::ReturnCode p9_cplt_stopclocks(const
                                     fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                     const bool i_stop_xb,
                                     const bool i_stop_ob,
                                     const bool i_stop_pcie,
                                     const bool i_stop_mc)
{
    fapi2::buffer<uint64_t> l_regions;
    fapi2::buffer<uint8_t> l_attr_mc_sync;
    fapi2::TargetFilter l_io_filter = fapi2::TARGET_FILTER_NONE ;
    FAPI_INF("Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip,
                           l_attr_mc_sync));


    if (i_stop_xb)
    {
        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_XBUS);
    }

    if (i_stop_ob)
    {
        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_ALL_OBUS);
    }

    if (i_stop_pcie)
    {
        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_ALL_PCI);
    }

    if( !l_attr_mc_sync )
    {
        if (i_stop_mc)
        {
            l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                          fapi2::TARGET_FILTER_ALL_MC);
        }
    }


    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_io_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Raise chiplet fence");
        FAPI_TRY(p9_common_stopclocks_raise_fence(l_trgt_chplt));

        FAPI_DBG("Region setup");
        FAPI_TRY(p9_perv_sbe_cmn_regions_setup_64(l_trgt_chplt, REGIONS_ALL_INCLUDING_PLL, l_regions));
        FAPI_DBG("Regions value: %#018lX", l_regions);

        FAPI_DBG("Call module clock start stop for xbus, obus, pcie, mc chiplets");
        FAPI_TRY(p9_sbe_common_clock_start_stop(l_trgt_chplt, CLOCK_CMD,
                                                DONT_STARTSLAVE, DONT_STARTMASTER, l_regions, CLOCK_TYPES));

        FAPI_DBG("Set vital fence and flushmode");
        FAPI_TRY(p9_common_stopclocks_set_vitalfence_flushmode(l_trgt_chplt));

        FAPI_DBG("Call p9_common_stopclocks_cplt_ctrl_action_function ");
        FAPI_TRY(p9_common_stopclocks_cplt_ctrl_action_function(l_trgt_chplt));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
