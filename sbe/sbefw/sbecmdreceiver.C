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

sbeFifoCmdReqBuf_t g_sbeFifoCmdHdr;
sbeCmdRespHdr_t g_sbeCmdRespHdr;
sbePsu2SbeCmdReqHdr_t g_sbePsu2SbeCmdReqHdr;
sbeSbe2PsuRespHdr_t g_sbeSbe2PsuRespHdr;
sbeIntrHandle_t g_sbeIntrSource ;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void sbeCommandReceiver_routine(void *i_pArg)
{
    #define SBE_FUNC " sbeCommandReceiver_routine "
    SBE_ENTER(SBE_FUNC);

    // Update SBE msgg reg to indicate that control loop
    // is ready now to receive data on its interfaces
    (void)SbeRegAccess::theSbeRegAccess().setSbeReady();

    do
    {
        uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

        // @TODO via RTC: 128944
        //       Read Scratchpad

        // Wait for new data in FIFO or FIFO reset interrupt or PSU interrupt
        int l_rcPk = pk_semaphore_pend (&g_sbeSemCmdRecv, PK_WAIT_FOREVER);

        // @TODO via RTC: 128658
        //       Review if Mutex protection is required
        //       for all the globals used between threads
        g_sbeCmdRespHdr.prim_status = SBE_PRI_OPERATION_SUCCESSFUL;
        g_sbeCmdRespHdr.sec_status  = SBE_SEC_OPERATION_SUCCESSFUL;
        g_sbeFifoCmdHdr.cmdClass = SBE_CMD_CLASS_UNKNOWN;
        g_sbePsu2SbeCmdReqHdr.init();

        // inner loop for command handling
        do
        {
            uint8_t l_cmdClass = SBE_CMD_CLASS_UNKNOWN;
            uint8_t l_command  = 0xFF;

            // pk API failure
            if (l_rcPk != PK_OK)
            {
                break;
            }

            SBE_DEBUG(SBE_FUNC"unblocked");

            // The responsibility of this thread is limited to reading off
            // the FIFO or PSU interfaces to be able to decode the command
            // class and the command opcode parameters.

            // Received FIFO Reset interrupt
            if (  g_sbeIntrSource.isSet(SBE_INTERFACE_FIFO_RESET) )
            {
                SBE_ERROR(SBE_FUNC"FIFO reset received");
                l_rc = SBE_FIFO_RESET_RECEIVED;
                break;
            }

            // Received PSU interrupt
            if ( g_sbeIntrSource.isSet(SBE_INTERFACE_PSU) )
            {
                // First clear PSU->SBE DB bit 0
                l_rc = sbeClearPsu2SbeDbBitX(SBE_PSU2SBE_DOORBELL_CLEAR_BIT0);
                if (SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
                {
                    break;
                }

                // Read the request field from PSU->SBE Mbx Reg0
                uint8_t l_count = sizeof(g_sbePsu2SbeCmdReqHdr)/
                                  sizeof(uint64_t);
                l_rc = sbeReadPsu2SbeMbxReg (SBE_HOST_PSU_MBOX_REG0,
                              l_count, (uint64_t *)&g_sbePsu2SbeCmdReqHdr);
                g_sbeSbe2PsuRespHdr.init();
                l_cmdClass  = g_sbePsu2SbeCmdReqHdr.cmdClass;
                l_command   = g_sbePsu2SbeCmdReqHdr.command;
            } // end if loop for PSU interface chipOp handling

            // Received FIFO New Data interrupt
            else if (  g_sbeIntrSource.isSet(SBE_INTERFACE_FIFO) ) {

            // This thread will attempt to unblock the command processor
            // thread on the following scenarios:
            //    - Normal scenarios where SBE would need to respond to FSP
            //      via downstream FIFO. This includes SUCCESS cases as well as
            //      the cases for Invalid Data sequence or Command validation
            //      failure.
            //    - if there is a need to handle FIFO reset

            // Accordingly, this will update g_sbeCmdRespHdr.prim_status
            // and g_sbeCmdRespHdr.sec_status for command processor thread
            // to handle them later in the sequence.

            l_cmdClass  = SBE_CMD_CLASS_UNKNOWN;
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
                g_sbeCmdRespHdr.prim_status =
                            SBE_PRI_INVALID_DATA;
                g_sbeCmdRespHdr.sec_status  = l_rc;

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
                if (  g_sbeIntrSource.isSet(SBE_INTERFACE_PSU) )
                {
                    g_sbeSbe2PsuRespHdr.primStatus = SBE_PRI_INVALID_COMMAND;
                    g_sbeSbe2PsuRespHdr.secStatus  = l_rc;
                }
                else if (  g_sbeIntrSource.isSet(SBE_INTERFACE_FIFO) )
                {
                    g_sbeCmdRespHdr.prim_status = SBE_PRI_INVALID_COMMAND;
                    g_sbeCmdRespHdr.sec_status  = l_rc;
                }

                // Reassign l_rc to Success to Unblock command processor
                // thread and let that take the necessary action.
                l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                break;
            }

            // @TODO via RTC: 126146
            //       validate state machine constraints

        } while (false); // Inner do..while ends

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

            if ( g_sbeIntrSource.isSet(SBE_INTERFACE_FIFO) )
            {
                g_sbeIntrSource.clearIntrSource(SBE_INTERFACE_FIFO);
            }

            if ( g_sbeIntrSource.isSet(SBE_INTERFACE_FIFO_RESET) )
            {
                g_sbeIntrSource.clearIntrSource(SBE_INTERFACE_FIFO_RESET);
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

        if ((l_rcPk != PK_OK) || (l_rc != SBE_SEC_OPERATION_SUCCESSFUL))
        {
            // It's likely a code bug or PK failure,
            // or any other PSU/FIFO access (scom) failure.

            // @TODO via RTC : 129166
            //       Review if we need to add ASSERT here

            // Add Error trace, collect FFDC and
            // continue wait for the next interrupt
            SBE_ERROR(SBE_FUNC"Unexpected failure, "
                "l_rcPk=[%d], g_sbeSemCmdProcess.count=[%d], l_rc=[%d]",
                l_rcPk, g_sbeSemCmdProcess.count, l_rc);

            if ( g_sbeIntrSource.isSet(SBE_INTERFACE_PSU) )
            {
                g_sbeIntrSource.clearIntrSource(SBE_INTERFACE_PSU);
                pk_irq_enable(SBE_IRQ_HOST_PSU_INTR);
            }
            else if ( g_sbeIntrSource.isSet(SBE_INTERFACE_FIFO) )
            {
                g_sbeIntrSource.clearIntrSource(SBE_INTERFACE_FIFO);
                pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
            }
            continue;
        }

        SBE_DEBUG(SBE_FUNC"Posted g_sbeSemCmdProcess, "
               "g_sbeSemCmdProcess.count=[%d]", g_sbeSemCmdProcess.count);

    } while (true); // thread always exists
    #undef SBE_FUNC
}
