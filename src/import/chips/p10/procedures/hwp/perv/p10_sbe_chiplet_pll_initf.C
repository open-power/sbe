/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_pll_initf.C $ */
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
/// @file  p10_sbe_chiplet_pll_initf.C
///
/// @brief procedure for scan initializing PLL bndy rings for IOHS, PCI, MC Chiplets
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_pll_initf.H"
#include "p10_ring_id.H"
#include <target_filters.H>
#include <multicast_defs.H>
#include <multicast_group_defs.H>
#include <p10_frequency_buckets.H>

fapi2::ReturnCode p10_sbe_chiplet_pll_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_chiplet_pll_initf: Entering ...");

    fapi2::ATTR_MC_PLL_BUCKET_Type l_mc_pll_bucket;
    fapi2::ATTR_IOHS_PLL_BUCKET_Type l_iohs_pll_bucket;

    // This variable (mc_ring_id/iohs_ring_id) is getting assigned to ro data
    // section per the map file, which is by default 8byte aligned.
    // Even then while creating these objects, PPE hits an alignment execption,
    // even after forcing the compiler with "__attribute__ ((aligned (8)))", ppe
    // hit the alignment execution. // TODO - To remove static const
    static const RingID mc_ring_id[P10_MAX_MC_PLL_BUCKETS] = { mc_pll_bndy_bucket_0,
                                                               mc_pll_bndy_bucket_1,
                                                               mc_pll_bndy_bucket_2,
                                                               mc_pll_bndy_bucket_3,
                                                               mc_pll_bndy_bucket_4,
                                                               // RTC: 253964
                                                               mc_pll_bndy_bucket_4,
                                                               mc_pll_bndy_bucket_4,
                                                               mc_pll_bndy_bucket_4,
                                                             };

    static const RingID iohs_ring_id[P10_MAX_IOHS_PLL_BUCKETS] = { iohs0_pll_bndy_bucket_0,
                                                                   iohs0_pll_bndy_bucket_1,
                                                                   iohs0_pll_bndy_bucket_2,
                                                                   iohs0_pll_bndy_bucket_3,
                                                                   iohs0_pll_bndy_bucket_4,
                                                                   iohs0_pll_bndy_bucket_5,
                                                                   iohs0_pll_bndy_bucket_6,
                                                                   iohs0_pll_bndy_bucket_7
                                                                 };

    auto l_pci = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                     static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_PCI),
                     fapi2::TARGET_STATE_FUNCTIONAL);

    auto l_mc_iohs = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                         static_cast<fapi2::TargetFilter>( fapi2::TARGET_FILTER_ALL_MC | fapi2::TARGET_FILTER_ALL_IOHS),
                         fapi2::TARGET_STATE_FUNCTIONAL);

    fapi2::ATTR_SCAN_CHIPLET_OVERRIDE_Type l_scan_chiplet_override = fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE;

    FAPI_INF("p10_sbe_chiplet_pll_initf: Entering ...");

    // PCI chiplets
    if (l_pci.size())
    {
        auto l_pci = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PCI);
        FAPI_TRY(fapi2::putRing(l_pci,
                                pci_pll_bndy,
                                fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (pci_pll_bndy)");
    }

    // determine mc pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_PLL_BUCKET, i_target_chip, l_mc_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_MC_PLL_BUCKET)");
    // determine iohs pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IOHS_PLL_BUCKET, i_target_chip, l_iohs_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_IOHS_PLL_BUCKET)");

    for (auto& l_cplt_target : l_mc_iohs)
    {
        uint32_t l_chipletID = l_cplt_target.getChipletNumber();

        // MC chiplets
        if(l_chipletID >= 0xC && l_chipletID <= 0xF)
        {
            int i = (l_chipletID - 0xC);

            FAPI_ASSERT(l_mc_pll_bucket[i] < P10_MAX_MC_PLL_BUCKETS,
                        fapi2::P10_SBE_CHIPLET_PLL_INITF_UNSUPPORTED_PLL_BUCKET().
                        set_BUCKET_INDEX(l_mc_pll_bucket[i]).
                        set_CHIPLET_ID(l_chipletID),
                        "Unsupported PLL bucket value!");

            FAPI_TRY(fapi2::putRing(l_cplt_target,
                                    mc_ring_id[l_mc_pll_bucket[i]],
                                    fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (mc_pll_bndy_bucket_%d), ChipletID: %#010lX", l_mc_pll_bucket[i], l_chipletID);
        }

        // IOHS chiplets
        else if(l_chipletID >= 0x18 && l_chipletID <= 0x1F)
        {
            int i = (l_chipletID - 0x18);

            FAPI_ASSERT(l_iohs_pll_bucket[i] < P10_MAX_IOHS_PLL_BUCKETS,
                        fapi2::P10_SBE_CHIPLET_PLL_INITF_UNSUPPORTED_PLL_BUCKET().
                        set_BUCKET_INDEX(l_iohs_pll_bucket[i]).
                        set_CHIPLET_ID(l_chipletID),
                        "Unsupported PLL bucket value!");

            l_scan_chiplet_override = l_chipletID;
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SCAN_CHIPLET_OVERRIDE, i_target_chip, l_scan_chiplet_override),
                     "Error from FAPI_ATTR_SET (ATTR_SCAN_CHIPLET_OVERRIDE)");

            FAPI_TRY(fapi2::putRing(i_target_chip,
                                    iohs_ring_id[l_iohs_pll_bucket[i]],
                                    fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (iohs0_pll_bndy_bucket_%d), ChipletID: %#010lX", l_iohs_pll_bucket[i], l_chipletID);

            l_scan_chiplet_override = fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE;
            FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_SCAN_CHIPLET_OVERRIDE, i_target_chip, l_scan_chiplet_override),
                     "Error from FAPI_ATTR_SET (ATTR_SCAN_CHIPLET_OVERRIDE)");
        }
    }

fapi_try_exit:
    // ensure attribute gets set back to no override on exit, even on error
    l_scan_chiplet_override = fapi2::ENUM_ATTR_SCAN_CHIPLET_OVERRIDE_NONE;
    (void) FAPI_ATTR_SET(fapi2::ATTR_SCAN_CHIPLET_OVERRIDE, i_target_chip, l_scan_chiplet_override);

    FAPI_INF("p10_sbe_chiplet_pll_initf: Exiting ...");
    return fapi2::current_err;
}
