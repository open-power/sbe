/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_dpll_setup.C $ */
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
/// @file  p9_hcd_cache_dpll_setup.C
/// @brief Quad DPLL Setup
///
/// Procedure Summary:
///   Note:
///     Initfiles in procedure defined on VBU ENGD wiki
///     DPLL tune bits are not dependent on frequency
///     Frequency is controlled by the Quad PPM
///       Actual frequency value for boot is stored into the Quad PPM by
///         p9_hcd_setup_evid.C in istep 2
///       In real cache STOP exit, the frequency value is persistent
///
///   Setup:
///     @todo  RTC158181 set DPLL FREQ CTRL regitster
///     (DONE) set DPLL CTRL register
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

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

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
    DPLL_LOCK_POLL_TIMEOUT_HW_NS    = 1000000, // 10^6ns = 1ms timeout
    DPLL_LOCK_POLL_DELAY_HW_NS      = 10000,   // 10us poll loop delay
    DPLL_LOCK_POLL_DELAY_SIM_CYCLE  = 320000,  // 320k sim cycle delay

    DPLL_START_POLL_TIMEOUT_HW_NS   = 1000000, // 10^6ns = 1ms timeout
    DPLL_START_POLL_DELAY_HW_NS     = 10000,   // 10us poll loop delay
    DPLL_START_POLL_DELAY_SIM_CYCLE = 320000   // 320k sim cycle delay
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
    uint8_t                                  l_dpll_bypass;
    uint32_t                                 l_poll_loops;
    auto l_parent_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

#ifndef __PPE__
    uint8_t                                  l_attr_is_simulation;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_SIMULATION,
                           fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                           l_attr_is_simulation));
