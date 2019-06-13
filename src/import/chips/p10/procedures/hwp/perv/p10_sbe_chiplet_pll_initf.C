/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_pll_initf.C $ */
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

#define MC_PLL_FREQ_BUCKETS 5
#define IOHS_PLL_FREQ_BUCKETS 5

fapi2::ReturnCode p10_sbe_chiplet_pll_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    uint8_t l_mc_pll_bucket[4], l_iohs_pll_bucket[8] ;
    RingID l_ring_id = mc_pll_bndy_bucket_0;

    RingID mc_ring_id[MC_PLL_FREQ_BUCKETS] = { mc_pll_bndy_bucket_0,
                                               mc_pll_bndy_bucket_1,
                                               mc_pll_bndy_bucket_2,
                                               mc_pll_bndy_bucket_3,
                                               mc_pll_bndy_bucket_4
                                             };

    RingID iohs_ring_id[IOHS_PLL_FREQ_BUCKETS] = { iohs_pll_bndy_bucket_0,
                                                   iohs_pll_bndy_bucket_1,
                                                   iohs_pll_bndy_bucket_2,
                                                   iohs_pll_bndy_bucket_3,
                                                   iohs_pll_bndy_bucket_4
                                                 };

    auto l_pci_mc_iohs = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                             static_cast<fapi2::TargetFilter>( fapi2::TARGET_FILTER_ALL_PCI |
                                     fapi2::TARGET_FILTER_ALL_MC | fapi2::TARGET_FILTER_ALL_IOHS),
                             fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_INF("p10_sbe_chiplet_pll_initf: Entering ...");

    // determine mc pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_PLL_BUCKET, i_target_chip, l_mc_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_MC_PLL_BUCKET)");
    // determine iohs pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IOHS_PLL_BUCKET, i_target_chip, l_iohs_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_IOHS_PLL_BUCKET)");

    for (auto& l_cplt_target : l_pci_mc_iohs)
    {
        uint32_t l_chipletID = l_cplt_target.getChipletNumber();

        // PCI0 and PCI1 chiplets
        if(l_chipletID >= 0x8 && l_chipletID <= 0x9)
        {
            FAPI_TRY(fapi2::putRing(l_cplt_target, pci_pll_bndy),
                     "Error from putRing (pci_pll_bndy), ChipletID: %#010lX", l_chipletID);
        }
        // MC chiplets
        else if(l_chipletID >= 0xC && l_chipletID <= 0xF)
        {
            int i = (l_chipletID - 0xC);

            FAPI_ASSERT(l_mc_pll_bucket[i] < MC_PLL_FREQ_BUCKETS,
                        fapi2::P10_SBE_CHIPLET_PLL_INITF_UNSUPPORTED_PLL_BUCKET().
                        set_BUCKET_INDEX(l_mc_pll_bucket[i]).
                        set_CHIPLET_ID(l_chipletID),
                        "Unsupported PLL bucket value!");

            l_ring_id = mc_ring_id[l_mc_pll_bucket[i]];

            FAPI_TRY(fapi2::putRing(l_cplt_target, l_ring_id),
                     "Error from putRing (mc_pll_bndy_bucket_%d), ChipletID: %#010lX", l_mc_pll_bucket[i], l_chipletID);
        }
        // IOHS chiplets
        else if(l_chipletID >= 0x18 && l_chipletID <= 0x1F)
        {
            int i = (l_chipletID - 0x18);

            FAPI_ASSERT(l_iohs_pll_bucket[i] < IOHS_PLL_FREQ_BUCKETS,
                        fapi2::P10_SBE_CHIPLET_PLL_INITF_UNSUPPORTED_PLL_BUCKET().
                        set_BUCKET_INDEX(l_iohs_pll_bucket[i]).
                        set_CHIPLET_ID(l_chipletID),
                        "Unsupported PLL bucket value!");

            l_ring_id = iohs_ring_id[l_iohs_pll_bucket[i]];

            FAPI_TRY(fapi2::putRing(l_cplt_target, l_ring_id),
                     "Error from putRing (iohs_pll_bndy_bucket_%d), ChipletID: %#010lX", l_iohs_pll_bucket[i], l_chipletID);
        }

    }

fapi_try_exit:
    FAPI_INF("p10_sbe_chiplet_pll_initf: Exiting ...");
    return fapi2::current_err;
}
