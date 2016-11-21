/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_chiplet_pll_initf.C $ */
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
/// @file  p9_sbe_chiplet_pll_initf.C
///
/// @brief procedure for scan initializing PLL config bits for XBus, OBus, PCIe, MC Chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : srinivas naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_pll_initf.H"
#include "p9_perv_scom_addresses.H"

fapi2::ReturnCode p9_sbe_chiplet_pll_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_chiplet_pll_initf: Entering ...");

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
                FAPI_DBG("Scan ob0_pll_bndy ring");
                l_ring_id = ob0_pll_bndy;
                break;

            case 0xa:
                FAPI_DBG("Scan ob1_pll_bndy ring");
                l_ring_id = ob1_pll_bndy;
                break;

            case 0xb:
                FAPI_DBG("Scan ob2_pll_bndy ring");
                l_ring_id = ob2_pll_bndy;
                break;

            case 0xc:
                FAPI_DBG("Scan ob3_pll_bndy ring");
                l_ring_id = ob3_pll_bndy;
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

fapi_try_exit:
    FAPI_INF("p9_sbe_chiplet_pll_initf: Exiting ...");
    return fapi2::current_err;
}
