/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdreceiver.C $                                  */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdreceiver.C
 *
 * @brief This file contains the SBE Command Receiver Thread Routine
 *
 */


#include "sbeexeintf.H"
#include "sbefifo.H"
#include "sbecmdparser.H"
#include "sbeirq.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "sbeerrorcodes.H"
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sberegaccess.H"
#include "sbeutil.H"

sbeFifoCmdReqBuf_t g_sbeFifoCmdHdr;
sbeCmdRespHdr_t g_sbeCmdRespHdr;
sbePsu2SbeCmdReqHdr_t g_sbePsu2SbeCmdReqHdr;
sbeSbe2PsuRespHdr_t g_sbeSbe2PsuRespHdr;
sbeIntrHandle_t g_sbeIntrSource;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void sbeCommandReceiver_routine(void *i_pArg)
{
    #define SBE_FUNC " sbeCommandReceiver_routine "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeInterfaceSrc_t curInterface = SBE_INTERFACE_UNKNOWN;

    // Set Current State to First State i.e. Unknown
    (void)SbeRegAccess::theSbeRegAccess().updateSbeState(SBE_STATE_UNKNOWN);

    do
    {
        // @TODO via RTC: 128944
        //       Read Scratchpad
        // Wait for new data in FIFO or FIFO reset interrupt or PSU interrupt
        int l_rcPk = pk_semaphore_pend (&g_sbeSemCmdRecv, PK_WAIT_FOREVER);

        do
        {
            uint8_t l_cmdClass = SBE_CMD_CLASS_UNKNOWN;
            uint8_t l_command  = 0xFF;

            // pk API failure
            if (l_rcPk != PK_OK)
            {
                break;
            }

            SBE_DEBUG(SBE_FUNC"Receiver unblocked");

            // The responsibility of this thread is limited to reading off
            // the FIFO or PSU interfaces to be able to decode the command
            // class and the command opcode parameters.

            // Received FIFO Reset interrupt
            if (  g_sbeIntrSource.isSet(SBE_INTERRUPT_ROUTINE,
                                         SBE_INTERFACE_FIFO_RESET) )
            {
                SBE_ERROR(SBE_FUNC"FIFO reset received");
                l_rc = SBE_FIFO_RESET_RECEIVED;
                break;
            }

            // Received PSU interrupt
            if ( g_sbeIntrSource.isSet(SBE_INTERRUPT_ROUTINE,
                                        SBE_INTERFACE_PSU) )
            {
                //Clear the Interrupt Source bit for PSU
                g_sbeIntrSource.clearIntrSource(SBE_INTERRUPT_ROUTINE,
                                                SBE_INTERFACE_PSU);
                curInterface = SBE_INTERFACE_PSU;
                // First clear PSU->SBE DB bit 0
                l_rc = sbeClearPsu2SbeDbBitX(SBE_PSU2SBE_DOORBELL_CLEAR_BIT0);
                if (l_rc)
                {
                    break;
                }

                // Initialize the g_sbePsu2SbeCmdReqHdr
                g_sbePsu2SbeCmdReqHdr.init();
                // Read the request field from PSU->SBE Mbx Reg0
                uint8_t l_cnt = sizeof(g_sbePsu2SbeCmdReqHdr)/sizeof(uint64_t);

                l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG0, l_cnt,
                                            (uint64_t *)&g_sbePsu2SbeCmdReqHdr);

                g_sbeSbe2PsuRespHdr.init();
                l_cmdClass  = g_sbePsu2SbeCmdReqHdr.cmdClass;
                l_command   = g_sbePsu2SbeCmdReqHdr.command;
            } // end if loop for PSU interface chipOp handling

            // Received FIFO New Data interrupt
            else if ( g_sbeIntrSource.isSet(SBE_INTERRUPT_ROUTINE,
                                             SBE_INTERFACE_FIFO) )
            {
                //Clear the Interrupt Source bit for FIFO
                g_sbeIntrSource.clearIntrSource(SBE_INTERRUPT_ROUTINE,
                                                SBE_INTERFACE_FIFO);
                curInterface = SBE_INTERFACE_FIFO;

                // This thread will attempt to unblock the command processor
                // thread on the following scenarios:
                //  - Normal scenarios where SBE would need to respond to FSP
                //    via downstream FIFO. This includes SUCCESS cases as well
                //    as the cases for Invalid Data sequence or Command
                //    validation failure.
                //  - if there is a need to handle FIFO reset

                // Accordingly, this will update g_sbeCmdRespHdr.prim_status
                // and g_sbeCmdRespHdr.sec_status for command processor thread
                // to handle them later in the sequence.

                g_sbeFifoCmdHdr.cmdClass = SBE_CMD_CLASS_UNKNOWN;
                g_sbeCmdRespHdr.init();
                uint32_t len = sizeof(g_sbeFifoCmdHdr)/sizeof(uint32_t);
                l_rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&g_sbeFifoCmdHdr,
                        false );

                // If FIFO reset is requested,
                if (l_rc == SBE_FIFO_RESET_RECEIVED)
                {
                    SBE_ERROR(SBE_FUNC"FIFO reset received");
                    break;
                }

                // If we received EOT out-of-sequence
                if ( (l_rc == SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA)  ||
                        (l_rc == SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA) )
                {
                    SBE_ERROR(SBE_FUNC"sbeUpFifoDeq_mult failure, "
                            " l_rc=[0x%08X]", l_rc);
                    g_sbeCmdRespHdr.setStatus(SBE_PRI_INVALID_DATA, l_rc);

                    // Reassign l_rc to Success to Unblock command processor
                    // thread and let that take the necessary action.
                    l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                    break;
                }

                l_cmdClass  = g_sbeFifoCmdHdr.cmdClass;
                l_command   = g_sbeFifoCmdHdr.command;
            } // end else if loop for FIFO interface chipOp handling

            // Any other FIFO access issue
            if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                break;
            }

            // validate the command class and sub-class opcodes
            l_rc = sbeValidateCmdClass (l_cmdClass, l_command) ;

            if (l_rc)
            {
                // Command Validation failed;
                SBE_ERROR(SBE_FUNC"Command validation failed");
                if ( SBE_INTERFACE_PSU == curInterface )
                {
                    g_sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INVALID_COMMAND,l_rc);
                }
                else if ( SBE_INTERFACE_FIFO == curInterface )
                {
                    g_sbeCmdRespHdr.setStatus(SBE_PRI_INVALID_COMMAND, l_rc);
                }

                // Reassign l_rc to Success to Unblock command processor
                // thread and let that take the necessary action.
                l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                break;
            }

            // Need to return from receiver thread itself for fenced rejection
            // of command, but there might be contention on the response sent
            // over FIFO/Mailbox usage.
            if(false == sbeIsCmdAllowedAtState(l_cmdClass, l_command))
            {
                // This command is not allowed in this state
                SBE_ERROR("Chip-Op CmdClass[0x%02X] Cmd[0x%02X] not allowed in "
                    "State - [0x%04X] ",l_cmdClass,l_command,
                    SbeRegAccess::theSbeRegAccess().getSbeState());

                if ( SBE_INTERFACE_PSU == curInterface )
                {
                    g_sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INVALID_COMMAND,
                                SBE_SEC_COMMAND_NOT_ALLOWED_IN_THIS_STATE);
                }
                else if ( SBE_INTERFACE_FIFO == curInterface )
                {
                    g_sbeCmdRespHdr.setStatus(SBE_PRI_INVALID_COMMAND,
                                SBE_SEC_COMMAND_NOT_ALLOWED_IN_THIS_STATE);
                }

                l_rc = SBE_SEC_COMMAND_NOT_ALLOWED_IN_THIS_STATE;
                break;
            }

        } while (false); // Inner do..while ends

        g_sbeIntrSource.setIntrSource(SBE_RX_ROUTINE, curInterface );
        // If there was a FIFO reset request,
        if (l_rc == SBE_FIFO_RESET_RECEIVED)
        {
            // Perform FIFO Reset
            uint32_t l_rc = sbeUpFifoPerformReset();
            if (l_rc)
            {
                // Perform FIFO Reset failed
                SBE_ERROR(SBE_FUNC"Perform FIFO Reset failed, "
                                "l_rc=[0x%08X]", l_rc);
                // Collect FFDC?
            }

            if ( g_sbeIntrSource.isSet(SBE_RX_ROUTINE, SBE_INTERFACE_FIFO) )
            {
                g_sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,
                                                 SBE_INTERFACE_FIFO);
            }

            if ( g_sbeIntrSource.isSet(SBE_INTERRUPT_ROUTINE,
                                        SBE_INTERFACE_FIFO_RESET) )
            {
                g_sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,
                                                 SBE_INTERFACE_FIFO_RESET);
            }

            pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
            pk_irq_enable(SBE_IRQ_SBEFIFO_RESET);
            continue;
        } // FIFO reset handling ends

        // Unblock the command processor thread
        // if we could dequeue the header successfully,
        if ((l_rcPk == PK_OK) && (l_rc == SBE_SEC_OPERATION_SUCCESSFUL))
        {
            l_rcPk = pk_semaphore_post(&g_sbeSemCmdProcess);
        }

        // Handle Cmd not in a valid state here

        if ((l_rcPk != PK_OK) || (l_rc != SBE_SEC_OPERATION_SUCCESSFUL))
        {
            if(l_rc != SBE_SEC_COMMAND_NOT_ALLOWED_IN_THIS_STATE)
            {
                // It's likely a code bug or PK failure,
                // or any other PSU/FIFO access (scom) failure.

                // Add Error trace, collect FFDC and
                // continue wait for the next interrupt
                SBE_ERROR(SBE_FUNC"Unexpected failure, "
                    "l_rcPk=[%d], g_sbeSemCmdProcess.count=[%d], l_rc=[%d]",
                    l_rcPk, g_sbeSemCmdProcess.count, l_rc);
                pk_halt();
            }
            if ( SBE_INTERFACE_PSU == curInterface )
            {
                sbeHandlePsuResponse(l_rc);
                g_sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,
                                                 SBE_INTERFACE_PSU);
                pk_irq_enable(SBE_IRQ_HOST_PSU_INTR);
            }
            else if ( SBE_INTERFACE_FIFO == curInterface )
            {
                sbeHandleFifoResponse(l_rc);
                g_sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,
                                                 SBE_INTERFACE_FIFO);
                pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
            }
            continue;
        }

        SBE_DEBUG(SBE_FUNC"Posted g_sbeSemCmdProcess, "
               "g_sbeSemCmdProcess.count=[%d]", g_sbeSemCmdProcess.count);

    } while (true); // thread always exists
    #undef SBE_FUNC
}
