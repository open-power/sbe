/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_stopclocks.C $     */
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
/// @file  p9_stopclocks.C
///
/// @brief The purpose of this procedure is to stop the clocks in the non-core chiplets of the P9 processor chip
//------------------------------------------------------------------------------
// *HWP HW Owner        : Soma BhanuTej <soma.bhanu@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 1
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------

//## auto_generated
#include "p9_stopclocks.H"

//------------------------------------------------------------------------------
// Function definition:  proc_stopclocks
// parameters: i_target                =>   chip target
//             i_stop_nest_clks        =>   True to stop NEST chiplet clocks                              (should default TRUE)
//             i_stop_mc_clks          =>   True to stop MC chiplet clocks                                (should default TRUE)
//             i_stop_xbus_clks        =>   True to stop XBUS chiplet clocks                              (should default TRUE)
//             i_stop_abus_clks        =>   True to stop OBUS chiplet clocks                              (should default TRUE)
//             i_stop_pcie_clks        =>   True to stop PCIE chiplet clocks                              (should default TRUE)
//             i_stop_tp_clks          =>   True to stop PERVASIVE (TP) chiplet clocks                    (should default FALSE)
//             i_stop_vitl_clks        =>   True to stop PERVASIVE VITL clocks                            (should default FALSE)
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------

fapi2::ReturnCode p9_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chiplet,
                                const bool i_stop_nest_clks,
                                const bool i_stop_mc_clks,
                                const bool i_stop_xbus_clks,
                                const bool i_stop_obus_clks,
                                const bool i_stop_pcie_clks,
                                const bool i_stop_tp_clks,
                                const bool i_stop_vitl_clks)
{
    FAPI_INF("p9_stopclocks : Entering ...");

    FAPI_INF("p9_stopclocks : Exiting ...");

    return fapi2::FAPI2_RC_SUCCESS;

}
