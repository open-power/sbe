/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdtpmextendmode.C $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
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

#include "sbecmdtpmextendmode.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_host_intf.H"
#include "sbeglobals.H"
#include "sbeHostUtils.H"
#include "sbestates.H"
#include "p10_sbe_stop_hb.H"
#include "sbeutil.H"
#include "p10_sbe_instruct_start.H"
#include "p10_sbe_exit_cache_contained.H"
#include "p10_scom_proc_9.H"
#include "sbeFFDC.H"
#include "sbeTimerSvc.H"
#include "p10_putmemproc.H"
#include "fapi2_mem_access.H"
#include "sbeRoleIdentifier.H"

/**
 * @brief Enum for operation's supported by ctrlHost()
 *        0x1 => Start Host Boot
 *        0x2 => Stop Host Boot
 *
 */
enum hostCtrlFlags: uint8_t
{
    STOP_HOST  = 0x1,
    START_HOST = 0x2,
};

/**
 * @brief Information received from Host as part of PSU chip-op in
 *        the mailbox registers.
 * Mbox Reg1 - timeValue in millisec to wait for HB to send
 *             TPM Exit mode chip-op after it is started by SBE
 * Mbox Reg2 - None
 * Mbox Reg3 - None
 */
typedef struct
{
    uint64_t timeValueMsec;
    uint64_t unused;
    uint64_t unused1;
} psu2SbeTpmExtendModeReg_t;

//Static initialization of the Host Alive Pk timer
static timerService g_sbe_pk_Host_Alive_timer;

/**
 * @brief Function to Start/Stop Host
 *
 * @param i_flag Decides the operation to be performed on Host (stop/Start Host)
 * @return uint32_t FAPI RC if any
 */
static uint32_t ctrlHost(hostCtrlFlags i_flag)
{
    #define SBE_FUNC " ctrlHost "
    SBE_ENTER(SBE_FUNC)

    uint32_t fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        Target<fapi2::TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
        std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_active_core_targets;
        std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_backing_cache_targets;
        fapi2::Target<fapi2::TARGET_TYPE_CORE> l_master_core_target;
        fapi2::Target<fapi2::TARGET_TYPE_CORE> l_master_core_pair_target;

        // process input targets
        SBE_EXEC_HWP(fapiRc,
                     p10_sbe_exit_cache_contained_validate_core_inputs,
                     procTgt,
                     l_active_core_targets,
                     l_backing_cache_targets,
                     l_master_core_target,
                     l_master_core_pair_target)

        if (fapiRc)
        {
            SBE_ERROR(SBE_FUNC "Error from p10_sbe_exit_cache_contained_validate_core_inputs");
            break;
        }

        if(i_flag == STOP_HOST)
        {
            SBE_EXEC_HWP(fapiRc,
                         p10_sbe_stop_hb,
                         l_active_core_targets);

            if (fapiRc)
            {
                SBE_ERROR(SBE_FUNC "Error from p10_sbe_stop_hb");
                break;
            }
        }

        if(i_flag == START_HOST)
        {
            fapi2::ATTR_FUSED_CORE_MODE_Type l_fused_core;
            //Need to derive the threads to start while resuming the HB based on
            //the FUSED Core Mode.
            // Check the fused core mode
            FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                                fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                                l_fused_core);

            if (l_fused_core)
            {
                //Start Instruction in thread0, thread1 for both masterCore
                //and associated fused Core Pair Target. T0 & T1 are fixed
                SBE_EXEC_HWP(fapiRc,
                            p10_sbe_instruct_start,
                            l_master_core_target,
                            static_cast<ThreadSpecifier>(THREAD0 | THREAD1));

                if (fapiRc)
                {
                    SBE_ERROR(SBE_FUNC "Error from p10_sbe_instruct_start for MasterCore"
                            " in Fused Core mode");
                    break;
                }

                SBE_EXEC_HWP(fapiRc,
                            p10_sbe_instruct_start,
                            l_master_core_pair_target,
                            static_cast<ThreadSpecifier>(THREAD0 | THREAD1));

                if (fapiRc)
                {
                    SBE_ERROR(SBE_FUNC "Error from p10_sbe_instruct_start for "
                            "MasterCore Pair Target in Fused Core mode");
                    break;
                }
            }
            else
            {
                // Normal Core, Just start all threads in Master Core
                SBE_EXEC_HWP(fapiRc,
                            p10_sbe_instruct_start,
                            l_master_core_target,
                            ALL_THREADS);

                if (fapiRc)
                {
                    SBE_ERROR(SBE_FUNC "Error from p10_sbe_instruct_start");
                    break;
                }
            }
        }
    } while (0);

    SBE_EXIT(SBE_FUNC)
    return fapiRc;
    #undef SBE_FUNC
}

