/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_stopclocks.H $     */
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
/// @file  p9_stopclocks.H
///
/// @brief This is the header file for p9_stopclocks.
//------------------------------------------------------------------------------
// *HWP HW Owner        : Soma BhanuTej <soma.bhanu@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


#ifndef _P9_STOPCLOCKS_H_
#define _P9_STOPCLOCKS_H_
#define btos(x) ((x)?"TRUE":"FALSE")

#include <fapi2.H>
#include <p9_hcd_common.H>

// Structure to select flags for stop clocks
struct p9_stopclocks_flags
{
    uint32_t stop_nest_clks       : 1; //True if NEST chiplet clocks should be stopped, else false
    uint32_t stop_mc_clks         : 1; //True if MC   chiplet clocks should be stopped, else false
    uint32_t stop_xbus_clks       : 1; //True if XBUS chiplet clocks should be stopped, else false
    uint32_t stop_obus_clks       : 1; //True if OBUS chiplet clocks should be stopped, else false
    uint32_t stop_pcie_clks       : 1; //True if PCIE chiplet clocks should be stopped, else false
    uint32_t stop_tp_clks         : 1; //True if PERV (TP) chiplet clocks all except PIB/NET should be stopped, else false
    uint32_t stop_pib_clks        : 1; //True if PERV (TP) chiplet PIB/NET clocks should be stopped, else false
    uint32_t stop_vitl_clks       : 1; //True if PERV VITL clocks should be stopped, else false
    uint32_t stop_cache_clks      : 1; //True if CACHE chiplet clocks should be stopped, else false
    uint32_t stop_core_clks       : 1; //True if CORE chiplet clocks should be stopped, else false
    // Default constructor - fill default values
    p9_stopclocks_flags()
    {
        stop_nest_clks = true;
        stop_mc_clks = true;
        stop_xbus_clks = true;
        stop_obus_clks = true;
        stop_pcie_clks = true;
        stop_tp_clks = false;
        stop_pib_clks = false;
        stop_vitl_clks = false;
        stop_cache_clks = true;
        stop_core_clks = true;
    }
    // Set all the flags to false
    void clearAll()
    {
        stop_nest_clks = false;
        stop_mc_clks = false;
        stop_xbus_clks = false;
        stop_obus_clks = false;
        stop_pcie_clks = false;
        stop_tp_clks = false;
        stop_pib_clks = false;
        stop_vitl_clks = false;
        stop_cache_clks = false;
        stop_core_clks = false;
    }
};

typedef fapi2::ReturnCode (*p9_stopclocks_FP_t)(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
        const p9_stopclocks_flags& i_flags,
        const p9hcd::P9_HCD_CLK_CTRL_CONSTANTS,
        const p9hcd::P9_HCD_EX_CTRL_CONSTANTS );

extern "C"
{
    /**
    * @brief p9_stopclocks procedure:  The purpose of this procedure is to stop the clocks of the P9 processor chip
    *
    * @param[in] i_target                Reference to processor chip target
    * @param[in] i_flags                 Flags as per the following definition
    *            i_flags.stop_nest_clks        True if NEST chiplet clocks should be stopped, else false
    *            i_flags.stop_mc_clks          True if MC   chiplet clocks should be stopped, else false
    *            i_flags.stop_xbus_clks        True if XBUS chiplet clocks should be stopped, else false
    *            i_flags.stop_obus_clks        True if OBUS chiplet clocks should be stopped, else false
    *            i_flags.stop_pcie_clks        True if PCIE chiplet clocks should be stopped, else false
    *            i_flags.stop_tp_clks          True if PERV (TP) chiplet clocks all except PIB/NET should be stopped, else false
    *            i_flags.stop_pib_clks         True if PERV (TP) chiplet PIB/NET clocks should be stopped, else false
    *            i_flags.stop_vitl_clks        True if PERV VITL clocks should be stopped, else false
    *            i_flags.stop_cache_clks       True if CACHE chiplet clocks should be stopped, else false
    *            i_flags.stop_core_clks        True if CORE chiplet clocks should be stopped, else false
    * @param[in] i_eq_clk_regions        EQ chiplet clock regions of which clocks should be stopped
    * @param[in] i_ex_select             EX chiplet selected for clocks stop
    *
    * @return ReturnCode
    */
    fapi2::ReturnCode p9_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                    const p9_stopclocks_flags& i_flags = p9_stopclocks_flags(),
                                    const p9hcd::P9_HCD_CLK_CTRL_CONSTANTS i_eq_clk_regions = p9hcd::CLK_REGION_ALL_BUT_PLL_REFR,
                                    const p9hcd::P9_HCD_EX_CTRL_CONSTANTS i_ex_select = p9hcd::BOTH_EX);
}

#endif
