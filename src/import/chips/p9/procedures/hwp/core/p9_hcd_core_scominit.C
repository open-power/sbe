/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/core/p9_hcd_core_scominit.C $ */
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
/// @file  p9_hcd_core_scominit.C
/// @brief Core SCOM Inits
///
/// Procedure Summary:
/// Apply any coded SCOM initialization to core
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

#include <p9_quad_scom_addresses.H>
#include <p9_quad_scom_addresses_fld.H>
#include <p9_hcd_common.H>
#include <p9_core_scom.H>
#include "p9_hcd_core_scominit.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

enum P9_HCD_CORE_SCOMINIT_CONSTANTS
{
    CORE_HANG_LIMIT_3_HANG_PULSES   = 0x9F,
    CORE_HANG_LIMIT_5_HANG_PULSES   = 0x27,
    CORE_HANG_LIMIT_10_HANG_PULSES  = 0xA1,
    CORE_HANG_LIMIT_50_HANG_PULSES  = 0x99,
    CORE_HANG_LIMIT_100_HANG_PULSES = 0x2D,
    CORE_HANG_LIMIT_150_HANG_PULSES = 0xF6,
    CORE_HANG_LIMIT_200_HANG_PULSES = 0x64,

    NEST_HANG_LIMIT_20_HANG_PULSES  = 0x5F,
    NEST_HANG_LIMIT_50_HANG_PULSES  = 0x99,
    NEST_HANG_LIMIT_100_HANG_PULSES = 0x2D,
    NEST_HANG_LIMIT_150_HANG_PULSES = 0xF6,
    NEST_HANG_LIMIT_200_HANG_PULSES = 0x64
};

//-----------------------------------------------------------------------------
// Procedure: Core SCOM Inits
//-----------------------------------------------------------------------------


fapi2::ReturnCode
p9_hcd_core_scominit(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_scominit");
    fapi2::ReturnCode       l_rc;
    fapi2::buffer<uint64_t> l_data64;
    uint8_t                 l_attr_dd1_core_hang_limit;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW399609, l_chip,
                           l_attr_dd1_core_hang_limit));

    /// @todo RTC158181 how about bit 6?
    FAPI_DBG("Restore SYNC_CONFIG[8] for stop1");
    FAPI_TRY(getScom(i_target, C_SYNC_CONFIG, l_data64));
    FAPI_TRY(putScom(i_target, C_SYNC_CONFIG, DATA_SET(8)));

    FAPI_DBG("Enable DTS via THERM_MODE_REG[5,6-9,20-21]");
    FAPI_TRY(getScom(i_target, C_THERM_MODE_REG, l_data64));
    // DTS sampling enable | sample pulse count | DTS loop1 enable
    l_data64.setBit<5>().insertFromRight<6, 4>(0xF).insertFromRight<20, 2>(0x3);
    FAPI_TRY(putScom(i_target, C_THERM_MODE_REG, l_data64));

    FAPI_DBG("Invoke Core SCOM Initfile");
    FAPI_EXEC_HWP(l_rc, p9_core_scom, i_target);

    if (l_rc)
    {
        FAPI_ERR("Error from p9_core_scom (p9.core.scom.initfile)");
        fapi2::current_err = l_rc;
        goto fapi_try_exit;
    }

    FAPI_DBG("Update Core Hang Pulse Dividers via C_HANG_CONTROL[0-15]");
    FAPI_TRY(getScom(i_target, C_HANG_CONTROL, l_data64));

    if (l_attr_dd1_core_hang_limit)
    {
        l_data64.insertFromRight<C_HANG_CONTROL_CORE_LIMIT,
                                 C_HANG_CONTROL_CORE_LIMIT_LEN>(CORE_HANG_LIMIT_100_HANG_PULSES);
    }
    else
    {
        l_data64.insertFromRight<C_HANG_CONTROL_CORE_LIMIT,
                                 C_HANG_CONTROL_CORE_LIMIT_LEN>(CORE_HANG_LIMIT_10_HANG_PULSES);
    }

    l_data64.insertFromRight<C_HANG_CONTROL_NEST_LIMIT,
                             C_HANG_CONTROL_NEST_LIMIT_LEN>(NEST_HANG_LIMIT_100_HANG_PULSES);

    FAPI_TRY(putScom(i_target, C_HANG_CONTROL, l_data64));


fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_scominit");
    return fapi2::current_err;
}


