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
/// @brief procedure for scan initializing PLL config bits for IOHS, PCI, MC Chiplets
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_pll_initf.H"

fapi2::ReturnCode p10_sbe_chiplet_pll_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_chiplet_pll_initf: Entering ...");
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    uint8_t l_mc_pll_bucket[4], l_iohs_pll_bucket[8] ;

    // determine mc pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_PLL_BUCKET, FAPI_SYSTEM, l_mc_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_MC_PLL_BUCKET)");
    // determine iohs pll buckets
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IOHS_PLL_BUCKET, FAPI_SYSTEM, l_iohs_pll_bucket),
             "Error from FAPI_ATTR_GET (ATTR_IOHS_PLL_BUCKET)");

fapi_try_exit:
    FAPI_INF("p10_sbe_chiplet_pll_initf: Exiting ...");
    return fapi2::current_err;
}
