/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_chiplet_pll_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_sbe_chiplet_pll_initf.C
///
/// @brief procedure for scan initializing PLL config bits for XBus, OBus, PCIe, MC Chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : srinivas naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_pll_initf.H"
#include "p9_perv_scom_addresses.H"
#include <p9_ring_id.h>
#include "p9_frequency_buckets.H"

static const uint8_t P9_DEFAULT_NEST_PLL_BUCKET = 1;
static const uint8_t P9_DEFAULT_MC_PLL_BUCKET = 1;


fapi2::ReturnCode p9_sbe_chiplet_pll_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_chiplet_pll_initf: Entering ...");
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint8_t l_ob0_pll_bucket = 0;
    uint8_t l_ob1_pll_bucket = 0;
    uint8_t l_ob2_pll_bucket = 0;
    uint8_t l_ob3_pll_bucket = 0;
    uint8_t l_nest_pll_bucket = 0;
    uint8_t l_mc_pll_bucket = 0;
    uint8_t l_sync_mode = 0;
    uint8_t l_set_mc_bucket = 0;

    // determine obus pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_OB0_PLL_BUCKET, i_target_chip, l_ob0_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_OB0_PLL_BUCKET)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_OB1_PLL_BUCKET, i_target_chip, l_ob1_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_OB1_PLL_BUCKET)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_OB2_PLL_BUCKET, i_target_chip, l_ob2_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_OB2_PLL_BUCKET)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_OB3_PLL_BUCKET, i_target_chip, l_ob3_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_OB3_PLL_BUCKET)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM , l_nest_pll_bucket ),
             "Error from FAPI_ATTR_GET (ATTR_NEST_PLL_BUCKET)");

    if (l_nest_pll_bucket == 0)
    {
        l_nest_pll_bucket = P9_DEFAULT_NEST_PLL_BUCKET;
    }

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_PLL_BUCKET, FAPI_SYSTEM , l_mc_pll_bucket ),
             "Error from FAPI_ATTR_GET (ATTR_MC_PLL_BUCKET)");

    if (l_mc_pll_bucket == 0)
    {
        l_mc_pll_bucket = P9_DEFAULT_MC_PLL_BUCKET;
    }

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_sync_mode));

    if (l_sync_mode)
    {
        l_set_mc_bucket = l_nest_pll_bucket;
    }
    else
    {
        l_set_mc_bucket = l_mc_pll_bucket;
    }

    FAPI_ASSERT((l_ob0_pll_bucket && (l_ob0_pll_bucket <= OBUS_PLL_FREQ_BUCKETS)) &&
                (l_ob1_pll_bucket && (l_ob1_pll_bucket <= OBUS_PLL_FREQ_BUCKETS)) &&
                (l_ob2_pll_bucket && (l_ob2_pll_bucket <= OBUS_PLL_FREQ_BUCKETS)) &&
                (l_ob3_pll_bucket && (l_ob3_pll_bucket <= OBUS_PLL_FREQ_BUCKETS)),
                fapi2::P9_SBE_CHIPLET_PLL_INITF_UNSUPPORTED_OBUS_BUCKET().
                set_TARGET(i_target_chip).
                set_OB0_BUCKET_INDEX(l_ob0_pll_bucket).
                set_OB1_BUCKET_INDEX(l_ob1_pll_bucket).
                set_OB2_BUCKET_INDEX(l_ob2_pll_bucket).
                set_OB3_BUCKET_INDEX(l_ob3_pll_bucket),
                "Unsupported OBUS PLL bucket value!");

    for (auto& l_chplt_trgt :  i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_XBUS |
                                           fapi2::TARGET_FILTER_ALL_OBUS |
                                           fapi2::TARGET_FILTER_ALL_PCI), fapi2::TARGET_STATE_FUNCTIONAL))
    {
        uint8_t l_unit_pos;
        RingID l_ring_id = xb_pll_bndy;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_unit_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)");

        switch (l_unit_pos)
        {
            case 0x6:
                FAPI_DBG("Scan xb_pll_bndy_ring");
                l_ring_id = xb_pll_bndy;
                break;

            case 0x9:
                if (l_ob0_pll_bucket == 1)
                {
                    FAPI_DBG("Scan ob0_pll_bndy_bucket1 ring");
                    l_ring_id = ob0_pll_bndy_bucket_1;
                }
                else if (l_ob0_pll_bucket == 2)
                {
                    FAPI_DBG("Scan ob0_pll_bndy_bucket2 ring");
                    l_ring_id = ob0_pll_bndy_bucket_2;
                }
                else
                {
                    FAPI_DBG("Scan ob0_pll_bndy_bucket3 ring");
                    l_ring_id = ob0_pll_bndy_bucket_3;
                }

                break;

            case 0xa:
                if (l_ob1_pll_bucket == 1)
                {
                    FAPI_DBG("Scan ob1_pll_bndy_bucket1 ring");
                    l_ring_id = ob1_pll_bndy_bucket_1;
                }
                else if (l_ob1_pll_bucket == 2)
                {
                    FAPI_DBG("Scan ob1_pll_bndy_bucket2 ring");
                    l_ring_id = ob1_pll_bndy_bucket_2;
                }
                else
                {
                    FAPI_DBG("Scan ob1_pll_bndy_bucket3 ring");
                    l_ring_id = ob1_pll_bndy_bucket_3;
                }

                break;

            case 0xb:
                if (l_ob2_pll_bucket == 1)
                {
                    FAPI_DBG("Scan ob2_pll_bndy_bucket1 ring");
                    l_ring_id = ob2_pll_bndy_bucket_1;
                }
                else if (l_ob2_pll_bucket == 2)
                {
                    FAPI_DBG("Scan ob2_pll_bndy_bucket2 ring");
                    l_ring_id = ob2_pll_bndy_bucket_2;
                }
                else
                {
                    FAPI_DBG("Scan ob2_pll_bndy_bucket3 ring");
                    l_ring_id = ob2_pll_bndy_bucket_3;
                }

                break;

            case 0xc:
                if (l_ob3_pll_bucket == 1)
                {
                    FAPI_DBG("Scan ob3_pll_bndy_bucket1 ring");
                    l_ring_id = ob3_pll_bndy_bucket_1;
                }
                else if (l_ob3_pll_bucket == 2)
                {
                    FAPI_DBG("Scan ob3_pll_bndy_bucket2 ring");
                    l_ring_id = ob3_pll_bndy_bucket_2;
                }
                else
                {
                    FAPI_DBG("Scan ob3_pll_bndy_bucket3 ring");
                    l_ring_id = ob3_pll_bndy_bucket_3;
                }

                break;

            case 0xd:
                FAPI_DBG("Scan pci0_pll_bndy ring");
                l_ring_id = pci0_pll_bndy;
                break;

            case 0xe:
                FAPI_DBG("Scan pci1_pll_bndy ring");
                l_ring_id = pci1_pll_bndy;
                break;

            case 0xf:
                FAPI_DBG("Scan pci2_pll_bndy ring");
                l_ring_id = pci2_pll_bndy;
                break;

            default:
                FAPI_ASSERT(false,
                            fapi2::P9_SBE_CHIPLET_PLL_INITF_INVALID_CHIPLET().
                            set_TARGET(l_chplt_trgt).
                            set_UNIT_POS(l_unit_pos),
                            "Unexpected chiplet!");
        }

        FAPI_TRY(fapi2::putRing(i_target_chip, l_ring_id, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (ringID: %d)", l_ring_id);

    }

    for (auto& l_chplt_trgt :  i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>(fapi2::TARGET_STATE_FUNCTIONAL))
    {

        FAPI_DBG("Scan mc_pll_bndy_bucket_1 ring");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_pll_bndy_bucket_1, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (mc_pll_bndy)");
    }

    for (auto& l_chplt_trgt :  i_target_chip.getChildren<fapi2::TARGET_TYPE_MC>(fapi2::TARGET_STATE_FUNCTIONAL))
    {

        RingID ringID = mc_pll_bndy_bucket_1;

        switch(l_set_mc_bucket)
        {
            case 1:
                ringID = mc_pll_bndy_bucket_1;
                break;

            case 2:
                ringID = mc_pll_bndy_bucket_2;
                break;

            case 3:
                ringID = mc_pll_bndy_bucket_3;
                break;

            case 4:
                ringID = mc_pll_bndy_bucket_4;
                break;

            case 5:
                ringID = mc_pll_bndy_bucket_5;
                break;

            default:
                FAPI_ASSERT(false,
                            fapi2::P9_SBE_CHIPLET_PLL_INITF_UNSUPPORTED_MC_BUCKET().
                            set_TARGET(i_target_chip).
                            set_BUCKET_INDEX(l_set_mc_bucket),
                            "Unsupported MC PLL bucket value!");
        }

        FAPI_DBG("Scan mc_pll_bndy_bucket_%d ring", l_nest_pll_bucket);
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, ringID, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (mc_pll_bndy, ringID: %d)", ringID);

    }

fapi_try_exit:
    FAPI_INF("p9_sbe_chiplet_pll_initf: Exiting ...");
    return fapi2::current_err;
}
