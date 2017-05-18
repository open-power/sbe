/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_ras_runtime_scom.C $ */
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
/// @file  p9_hcd_cache_ras_runtime_scom.C
/// @brief EQ FSP/Host runtime scoms
///
/// Procedure Summary:
///   Run-time updates by FSP/Host(including HostServices and Hypervisors)
///     that are put on the cache image by STOP API calls
///   Dynamically built pointer where a NULL is checked before execution
///   If NULL (the SBE case), return
///   Else call the function at the pointer;  pointer is filled in by
///     STOP image build
///   Powerbus (MCD) and L3 BAR settings
///   Runtime FIR mask updates from PRD
///   L2/L3 Repairs
///

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p9_hcd_cache_ras_runtime_scom.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: EQ FSP/HOST runtime scoms
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_ras_runtime_scom(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_ras_runtime_scom");

    FAPI_INF("<<p9_hcd_cache_ras_runtime_scom");

    return fapi2::FAPI2_RC_SUCCESS;
}



