/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdcntrldmt.C $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
#include "p9_sbe_check_master_stop15.H"
#include "p9_perv_scom_addresses.H"
#include "p9_block_wakeup_intr.H"

using namespace fapi2;

// Using Function pointer to force long call
p9_sbe_check_master_stop15_FP_t p9_sbe_check_master_stop15_hwp =
                                            &p9_sbe_check_master_stop15;

////////////////////////////////////////////////////////////////////
//Static initialization of the Dmt Pk timer
PkTimer g_sbe_pk_dmt_timer;
// Global Flag to indicate Dmt Timer Expiry
bool g_SbeDmtTimerExpired = false;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sbeDmtPkExpiryCallback(void *)
{
    #define SBE_FUNC "sbeDmtPkExpiryCallback"
    SBE_INFO(SBE_FUNC" DMT Callback Timer has expired..Checkstop the system ");
    g_SbeDmtTimerExpired = true;

    (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_DUMP_FAILURE_EVENT);

    // check stop the system
    uint32_t l_status = SBE_PCB_PIB_ERROR_NONE;
    l_status = putscom_abs (PERV_N3_LOCAL_FIR_OR, N3_FIR_CORE_CHECKSTOP_BIT);
    if(SBE_PCB_PIB_ERROR_NONE != l_status)
    {
        // Scom failed
        SBE_ERROR(SBE_FUNC "PutScom failed for REG PERV_N3_LOCAL_FIR");
        // TODO - Store the response in Async Response
        // RTC:149074
    }
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeStartCntlDmt()
{
    #define SBE_FUNC "sbeStartCntlDmt"
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    int l_pkRc = 0;
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    g_SbeDmtTimerExpired = false;

    do
    {
        // Fetch the Timer Value and Start a Pk Timer
        uint64_t l_timerVal = 0;
        l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(l_timerVal)/sizeof(uint64_t)),
                                    &l_timerVal);
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1");
            break;
        }

        // Attach Callback
        PkTimerCallback l_callback = &sbeDmtPkExpiryCallback;

        // Create Timer with the above value
        l_pkRc = pk_timer_create(&g_sbe_pk_dmt_timer, l_callback, NULL);
        if(l_pkRc)
        {
            SBE_ERROR(SBE_FUNC" Pk Timer Create failed, RC=[%d]", l_pkRc);
            l_rc = SBE_SEC_OS_FAILURE;
            break;
        }

        // Schedule the timer
        l_pkRc = pk_timer_schedule(&g_sbe_pk_dmt_timer,
                                   PK_MILLISECONDS((uint32_t)l_timerVal));
        if(l_pkRc)
        {
            SBE_ERROR(SBE_FUNC" Pk Timer Schedule failed, RC=[%d]", l_pkRc);
            l_rc = SBE_SEC_OS_FAILURE;
            break;
        }

        l_rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG4,
                                (uint64_t*)(&g_sbeSbe2PsuRespHdr),
                                (sizeof(g_sbeSbe2PsuRespHdr)/sizeof(uint64_t)),
                                true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to write to "
                    "SBE_HOST_PSU_MBOX_REG4");
            break;
        }
        // Set DMT State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_DMT_ENTER_EVENT);

        // Fetch the master core
        Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();

        // Fetch the MASTER_CORE attribute
        uint8_t l_coreId = 0;
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,l_procTgt,l_coreId);

        // Construct the Master Core Target
        fapi2::Target<fapi2::TARGET_TYPE_CORE > l_coreTgt(
                (uint64_t)l_coreId);

        // Call Hwp p9_sbe_check_master_stop15 and loop
        // Go around a loop till you get FAPI2_RC_SUCCESS
        do
        {
            l_fapiRc = p9_sbe_check_master_stop15_hwp(l_coreTgt);
            //Conversion is required here, since ReturnCode doesn't support
            //comparision '!=' or '=='
            //TODO RTC:149021
            uint32_t l_rcFapi = l_fapiRc;
            if( (l_rcFapi != fapi2::RC_CHECK_MASTER_STOP15_PENDING) &&
                    (l_rcFapi != FAPI2_RC_SUCCESS))
            {
                SBE_ERROR(SBE_FUNC" p9_sbe_check_master_stop15 "
                        "returned failure");
                // Async Response to be stored
                // RTC:149074
                break;
            }

            // Only for Pending and Success case
            if(RC_CHECK_MASTER_STOP15_PENDING != l_rcFapi) // Success
            {
                l_fapiRc = p9_block_wakeup_intr(l_coreTgt,
                                                p9pmblockwkup::CLEAR );
                if( l_fapiRc )
                {
                    SBE_ERROR(SBE_FUNC" p9_block_wakeup_intr failed ");
                    // TODO via RTC 149074
                    // Async Response to be stored.
                    // Also checkstop the system.
                    break;
                }
                // indicate the Host via Bit SBE_SBE2PSU_DOORBELL_SET_BIT2
                // that Stop15 exit
                l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT2);
                if(l_rc)
                {
                    SBE_ERROR(SBE_FUNC " Failed to Write "
                            "SBE_SBE2PSU_DOORBELL_SET_BIT2");
                }
                break; // Breakout from do..while()
            }
            // Stop 15 Pending Case
            pk_sleep(PK_MILLISECONDS(SBE_DMT_SLEEP_INTERVAL));

        }while(false == g_SbeDmtTimerExpired); // Inner Loop

    }while(0); // Outer loop

    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeStopCntlDmt()
{
    #define SBE_FUNC "sbeStopCntlDmt"
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    int l_pkRc = 0;

    do
    {
        // Stop the Pk Timer - There is no call to check if the timer is
        // still alive, if i call cancel on already expired timer, it
        // returns error code.
        if(false == g_SbeDmtTimerExpired)
        {
            SBE_INFO(SBE_FUNC " DmTimer hasn't expired yet.. stop it");
            l_pkRc = pk_timer_cancel(&g_sbe_pk_dmt_timer);
            if(l_pkRc)
            {
                // Check again if the failure is because of the timer already
                // expired, in that case don't need to send any error response
                // to hostboot
                if(false == g_SbeDmtTimerExpired)
                {
                    SBE_ERROR(SBE_FUNC " Pk Timer Cancel failed, RC=[%d]",
                        l_pkRc);
                    l_rc = SBE_SEC_OS_FAILURE;
                }
                break;
            }
        }

        l_rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG4,
                                 (uint64_t*)(&g_sbeSbe2PsuRespHdr),
                                 (sizeof(g_sbeSbe2PsuRespHdr)/sizeof(uint64_t)),
                                 true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to write to SBE_HOST_PSU_MBOX_REG4");
            break;
        }
        // Set Runtime State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                            SBE_DMT_COMP_EVENT);
    }while(0);

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

        if(g_sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_START_DMT)
        {
            l_rc = sbeStartCntlDmt();
            if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
            {
                SBE_ERROR(SBE_FUNC " Failed sbeStartCntlDmt");
                break;
            }
        }
        else if(g_sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_STOP_DMT)
        {
            l_rc = sbeStopCntlDmt();
            if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
            {
                SBE_ERROR(SBE_FUNC " Failed sbeStopCntlDmt");
                break;
            }
        }
        else
        {
            SBE_ERROR(SBE_FUNC" Not a valid command ");
            l_rc = SBE_SEC_COMMAND_NOT_SUPPORTED;
        }
    }while(0); // End of do-while

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