/**
 * @brief Callback function in case Host Alive timer Expires
 *
 */
static void hostAliveTimerPkExpiryCallback(void *)
{
    #define SBE_FUNC "hostAliveTimerPkExpiryCallback"
    SBE_ENTER(SBE_FUNC)

    SBE_INFO (SBE_FUNC "Host Alive Callback Timer has expired.."
                       "No-Checkstop on the system for now"
                       "and FFDC will not be collected.");
    // SBE async ffdc
    captureAsyncFFDC(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                     SBE_SEC_HOST_ALIVE_TIMER_TIMEOUT);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    fapi2::buffer<uint64_t> data(0);

    // check stop the system
    Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    data.setBit<scomt::proc::TP_TCN1_N1_LOCAL_FIR_IN58>();
    //Set bit 58 of TP_TCN1_N1_LOCAL_FIR_WO_OR
    fapiRc = putscom_abs_wrap(&procTgt, scomt::proc::TP_TCN1_N1_LOCAL_FIR_WO_OR, data());

    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        // Scom failed
        SBE_ERROR (SBE_FUNC "PutScom failed: REG TP_TCN1_N1_LOCAL_FIR");
        (void)SbeRegAccess::theSbeRegAccess().
              updateSbeState(SBE_STATE_TPM_EXTEND_MODE_HALT);
        SBE_INFO("Halting PPE...");
        pk_halt();
    }

    SBE_EXIT(SBE_FUNC)
    #undef SBE_FUNC
}

/**
 * @brief Function to start Host Alive timer
 *
 * @return uint32_t RC if any
 */
static uint32_t startHostAliveTimer()
{
    #define SBE_FUNC " startHostAliveTimer "
    SBE_ENTER(SBE_FUNC)

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        // Fetch the Timer Value and then Start a Pk Timer
        psu2SbeTpmExtendModeReg_t req = {};
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                           (sizeof(psu2SbeTpmExtendModeReg_t)/sizeof(uint64_t)),
                           (uint64_t *)&req, true );
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1/2/3");
            break;
        }

        // Pass in the time in micro-second to the start timer interface
        // TODO - We need some kind of check here that it doesn't overflow
        // uint32 size
        rc = g_sbe_pk_Host_Alive_timer.startTimer( (uint32_t )req.timeValueMsec * 1000,
                                     (PkTimerCallback)&hostAliveTimerPkExpiryCallback );

        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INTERNAL_ERROR, rc);
            SBE_ERROR(SBE_FUNC" g_sbe_pk_Host_Alive_timer.startTimer failed, TimerVal "
                "[0x%08X]", (uint32_t )req.timeValueMsec);
            break;
        }
    }while(0);

    SBE_EXIT(SBE_FUNC)
    return rc;
    #undef SBE_FUNC
}

/**
 * @brief Funtion to stop Host Alive timer
 *
 * @return uint32_t RC if any
 */
static uint32_t stopHostAliveTimer()
{
    #define SBE_FUNC "stopHostAliveTimer "
    SBE_ENTER(SBE_FUNC)

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        SBE_INFO(SBE_FUNC "Stop Timer.");
        rc = g_sbe_pk_Host_Alive_timer.stopTimer( );
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INTERNAL_ERROR, rc);
            SBE_ERROR(SBE_FUNC"g_sbe_pk_Host_Alive_timer.stopTimer failed");
            rc = SBE_SEC_OPERATION_SUCCESSFUL;
            break;
        }

    }while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

