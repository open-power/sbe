/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_poweron.C $ */
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
/// @file  p9_hcd_cache_poweron.C
/// @brief Cache Chiplet Power-on
///
/// Procedure Summary:
///   Set glsmux async reset
///   Set DPLL ff_bypass
///   Command the cache PFET controller to power-on
///   Check for valid power on completion
///   Polled Timeout:  100us

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include <p9_common_poweronoff.H>
#include "p9_hcd_cache_poweron.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Chiplet Power-on
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_poweron");
    fapi2::buffer<uint64_t> l_data64;
    uint8_t                 l_attr_dd1_vcs_workaround   = 0;
    uint8_t                 l_attr_chip_unit_pos        = 0;
    uint32_t                l_scom_addr                 = 0;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    auto l_core_functional_vector =
        i_target.getChildren<fapi2::TARGET_TYPE_CORE>
        (fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW388878, l_chip,
                           l_attr_dd1_vcs_workaround));

    // Gate the PCBMux request so scanning doesn't cause random requests
    for(auto& it : l_core_functional_vector)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it.getParent<fapi2::TARGET_TYPE_PERV>(),
                               l_attr_chip_unit_pos));
        FAPI_DBG("Assert core[%d] PCB Mux Disable via C_SLAVE_CONFIG[7]",
                 (l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET));
        l_scom_addr = (C_SLAVE_CONFIG_REG + (0x1000000 *
                                             (l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET)));
        FAPI_TRY(getScom(l_chip, l_scom_addr, l_data64));
        FAPI_TRY(putScom(l_chip, l_scom_addr, DATA_SET(7)));
    }

    //--------------------------
    // Prepare to power on cache
    //--------------------------

    FAPI_DBG("Drop chiplet enable via NET_CTRL0[0]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(0)));

    FAPI_DBG("Assert L2 glsmux reset via EXCLK_GRID_CTRL[32:33]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, MASK_OR(32, 2, 0x3)));

    FAPI_DBG("Assert cache glsmux reset via CLOCK_GRID_CTRL[0]");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_SET(0)));

    FAPI_DBG("Assert analog logic fence via QPPM_PFCS[11]");
    FAPI_TRY(putScom(i_target, EQ_PPM_PFCS_WOR, MASK_SET(11)));

    //-----------------------
    // Power on cache chiplet
    //-----------------------

    FAPI_DBG("Power on cache chiplet");

    if (l_attr_dd1_vcs_workaround)
    {
        FAPI_TRY(p9_common_poweronoff<fapi2::TARGET_TYPE_EQ>(i_target, p9power::POWER_ON_VDD));
    }
    else
    {
        FAPI_TRY(p9_common_poweronoff<fapi2::TARGET_TYPE_EQ>(i_target, p9power::POWER_ON));
    }

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_poweron");
    return fapi2::current_err;
}
