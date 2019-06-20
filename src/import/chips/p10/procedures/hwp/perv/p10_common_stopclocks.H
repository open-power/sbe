/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_common_stopclocks.H $ */
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
/// @file  p10_common_stopclocks.H
///
/// @brief Common module for stopclocks
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------

#ifndef _P10_COMMON_STOPCLOCKS_H_
#define _P10_COMMON_STOPCLOCKS_H_


#include <fapi2.H>

fapi2::ReturnCode p10_common_stopclocks_regions_setup(const
        fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
        fapi2::buffer<uint64_t> i_regions_value,
        fapi2::buffer<uint64_t>& o_regions_value);

fapi2::ReturnCode p10_common_stopclocks_cplt_ctrl_action_function(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet, fapi2::buffer<uint64_t> i_regions = 0xFFFFFFFFFFFFFFFF);

fapi2::ReturnCode p10_common_stopclocks_raise_fence(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p10_common_stopclocks_set_vitalfence_flushmode(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p10_common_stopclocks_sbe_clkstop(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_common_stopclocks_poll_cbs_cmd_complete(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_common_stopclock_is_scommable(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt,
    uint8_t& o_isScommable);

fapi2::ReturnCode p10_common_stopclock_chiplet_accessible(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    bool& o_isAccessible);

fapi2::ReturnCode p10_common_stopclock_tp_chiplet_accessible(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    bool& o_isAccessible);
#endif
