/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_check_master_stop15.H $ */
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
/// @file  p10_sbe_check_master_stop15.H
///
///------------------------------------------------------------------------------
// *HWP HWP Owner   : Greg Still <stillgsg@us.ibm.com>
// *HWP FW Owner    : Prem S Jha <premjha2@in.ibm.com>
// *HWP Team        : PM
// *HWP Level       : 1
// *HWP Consumed by : SBE, FSP
//------------------------------------------------------------------------------


#ifndef _P10_SBE_CHECK_MASTER_STOP15_H_
#define _P10_SBE_CHECK_MASTER_STOP15_H_

#include <fapi2.H>

typedef fapi2::ReturnCode (*p10_sbe_check_master_stop15_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_CORE>&);


/// @brief Check if the targeted core (master) is fully in STOP15
///
/// @param[in]  i_target   Reference to TARGET_TYPE_CORE target
///
/// @return     FAPI2_RC_SUCCESS if success
/// @return     STOP15_PENDING  STOP 15 not reached, but no error
///             HW state (still in progress)
/// @return     Others indicate hardware failure
/// @note       On non-SBE platforms, regular FAPI_ASSERT based error handling
///             and related FFDC and callouts
///             On SBE, for code space optimization, FAPI_ASSERT is not used.
///             Please use p10_collect_deadman_ffdc to generate appropriate RC
///             and FFDC
///
extern "C"
{
    fapi2::ReturnCode
    p10_sbe_check_master_stop15(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);
}

#endif  // _P10_SBE_CHECK_MASTER_STOP15_H_
