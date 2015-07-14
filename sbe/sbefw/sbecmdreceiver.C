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

sbeCmdReqBuf_t g_sbeCmdHdr;
sbeCmdRespHdr_t g_sbeCmdRespHdr;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void sbeCommandReceiver_routine(void *i_pArg)
{
    #define SBE_FUNC " sbeCommandReceiver_routine "
    SBE_ENTER(SBE_FUNC);

    do
    {
        uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

        // @TODO via RTC: 128944
        //       Read Scratchpad

        // Wait for new data in FIFO or FIFO reset interrupt
        int l_rcPk = pk_semaphore_pend (&g_sbeSemCmdRecv, PK_WAIT_FOREVER);

        // @TODO via RTC: 128658
        //       Review if Mutex protection is required
        //       for all the globals used between threads
        g_sbeCmdRespHdr.prim_status = SBE_PRI_OPERATION_SUCCESSFUL;
        g_sbeCmdRespHdr.sec_status  = SBE_SEC_OPERATION_SUCCESSFUL;
        g_sbeCmdHdr.cmdClass = SBE_CMD_CLASS_UNKNOWN;

        // inner loop for command handling
        do
        {
            // pk API failure
            if (l_rcPk != PK_OK)
            {
                break;
            }

            SBE_DEBUG(SBE_FUNC"unblocked");

            // @TODO via RTC: 128943
            //       Host services / OPAL handling

            // @TODO via RTC: 128945
            // Handle protocol violation if needed (a long term goal)

            // The responsibility of this thread is limited to dequeueing
            // only the first two word entries from the protocol header.

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

            uint32_t len = sizeof( g_sbeCmdHdr)/ sizeof(uint32_t);
            l_rc = sbeUpFifoDeq_mult ( len, (uint32_t *)&g_sbeCmdHdr, false );

            // If FIFO reset is requested,
            if (l_rc == SBE_FIFO_RESET_RECEIVED)
            {
                SBE_ERROR(SBE_FUNC"FIFO reset received");
                g_sbeCmdRespHdr.prim_status =
                    (uint16_t)l_rc;
                g_sbeCmdRespHdr.sec_status  =
                    (uint16_t)l_rc;

                // Reassign l_rc to Success to Unblock command processor
                // thread and let that take the necessary action.
                l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
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

            // Any other FIFO access issue
            if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC"sbeUpFifoDeq_mult failue, "
                          "l_rc=[0x%08X]", l_rc);
                break;
            }

            // validate the command class and sub-class opcodes
            l_rc = sbeValidateCmdClass (
                        g_sbeCmdHdr.cmdClass,
                        g_sbeCmdHdr.command ) ;

            if (l_rc)
            {
                // Command Validation failed;
                SBE_ERROR(SBE_FUNC"Command validation failed");
                g_sbeCmdRespHdr.prim_status = SBE_PRI_INVALID_COMMAND;
                g_sbeCmdRespHdr.sec_status  = l_rc;

                // Reassign l_rc to Success to Unblock command processor
                // thread and let that take the necessary action.
                l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                break;
            }

            // @TODO via RTC: 126146
            //       validate state machine constraints

        } while (false); // Inner do..while ends

        // Unblock the command processor thread
        // if we could dequeue the header successfully
        if ((l_rcPk == PK_OK) && (l_rc == SBE_SEC_OPERATION_SUCCESSFUL))
        {
            l_rcPk = pk_semaphore_post(&g_sbeSemCmdProcess);
        }

        if ((l_rcPk != PK_OK) || (l_rc != SBE_SEC_OPERATION_SUCCESSFUL))
        {
            // It's likely a code bug or PK failure,
            // or any other FIFO access failure.

            // @TODO via RTC : 129166
            //       Review if we need to add ASSERT here

            // Add Error trace, collect FFDC and
            // continue wait for the next interrupt
            SBE_ERROR(SBE_FUNC"Unexpected failure, "
                "l_rcPk=[%d], g_sbeSemCmdProcess.count=[%d], l_rc=[%d]",
                l_rcPk, g_sbeSemCmdProcess.count, l_rc);

            pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);

            continue;
        }

        SBE_DEBUG(SBE_FUNC"Posted g_sbeSemCmdProcess, "
               "g_sbeSemCmdProcess.count=[%d]", g_sbeSemCmdProcess.count);

    } while (true); // thread always exists
    #undef SBE_FUNC
}
