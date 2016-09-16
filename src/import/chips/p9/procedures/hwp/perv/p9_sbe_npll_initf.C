/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_npll_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_npll_initf.H"

fapi2::ReturnCode p9_sbe_npll_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_npll_initf: Entering ...");

    uint8_t l_read_attr = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    RingID ringID = perv_pll_bndy_bucket_1;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM , l_read_attr),
             "Error from FAPI_ATTR_GET (ATTR_NEST_PLL_BUCKET)");

    switch(l_read_attr)
    {
        case 1:
            ringID = perv_pll_bndy_bucket_1;
            break;

        case 2:
            ringID = perv_pll_bndy_bucket_2;
            break;

        case 3:
            ringID = perv_pll_bndy_bucket_3;
            break;

        case 4:
            ringID = perv_pll_bndy_bucket_4;
            break;

        case 5:
            ringID = perv_pll_bndy_bucket_5;
            break;

        default:
            FAPI_ASSERT(false,
                        fapi2::P9_SBE_NPLL_INITF_UNSUPPORTED_BUCKET().
                        set_TARGET(i_target_chip).
                        set_BUCKET_INDEX(l_read_attr),
                        "Unsupported Nest PLL bucket value!");
    }

    FAPI_DBG("Scan perv_pll_bndy_bucket_%d ring", l_read_attr);
    FAPI_TRY(fapi2::putRing(i_target_chip, ringID, fapi2::RING_MODE_SET_PULSE_NSL),
             "Error from putRing (perv_pll_bndy, ringID: %d)", ringID);

fapi_try_exit:
    FAPI_INF("p9_sbe_npll_initf: Exiting ...");
    return fapi2::current_err;
}
