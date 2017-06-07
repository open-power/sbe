/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_hcd_cache_dcc_skewadjust_setup.C $ */
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
#include <p9_ring_id.h>


fapi2::ReturnCode p9_hcd_cache_dcc_skewadjust_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_cache)
{
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_cache.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = i_cache.getParent<fapi2::TARGET_TYPE_PERV>();
    auto l_core_functional_vector = i_cache.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    uint8_t l_attr_chip_unit_pos = 0;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint8_t> l_attr_dcadj_p9dd1_init;
    fapi2::buffer<uint8_t> l_attr_skewadj_p9dd1_init;
    fapi2::buffer<uint8_t> l_attr_dcadj_disable;
    fapi2::buffer<uint8_t> l_attr_skewadj_disable;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_CORECACHE_SKEWADJ_DISABLE,
                           l_sys, l_attr_dcadj_disable));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_CORECACHE_DCADJ_DISABLE,
                           l_sys, l_attr_skewadj_disable));

    FAPI_DBG("Entering ...");

    if (! l_attr_dcadj_disable) // When dcadj is enabled, execute the if part
    {

        FAPI_DBG("Release L2-0, L2-1 DC Adjust reset");
        l_data64.flush<1>();
        l_data64.clearBit<23>();
        l_data64.clearBit<24>();
        FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));

        for(auto& it : l_core_functional_vector)
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

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_DCADJ_P9NDD1_INIT,
                               l_chip, l_attr_dcadj_p9dd1_init));

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

        if( l_attr_dcadj_p9dd1_init )
        {
            FAPI_DBG("Release DCC bypass");
            l_data64.flush<1>();
            l_data64.clearBit<1>();
            FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));
        }

        FAPI_DBG("Scan eq_ana_bndy_bucket_14 ring");
        FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_14, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (eq_ana_bndy_bucket_14)");

        FAPI_DBG("Scan eq_ana_bndy_bucket_15 ring");
        FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_15, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (eq_ana_bndy_bucket_15)");

        if(! l_attr_dcadj_p9dd1_init ) //DD2 bucket_16, 17 -- dcadso
        {

            FAPI_DBG("Release DCC bypass");
            l_data64.flush<1>();
            l_data64.clearBit<1>();
            FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));

            FAPI_DBG("Scan eq_ana_bndy_bucket_16 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_16, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_16)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_17 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_17, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_17)");
        }
    }


    if (! l_attr_skewadj_disable) // when skewadj is enabled, execute the if part
    {

        FAPI_DBG("Release Skew Adjust Reset");
        l_data64.flush<1>().clearBit<2>();
        FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL0_WAND, l_data64));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SKEWADJ_P9NDD1_INIT,
                               l_chip, l_attr_skewadj_p9dd1_init));

        if( l_attr_skewadj_p9dd1_init )   //DD1 bucket_16, 17 -- skewadjust
        {
            FAPI_DBG("Scan eq_ana_bndy_bucket_16 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_16, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_16)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_17 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_17, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_17)");
        }

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

        if (l_attr_skewadj_p9dd1_init) //DD1
        {
            FAPI_DBG("Release Progdly bypass");
            l_data64.flush<1>();
            l_data64.clearBit<2>();
            FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));
        }

        FAPI_DBG("Scan eq_ana_bndy_bucket_24 ring");
        FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_24, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (eq_ana_bndy_bucket_24)");

        FAPI_DBG("Scan eq_ana_bndy_bucket_25 ring");
        FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_25, fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (eq_ana_bndy_bucket_25)");

        if (! l_attr_skewadj_p9dd1_init) //DD2
        {
            FAPI_DBG("Scan eq_ana_bndy_bucket_26 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_26, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_26)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_27 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_27, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_27)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_28 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_28, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_28)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_29 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_29, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_29)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_30 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_30, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_30)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_31 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_31, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_31)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_32 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_32, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_32)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_33 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_33, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_33)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_34 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_34, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_34)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_35 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_35, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_35)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_36 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_36, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_36)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_37 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_37, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_37)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_38 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_38, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_38)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_39 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_39, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_39)");

            FAPI_DBG("Release Progdly bypass");
            l_data64.flush<1>();
            l_data64.clearBit<2>();
            FAPI_TRY(fapi2::putScom(l_perv, PERV_NET_CTRL1_WAND, l_data64));

            FAPI_DBG("Scan eq_ana_bndy_bucket_40 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_40, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_40)");

            FAPI_DBG("Scan eq_ana_bndy_bucket_41 ring");
            FAPI_TRY(fapi2::putRing(i_cache, eq_ana_bndy_bucket_41, fapi2::RING_MODE_SET_PULSE_NSL),
                     "Error from putRing (eq_ana_bndy_bucket_41)");
        }
    }

    FAPI_DBG("Exiting ...");
fapi_try_exit:
    return fapi2::current_err;

}
