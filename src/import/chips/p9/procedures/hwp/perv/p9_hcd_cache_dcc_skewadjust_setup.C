/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_hcd_cache_dcc_skewadjust_setup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
/// @file  p9_hcd_cache_dcc_skewadjust_setup.C
///
/// @brief Drop DCCs reset and bypass, Drop skewadjust reset and bypass
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE:SGPE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_hcd_cache_dcc_skewadjust_setup.H"
#include <p9_perv_scom_addresses.H>
#include <p9_quad_scom_addresses.H>




fapi2::ReturnCode p9_hcd_cache_dcc_skewadjust_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_cache)
{
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_cache.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = i_cache.getParent<fapi2::TARGET_TYPE_PERV>();
    auto l_core_functional_vector = i_cache.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);
    uint8_t l_attr_chip_unit_pos = 0;
    fapi2::buffer<uint64_t> l_data64;


    FAPI_DBG("Entering ...");

    FAPI_DBG("Release Skew Adjust Reset");
    l_data64.flush<1>().clearBit<2>();
    FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("Release L2-0, L2-1 DC Adjust reset");
    l_data64.flush<1>();
    l_data64.clearBit<23>();
    l_data64.clearBit<24>();
    FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));

    for(auto it : l_core_functional_vector)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it.getParent<fapi2::TARGET_TYPE_PERV>(),
                               l_attr_chip_unit_pos));

        FAPI_DBG("Release CORE DC Adjust reset");
        l_data64.flush<1>();
        l_data64.clearBit<2>();
        FAPI_TRY(fapi2::putScom(l_chip, (C_NET_CTRL0_WAND + (0x1000000 * (l_attr_chip_unit_pos - 0x20))) ,
                                l_data64));
    }

    FAPI_DBG("Scan eq_ana_bndy_bucket_0 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_0, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_0)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_1 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_1, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_1)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_2 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_2, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_2)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_3 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_3, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_3)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_4 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_4, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_4)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_5 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_5, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_5)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_6 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_6, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_6)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_7 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_7, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_7)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_8 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_8, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_8)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_9 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_9, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_9)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_10 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_10, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_10)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_11 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_11, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_11)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_12 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_12, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_12)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_13 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_13, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_13)");

    FAPI_DBG("Release DCC bypass");
    l_data64.flush<1>();
    l_data64.clearBit<1>();
    FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));

    FAPI_DBG("Scan eq_ana_bndy_bucket_14 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_14, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_14)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_15 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_15, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_15)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_16 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_16, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_16)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_17 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_17, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_17)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_18 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_18, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_18)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_19 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_19, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_19)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_20 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_20, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_20)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_21 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_21, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_21)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_22 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_22, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_22)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_23 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_23, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_23)");

    FAPI_DBG("Release Progdly bypass");
    l_data64.flush<1>();
    l_data64.clearBit<2>();
    FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));

    FAPI_DBG("Scan eq_ana_bndy_bucket_24 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_24, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_24)");
    FAPI_DBG("Scan eq_ana_bndy_bucket_25 ring");
    FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_25, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (eq_ana_bndy_bucket_25)");

    FAPI_DBG("Exiting ...");
fapi_try_exit:
    return fapi2::current_err;

}
