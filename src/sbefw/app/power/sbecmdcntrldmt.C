/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdcntrldmt.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbe_sp_intf.H"
#include "fapi2.H"
#include "plat_hw_access.H"
#include "sbeTimerSvc.H"
#include "sbeglobals.H"
#include "sbeFFDC.H"

using namespace fapi2;

#if 0
#ifdef SEEPROM_IMAGE
// Using Function pointer to force long call
p9_sbe_check_master_stop15_FP_t p9_sbe_check_master_stop15_hwp =
                                &p9_sbe_check_master_stop15;
p9_block_wakeup_intr_FP_t p9_block_wakeup_intr_hwp =
                          &p9_block_wakeup_intr;
#endif

////////////////////////////////////////////////////////////////////
//Static initialization of the Dmt Pk timer
static timerService g_sbe_pk_dmt_timer;
#endif

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sbeDmtPkExpiryCallback(void *)
{
    #define SBE_FUNC "sbeDmtPkExpiryCallback"
    SBE_INFO (SBE_FUNC "DMT Callback Timer has expired..Checkstop the system");
#if 0
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;

    // SBE async ffdc
    captureAsyncFFDC(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                     SBE_SEC_DMT_TIMEOUT);

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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeCollectDeadmanFfdc (void)
{
    #define SBE_FUNC "sbeCollectDeadmanFfdc"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // trace the saved aync ffdc reason and SBE state as info for debug
    SBE_INFO (SBE_FUNC "FFDC Reason: 0x%08X States - Curr: %d Prev: %d",
              SBE_GLOBAL->asyncFfdcRC,
              SbeRegAccess::theSbeRegAccess().getSbeState(),
              SbeRegAccess::theSbeRegAccess().getSbePrevState());

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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeStartCntlDmt()
{
    #define SBE_FUNC "sbeStartCntlDmt"
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    do
    {
        // Fetch the Timer Value and Start a Pk Timer
        uint64_t l_timerVal = 0;
        l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(l_timerVal)/sizeof(uint64_t)),
                                    &l_timerVal, true );
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1");
            break;
        }

        l_rc = g_sbe_pk_dmt_timer.startTimer( (uint32_t )l_timerVal*1000,
                                     (PkTimerCallback)&sbeDmtPkExpiryCallback);
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INTERNAL_ERROR, l_rc);
            SBE_ERROR(SBE_FUNC" g_sbe_pk_dmt_timer.startTimer failed");
            l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
        }

        sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, l_rc);

        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to send response to Hostboot ");
            break;
        }

        // Set DMT State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_DMT_ENTER_EVENT);
        // To start, assume no errors will hit when starting DMT and hence
        // default to potential timeout in stopping DMT for FFDC
        SBE_GLOBAL->asyncFfdcRC = RC_CHECK_MASTER_STOP15_DEADMAN_TIMEOUT;

        Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
        // Fetch the Master EX
        uint8_t exId = 0;
        uint8_t fuseMode = 0;
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_EX,l_procTgt,exId);
        FAPI_ATTR_GET(ATTR_FUSED_CORE_MODE, Target<TARGET_TYPE_SYSTEM>(), fuseMode);
        fapi2::Target<fapi2::TARGET_TYPE_EX >
            exTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_EX>(exId));

        bool hwpFailed = false;
        // Initialise both cores with fapi2::RC_CHECK_MASTER_STOP15_PENDING
        uint32_t rcFapi[2] = {RC_CHECK_MASTER_STOP15_PENDING};

        // Call HWP p9_sbe_check_master_stop15 in a loop as long as the timer is
        // active and HWP returns RC_CHECK_MASTER_STOP15_PENDING
        do
        {
            uint8_t coreCnt = 0;

            for (auto &coreTgt : exTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
            {
                // Skip calling on core that already entered stop15
                if (rcFapi[coreCnt] == RC_CHECK_MASTER_STOP15_PENDING)
                {
                    SBE_GLOBAL->deadmanCore = coreTgt.get().getTargetInstance();
                    // Core0 is assumed to be the master core
                    SBE_INFO ( SBE_FUNC
                               "Executing p9_sbe_check_master_stop15_hwp for"
                               " Core[%d]", SBE_GLOBAL->deadmanCore );
                    SBE_EXEC_HWP ( l_fapiRc,
                                   p9_sbe_check_master_stop15_hwp,
                                   coreTgt);
                    rcFapi[coreCnt++] = l_fapiRc;

                    if (! ((FAPI2_RC_SUCCESS == l_fapiRc) ||
                           (RC_CHECK_MASTER_STOP15_PENDING == l_fapiRc)) )
                    {
                        hwpFailed = true;
                        // Mark the failure point ..
                        SBE_GLOBAL->asyncFfdcRC =
                                    RC_CHECK_MASTER_STOP15_INVALID_STATE;
                        SBE_ERROR ( SBE_FUNC" p9_sbe_check_master_stop15 failed"
                                    "on core[%d]", SBE_GLOBAL->deadmanCore );
                        break;
                    }

                    if (!fuseMode)
                    {   // mark odd core as succeeded & exit the core loop
                        rcFapi[coreCnt] = FAPI2_RC_SUCCESS;
                        break;
                    }
                }
            }   // Core loop for check master stop 15

            // Either Core failed or Both Cores succeeded
            if ( hwpFailed || ((FAPI2_RC_SUCCESS == rcFapi[0]) &&
                               (FAPI2_RC_SUCCESS == rcFapi[1])))
            {   // Exit timer loop
                break;
            }

            // Wait if either or both cores are pending to enter stop 15
            // and no error on either cores
            pk_sleep(PK_MILLISECONDS(SBE_DMT_SLEEP_INTERVAL));

            // loop back only if timer is still active
        }   while (g_sbe_pk_dmt_timer.isActive());

        if (hwpFailed)
        {   // exit the do .. while (0) outermost loop
            break;
        }

        // Both cores entered stop 15 successfully, now unblock interrupts
        for (auto coreTgt : exTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            SBE_GLOBAL->deadmanCore = coreTgt.get().getTargetInstance();
            SBE_INFO(SBE_FUNC "Executing p9_block_wakeup_intr_hwp for Core[%d]",
                SBE_GLOBAL->deadmanCore);
            SBE_EXEC_HWP(l_fapiRc, p9_block_wakeup_intr_hwp, coreTgt,
                                        p9pmblockwkup::CLEAR);
            if (l_fapiRc)
            {
                // Mark the failure point .. SBE waits for DMT timer to expire
                SBE_GLOBAL->asyncFfdcRC = RC_BLOCK_WAKEUP_INTR_CHECK_FAIL;
                SBE_ERROR(SBE_FUNC" p9_block_wakeup_intr failed for "
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

        // Break out for the p9_block_wakeup_intr failure above
        // Dont press the Door bell
        if(l_fapiRc)
        {
            break;
        }

        // Entered stop15 and unblocked interrupts ..
        // Indicate the Host via Bit SBE_SBE2PSU_DOORBELL_SET_BIT2
        // that Stop15 exit
        l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT2);
        if(l_rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Write "
                 "SBE_SBE2PSU_DOORBELL_SET_BIT2");
        }
    }   while(0); // Outer loop
#endif
    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeStopCntlDmt()
{
    #define SBE_FUNC "sbeStopCntlDmt "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        SBE_INFO(SBE_FUNC "Stop Timer.");
        l_rc = g_sbe_pk_dmt_timer.stopTimer( );
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus ( SBE_PRI_INTERNAL_ERROR,
                                                      l_rc );
            SBE_ERROR(SBE_FUNC"g_sbe_pk_dmt_timer.stopTimer failed");
            l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
            break;
        }

        // Reset Async FFDC RC to default success
        SBE_GLOBAL->asyncFfdcRC = FAPI2_RC_SUCCESS;
        // Set Runtime State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_DMT_COMP_EVENT);
    }while(0);
    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, l_rc);
#endif
    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeControlDeadmanTimer (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeControlDeadmanTimer"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_START_DMT)
        {
            l_rc = sbeStartCntlDmt();
            if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
            {
                SBE_ERROR(SBE_FUNC " Failed sbeStartCntlDmt");
            }
            break;
        }
        // Send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        // This util method will check internally on the mbox0 register if ACK
        // is requested.
        l_rc = sbeAcknowledgeHost();
        if(l_rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                    "SBE_SBE2PSU_DOORBELL_SET_BIT1");
            break;
        }

        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_STOP_DMT)
        {
            l_rc = sbeStopCntlDmt();
            if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
            {
                SBE_ERROR(SBE_FUNC " Failed sbeStopCntlDmt");
            }
            break;
        }
        SBE_ERROR(SBE_FUNC" Not a valid command ");
        l_rc = SBE_SEC_COMMAND_NOT_SUPPORTED;
    }while(0); // End of do-while

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
