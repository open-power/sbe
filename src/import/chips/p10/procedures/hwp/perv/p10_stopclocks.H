/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_stopclocks.H $   */
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
/// @file  p10_stopclocks.H
///
/// @brief This is the header file for p10_stopclocks.
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP:SBE:HB
//------------------------------------------------------------------------------


#ifndef _P10_STOPCLOCKS_H_
#define _P10_STOPCLOCKS_H_
#define btos(x) ((x)?"TRUE":"FALSE")

#include <fapi2.H>
#include <p10_hcd_common.H>

// Structure to select flags for stop clocks
struct p10_stopclocks_flags
{
    uint32_t stop_nest_clks       : 1; //True if NEST chiplet clocks should be stopped, else false
    uint32_t stop_pcie_clks       : 1; //True if PCIE chiplet clocks should be stopped, else false
    uint32_t stop_mc_clks         : 1; //True if MC   chiplet clocks should be stopped, else false
    uint32_t stop_pau_clks        : 1; //True if PAU chiplet clocks should be stopped, else false
    uint32_t stop_axon_clks       : 1; //True if AXON chiplet clocks should be stopped, else false
    uint32_t stop_tp_clks         : 1; //True if PERV (TP) chiplet clocks all except SBE should be stopped, else false
    uint32_t stop_sbe_clks        : 1; //True if PERV (TP) chiplet SBE clocks should be stopped, else false
    uint32_t stop_vitl_clks       : 1; //True if PERV VITL clocks should be stopped, else false
    uint32_t stop_cache_clks      : 1; //True if CACHE chiplet clocks should be stopped, else false
    uint32_t stop_core_clks       : 1; //True if CORE chiplet clocks should be stopped, else false

    // Default constructor - fill default values
    p10_stopclocks_flags()
    {
        stop_nest_clks = true;
        stop_pcie_clks = true;
        stop_mc_clks = true;
        stop_pau_clks = true;
        stop_axon_clks = true;
        stop_tp_clks = false;
        stop_sbe_clks = false;
        stop_vitl_clks = false;
        stop_cache_clks = true;
        stop_core_clks = true;
    }
    // Set all the flags to false
    void clearAll()
    {
        stop_nest_clks = false;
        stop_pcie_clks = false;
        stop_mc_clks = false;
        stop_pau_clks = false;
        stop_axon_clks = false;
        stop_tp_clks = false;
        stop_sbe_clks = false;
        stop_vitl_clks = false;
        stop_cache_clks = false;
        stop_core_clks = false;
    }
};

typedef fapi2::ReturnCode (*p10_stopclocks_FP_t)(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
        const p10_stopclocks_flags& i_flags );

extern "C"
{
    /**
    * @brief p10_stopclocks procedure:  The purpose of this procedure is to stop the clocks of the P10 processor chip
    *
    * @param[in] i_target                      Reference to processor chip target
    * @param[in] i_flags                       Flags as per the following definition
    *            i_flags.stop_nest_clks        True if NEST chiplet clocks should be stopped, else false
    *            i_flags.stop_pcie_clks        True if PCIE chiplet clocks should be stopped, else false
    *            i_flags.stop_mc_clks          True if MC   chiplet clocks should be stopped, else false
    *            i_flags.stop_pau_clks         True if PAU chiplet clocks should be stopped, else false
    *            i_flags.stop_axon_clks        True if AXON chiplet clocks should be stopped, else false
    *            i_flags.stop_tp_clks          True if PERV (TP) chiplet clocks all except SBE should be stopped, else false
    *            i_flags.stop_sbe_clks         True if PERV (TP) chiplet SBE clocks should be stopped, else false
    *            i_flags.stop_vitl_clks        True if PERV VITL clocks should be stopped, else false
    *            i_flags.stop_cache_clks       True if CACHE chiplet clocks should be stopped, else false
    *            i_flags.stop_core_clks        True if CORE chiplet clocks should be stopped, else false
    *
    *
    * @return ReturnCode
    */
    fapi2::ReturnCode p10_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                     const p10_stopclocks_flags& i_flags = p10_stopclocks_flags());
}

#endif
