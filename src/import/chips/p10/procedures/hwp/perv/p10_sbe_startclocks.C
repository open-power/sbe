/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_startclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file  p10_sbe_startclocks.C
///
/// @brief Start clock procedure for all chiplets NEST, PCI, MC, PAU, IOHS, EQ
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#include "p10_sbe_startclocks.H"
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_STARTCLOCKS_Private_Constants
{
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    CLOCK_CMD_START = 0x1,
    CLOCK_CMD_STOP = 0x2,
    CLOCK_TYPES_ALL = 0x7,
    PGOOD_REGIONS_STARTBIT = 4,
    PGOOD_REGIONS_LENGTH = 15,
    PGOOD_REGIONS_OFFSET = 12,
    REGIONS_EXCEPT_VITAL_AND_PLL = 0x7FEF,
    REGIONS_ONLY_CLKADJ = 0x0010,
};


fapi2::ReturnCode p10_sbe_startclocks(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint32_t> l_attr_pg;

    FAPI_DBG("p10_sbe_startclocks: Entering ...");

    auto l_perv_all_good_but_TP_and_EQ = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
            static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST |
                    fapi2::TARGET_FILTER_ALL_MC  |  fapi2::TARGET_FILTER_ALL_PCI  |
                    fapi2::TARGET_FILTER_ALL_PAU |  fapi2::TARGET_FILTER_ALL_IOHS ),
            fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_mc_all = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    auto l_mc_eq = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST,
          fapi2::MULTICAST_COMPARE > l_mcast_cmp_target = l_mc_all;

    FAPI_DBG("Drop chiplet fence");
    l_data64.flush<1>().clearBit<PERV_1_NET_CTRL0_FENCE_EN>();
    FAPI_TRY(fapi2::putScom(l_mc_all, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("For all good chiplets except TP and EQ: Drop partial good fences");

    for (auto& targ : l_perv_all_good_but_TP_and_EQ)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, targ, l_attr_pg));
        l_attr_pg.invert();
        l_data64.flush<0>()
        .insert< PGOOD_REGIONS_STARTBIT, PGOOD_REGIONS_LENGTH, PGOOD_REGIONS_OFFSET >(l_attr_pg)
        .writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>(l_attr_pg.getBit<11>());
        FAPI_TRY(fapi2::putScom(targ, PERV_CPLT_CTRL1_CLEAR, l_data64));
    }

    FAPI_DBG("For all EQ chiplets: Drop partial good fences for perv, qme, clkadj");
    l_data64.flush<0>();
    // drop region fence for only regions vital,perv, qme, clkadj
    l_data64.setBit<3>().setBit<4>().setBit<13>().setBit<14>();
    FAPI_TRY(fapi2::putScom(l_mc_eq, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_DBG("Reset abistclk_muxsel and syncclk_muxsel");
    l_data64.flush<0>()
    .setBit<PERV_1_CPLT_CTRL0_CTRL_CC_ABSTCLK_MUXSEL_DC>()
    .setBit<PERV_1_CPLT_CTRL0_TC_UNIT_SYNCCLK_MUXSEL_DC>();
    FAPI_TRY(fapi2::putScom(l_mc_all, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_DBG("Disable listen to sync for all chiplets");
    FAPI_TRY(fapi2::getScom(l_mcast_cmp_target, PERV_SYNC_CONFIG, l_data64));
    l_data64.setBit<4>();
    FAPI_TRY(fapi2::putScom(l_mc_all, PERV_SYNC_CONFIG, l_data64));

    // Align_chiplets  module
    FAPI_DBG("Align all chiplets");
    FAPI_TRY(p10_perv_sbe_cmn_align_chiplets(l_mc_all));

    FAPI_DBG("Start all clocks except PLL on all chiplets");
    FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_mc_all, CLOCK_CMD_START, 0, 0,
             REGIONS_EXCEPT_VITAL_AND_PLL, CLOCK_TYPES_ALL));

    FAPI_DBG("Start clkadj clocks on EQ chiplets");
    FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_mc_eq, CLOCK_CMD_START, 0, 0,
             REGIONS_ONLY_CLKADJ, CLOCK_TYPES_ALL));

    FAPI_DBG("Clear flush_inhibit to go in to flush mode");
    l_data64.flush<0>()
    .setBit<PERV_1_CPLT_CTRL0_CTRL_CC_FLUSHMODE_INH_DC>();
    FAPI_TRY(fapi2::putScom(l_mc_all, PERV_CPLT_CTRL0_CLEAR, l_data64));

    FAPI_DBG("p10_sbe_startclocks: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
