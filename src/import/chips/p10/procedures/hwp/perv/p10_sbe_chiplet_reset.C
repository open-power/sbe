/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_reset.C $ */
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
/// @file  p10_sbe_chiplet_reset.C
///
/// @brief
///
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_reset.H"
#include "p10_scom_perv.H"
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_CHIPLET_RESET_Private_Constants
{
    PGOOD_REGIONS_STARTBIT = 4,
    PGOOD_REGIONS_LENGTH = 15,
    PGOOD_REGIONS_OFFSET = 12,
};

fapi2::ReturnCode p10_sbe_chiplet_reset(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    uint8_t l_attr_nest_dpll_bypass;
    uint8_t l_attr_pau_dpll_bypass;
    fapi2::buffer<uint32_t> l_read_attr_pg;
    fapi2::buffer<uint64_t> l_data64_nc0, l_data64;

    FAPI_INF("p10_sbe_chiplet_reset: Entering ...");

    auto l_perv_eq_nest_func = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                   static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_EQ | fapi2::TARGET_FILTER_ALL_NEST),
                                   fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_perv_all_but_tp = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                 static_cast<fapi2::TargetFilter>(
                                     fapi2::TARGET_FILTER_ALL_MC  |  fapi2::TARGET_FILTER_ALL_NEST |
                                     fapi2::TARGET_FILTER_ALL_PAU |  fapi2::TARGET_FILTER_ALL_PCI  |
                                     fapi2::TARGET_FILTER_ALL_IOHS), fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_mc_all = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    auto l_mc_pau = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PAU);
    auto l_mc_eq = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_DPLL_BYPASS, i_target_chip, l_attr_nest_dpll_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PAU_DPLL_BYPASS, i_target_chip, l_attr_pau_dpll_bypass));

    FAPI_DBG("Enable chiplet");
    l_data64_nc0.flush<0>().setBit<NET_CTRL0_CHIPLET_ENABLE>();
    FAPI_TRY(fapi2::putScom(l_mc_all, NET_CTRL0_RW_WOR, l_data64_nc0));

    FAPI_DBG("Reset PCB slave error registers");
    l_data64.flush<1>();
    FAPI_TRY(fapi2::putScom(l_mc_all, ERROR_REG, l_data64));

    FAPI_DBG("Drop endpoint reset");
    l_data64_nc0.flush<1>().clearBit<NET_CTRL0_PCB_EP_RESET>();
    FAPI_TRY(fapi2::putScom(l_mc_all, NET_CTRL0_RW_WAND, l_data64_nc0));

    // Configuring cplt_ctrl2(Region pg), cplt_ctrl3(region multicast enable),
    // cplt_ctrl5(power_gate) registers with ATTR_PG values
    FAPI_DBG("Transfer PGOOD attribute into region good,region enable and power gate register : ALL but TP and EQ");

    for (auto& targ : l_perv_all_but_tp)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, targ, l_read_attr_pg));

        l_data64.flush<0>();
        l_read_attr_pg.invert();
        l_data64.insert< PGOOD_REGIONS_STARTBIT, PGOOD_REGIONS_LENGTH, PGOOD_REGIONS_OFFSET >(l_read_attr_pg);

        FAPI_TRY(fapi2::putScom(targ, CPLT_CTRL2_RW, l_data64));
        FAPI_TRY(fapi2::putScom(targ, CPLT_CTRL3_RW, l_data64));
    }

    FAPI_DBG("Transfer PGOOD attribute into region good,region enable and power gate register : ALL EQ");
    l_data64.flush<0>();
    // only regions perv, qme, clkadj enabled
    l_data64.setBit<4>().setBit<13>().setBit<14>();

    FAPI_TRY(fapi2::putScom(l_mc_eq, CPLT_CTRL2_RW, l_data64));
    FAPI_TRY(fapi2::putScom(l_mc_eq, CPLT_CTRL3_RW, l_data64));

    FAPI_DBG("Initialize SYNC_CONFIG regs with default values");
    FAPI_TRY(fapi2::putScom(l_mc_all, SYNC_CONFIG, p10SbeChipletReset::SYNC_CONFIG_DEFAULT_VAL));

    FAPI_DBG("Initialize OPCG_ALIGN regs with default values");
    FAPI_TRY(fapi2::putScom(l_mc_all, OPCG_ALIGN, p10SbeChipletReset::OPCG_ALIGN_DEFAULT_VAL));

    if (!l_attr_pau_dpll_bypass)
    {
        FAPI_DBG("Initialize PAU OPCG_ALIGN regs with default values");
        FAPI_TRY(fapi2::putScom(l_mc_pau, OPCG_ALIGN, p10SbeChipletReset::PAU_OPCG_ALIGN_DEFAULT_VAL));
    }

    l_data64.flush<0>();
    l_data64.insertFromRight< OPCG_ALIGN_INOP_ALIGN, OPCG_ALIGN_INOP_ALIGN_LEN>
    (p10SbeChipletReset::OPCG_ALIGN_INOP_ALIGN);
    l_data64.insertFromRight< OPCG_ALIGN_INOP_WAIT, OPCG_ALIGN_INOP_WAIT_LEN >
    (p10SbeChipletReset::OPCG_ALIGN_INOP_WAIT);

    if(l_attr_nest_dpll_bypass == 0x0)
    {
        l_data64.insertFromRight< OPCG_ALIGN_SCAN_RATIO, OPCG_ALIGN_SCAN_RATIO_LEN>
        (p10SbeChipletReset::OPCG_ALIGN_SCANRATIO_4to1);
    }

    for (auto& targ : l_perv_eq_nest_func)
    {
        l_data64.insertFromRight< OPCG_ALIGN_OPCG_WAIT_CYCLES, OPCG_ALIGN_OPCG_WAIT_CYCLES_LEN >
        (0x020 + chiplet_delay_cycles(targ.getChipletNumber()));
        FAPI_TRY(fapi2::putScom(targ, OPCG_ALIGN, l_data64));
    }

    // Scan0 call to support Multicast
    FAPI_DBG("Run scan0 module for region except vital, scan types GPTR, TIME, REPR");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_mc_all, p10SbeChipletReset::REGIONS_EXCEPT_VITAL,
                                           p10SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("Run scan0 module for region except vital, scan types except GPTR, TIME, REPR");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_mc_all, p10SbeChipletReset::REGIONS_EXCEPT_VITAL,
                                           p10SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));


    FAPI_INF("p10_sbe_chiplet_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
