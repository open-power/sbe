/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdcntrldmt.C $                        */
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
/*
 * @file: sbe/sbefw/sbecmdcntrldmt.C
 *
 * @brief This file contains the Core State Control Messages
 *
 */

#include "sbecmdcntrldmt.H"
#include "sbetrace.H"
#include "sbe_build_info.H"
#include "sberegaccess.H"
#include "plat_hw_access.H"
#include "sbeTimerSvc.H"
#include "sbeglobals.H"
#include "sbeFFDC.H"
#include "p10_sbe_check_master_stop15.H"
#include "p10_sbe_apply_fbc_rt_settings.H"
#include "p10_sbe_powerdown_backing_caches.H"
#include "p10_block_wakeup_intr.H"
#include "sbecmdexitcachecontained.H"
using namespace fapi2;

static const uint32_t CHECK_MASTER_STOP5_POLL_INTERVAL_NS = 1000000000;    // 1000 ms/1s
static const uint32_t CHECK_MASTER_STOP5_POLL_INTERVAL_SIMICS = 100000000; // 100 ms

///////////////////////////////////////////////////////////////////////////////
// Procedure function pointers
p10_sbe_check_master_stop15_FP_t p10_sbe_check_master_stop15_hwp =
                                &p10_sbe_check_master_stop15;

p10_sbe_apply_fbc_rt_settings_FP_t p10_sbe_apply_fbc_rt_settings_hwp =
                                   &p10_sbe_apply_fbc_rt_settings;

p10_sbe_powerdown_backing_caches_FP_t p10_sbe_powerdown_backing_caches_hwp =
                                      &p10_sbe_powerdown_backing_caches;

p10_block_wakeup_intr_FP_t p10_block_wakeup_intr_hwp =
                          &p10_block_wakeup_intr;

///////////////////////////////////////////////////////////////////////////////
//Static initialization of the Dmt Pk timer
static timerService g_sbe_pk_dmt_timer;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sbeDmtPkExpiryCallback(void *)
{
    #define SBE_FUNC "sbeDmtPkExpiryCallback"
    SBE_INFO (SBE_FUNC "DMT Callback Timer has expired.."
                       "No-Checkstop on the system for now"
                       "and FFDC will not be collected.");
    SBE_INFO (SBE_FUNC "HALTING SBE! - NO-OP");
#if 0
    // SBE async ffdc
    captureAsyncFFDC(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                     SBE_SEC_DMT_TIMEOUT);

    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    // check stop the system
    plat_target_handle_t l_hndl;
    fapiRc = putscom_abs_wrap(&l_hndl, PERV_N3_LOCAL_FIR_OR,
                              ((uint64_t)1 << N3_FIR_CORE_CHECKSTOP_BIT));
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        // Scom failed
        SBE_ERROR (SBE_FUNC "PutScom failed: REG PERV_N3_LOCAL_FIR");
        pk_halt();
    }
#endif
    #undef SBE_FUNC

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint32_t sbeCollectDeadmanFfdc (void)
{
    #define SBE_FUNC "sbeCollectDeadmanFfdc"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // trace the saved aync ffdc reason and SBE state as info for debug
    SBE_INFO (SBE_FUNC "FFDC Reason: 0x%08X States - Curr: %d Prev: %d",
              SBE_GLOBAL->asyncFfdcRC,
              SbeRegAccess::theSbeRegAccess().getSbeState(),
              SbeRegAccess::theSbeRegAccess().getSbePrevState());

// TODO - We need the new p10_collect_deadman_ffdc from PM Team
#if 0
    fapi2::Target<fapi2::TARGET_TYPE_CORE> coreTarget (
           plat_getTargetHandleByChipletNumber <fapi2::TARGET_TYPE_CORE> (
           (SBE_GLOBAL->deadmanCore + CORE_CHIPLET_OFFSET) ));
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    // p9_collect_deadman_ffdc collects the required ffdc into the fapi rc
    // which will be available in the SBE Global HWP FFDC region
    SBE_EXEC_HWP ( fapiRc,
                   p9_collect_deadman_ffdc,
                   coreTarget,
                   SBE_GLOBAL->asyncFfdcRC );
#endif
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint32_t sbeStartCntlDmt()
{
    #define SBE_FUNC "sbeStartCntlDmt"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    uint8_t fuseMode = 0;
    uint8_t l_is_mpipl = 0;
    do
    {
        // Fetch the Timer Value, Number of Xscom entries, Starting address for
        // the Xscom entries, Then Start a Pk Timer
        psu2SbeDeadmanTimerReg_t req = {};
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                           (sizeof(psu2SbeDeadmanTimerReg_t)/sizeof(uint64_t)),
                           (uint64_t *)&req, true );
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1/2/3");
            break;
        }

        // Pass in the time in micro-second to the start timer interface
        // TODO - We need some kind of check here that it doesn't overflow
        // uint32 size
#if 0   // step 16.1 HW PON BU Hack >>
        // Do not time-out until the activate primary cores BU issues are fixed
        rc = g_sbe_pk_dmt_timer.startTimer( (uint32_t )req.timeValueMsec * 1000,
                                     (PkTimerCallback)&sbeDmtPkExpiryCallback );

        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INTERNAL_ERROR, rc);
            SBE_ERROR(SBE_FUNC" g_sbe_pk_dmt_timer.startTimer failed, TimerVal "
                "[0x%08X]", (uint32_t )req.timeValueMsec);
            rc = SBE_SEC_OPERATION_SUCCESSFUL;
        }
