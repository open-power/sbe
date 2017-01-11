/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/core/p9_hcd_core_chiplet_reset.C $ */
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
/// @file  p9_hcd_core_chiplet_reset.C
/// @brief Core Chiplet Reset
///
/// Procedure Summary:
///   Reset core chiplet logic
///   Clocking:
///    - setup cache sector buffer strength,
///      pulse mode and pulsed mode enable values
///      (attribute dependency Nimbus/Cumulus)
///    - Drop glsmux async reset
///   Scan0 flush entire core chiplet

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_core_chiplet_reset.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P9_HCD_CORE_CHIPLET_RESET_CONSTANTS
{
    // (1)PCB_EP_RESET
    // (2)CLK_ASYNC_RESET
    // (3)PLL_TEST_EN
    // (4)PLLRST
    // (5)PLLBYP
    // (11)EDIS
    // (12)VITL_MPW1
    // (13)VITL_MPW2
    // (14)VITL_MPW3
    // (16)VITL_THOLD
    // (18)FENCE_EN
    // (22)FUNC_CLKSEL
    // (25)PCB_FENCE
    // (26)LVLTRANS_FENCE
    C_NET_CTRL0_INIT_VECTOR = (BIT64(1) | BITS64(3, 3) | BITS64(11, 4) |
                               BIT64(16) | BIT64(18) | BIT64(22) | BITS64(25, 2)),
    CORE_GLSMUX_RESET_DELAY_CORE_CYCLES = 200
};

//------------------------------------------------------------------------------
// Procedure: Core Chiplet Reset
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_chiplet_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_chiplet_reset");
    fapi2::buffer<uint64_t>                        l_data64;
    uint8_t                                        l_attr_dpll_bypass;
    uint8_t                                        l_attr_vdm_enable;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> l_sys;
    auto l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DPLL_BYPASS, l_chip, l_attr_dpll_bypass));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_VDM_ENABLE,  l_sys,  l_attr_vdm_enable));

    //--------------------------
    // Reset core chiplet logic
    //--------------------------
    // If there is an unused, powered-off core chiplet which needs to be
    // configured in the following steps to setup the PCB endpoint.

    FAPI_DBG("Init NET_CTRL0[1,3-5,11-14,16,18,22,25,26],step needed for hotplug");
    l_data64 = C_NET_CTRL0_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, l_data64));
    l_data64 |= BIT64(2); // make sure bit 2 is untouched
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, l_data64));

    FAPI_DBG("Flip core glsmux to refclk via PPM_CGCR[3]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, MASK_SET(0)));

    FAPI_DBG("Assert core progdly and DCC bypass via NET_CTRL1[1,2]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL1_WOR, MASK_OR(1, 2, 3)));

    FAPI_DBG("Drop vital thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(16)));

    /// @todo RTC158181 setup sector buffer strength, pulse mode and pulsed mode enable

    FAPI_DBG("Drop core glsmux reset via PPM_CGCR[0]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, 0));

    FAPI_TRY(fapi2::delay(
                 CORE_GLSMUX_RESET_DELAY_CORE_CYCLES * p9hcd::CLK_PERIOD_250PS / 1000,
                 CORE_GLSMUX_RESET_DELAY_CORE_CYCLES * p9hcd::SIM_CYCLE_4U4D));

    FAPI_DBG("Flip core glsmux to DPLL via PPM_CGCR[3]");
    FAPI_TRY(putScom(i_target, C_PPM_CGCR, MASK_SET(3)));

    FAPI_DBG("Assert chiplet enable via NET_CTRL0[0]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WOR, MASK_SET(0)));

    FAPI_DBG("Drop PCB endpoint reset via NET_CTRL0[1]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(1)));

    FAPI_DBG("Drop chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(26)));

    FAPI_DBG("Drop PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL0_WAND, MASK_UNSET(25)));

    if (l_attr_dpll_bypass == 0)
    {
        // HW390253: The core clock controller itself is clocked at 2:1 versus the core clock,
        // so it will introduce an additional 2:1 into whatever scan raito is set up. Hence,
        // to get the core to scan at 4:1, need to put a scan ratio of 2:1 if run at pll speed.
        FAPI_DBG("Set scan ratio to 2:1 in non-bypass mode via OPCG_ALIGN[47-51]");
        FAPI_TRY(getScom(i_target, C_OPCG_ALIGN, l_data64));
        l_data64.insertFromRight<47, 5>(0x1);
        FAPI_TRY(putScom(i_target, C_OPCG_ALIGN, l_data64));
    }
    else
    {
        FAPI_DBG("Set scan ratio to 1:1 in bypass mode via OPCG_ALIGN[47-51]");
        FAPI_TRY(getScom(i_target, C_OPCG_ALIGN, l_data64));
        l_data64.insertFromRight<47, 5>(0x0);
        FAPI_TRY(putScom(i_target, C_OPCG_ALIGN, l_data64));
    }

#ifndef P9_HCD_STOP_SKIP_FLUSH
    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    // Putting in block to avoid c++ crosses initialization compile error
    {
        uint32_t l_loop;
        fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
            i_target.getParent<fapi2::TARGET_TYPE_PERV>();

        FAPI_DBG("Scan0 region:all_but_vital type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_ALL,
                                                  p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:all_but_vital type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_ALL,
                                                  p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }
#endif

    if (l_attr_vdm_enable == fapi2::ENUM_ATTR_VDM_ENABLE_ON)
    {
        FAPI_DBG("Assert vdm enable via CPPM_VDMCR[0]");
        FAPI_TRY(putScom(i_target, C_PPM_VDMCR_OR, MASK_SET(0)));
    }

    // content of p9_hcd_core_dcc_skewadjust below:

    FAPI_DBG("Drop core DCC bypass via NET_CTRL[1]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL1_WAND, MASK_UNSET(1)));

    FAPI_DBG("Drop core progdly bypass(skewadjust) via NET_CTRL1[2]");
    FAPI_TRY(putScom(i_target, C_NET_CTRL1_WAND, MASK_UNSET(2)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_core_chiplet_reset");
    return fapi2::current_err;
}

