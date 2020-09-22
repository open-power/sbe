/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_check_master.C $ */
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
/// @file  p10_sbe_check_master.C
///
/// @brief Deremine if this is master SBE -- External FSI/GP bitIf master continue, else enable runtime chipOps
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------
// EKB-Mirror-To: hw/ppe

#include "p10_sbe_check_master.H"
#include <p10_perv_sbe_cmn.H>
#include <p10_hang_pulse_mc_setup_tables.H>

fapi2::ReturnCode p10_sbe_check_master(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_DBG("p10_sbe_check_master: Entering ...");

    // Set up multicast groups, MCGROUP_GOOD_CORES gets all EQs that
    // are good according to ATTR_PG
    FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target_chip,
             SELECT_EX_MC_GROUPS),
             "Error from p10_perv_sbe_cmn_setup_multicast_groups");

fapi_try_exit:
    FAPI_DBG("p10_sbe_check_master: Exiting ...");
    return fapi2::current_err;
}
