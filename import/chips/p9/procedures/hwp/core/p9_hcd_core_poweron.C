/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/core/p9_hcd_core_poweron.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file  p9_hcd_core_poweron.C
/// @brief Core Chiplet Power-on
///
/// Procedure Summary:
///   Set glsmux async reset
///   Command the core PFET controller to power-on, via putscom to CPPM
///   Check for valid power on completion, via getscom from CPPM
///     Polled Timeout:  100us

// *HWP HWP Owner          : David Du          <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still        <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S     <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include <p9_common_poweronoff.H>
#include <p9_common_poweronoff.C>
#include "p9_hcd_core_poweron.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Core Chiplet Power-on
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_poweron");
    fapi2::buffer<uint64_t> l_data64;

    //-------------------------
    // Prepare to power on core
    //-------------------------

    FAPI_DBG("Drop chiplet enable via NET_CTRL0[0]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(0)));

    FAPI_DBG("Assert PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, MASK_SET(25)));

    FAPI_DBG("Assert chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, MASK_SET(26)));

    FAPI_DBG("Assert vital thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, MASK_SET(16)));

    FAPI_DBG("Assert core glsmux reset via PPM_CGCR[0]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, MASK_SET(0)));

    //----------------------
    // Power on core chiplet
    //----------------------

    FAPI_DBG("Power on core chiplet");
    FAPI_TRY(p9_common_poweronoff<fapi2::TARGET_TYPE_CORE>(i_target, p9power::POWER_ON_VDD));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_poweron");
    return fapi2::current_err;
}
