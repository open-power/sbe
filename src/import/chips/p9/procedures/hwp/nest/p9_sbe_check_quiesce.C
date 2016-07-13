/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_check_quiesce.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
/// @file p9_sbe_check_quiesce.C
/// @brief Check quiesce state for all units on the powerbus
///
// *HWP HWP Owner Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_sbe_check_quiesce.H>

extern "C" {

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_sbe_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        // a) SBE will check quiesce state for all units on the powerbus on its chip
        //      If the queisce fails then this HWP will checkstop the system
        FAPI_TRY(p9_occ_pm_check_quiesce(i_target), "Error from p9_occ_pm_check_quiesce");
        FAPI_TRY(p9_ec_eq_check_quiesce(i_target), "Error from p9_ec_eq_check_quiesce");
        FAPI_TRY(p9_afu_check_quiesce(i_target), "Error from p9_afu_check_quiesce");
        FAPI_TRY(p9_psl_check_quiesce(i_target), "Error from p9_psl_check_quiesce");
        FAPI_TRY(p9_capp_check_quiesce(i_target), "Error from p9_capp_check_quiesce");
        FAPI_TRY(p9_phb_check_quiesce(i_target), "Error from p9_phb_check_quiesce");
        FAPI_TRY(p9_npu_check_quiesce(i_target), "Error from p9_npu_check_quiesce");
        FAPI_TRY(p9_vas_check_quiesce(i_target), "Error from p9_vas_check_quiesce");
        FAPI_TRY(p9_nx_check_quiesce(i_target), "Error from p9_nx_check_quiesce");
        FAPI_TRY(p9_hca_check_quiesce(i_target), "Error from p9_hca_check_quiesce");
        FAPI_TRY(p9_psihb_check_quiesce(i_target), "Error from p9_psihb_check_quiesce");
        FAPI_TRY(p9_intp_check_quiesce(i_target), "Error from p9_intp_check_quiesce");
        FAPI_TRY(p9_set_lqa(i_target), "Error from p9_set_lqa");

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

//---------------------------------------------------------------------------
//  Helper Functions
//---------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_occ_pm_check_quiesce(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        //  OCC/PM (halted first to prevent it from restarting Host Cores)
        //   Not  worried about CME (EC/EQ flush)
        //   Need to stop OCC/SGPE/PGPE engines
        //   Use existing flag register to request OCC/SGPE/PGPE to quiesce
        //   SBE would wait for quiesce
        //   SBE would then issue the "halt OCC complex"
        //   The "halt OCC complex" is that when it is removed the OCC complex won't start executing instructions.  The prevent execution overrides any incoming start requests until released
        //   Disable the PBA BARs

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_ec_eq_check_quiesce(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // check for maint mode or STOP state in core RAS Status register (try 5 times), if not reached fail
        // All ECs should already be there per initial instruction stop
        // After reaching maint mode, inject local core xstop to prevent EC from being restarted
        // p9_inject_local_xstop.C
        // inject a local core checkstop on each core to prevent them from restarting

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_afu_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Need mechanism to quiesce -- how -- PHB freeze?

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_psl_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Need mechanism to quiesce  -- how -- PHB freeze?

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_capp_check_quiesce(const
                                            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Need mechanism to quiesce -- how -- PHB freeze?

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_phb_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // force freeze, check quiesce status

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_npu_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Fence NPU (need scoms to do) -- a bigger hammer than freeze
        // Does not return responses or credits to GPU -- going to hang the GPU
        // If NPU sees commands on power bus it will respond with SUE on reads, drop writes
        // From Interrupt -- will send all pending interrupts, prevents/drops any new ones
        // Need to check for interrupts to quiesce (get scoms/bits from John)
        // NVLink is not working and Trusted code will need to reset/retrain on the reipl

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_vas_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // VAS needs to be quiesced before NX
        // Has a quiesce documented for DTRM already
        // Bit to stop fetching new work, finish up all current work
        // Bit defined that indicates work is finished (quiesce request bit, and quiesce done)

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_nx_check_quiesce(const
                                          fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Has a quiesce documented for DTRM already
        // Bit to stop fetching new work, finish up all current work
        // Bit defined that indicates work is finished (quiesce request bit, and quiesce done)

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_hca_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Need to flush

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_psihb_check_quiesce(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Need mechanism to quiesce PSI DMAs
        // There are bits on the PSIHB to force the DMAs to be rejected

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_intp_check_quiesce(const
                                            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Step 1 -- When Cores quiesce -- by definition all outstanding loads are complete
        // All Units capable of presenting interrupts need to send response to all outstanding loads (isn't any code neccessary, PB queises naturally after EXs stop driving traffic)
        // Step 2 -- INTP must be last unit to be reset, SBE must reset all other units, then INTP
        // Step 3  -- On interrupt presenter unit need a new bit to drain Common Queue and enter "freeze" state
        //            Corresponding two bits to check when done (input queue and output queue)
        //            SBE can set drain/freeze bit it's chip's INTP, and then poll for that INTP to quiesce.
        //            No worries about the ordering between chip interrupt units
        //            Note the actual reset is done later by Hostboot

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_set_lqa(const
                                 fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Upon achieving quiesced state, SBE will set LQA

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }


} // extern "C"