#endif

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_DPLL_BYPASS, l_parent_chip, l_dpll_bypass),
             "Error from FAPI_ATTR_GET (ATTR_DPLL_BYPASS)");

    //----------------------------
    // Prepare to start DPLL clock
    //----------------------------

    FAPI_DBG("Drop analog logic fence via QPPM_PFCS[11]");
    FAPI_TRY(putScom(i_target, EQ_PPM_PFCS_WCLEAR, MASK_SET(11)));

    FAPI_DBG("Assert DPLL in mode 1,set slew rate via QPPM_DPLL_CTRL[1,2,16,6-15,22-23]");
    l_data64.flush<0>().setBit<2>().insertFromRight<6, 10>(0x01);
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, l_data64));

    // make sure mode1 by clearing
    // 1) enable_jump_protect
    // 16)ss_enable
    // 22)enable_fmin_target
    // 23)enable_fmax_target
    l_data64.flush<0>().setBit<1>().setBit<16>().setBit<22>().setBit<23>();
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_CLEAR, l_data64));

    FAPI_DBG("Drop flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_CLEAR, MASK_SET(2)));

    if (l_dpll_bypass == 0)
    {
        FAPI_DBG("Drop DPLL test mode and reset via NET_CTRL0[3,4]");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_AND(3, 2, 0)));
    }

    FAPI_DBG("Drop ANEP+DPLL clock region fences via NET_CTRL1[10, 14]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL1_CLEAR, MASK_CLR(10, 5, 0x11)));

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
    l_poll_loops = DPLL_START_POLL_TIMEOUT_HW_NS /
                   DPLL_START_POLL_DELAY_HW_NS;

    do
    {
        fapi2::delay(DPLL_START_POLL_DELAY_HW_NS,
                     DPLL_START_POLL_DELAY_SIM_CYCLE);

        FAPI_TRY(getScom(i_target, EQ_CPLT_STAT0, l_data64));
    }
    while((l_data64.getBit<8>() != 1) && ((--l_poll_loops) != 0));

    FAPI_ASSERT((l_poll_loops != 0),
                fapi2::CACHE_DPLL_CLK_START_TIMEOUT()
                .set_EQ_CPLT_STAT(l_data64)
                .set_DPLL_START_POLL_DELAY_HW_NS(DPLL_START_POLL_DELAY_HW_NS)
                .set_DPLL_START_POLL_TIMEOUT_HW_NS(DPLL_START_POLL_TIMEOUT_HW_NS)
                .set_CACHE_TARGET(i_target),
                "DPLL Clock Start Timeout");

    FAPI_DBG("Check DPLL clock running via CLOCK_STAT_SL[14]");
    FAPI_TRY(getScom(i_target, EQ_CLOCK_STAT_SL, l_data64));

    FAPI_ASSERT((l_data64.getBit<14>() == 0),
                fapi2::CACHE_DPLL_CLK_START_FAILED()
                .set_EQ_CLK_STAT(l_data64)
                .set_CACHE_TARGET(i_target),
                "DPLL Clock Start Failed");
    FAPI_DBG("DPLL clock running now");

    // This is necessary to ensure that the DPLL is in Mode 1(ff_bypass = 1)
    // If not, the lock times will go from ~30us to 3-5ms
    if (l_dpll_bypass == 0)
    {
        FAPI_DBG("Poll for DPLL to lock via QPPM_DPLL_STAT");
        l_poll_loops = DPLL_LOCK_POLL_TIMEOUT_HW_NS /
                       DPLL_LOCK_POLL_DELAY_HW_NS;

        do
        {
            fapi2::delay(DPLL_LOCK_POLL_DELAY_HW_NS,
                         DPLL_LOCK_POLL_DELAY_SIM_CYCLE);

            FAPI_TRY(getScom(i_target, EQ_QPPM_DPLL_STAT, l_data64));

#ifndef __PPE__

            if (l_attr_is_simulation)
            {
                FAPI_DBG("Skipping DPLL lock check in Sim");
                break;
            }

#else
#ifdef SIM_DPLL_LOCK_CHK
            FAPI_DBG("Skipping DPLL lock check in Sim");
            break;
#endif
#endif

        }
        while ((l_data64.getBit<63>() != 1 ) && (--l_poll_loops != 0));

        FAPI_ASSERT((l_poll_loops != 0),
                    fapi2::CACHE_DPLL_LOCK_TIMEOUT()
                    .set_EQ_QPPM_DPLL_STAT(l_data64)
                    .set_DPLL_LOCK_POLL_TIMEOUT_HW_NS(DPLL_LOCK_POLL_TIMEOUT_HW_NS)
                    .set_DPLL_LOCK_POLL_DELAY_HW_NS(DPLL_LOCK_POLL_DELAY_HW_NS)
                    .set_CACHE_TARGET(i_target),
                    "DPLL Lock Timeout");
        FAPI_DBG("DPLL is locked now");

        FAPI_DBG("Drop DPLL bypass via NET_CTRL0[5]");
        FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(5)));

        FAPI_DBG("Drop DPLL ff_bypass via QPPM_DPLL_CTRL[2]");
        FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_CLEAR, MASK_SET(2)));
    }

    //--------------------------
    // Cleaning up
    //--------------------------

    FAPI_DBG("Assert flushmode_inhibit via CPLT_CTRL0[2]");
    FAPI_TRY(putScom(i_target, EQ_CPLT_CTRL0_OR, MASK_SET(2)));

    if (l_dpll_bypass == 0)
    {
        FAPI_DBG("Set scan ratio to 4:1 in non-bypass mode via OPCG_ALIGN[47-51]");
        FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
        l_data64.insertFromRight<47, 5>(0x3);
        FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

        FAPI_DBG("Clear PLL unlock errors in PCB slave, unmask FIR propagation");
        FAPI_TRY(putScom(i_target, EQ_ERROR_REG, 0xFFFFFFFFFFFFFFFF));
    }

    FAPI_DBG("Drop skew/duty cycle adjust func_clksel via NET_CTRL0[22]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(22)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_dpll_setup");
    return fapi2::current_err;
}