/**
 * @brief Function to Activate SMP Switch
 *        SBE will only perform funtions of SMP_ACTIVATE_SWITCH
 *        as in p10_build_smp() HWP for primary proc only.
 *        HB would have already performed other parts og the HWP
 *        Reference: ekb-p10/chips/p10/procedures/hwp/nest/p10_build_smp.C
 *
 * @return uint32_t FAPI RC if any
 */
static uint32_t buildSmpSwitchAB(void)
{
    #define SBE_FUNC " buildSmpSwitchAB "
    SBE_ENTER(SBE_FUNC)

    uint32_t fapiRc = FAPI2_RC_SUCCESS;

    uint32_t l_flags = fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PROC |
                       fapi2::SBE_MEM_ACCESS_FLAGS_SWITCH_MODE;
    uint32_t l_bytes = 1;
    uint64_t l_addr = 0x0ULL;
    uint8_t l_data_unused[1];
    Target<fapi2::TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();

    SBE_EXEC_HWP(fapiRc,
                 p10_putmemproc,
                 procTgt,
                 l_addr,
                 l_bytes,
                 l_data_unused,
                 l_flags);

    SBE_EXIT(SBE_FUNC)
    return fapiRc;
    #undef SBE_FUNC
}

uint32_t sbeTpmExtendMode(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeTpmExtendMode "
    SBE_ENTER(SBE_FUNC)

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    uint32_t sbeRole;

    do
    {
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

        // This chip-op is expected only on primary proc, if not primary
        // return RC.
        sbeRole = checkSbeRole();
        SBE_INFO(SBE_FUNC "SBE Role is %x", sbeRole);
        if(sbeRole != SBE_ROLE_MASTER)
        {
            SBE_ERROR(SBE_FUNC " PSU Chip-op allowed only on primary SBE");
            rc = SBE_SEC_COMMAND_NOT_SUPPORTED_ON_SECONDARY_CHIP;
            break;
        }

        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_ENTER_TPM_EXTEND_MODE)
        {
            // Stop HB
            fapiRc = ctrlHost(STOP_HOST);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in p10_sbe_stop_hb() ");
                break;
            }

            // Disable Xscoms
            SBE::disableXscoms();

            // Run SMP HWP only Switch AB for master proc
            fapiRc = buildSmpSwitchAB();
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in buildSmpSwitchAB() ");
                break;
            }

            // Collect secondary TPM measurements

            // Enable Xscoms
            SBE::enableXscoms();

            // Start HB Alive timer
            rc = startHostAliveTimer();
            if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC "Failed to start host alive timer");
                break;
            }

            // Start HB
            fapiRc = ctrlHost(START_HOST);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in p10_sbe_instruct_start() ");

                // If we fail to start HB lets stop the Host Alive timer
                SBE_INFO(SBE_FUNC "Stoping Host Alive timer");
                rc = stopHostAliveTimer();
                if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    SBE_ERROR(SBE_FUNC "Failed to stop host alive timer");
                    break;
                }
                break;
            }

            break;
        }

        if(SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_EXIT_TPM_EXTEND_MODE)
        {
            rc = stopHostAliveTimer();
            if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC "Failed to stop host alive timer");
                break;
            }

            break;
        }

        SBE_ERROR(SBE_FUNC " Not a valid command ");
        rc = SBE_SEC_COMMAND_NOT_SUPPORTED;

    }while(0);

    if( (fapiRc != FAPI2_RC_SUCCESS) && (rc == SBE_SEC_OPERATION_SUCCESSFUL) )
    {
        SBE_ERROR(SBE_FUNC "fapiRc=0x%.8x rc[0x%X]",fapiRc,rc);
    }

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC" Failed to write SBE_HOST_PSU_MBOX_REG4. rc[0x%X]", rc);
        (void)SbeRegAccess::theSbeRegAccess().
              updateSbeState(SBE_STATE_TPM_EXTEND_MODE_HALT);
        SBE_INFO("Halting PPE...");
        pk_halt();
    }

    SBE_EXIT(SBE_FUNC)
    return rc;
    #undef SBE_FUNC
}