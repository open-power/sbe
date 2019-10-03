/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_fir_init.C $ */
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
/// @file  p10_sbe_chiplet_fir_init.C
///
/// @brief Setup per-chiplet FIRs
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_fir_init.H"
#include "p10_scom_perv_e.H"
#include <multicast_group_defs.H>

enum P10_SBE_CHIPLET_FIR_INIT_Private_Constants
{
    LFIR_ACTION1_VALUE = 0xFFFFFFFFFFFFFFFF
};

fapi2::ReturnCode p10_sbe_chiplet_fir_init(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    auto l_mc_all = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);

    FAPI_DBG("p10_sbe_chiplet_fir_init: Entering ...");

    FAPI_DBG("Set up pervasive LFIRs on all chiplets");
    FAPI_TRY(fapi2::putScom(l_mc_all, EPS_FIR_LOCAL_ACTION1, LFIR_ACTION1_VALUE));

    FAPI_DBG("p10_sbe_chiplet_fir_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
