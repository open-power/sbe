/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_setup.C $ */
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
/// @file  p10_sbe_chiplet_setup.C
///
/// @brief Steps:-
///     1) Identify Partial good chiplet and configure Multicasting register
///     2) Configure hang pulse counter for Nest/MC/PCIe/PAU/AXON/CQ
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_setup.H"
#include "p9_const_common.H"

#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <multicast_group_defs.H>
#include <p10_hang_pulse_mc_setup_tables.H>

fapi2::ReturnCode p10_sbe_chiplet_setup_net_ctrl_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target);

fapi2::ReturnCode p10_sbe_chiplet_setup(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    fapi2::buffer<uint64_t> l_data64;

    const uint8_t PRE_DIVIDER = 0x1;

    auto l_n0       = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_NEST_NORTH,
                      fapi2::TARGET_STATE_PRESENT)[0];
    auto l_n1       = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_NEST_SOUTH,
                      fapi2::TARGET_STATE_PRESENT)[0];

    FAPI_INF("p10_sbe_chiplet_setup: Entering ...");

    FAPI_DBG("Setup multicast groups for istep s3");
    FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target_chip, ISTEP3_MC_GROUPS));

    {

        auto l_mc_all = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
        auto l_mc_mctrl = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_MC);
        auto l_mc_pau   = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PAU);
        auto l_mc_iohs  = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_IOHS);
        auto l_mc_pci   = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PCI);
        auto l_mc_eq    = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);

        FAPI_DBG("Restore NET_CTRL0&1 init value - for all chiplets except TP");
        FAPI_TRY(p10_sbe_chiplet_setup_net_ctrl_setup(l_mc_all));

        FAPI_DBG("Setup hangcounters");
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_n0,       false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_N0));
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_n1,       false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_N1));
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_mc_pci,   false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_PCI));
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_mc_mctrl, false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_MC));
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_mc_pau,   false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_PAU));
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_mc_iohs,  false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_IOHS));
        FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_mc_eq,    false, PERV_HANG_PULSE_0_REG,
                 PRE_DIVIDER, SETUP_HANG_COUNTERS_EQ));
    }

    FAPI_INF("p10_sbe_chiplet_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Configuring NET control registers into Default required value
///
/// @param[in]     i_mcast_target   Reference to TARGET_TYPE_PERV target or a MC target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode p10_sbe_chiplet_setup_net_ctrl_setup(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target)
{
    FAPI_INF("p10_sbe_chiplet_setup_net_ctrl_setup: Entering ...");

    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL0,
                            p10SbeChipletSetup::NET_CNTL0_HW_INIT_VALUE));

    FAPI_TRY(fapi2::putScom(i_mcast_target, PERV_NET_CTRL1,
                            p10SbeChipletSetup::NET_CNTL1_HW_INIT_VALUE));

    FAPI_INF("p10_sbe_chiplet_setup_net_ctrl_setup:Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
