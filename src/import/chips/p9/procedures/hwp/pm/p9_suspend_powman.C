/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/pm/p9_suspend_powman.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
//--------------------------------------------------------------------------
//
//
/// @file p9_suspend_powman.C
/// @brief Suspend the OCC/PM
///
// *HWP HWP Owner Adam Hale Adam.Samuel.Hale@ibm.com
// *HWP FW Owner: Raja Das rajadas2@in.ibm.com
// *HWP Team: PM
// *HWP Level: 3
// *HWP Consumed by: SBE
//
//--------------------------------------------------------------------------


//PGPE PM Suspend Flow
//  enters safe mode and notifies sgpe
//  sgpe ignores future stop requests, notifies cme
//  cme ignores future requests
//  cme acks -> sgpe acks -> pgpe sets suspended bit in OCCS2

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_suspend_powman.H>
#include <p9_misc_scom_addresses.H>
#include <p9n2_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>
#include <p9_query_cache_access_state.H>
#include <p9_quad_scom_addresses.H>
#include <p9_quad_scom_addresses_fld.H>
#include <p9_pm_hcd_flags.h>
#include <p9_collect_suspend_ffdc.H>

// 1000000 nanosecond = 1 millisecond
// total timeout = 10 milliseconds
static const uint64_t POLLTIME_NS = 1000000;
static const uint64_t POLLTIME_MCYCLES = 4000;
static const uint32_t TRIES_BEFORE_TIMEOUT = 500;

