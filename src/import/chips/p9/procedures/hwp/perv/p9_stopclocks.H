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
// *HWP Level           : 1
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


#ifndef _P9_STOPCLOCKS_H_
#define _P9_STOPCLOCKS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_stopclocks_FP_t)(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
        const bool,
        const bool,
        const bool,
        const bool,
        const bool,
        const bool,
        const bool);

extern "C"
{
    /**
    * @brief proc_stopclocks procedure:  The purpose of this procedure is to stop the clocks in the non-core chiplets of the P9 processor chip
    *
    * @param[in] i_target                Reference to processor chip target
    * @param[in] i_stop_nest_clks        True if NEST chiplet clocks should be stopped, else false
    * @param[in] i_stop_mc_clks          True if MC   chiplet clocks should be stopped, else false
    * @param[in] i_stop_xbus_clks        True if XBUS chiplet clocks should be stopped, else false
    * @param[in] i_stop_obus_clks        True if OBUS chiplet clocks should be stopped, else false
    * @param[in] i_stop_pcie_clks        True if PCIE chiplet clocks should be stopped, else false
    * @param[in] i_stop_tp_clks          True if PERV (TP) chiplet clocks should be stopped, else false
    * @param[in] i_stop_vitl_clks        True if PERV VITL clocks should be stopped, else false
    *
    * @return ReturnCode
    */
    fapi2::ReturnCode p9_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplet,
                                    const bool i_stop_nest_clks,
                                    const bool i_stop_mc_clks,
                                    const bool i_stop_xbus_clks,
                                    const bool i_stop_obus_clks,
                                    const bool i_stop_pcie_clks,
                                    const bool i_stop_tp_clks,
                                    const bool i_stop_vitl_clks);
}

#endif
