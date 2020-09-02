/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_tp_stopclocks.H $ */
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
/// @file  p10_tp_stopclocks.H
///
/// @brief Stop clocks for tp chiplet
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------


#ifndef _P10_TP_STOPCLOCKS_H_
#define _P10_TP_STOPCLOCKS_H_
#define btos(x) ((x)?"TRUE":"FALSE")

#include <fapi2.H>


typedef fapi2::ReturnCode (*p10_tp_stopclocks_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&, bool, bool, bool);

/// @brief Stop clocks for TP chiplet
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p10_tp_stopclocks(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip, const bool i_stop_tp_clks, const bool i_stop_sbe_clks,
                                        const bool i_ignore_pib_net_dpllnest_ppll);
}

#endif