extern "C" {

//--------------------------------------------------------------------------
// Constant definitions
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_suspend_powman(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p9_suspend_powman...\n")
        fapi2::buffer<uint64_t> l_occflg_data(0);
        fapi2::buffer<uint64_t> l_occs2_data(0);
        fapi2::buffer<uint64_t> l_ocr_reg_data(0);
        fapi2::buffer<uint64_t> l_xsr(0);
        fapi2::buffer<uint64_t> l_data64 = 0;
        auto l_core_vector = i_target.getChildren<fapi2::TARGET_TYPE_CORE>();
        static const uint64_t  PPE_XIXCR_XCR_HALT      = 0x1000000000000000;

        bool l_pgpe_in_safe_mode = false;
        bool l_pgpe_suspended = false;

        // by default disable malf alert handling in PM suspend to avoid
        // spurious alerts
        l_data64.flush<0>().setBit<p9hcd::STOP_RECOVERY_TRIGGER_ENABLE>();
        FAPI_TRY(fapi2::putScom (i_target, P9N2_PU_OCB_OCI_OCCFLG2_SCOM1,
                                 l_data64));

        FAPI_TRY(fapi2::getScom(i_target, PU_GPE2_GPEXIXSR_SCOM, l_xsr),
                 "Error checking PGPE XSR");

        if(!(l_xsr >> 63))
        {

            // SBE waits for PGPE to set OCC Scratch2[PGPE_SAFE_MODE_ACTIVE]
            for(uint32_t method = 0; method < 2; method++)
            {
                if(method == 0)
                {
                    // SBE requests OCC enter safe state by setting OCC_Flag[REQUEST_OCC_SAFE_STATE]
                    // OCC polls this bit every 500us,
                    // if detected heartbeat stop PGPE is interrupted and enters suspend
                    l_occflg_data.setBit<p9hcd::REQUEST_OCC_SAFE_STATE>();
                    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data),
                             "Error setting OCC Flag register bit REQUEST_OCC_SAFE_STATE");
                }
                else
                {
                    FAPI_DBG("Safe Mode bit failed after requesting occ safe state\n Requesting in PGPE...\n");
                    l_occflg_data.flush<0>().setBit<p9hcd::PGPE_SAFE_MODE>();
                    //PGPE polls this bit on a reduced FIT timer period
                    //if detected enters safe mode
                    FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data), "Error setting OCC Flag register bit 2");
                }

                for (uint32_t i = 0; i < TRIES_BEFORE_TIMEOUT; i++)
                {
                    FAPI_TRY(fapi2::getScom(i_target, PU_OCB_OCI_OCCS2_SCOM, l_occs2_data), "Error reading OCC Scratch 2 register");

                    if(l_occs2_data.getBit<p9hcd::PGPE_SAFE_MODE_ACTIVE>())
                    {
                        l_pgpe_in_safe_mode = true;
                        break;
                    }

                    fapi2::delay(POLLTIME_NS , POLLTIME_MCYCLES * 1000 * 1000);
                }

                if(l_pgpe_in_safe_mode)
                {
                    break;
                }
            }

            FAPI_INF("Halting OCC PPC405, GPE0 and GPE1");
            //OCC PPC405
            l_ocr_reg_data.setBit<PU_OCB_PIB_OCR_OCC_DBG_HALT>();
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCR_OR, l_ocr_reg_data), "Error writing to OCR register");

            //GPE0 and GPE1
            FAPI_TRY(putScom(i_target, PU_GPE0_GPENXIXCR_SCOM, PPE_XIXCR_XCR_HALT), "Error writing to OCC GPE0 XCR register");
            FAPI_TRY(putScom(i_target, PU_GPE1_GPENXIXCR_SCOM, PPE_XIXCR_XCR_HALT), "Error writing to OCC GPE1 XCR register");

            //PGPE polls this bit on a reduced FIT timer period
            //if detected executes PGPE pm_suspend flow
            l_occflg_data.flush<0>().setBit<p9hcd::PM_COMPLEX_SUSPEND>();
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data), "Error setting OCC Flag register bit 3");

            //OCC being halted should cause the PGPE to observe a OCC Heartbeat interrupt,
            //causing it to enter PGPE pm_suspend flow
            for (uint32_t i = 0; i < TRIES_BEFORE_TIMEOUT; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_OCB_OCI_OCCS2_SCOM, l_occs2_data), "Error reading OCC Scratch 2 register");

                if(l_occs2_data.getBit<p9hcd::PM_COMPLEX_SUSPENDED>())
                {
                    l_pgpe_suspended = true;
                    break;
                }

                fapi2::delay(POLLTIME_NS, POLLTIME_MCYCLES * 1000 * 1000);
            }

            //if timeout, hwp fails
            if(!l_pgpe_suspended)
            {
                FAPI_ERR("PGPE did not signal that PM Complex Suspend Finished");
                FAPI_TRY ( p9_collect_suspend_ffdc (
                               i_target));
            }

            FAPI_DBG("Suspend Power Management Successful!\n");
        }
        else
        {

            FAPI_INF("Halting OCC PPC405, GPE0 and GPE1");
            //OCC PPC405
            l_ocr_reg_data.setBit<PU_OCB_PIB_OCR_OCC_DBG_HALT>();
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCR_OR, l_ocr_reg_data), "Error writing to OCR register");

            //GPE0 and GPE1
            FAPI_TRY(putScom(i_target, PU_GPE0_GPENXIXCR_SCOM, PPE_XIXCR_XCR_HALT), "Error writing to OCC GPE0 XCR register");
            FAPI_TRY(putScom(i_target, PU_GPE1_GPENXIXCR_SCOM, PPE_XIXCR_XCR_HALT), "Error writing to OCC GPE1 XCR register");

            FAPI_TRY(fapi2::getScom(i_target, PU_GPE3_GPEXIXSR_SCOM, l_xsr),
                     "Error reading SGPE XSR");

            //Make sure that the SGPE is not halted before requesting halt
            if(!(l_xsr >> 63))
            {
                FAPI_INF("WARNING! PGPE Already Halted, halting SGPE via XIXSR");
                FAPI_TRY(fapi2::putScom(i_target, PU_GPE3_GPEXIXCR_SCOM, PPE_XIXCR_XCR_HALT));
            }
            else
            {
                FAPI_INF("WARNING! SGPE and PGPE Already Halted, skipping procedure");
            }
        }

        // Ensure that the PPM write disable bit on the Core Power Management Mode Register
        // is cleared for all cores
        for (auto& core : l_core_vector)
        {
            FAPI_INF("Clearing WRITE_DISABLE bit in core %d", core.getChipletNumber());
            l_data64.flush<0>().setBit<C_CPPM_CPMMR_PPM_WRITE_DISABLE>();
            FAPI_TRY(fapi2::putScom(core, C_CPPM_CPMMR_CLEAR , l_data64));
        }

    fapi_try_exit:
        FAPI_DBG("Exiting p9_suspend_powman...");
        return fapi2::current_err;

    }
} // extern "C"

