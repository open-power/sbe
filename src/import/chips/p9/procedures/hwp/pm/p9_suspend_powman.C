/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/pm/p9_suspend_powman.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
// *HWP HWP Owner Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: PM
// *HWP Level: 2
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
#include <p9_misc_scom_addresses_fld.H>
#include <p9_query_cache_access_state.H>
#include <p9_quad_scom_addresses.H>
#include <p9_quad_scom_addresses_fld.H>
#include <p9_pm_hcd_flags.h>

static const uint64_t GPE2_BASE_ADDRESS = 0x00064010;
static const uint64_t GPE3_BASE_ADDRESS = 0x00066010;
static const uint64_t CME_BASE_ADDRESS  = 0x10012010;

static const uint64_t POLLTIME_US = 1;
static const uint64_t POLLTIME_MCYCLES = 4000;

extern "C" {

//--------------------------------------------------------------------------
// Constant definitions
//--------------------------------------------------------------------------

    const uint32_t c_tries_before_timeout = 10;

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_suspend_powman(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p9_suspend_powman...\n")

        fapi2::buffer<uint64_t> l_occflg_data(0);
        fapi2::buffer<uint64_t> l_occs2_data(0);
        fapi2::buffer<uint64_t> l_ocr_reg_data(0);
        fapi2::buffer<uint64_t> l_pgpe_xsr(0);
        auto l_quad_vector = i_target.getChildren<fapi2::TARGET_TYPE_EQ>();
        auto l_core_vector = i_target.getChildren<fapi2::TARGET_TYPE_CORE>();

        bool l_pgpe_in_safe_mode = false;
        bool l_pgpe_suspended = false;

        FAPI_TRY(fapi2::getScom(i_target, PU_GPE2_GPEXIXSR_SCOM, l_pgpe_xsr),
                 "Error checking PGPE XSR");

        if(!(l_pgpe_xsr >> 63))
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

                for (uint32_t i = 0; i < c_tries_before_timeout; i++)
                {
                    FAPI_TRY(fapi2::getScom(i_target, PU_OCB_OCI_OCCS2_SCOM, l_occs2_data), "Error reading OCC Scratch 2 register");

                    if(l_occs2_data.getBit<p9hcd::PGPE_SAFE_MODE_ACTIVE>())
                    {
                        l_pgpe_in_safe_mode = true;
                        break;
                    }

                    fapi2::delay(POLLTIME_US * 1000, POLLTIME_MCYCLES * 1000 * 1000);
                }

                if(l_pgpe_in_safe_mode)
                {
                    break;
                }
            }

            // if timeout, hwp fails
            /*FAPI_ASSERT(l_pgpe_in_safe_mode,
            fapi2::P9_PGPE_SAFEMODE_TIMEOUT().set_PROC_CHIP_TARGET(i_target).set_PPE_BASE_ADDRESSES(ppe_addresses),
            "PGPE did not signal that it entered safe mode");
            */
            //FAPI_DBG("Entered Safe Mode Successfully!\n");


            //SBE issues "halt OCC complex" to stop OCC instructions
            l_ocr_reg_data.setBit<PU_OCB_PIB_OCR_OCC_DBG_HALT>();
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCR_OR, l_ocr_reg_data), "Error writing to OCR register");

            //PGPE polls this bit on a reduced FIT timer period
            //if detected executes PGPE pm_suspend flow
            l_occflg_data.flush<0>().setBit<p9hcd::PM_COMPLEX_SUSPEND>();
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data), "Error setting OCC Flag register bit 3");

            //This should cause the PGPE to observe a OCC Heartbeat interrupt,
            //causing it to enter PGPE pm_suspend flow
            for (uint32_t i = 0; i < c_tries_before_timeout; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_OCB_OCI_OCCS2_SCOM, l_occs2_data), "Error reading OCC Scratch 2 register");

                if(l_occs2_data.getBit<p9hcd::PM_COMPLEX_SUSPENDED>())
                {
                    l_pgpe_suspended = true;
                    break;
                }

                fapi2::delay(POLLTIME_US * 1000, POLLTIME_MCYCLES * 1000 * 1000);
            }

            //if timeout, hwp fails
            FAPI_ASSERT(l_pgpe_suspended,
                        fapi2::P9_PGPE_SUSPEND_TIMEOUT().set_PROC_CHIP_TARGET(i_target),
                        "PGPE did not signal that PM Complex Suspend Finished");
            FAPI_DBG("Suspend Power Management Successful!\n");
        }
        else
        {
            FAPI_INF("WARNING! PGPE Already Halted, skipping procedure");
        }

    fapi_try_exit:
        FAPI_DBG("Exiting p9_suspend_powman...");
        return fapi2::current_err;

    }
} // extern "C"

