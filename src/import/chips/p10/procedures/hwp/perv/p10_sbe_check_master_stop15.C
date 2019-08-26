/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_check_master_stop15.C $ */
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
///
/// @file  p10_sbe_check_master_stop15.H
/// @brief Check if the targeted core (master) is fully in STOP15
///
// *HWP HWP Owner          : Yue Du     <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem S Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Level              : 2
// *HWP Consumed by        : SBE
///
/// High-level procedure flow:
/// @verbatim
///    - Read the STOP History Register from the target core
///    - Return SUCCESS if::
///        - STOP_GATED is set (indicating it is not running)
///        - STOP_TRANSITION is COMPLETE (indicating it is stable)
///        - ACT_STOP_LEVEL is at the appropriate value (either 11 (0xB) or
///          15 (0xF)
///    - Return PENDING if
///        - STOP_GATED is RUNNING
///         or
///        - STOP_GATED is GATED  (indicating it is not running)
///        - STOP_TRANSITION is not COMPLETE (indicating transtion is progress)
///    - Return ERROR if
///        - STOP_GATED is set, STOP_TRANSITION is COMPLETE and ACT_STOP_LEVEL
///          is not appropriate
///        - Hardware access errors
/// @endverbatim

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include <p10_sbe_check_master_stop15.H>
#include <p10_query_core_stop_state.H>

// -----------------------------------------------------------------------------
//  Function definitions
// -----------------------------------------------------------------------------

fapi2::ReturnCode p10_sbe_check_master_stop15(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF("> p10_sbe_check_master_stop15");

    FAPI_TRY(p10_query_core_stop_state(i_target, 15));

fapi_try_exit:
    FAPI_INF("< p10_sbe_check_master_stop15");

    return fapi2::current_err;
}
