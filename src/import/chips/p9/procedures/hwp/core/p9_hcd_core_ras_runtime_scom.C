/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/core/p9_hcd_core_ras_runtime_scom.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_hcd_core_ras_runtime_scom.C
/// @brief Core FSP/Host run-time SCOMS
///
/// Procedure Summary:
///   Run-time updates from FSP based PRD, etc that are put on the core image
///   by STOP API calls
///   Dynamically built pointer where a NULL is checked before execution
///   If NULL (the SBE case), return
///   Else call the function at the pointer;
///   pointer is filled in by STOP image build
///   Run-time updates from Host code that are put on the core image by
///   STOP API calls
///     Restore Hypervisor, Host PRD, etc. SCOMs
///

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 3

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "p9_hcd_core_ras_runtime_scom.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Core FSP/Host run-time SCOMS
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_ras_runtime_scom(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_ras_runtime_scom");

    FAPI_INF("<<p9_hcd_core_ras_runtime_scom");

    return fapi2::FAPI2_RC_SUCCESS;
}



