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

extern "C" {

//--------------------------------------------------------------------------
// Constant definitions
//--------------------------------------------------------------------------

    const uint32_t c_tries_before_timeout = 20;

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_suspend_powman(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        fapi2::buffer<uint64_t> l_pba_data(0);
        fapi2::buffer<uint64_t> l_occflg_data(0);
        fapi2::buffer<uint64_t> l_occs2_data(0);
        fapi2::buffer<uint64_t> l_ocr_reg_data(0);
        fapi2::buffer<uint64_t> l_ppe_xixcr_data(0);
        uint64_t l_xixcr_force_halt_cmd = 0b111;
        bool l_pgpe_in_safe_mode = false;

        // SBE messages request to OCC to enter safe state via OCC_Flag[REQUEST_OCC_SAFE_STATE] , which OCC polls every 500us. This does not cause any errors to be logged.
        // In response PGPE will detect missing OCC heartbeat after an additional 8ms and go into "safe mode". This is cleanly handled on OCC across all chips.
        l_occflg_data.setBit<p9hcd::REQUEST_OCC_SAFE_STATE>();
        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data),
                 "Error setting OCC Flag register bit REQUEST_OCC_SAFE_STATE");

        // SBE waits for PGPE to set OCC Scratch2[PGPE_SAFE_MODE_ACTIVE]. SBE Timeout after 33ms, then sets OCC Flag[PGPE_SAFE_MODE] to request PGPE into "safe mode" manually. SBE polls on OCC Scratch2[PGPE_SAFE_MODE_ACTIVE] again, timeout after 8ms
        //TODO put in the delays here for 33ms and 8ms
        for (uint32_t i = 0; i < c_tries_before_timeout; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_OCB_OCI_OCCS2_SCOM, l_occs2_data), "Error reading OCC Scratch 2 register");

            if(l_occs2_data.getBit<p9hcd::PGPE_SAFE_MODE_ACTIVE>())
            {
                l_pgpe_in_safe_mode = true;
                break;
            }
        }

        if (!l_pgpe_in_safe_mode)
        {

            l_occflg_data.flush<0>().setBit<p9hcd::PGPE_SAFE_MODE>();
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data), "Error setting OCC Flag register bit 2");

            for (uint32_t i = 0; i < c_tries_before_timeout; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_OCB_OCI_OCCS2_SCOM, l_occs2_data), "Error reading OCC Scratch 2 register");

                if(l_occs2_data.getBit<p9hcd::PGPE_SAFE_MODE_ACTIVE>())
                {
                    l_pgpe_in_safe_mode = true;
                    break;
                }
            }
        }

        //TODO @todo RTC 164109 for when this is implemented in PGPE Hcode
        /*FAPI_ASSERT(l_pgpe_in_safe_mode, fapi2::P9_PGPE_SAFEMODE_TIMEOUT().set_TARGET(i_target).set_OCCFLGREG(l_occflg_data),
                    "PGPE did not set OCC Flag bit 6");
        */
        // In response, PGPE safe mode tell CME to go into safe mode.

        // Suspend PGPE, SGPE, and CME from processing Pstate, WOF, and STOP entry/exit requests.
        // (see above) Use OCC flag register bit 3 to request PM Complex suspend.:  In response, PGPE will stop processing Pstates, message SGPE to stop calling 24x7 thread andsuspend responding to Stop entry & exit. PGPE will message CME to stop processing Stop states??????.; then wait for ACK from each engine
        l_occflg_data.flush<0>().setBit<p9hcd::PM_COMPLEX_SUSPEND>();
        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_OCI_OCCFLG_SCOM2, l_occflg_data), "Error setting OCC Flag register bit 3");
        // TODO @todo RTC 164110 How to wait for ACK from each engine - an update is needed once the PGPE implements the function

        // SBE would then issue the "halt OCC complex" (2 SCOMs) to stop OCC instructions (The "halt OCC complex" is that when it is removed the OCC complex won't start executing instructions. The prevent execution overrides any incoming start requests until released)
        l_ocr_reg_data.setBit<PU_OCB_PIB_OCR_OCC_DBG_HALT>();
        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCR_OR, l_ocr_reg_data), "Error writing to OCR register");

        // SBE then halts GPE0-3.
        l_ppe_xixcr_data.insertFromRight<PU_GPE0_PPE_XIXCR_XCR, PU_GPE0_PPE_XIXCR_XCR_LEN>(l_xixcr_force_halt_cmd);
        FAPI_TRY(fapi2::putScom(i_target, PU_GPE0_PPE_XIXCR, l_ppe_xixcr_data), "Error writing to GPE0 XIXCR register");
        FAPI_TRY(fapi2::putScom(i_target, PU_GPE1_PPE_XIXCR, l_ppe_xixcr_data), "Error writing to GPE1 XIXCR register");
        FAPI_TRY(fapi2::putScom(i_target, PU_GPE2_PPE_XIXCR, l_ppe_xixcr_data), "Error writing to GPE2 XIXCR register");
        FAPI_TRY(fapi2::putScom(i_target, PU_GPE3_PPE_XIXCR, l_ppe_xixcr_data), "Error writing to GPE3 XIXCR register");

        // SBE calls query_cache_access_state.C. For each Quad, if is_scomable then issue Halt to both CMEs in that Quad (ignoring core/EX partial good)
        {
            auto l_quad_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL);

            for (auto quad_functional_it : l_quad_functional_vector)
            {
                bool l_l2_is_scomable = false;
                bool l_l2_is_scanable = false;
                bool l_l3_is_scomable = false;
                bool l_l3_is_scanable = false;

                FAPI_TRY(p9_query_cache_access_state(quad_functional_it, l_l2_is_scomable, l_l2_is_scanable, l_l3_is_scomable,
                                                     l_l3_is_scanable), "Error getting cache access state");

                if (l_l2_is_scomable && l_l3_is_scomable)
                {
                    FAPI_TRY(fapi2::putScom(quad_functional_it, EQ_PPE_XIXCR, l_ppe_xixcr_data), "Error writing to the Quad's CME");
                }
            }
        }

        // Clear PBA BARs 0-2 and disable PBA slaves 0-2 (DRTM paranoia to restore trust, but not BAR3 or Slave3 because SBE still needs memory access).
        FAPI_TRY(fapi2::putScom(i_target, PU_PBABAR0, l_pba_data), "Error clearing PBABAR0");
        FAPI_TRY(fapi2::putScom(i_target, PU_PBABAR1, l_pba_data), "Error clearing PBABAR1");
        FAPI_TRY(fapi2::putScom(i_target, PU_PBABAR2, l_pba_data), "Error clearing PBABAR2");
        FAPI_TRY(fapi2::putScom(i_target, PU_PBASLVCTL0_PIB, l_pba_data), "Error clearing PBASLVCTL0");
        FAPI_TRY(fapi2::putScom(i_target, PU_PBASLVCTL1_PIB, l_pba_data), "Error clearing PBASLVCTL1");
        FAPI_TRY(fapi2::putScom(i_target, PU_PBASLVCTL2_PIB, l_pba_data), "Error clearing PBASLVCTL2");

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

} // extern "C"