#endif  // << step 16.1 HW PON BU Hack

        sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to send response to Hostboot ");
            break;
        }
        // Set DMT State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(SBE_DMT_ENTER_EVENT);
        SBE_GLOBAL->asyncFfdcRC = FAPI2_RC_SUCCESS;

        // To start, assume no errors when starting DMT and hence default to
        // potential timeout in stopping DMT for FFDC

        // TODO Remove the p9 RC and 
        // Add a new RC for DEADMAN timeout and update the sbeFFDC.C/H
        SBE_GLOBAL->asyncFfdcRC = RC_CHECK_MASTER_STOP15_DEADMAN_TIMEOUT;
        Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();

        // Find the fuse mode, if fuse mode then Deadman timer loop should take
        // care of Master and associated slave core (Core0/1, Core2/3, so on)
        FAPI_ATTR_GET(ATTR_FUSED_CORE_MODE,Target<TARGET_TYPE_SYSTEM>(),fuseMode);
        fapi2::ATTR_MASTER_CORE_Type masterCore;
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,procTgt,masterCore);
        std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_core_targets;
        for (auto l_core_target : procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            fapi2::ATTR_CHIP_UNIT_POS_Type l_core_num;
            FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_core_target, l_core_num);

            if (l_core_num == masterCore)
            {
                l_core_targets.push_back(l_core_target);
                if(fuseMode)
                {
                    //The fused core will be either (0,1) or (2,3)
                    //If the master core is odd, partner core will be mastercore -1
                    //If the master core is even, partner core will be mastercorecore + 1
                    fapi2::ATTR_MASTER_CORE_Type l_partner_core_num =
                         (masterCore % 2) ? (masterCore - 1) : (masterCore + 1);
                    fapi2::Target<fapi2::TARGET_TYPE_CORE> l_partner_core_trgt =
                    plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(l_partner_core_num);
                    l_core_targets.push_back(l_partner_core_trgt);
                }
                break;
            }
        }
        if(fuseMode)
        {
           if(l_core_targets.size() != 2)
           {
               SBE_ERROR("Failed to get the fused core targets % d", l_core_targets.size());
               break;
           }
        }
        else
        {
           if(l_core_targets.size() != 1)
           {
               SBE_ERROR("Failed to get the master core target % d", l_core_targets.size());
               break;
           }

        }
        bool hwpFailed = false;
        // Initialise both cores with fapi2::RC_STOP_TRANSITION_PENDING
        uint32_t rcFapi[2] = {RC_STOP_TRANSITION_PENDING,RC_STOP_TRANSITION_PENDING};

        // Call HWP p10_sbe_check_master_stop15 in a loop as long as the timer is
        // active and HWP returns RC_STOP_TRANSITION_PENDING
        SBE_INFO (SBE_FUNC "1. Entering Check Master Stop 15 Loop .. ");
        do
        {
            uint8_t coreCnt = 0;
            uint8_t i = 0;
            SBE_INFO (SBE_FUNC "Check Master Stop 15 Loop Iteration #%d", i++);

            // P10 HW BU workaround .. wait for 1s
            // Adding a higher delay here to mitigate any potential SCOM conflict
            // related issue and give QME+SR enought time to enter stop15
            SBE_INFO (SBE_FUNC "Waiting ~1 s before next poll on core(s) ..");
            delay (CHECK_MASTER_STOP5_POLL_INTERVAL_NS, CHECK_MASTER_STOP5_POLL_INTERVAL_SIMICS);

            for(auto core : l_core_targets)
            {
                // Skip calling on core that already entered stop15
                if (rcFapi[coreCnt] == RC_STOP_TRANSITION_PENDING)
                {
                    SBE_GLOBAL->deadmanCore = core.get().getTargetInstance();
                    SBE_INFO (SBE_FUNC "Poll on core [%d]", SBE_GLOBAL->deadmanCore);
                    SBE_EXEC_HWP ( fapiRc,
                                   p10_sbe_check_master_stop15_hwp,
                                   core);
                    rcFapi[coreCnt++] = fapiRc;
                    if ( ((FAPI2_RC_SUCCESS != fapiRc) &&
                           (RC_STOP_TRANSITION_PENDING != fapiRc)) )
                    {
                        hwpFailed = true;
                        // Mark the failure point ..
                        // TODO Remove the p9 RC and 
                        // Add a new RC for Invalid stop sate and update the sbeFFDC.C/H
                        SBE_GLOBAL->asyncFfdcRC =
                                    RC_CHECK_MASTER_STOP15_INVALID_STATE;
                        SBE_ERROR(SBE_FUNC" p10_sbe_check_master_stop15 failed"
                                    "on core[%d]", SBE_GLOBAL->deadmanCore );
                        break;
                    }
                    if (!fuseMode)
                    {   // mark odd core as succeeded & exit the core loop
                        rcFapi[coreCnt] = FAPI2_RC_SUCCESS;
                        break;
                    }
                }
                // Adding a small delay before going to next core
                pk_sleep(PK_MILLISECONDS(SBE_DMT_SLEEP_INTERVAL));
            } // Core loop for check master stop 15
            // Either Core failed or Both Cores succeeded
            if ( hwpFailed || ((FAPI2_RC_SUCCESS == rcFapi[0]) &&
                               (FAPI2_RC_SUCCESS == rcFapi[1])))
            {   // Exit timer loop
                break;
            }
            // Wait if either or both cores are pending to enter stop 15
            // and no error on either cores
            // pk_sleep(PK_MILLISECONDS(SBE_DMT_SLEEP_INTERVAL)); // alredy have 1s delay
            // loop back forever, unless there was success/failure
        }   while (1);

        SBE_INFO (SBE_FUNC "2. Check Master Stop 15 Loop Passed??: %d", (hwpFailed)? true:false);

        if (hwpFailed)
        {
            // exit the do .. while (0) outermost loop
            break;
        }
        // Call p10_sbe_fbc_apply_rt_settings.C
        // Use the count and address for Xscom sent by Host in the chip-op
        if(req.numXscoms)
        {
            SBE_INFO (SBE_FUNC "3. p10_sbe_fbc_apply_rt_settings with %d XScoms", req.numXscoms);

            //As per IPL Doc, SBE should call p10_sbe_fbc_apply_rt_settings.C.
            //Also, we are ignoring the pre checks for master proc, HB_IPL and MPIPL.
            //All this will be handled by HB.
            fapiRc = sbeFetchAndApplyXSCOMInit(req.numXscoms,req.xscomStartAddr);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in sbeFetchAndApplyXSCOMInit()"
                    "Number of Xscom entries[0x%08X][%08X] Address[0x%08X][%08X]",
                    SBE::higher32BWord(req.numXscoms),SBE::lower32BWord(req.numXscoms),
                    SBE::higher32BWord(req.xscomStartAddr),SBE::lower32BWord(req.xscomStartAddr));
                break;
            }
        }

        //Skip Powering down of backing caches in MPIPL Path
        fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
        FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL,FAPI_SYSTEM,l_is_mpipl);
        if( !l_is_mpipl )
        {
            //Call p10_sbe_powerdown_backing_caches.C
            //Doorbell to the QMEs to have a “virtual” STOP 11 a “virtual” STOP 11
            SBE_INFO (SBE_FUNC "4. Powering Down Backing Caches .. ");
            SBE_EXEC_HWP(fapiRc, p10_sbe_powerdown_backing_caches_hwp, procTgt);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in p10_sbe_powerdown_backing_caches()");
                break;
            }
        }
        else
        {
            SBE_INFO(SBE_FUNC "MPIPL Path: Skip Powering Down Backing Caches!");
            //Clear the MPIPL Attribute
            l_is_mpipl = 0;
            PLAT_ATTR_INIT(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), l_is_mpipl);
        }

        // Both cores entered stop 15 successfully, now unblock interrupts
        for(auto core : l_core_targets)
        {
            // Get the Core Target
            SBE_GLOBAL->deadmanCore = core.get().getTargetInstance();
            SBE_INFO(SBE_FUNC "Executing p10_block_wakeup_intr_hwp for Core[%d]",
                SBE_GLOBAL->deadmanCore);
            SBE_EXEC_HWP(fapiRc, p10_block_wakeup_intr_hwp, core,
                         p10pmblockwkup::DISABLE_BLOCK_EXIT);
            if (fapiRc)
            {
                // Mark the failure point .. SBE waits for DMT timer to expire
                // TODO Remove the p9 RC and 
                // Add a new RC for wake up check and update the sbeFFDC.C/H
                SBE_GLOBAL->asyncFfdcRC = RC_BLOCK_WAKEUP_INTR_CHECK_FAIL;
                SBE_ERROR(SBE_FUNC" p10_block_wakeup_intr() failed for "
                    "Core[%d]", SBE_GLOBAL->deadmanCore);
                break;
            }
            // If Success for the First core & it's a Fuse core then
            // continue here for the Second core then go on to press the
            // Door Bell
            if(!fuseMode)
            {
                break;
            }
        }
        // Break out for the p10_block_wakeup_intr failure above
        // Dont press the Door bell
        if(fapiRc)
        {
            break;
        }
        // Entered stop15 and unblocked interrupts ..
        // Indicate the Host via Bit SBE_SBE2PSU_DOORBELL_SET_BIT2
        // that Stop15 exit
        SBE_INFO (SBE_FUNC "5. Waking up HB master core .. ");
        rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT2);
        if(rc)
        {
            SBE_ERROR(SBE_FUNC "Failed to Write SBE_SBE2PSU_DOORBELL_SET_BIT2");
        }
        else
        {
            //Update teh Global variable which indicates the SBE has completed
            //DMT state
            SBE_GLOBAL->sbeDmtStateComplete = 0x1;
        }
    }   while(0); // Outer loop

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint32_t sbeStopCntlDmt()
{
    #define SBE_FUNC "sbeStopCntlDmt "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        SBE_INFO(SBE_FUNC "Stop Timer.");
        rc = g_sbe_pk_dmt_timer.stopTimer( );
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INTERNAL_ERROR, rc);
            SBE_ERROR(SBE_FUNC"g_sbe_pk_dmt_timer.stopTimer failed");
            rc = SBE_SEC_OPERATION_SUCCESSFUL;
            break;
        }

        // Reset Async FFDC RC to default success
        SBE_GLOBAL->asyncFfdcRC = FAPI2_RC_SUCCESS;
        // Set Runtime State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_DMT_COMP_EVENT);
    }while(0);
    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint32_t sbeControlDeadmanTimer (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeControlDeadmanTimer"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_START_DMT)
        {
            rc = sbeStartCntlDmt();
            if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
            {
                SBE_ERROR(SBE_FUNC " Failed sbeStartCntlDmt");
            }
            break;
        }
        // Send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        // This util method will check internally on the mbox0 register if ACK
        // is requested.
        rc = sbeAcknowledgeHost();
        if(rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                    "SBE_SBE2PSU_DOORBELL_SET_BIT1");
            break;
        }

        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_STOP_DMT)
        {
            rc = sbeStopCntlDmt();
            if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
            {
                SBE_ERROR(SBE_FUNC " Failed sbeStopCntlDmt");
            }
            break;
        }
        SBE_ERROR(SBE_FUNC" Not a valid command ");
        rc = SBE_SEC_COMMAND_NOT_SUPPORTED;
    }while(0); // End of do-while

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
