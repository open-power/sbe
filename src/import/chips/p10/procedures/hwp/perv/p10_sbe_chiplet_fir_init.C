/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_fir_init.C $ */
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
/// @file  p10_sbe_chiplet_fir_init.C
///
/// @brief Setup per-chiplet FIRs
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_fir_init.H"
#include "p10_scom_perv.H"
#include "p10_scom_proc.H"
#include "p10_sbe_chiplet_reset.H"
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_CHIPLET_FIR_INIT_Private_Constants
{
    // Values translated from (long link, undo line breaks):
    // https://w3-connections.ibm.com/communities/service/html/communityview?
    //   communityUuid=340f24c2-a7a1-4377-8104-e443d5836678#
    //   fullpageWidgetId=We97b5997bab6_46f0_989d_4c47c6ed3356&
    //   file=361e143b-4808-4e36-a594-9d3d64bd5ad6
    // using chips/p10/procedures/utils/perv_lfir/gen_lfir_settings.sh

    OTH_LFIR_ACTION0_VALUE  = 0b0000000000000000000000000000000000000000000000000000000000000000,
    OTH_LFIR_ACTION1_VALUE  = 0b1111111111111111111111111111111111111111111111111111111111111111,
    OTH_LFIR_MASK_VALUE     = 0b0000000011101111111100111111111111111111111111111111111111111111,
    XSTOP_MASK_VALUE        = 0b0010000000000000000000000000000000000000000000000000000000000000,
    XSTOP_MASK_VALUE_SPATTN = 0b0000000000000000000000000000000000000000000000000000000000000000,
    // Constants to set up clockstop-on-xstop
    CLKSTOP_CC_XSTOP1       = 0x97FFE00000000000,
};

fapi2::ReturnCode p10_sbe_chiplet_fir_init(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;
    using namespace scomt::proc;

    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint8_t>  l_xstop_on_spattn;
    fapi2::buffer<uint8_t>  l_clkstop_on_xstop;

    auto l_mc_mctl = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_MC);
    auto l_mc_iohs = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_IOHS);
    auto l_mc_pau  = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PAU);
    auto l_mc_pci  = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PCI);
    auto l_mc_all  = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    auto l_perv_all_but_tp = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                 static_cast<fapi2::TargetFilter>(
                                     fapi2::TARGET_FILTER_ALL_MC  |  fapi2::TARGET_FILTER_ALL_NEST |
                                     fapi2::TARGET_FILTER_ALL_PAU |  fapi2::TARGET_FILTER_ALL_PCI  |
                                     fapi2::TARGET_FILTER_ALL_IOHS), fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_DBG("p10_sbe_chiplet_fir_init: Entering ...");
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_XSTOP_ON_SPATTN, FAPI_SYSTEM, l_xstop_on_spattn));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CLOCKSTOP_ON_XSTOP, i_target_chip, l_clkstop_on_xstop));

    FAPI_DBG("Set up pervasive LFIR on all IO chiplets");
    FAPI_TRY(fapi2::putScom(l_mc_mctl, EPS_FIR_LOCAL_ACTION0, OTH_LFIR_ACTION0_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_mctl, EPS_FIR_LOCAL_ACTION1, OTH_LFIR_ACTION1_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_mctl, EPS_FIR_LOCAL_MASK_RW, OTH_LFIR_MASK_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_iohs, EPS_FIR_LOCAL_ACTION0, OTH_LFIR_ACTION0_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_iohs, EPS_FIR_LOCAL_ACTION1, OTH_LFIR_ACTION1_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_iohs, EPS_FIR_LOCAL_MASK_RW, OTH_LFIR_MASK_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_pau,  EPS_FIR_LOCAL_ACTION0, OTH_LFIR_ACTION0_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_pau,  EPS_FIR_LOCAL_ACTION1, OTH_LFIR_ACTION1_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_pau,  EPS_FIR_LOCAL_MASK_RW, OTH_LFIR_MASK_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_pci,  EPS_FIR_LOCAL_ACTION0, OTH_LFIR_ACTION0_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_pci,  EPS_FIR_LOCAL_ACTION1, OTH_LFIR_ACTION1_VALUE));
    FAPI_TRY(fapi2::putScom(l_mc_pci,  EPS_FIR_LOCAL_MASK_RW, OTH_LFIR_MASK_VALUE));

    FAPI_DBG("Drop pervasive CFIR masks on all chiplets");
    FAPI_TRY(fapi2::putScom(l_mc_all, XSTOP_MASK_RW, l_xstop_on_spattn ? (XSTOP_MASK_VALUE_SPATTN) : (XSTOP_MASK_VALUE)));
    FAPI_TRY(fapi2::putScom(l_mc_all, RECOV_MASK_RW, 0));
    FAPI_TRY(fapi2::putScom(l_mc_all, SPATTN_MASK_RW, 0));
    FAPI_TRY(fapi2::putScom(l_mc_all, LOCAL_XSTOP_MASK_RW, 0));
    FAPI_TRY(fapi2::putScom(l_mc_all, HOSTATTN_MASK_RW, 0));

    if (l_clkstop_on_xstop)
    {
        if (l_clkstop_on_xstop.getBit<EPS_FIR_CLKSTOP_ON_XSTOP_MASK1_SYS_XSTOP_STAGED_ERR>())
        {
            // staged xstop is masked, leave all delays at 0 for fast stopping
            FAPI_TRY(fapi2::putScom(l_mc_all, XSTOP1, CLKSTOP_CC_XSTOP1));
        }
        else
        {
            FAPI_DBG("Staged xstop is unmasked; set up per-chiplet delays");

            for (auto& l_chiplet : l_perv_all_but_tp)
            {
                l_data64 = CLKSTOP_CC_XSTOP1;
                l_data64.insertFromRight<XSTOP1_WAIT_CYCLES, XSTOP1_WAIT_CYCLES_LEN>
                ((uint64_t)chiplet_delay_cycles(l_chiplet.getChipletNumber()));
                FAPI_TRY(fapi2::putScom(l_chiplet, XSTOP1, l_data64));
            }
        }

        FAPI_DBG("Enable clockstop on checkstop");
        l_data64.flush<1>().insert<0, 8>(l_clkstop_on_xstop);
        FAPI_TRY(fapi2::putScom(l_mc_all, EPS_FIR_CLKSTOP_ON_XSTOP_MASK1, l_data64));
    }

    FAPI_DBG("p10_sbe_chiplet_fir_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
