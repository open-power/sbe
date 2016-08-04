/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_inject_local_xstop.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
//-----------------------------------------------------------------------------------
//
// @file p9_inject_local_xstop.H
// @brief header file for triggering local checkstop in the core
//
// *HWP HWP Owner: Joshua Hannan jlhannan@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Core
// *HWP Level: 2
// *HWP Consumed by: SBE
// ----------------------------------------------------------------------------------
//
// *! ADDITIONAL COMMENTS :
// *!
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P9_INJECT_LOCAL_XSTOP_H_
#define _P9_INJECT_LOCAL_XSTOP_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>

//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode
(*p9_inject_local_xstop_FP_t) (
    fapi2::Target<fapi2::TARGET_TYPE_CORE>& );

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------

/// @brief Clears bit 51 of FIRMASK register, sets bit 51 of FIR ACTION registers and
///        FIR register to generate a local checkstop
/// @param[in] i_target => Core target
/// @return FAPI_RC_SUCCESS if the local checkstop completes successfully

    fapi2::ReturnCode p9_inject_local_xstop(fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);

} //extern "C"

#endif //_P9_INJECT_LOCAL_XSTOP_H_

