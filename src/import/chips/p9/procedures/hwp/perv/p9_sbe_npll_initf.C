/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_npll_initf.C $ */
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
/// @file  p9_sbe_npll_initf.C
///
/// @brief apply initfile for level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_npll_initf.H"
#include <p9_ring_id.h>

fapi2::ReturnCode p9_sbe_npll_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_npll_initf: Entering ...");

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint8_t l_npll_bucket = 0;
    RingID l_npll_ring_id = perv_pll_bndy_bucket_1;
    uint8_t l_fpll_bucket = 0;
    RingID l_fpll_ring_id = perv_pll_bndy_flt_1;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_PLL_BUCKET,
                           FAPI_SYSTEM ,
                           l_npll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_NEST_PLL_BUCKET)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FILTER_PLL_BUCKET,
                           i_target_chip,
                           l_fpll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_FILTER_PLL_BUCKET)");

    switch (l_npll_bucket)
    {
        case 1:
            l_npll_ring_id = perv_pll_bndy_bucket_1;
            break;

        case 2:
            l_npll_ring_id = perv_pll_bndy_bucket_2;
            break;

        case 3:
            l_npll_ring_id = perv_pll_bndy_bucket_3;
            break;

        case 4:
            l_npll_ring_id = perv_pll_bndy_bucket_4;
            break;

        case 5:
            l_npll_ring_id = perv_pll_bndy_bucket_5;
            break;

        default:
            FAPI_ASSERT(false,
                        fapi2::P9_SBE_NPLL_INITF_UNSUPPORTED_NPLL_BUCKET().
                        set_TARGET(i_target_chip).
                        set_BUCKET_INDEX(l_npll_bucket),
                        "Unsupported Nest PLL bucket value!");
    }

    // scan PLL ring once to establish nest PLL
    FAPI_DBG("Scan perv_pll_bndy_bucket_%d ring",
             l_npll_bucket);
    FAPI_TRY(fapi2::putRing(i_target_chip,
                            l_npll_ring_id,
                            fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (perv_pll_bndy, l_npll_ring_id: %d)",
             l_npll_ring_id);

    if (l_fpll_bucket)
    {
        switch (l_fpll_bucket)
        {
            case 1:
                l_fpll_ring_id = perv_pll_bndy_flt_1;
                break;

            case 2:
                l_fpll_ring_id = perv_pll_bndy_flt_2;
                break;

            case 3:
                l_fpll_ring_id = perv_pll_bndy_flt_3;
                break;

            case 4:
                l_fpll_ring_id = perv_pll_bndy_flt_4;
                break;

            default:
                FAPI_ASSERT(false,
                            fapi2::P9_SBE_NPLL_INITF_UNSUPPORTED_FPLL_BUCKET().
                            set_TARGET(i_target_chip).
                            set_BUCKET_INDEX(l_fpll_bucket),
                            "Unsupported Filter PLL bucket value!");
        }

        // re-scan PLL ring to apply overlay containing filter PLL BGoffset
        // selected from MVPD
        FAPI_DBG("Re-scan perv_pll_bndy to apply perv_pll_bndy_flt_%d ring",
                 l_fpll_bucket);
        FAPI_TRY(fapi2::putRing(i_target_chip,
                                l_fpll_ring_id,
                                fapi2::RING_MODE_SET_PULSE_NSL),
                 "Error from putRing (perv_pll_bndy, l_fpll_ring_id: %d)",
                 l_fpll_ring_id);
    }

fapi_try_exit:
    FAPI_INF("p9_sbe_npll_initf: Exiting ...");
    return fapi2::current_err;
}
