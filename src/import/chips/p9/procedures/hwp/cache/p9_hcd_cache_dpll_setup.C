/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/cache/p9_hcd_cache_dpll_setup.C $ */
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
/// @file  p9_hcd_cache_dpll_setup.C
/// @brief Quad DPLL Setup
///
/// Procedure Summary:
///   Note:
///     Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///     DPLL tune bits are not dependent on frequency
///     Frequency is controlled by the Quad PPM
///       Actual frequency value for boot is stored into the Quad PPM by
///         p9_hcd_setup_evid.C in istep 2
///       In real cache STOP exit, the frequency value is persistent
///
///   Pre-Scan:
///
///   Scan:
///     (TODO) Set clock controller scan ratio to 1:1 as this is done at refclk
///     (TODO) scan0 (region = DPLL and ANEP, scan_type = GPTR)
///     (TODO) scan0 (region = DPLL and ANEP, scan_type = FUNC)
///     (TODO) Set clock controller scan ratio to 8:1 for future scans
///
///   Setup:
///     (TODO) set DPLL FREQ CTRL regitster
///     (TODO) set DPLL CTRL register
///     (Done) Drop DPLL test mode;
///     (Done) Drop DPLL into Reset;
///     (Done) Start DPLL clock via quad clock controller
///     (Done) Check for DPLL lock, Timeout: 200us
///     (Done) Remove DPLL bypass
///     (Done) Switch cache glitchless mux to use the DPLL
///     (Done) Drop ff_bypass to enable slewing
///
/// 1) reset, dpll_func_clksel, and all SL_HLD inputs are asserted
/// 2) If grid clock connected to dpll clkout,
///    bypass also has to be asserted to allow refclk on grid

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_dpll_setup.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

enum P9_HCD_CACHE_DPLL_SETUP_CONSTANTS
{
    CACHE_DPLL_LOCK_TIMEOUT_IN_MS      = 1,
    CACHE_DPLL_CLK_START_TIMEOUT_IN_MS = 1,
    CACHE_ANEP_CLK_START_TIMEOUT_IN_MS = 1
};

//-----------------------------------------------------------------------------
// Procedure: Quad DPLL Setup
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_dpll_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_dpll_setup");
    fapi2::buffer<uint64_t>                  l_data64;
    uint32_t                                 l_timeout;

    //----------------------------
    // Prepare to start DPLL clock
    //----------------------------

    FAPI_DBG("Assert DPLL in mode 1,set slew rate via QPPM_DPLL_CTRL[2,6-15]");
    l_data64.flush<0>().setBit<2>().insertFromRight<6, 10>(0x40);
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, l_data64));

    FAPI_DBG("Drop flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(2)));

    FAPI_DBG("Drop DPLL test mode and reset via NET_CTRL0[3,4]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_AND(3, 2, 0)));

    FAPI_DBG("Drop DPLL clock region fence via NET_CTRL1[14]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(14)));

    // ----------------
    // Start DPLL clock
    // ----------------

    FAPI_DBG("Clear all bits prior start DPLL clock via SCAN_REGION_TYPE");
    FAPI_TRY(putScom(i_target, EQ_SCAN_REGION_TYPE, MASK_ZERO));

    FAPI_DBG("Start DPLL clock via CLK_REGION");
    l_data64 = (p9hcd::CLK_START_CMD   |
                p9hcd::CLK_REGION_DPLL |
                p9hcd::CLK_THOLD_ALL);
    FAPI_TRY(putScom(i_target, EQ_CLK_REGION, l_data64));

    FAPI_DBG("Poll for DPLL clock running via CPLT_STAT0[8]");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_DPLL_CLK_START_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
    }
    while((l_data64.getBit<8>() != 1) && ((--l_timeout) != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_DPLLCLKSTART_TIMEOUT().set_EQCPLTSTAT(l_data64),
                "DPLL Clock Start Timeout");

    FAPI_DBG("Check DPLL clock running via CLOCK_STAT_SL[14]");
    FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));

    FAPI_ASSERT((l_data64.getBit<14>() == 0),
                fapi2::PMPROC_DPLLCLKSTART_FAILED().set_EQCLKSTAT(l_data64),
                "DPLL Clock Start Failed");
    FAPI_DBG("DPLL clock running now");

    // This is necessary to ensure that the DPLL is in Mode 1(ff_bypass = 1)
    // If not, the lock times will go from ~30us to 3-5ms
    FAPI_DBG("Poll for DPLL to lock via QPPM_DPLL_STAT");
    l_timeout = (p9hcd::CYCLES_PER_MS / p9hcd::INSTS_PER_POLL_LOOP) *
                CACHE_DPLL_LOCK_TIMEOUT_IN_MS;

    do
    {
        FAPI_TRY(getScom(i_target, EQ_QPPM_DPLL_STAT, l_data64));
        ///@todo disable poll for DPLL lock until model setting in place
        break;
    }
    while ((l_data64.getBit<63>() != 1 ) && (--l_timeout != 0));

    FAPI_ASSERT((l_timeout != 0),
                fapi2::PMPROC_DPLL_LOCK_TIMEOUT()
                .set_EQQPPMDPLLSTAT(l_data64),
                "DPLL Lock Timeout");
    FAPI_DBG("DPLL is locked now");

    //--------------------------
    // Cleaning up
    //--------------------------

    FAPI_DBG("Drop DPLL bypass via NET_CTRL0[5]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(5)));

    FAPI_DBG("Drop DPLL ff_bypass via QPPM_DPLL_CTRL[2]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_CLEAR, MASK_SET(2)));

    FAPI_DBG("Assert flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(2)));

    FAPI_DBG("Set scan ratio to 4:1 in non-bypass mode via OPCG_ALIGN[47-51]");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<47, 5>(0x3);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

    FAPI_DBG("Drop ANEP clock region fence via CPLT_CTRL1[10]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_SET(10)));

    FAPI_DBG("Drop skew/duty cycle adjust func_clksel via NET_CTRL0[22]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(22)));

    FAPI_DBG("Drop skew adjust reset via NET_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(2)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_dpll_setup");
    return fapi2::current_err;
}



