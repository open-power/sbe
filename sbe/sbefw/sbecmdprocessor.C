/*
 * @file: ppe/sbe/sbefw/sbecmdprocessor.C
 *
 * @brief This file contains the SBE Command processing Thread Routines
 *
 */


#include "sbeexeintf.H"
#include "sbefifo.H"
#include "sbecmdparser.H"
#include "sbeirq.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "assert.h"
#include "sbeFifoMsgUtils.H"
#include "sbeerrorcodes.H"

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void sbeSyncCommandProcessor_routine(void *i_pArg)
{
    #define SBE_FUNC " sbeSyncCommandProcessor_routine "
    SBE_ENTER(SBE_FUNC);

    do
    {
        uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
        uint16_t l_primStatus = SBE_PRI_OPERATION_SUCCESSFUL;

        // Wait for new command processing
        int l_rcPk = pk_semaphore_pend (
                    &g_sbeSemCmdProcess, PK_WAIT_FOREVER);

        do
        {
            l_primStatus = g_sbeCmdRespHdr.prim_status;
            l_rc         = g_sbeCmdRespHdr.sec_status;

            SBE_DEBUG (SBE_FUNC"l_primStatus=[0x%04X], l_rc=[0x%04X]",
                            l_primStatus, l_rc);

            // PK API failure
            if (l_rcPk != PK_OK)
            {
                SBE_ERROR(SBE_FUNC"pk_semaphore_pend failed, "
                          "l_rcPk=%d, g_sbeSemCmdRecv.count=%d",
                           l_rcPk, g_sbeSemCmdRecv.count);

                // if the command receiver thread already updated
                // the response status codes, don't override them.
                if (l_primStatus == SBE_PRI_OPERATION_SUCCESSFUL)
                {
                    l_primStatus = SBE_PRI_INTERNAL_ERROR;
                    l_rc         = SBE_SEC_OS_FAILURE;
                }
            }

            SBE_DEBUG(SBE_FUNC"unblocked");

            if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                break;
            }

            SBE_DEBUG(SBE_FUNC"New cmd arrived, g_sbeSemCmdProcess.count=%d",
                         g_sbeSemCmdProcess.count);

            uint8_t  l_cmdClass  = 0;
            uint8_t  l_cmdOpCode = 0;

            // @TODO via RTC: 128658
            //       Review if Mutex protection is required
            //       for all the globals used between threads
            l_cmdClass  = g_sbeCmdHdr.cmdClass;
            l_cmdOpCode = g_sbeCmdHdr.command;

            // Get the command function
            sbeCmdFunc_t l_pFuncP = sbeFindCmdFunc (l_cmdClass, l_cmdOpCode) ;

            assert( l_pFuncP )

            // Call the ChipOp function
            l_rc = l_pFuncP ((uint8_t *)i_pArg);

        } while(false); // Inner do..while loop ends here

        SBE_DEBUG(SBE_FUNC"l_rc=[0x%08X]", l_rc);

        // Handle FIFO reset case
        if (l_rc == SBE_FIFO_RESET_RECEIVED)
        {
            // @TODO via RTC : 126147
            //       Handle FIFO reset flow
            pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
            continue;
        }

        if ( (l_rc == SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA) ||
             (l_rc == SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA) )
        {
            l_primStatus = SBE_PRI_INVALID_DATA;
        }

        uint32_t l_len2dequeue            = 0;
        uint32_t l_dist2StatusHdr         = 0;
        uint32_t l_sbeDownFifoRespBuf[4] = {0};
        uint32_t l_secStatus = l_rc;

        switch (l_rc)
        {
            case SBE_SEC_COMMAND_CLASS_NOT_SUPPORTED:
            case SBE_SEC_COMMAND_NOT_SUPPORTED:
                // Caller sent Invalid Command

            case SBE_SEC_OS_FAILURE:
                // PK API Failures

                // Flush out the upstream FIFO till EOT arrives
                l_len2dequeue = 1;
                l_rc = sbeUpFifoDeq_mult (l_len2dequeue, NULL,
                                                true, true);

                if ( (l_rc == SBE_FIFO_RESET_RECEIVED) ||
                     (l_rc == SBE_SEC_FIFO_ACCESS_FAILURE) )
                {
                    break;
                }

                if (l_rc)
                {
                    l_secStatus = l_rc;
                }

                // Don't break here to force the flow through
                // the next case to enqueue the response into
                // the downstream FIFO

            case SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA:
            case SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA:
                // EOT arrived prematurely in upstream FIFO
                // or there were unexpected data in upstream FIFO

                SBE_ERROR(SBE_FUNC"Operation failure, "
                             "l_primStatus[0x%08X], "
                             "l_secStatus[0x%08X]",
                             l_primStatus, l_secStatus);

                sbeBuildMinRespHdr(&l_sbeDownFifoRespBuf[0],
                                    l_dist2StatusHdr,
                                    l_primStatus,
                                    l_secStatus,
                                    0);
                l_rc = sbeDownFifoEnq_mult (++l_dist2StatusHdr,
                                        &l_sbeDownFifoRespBuf[0]);
                if (l_rc)
                {
                    SBE_ERROR(SBE_FUNC"sbeDownFifoEnq_mult failure,"
                        " l_rc[0x0%08X]", l_rc);
                    // not attempting to signal EOT
                    break;
                }
                // Follow through to signal EOT in downstream

            case SBE_SEC_OPERATION_SUCCESSFUL: // Successful execution
                // Signal EOT in Downstream FIFO
                l_rc = sbeDownFifoSignalEot();
                if (l_rc)
                {
                    SBE_ERROR(SBE_FUNC"sbeDownFifoSignalEot failure,"
                        " l_rc[0x0%08X]", l_rc);
                }
                SBE_INFO(SBE_FUNC"ChipOp Done");
                break;

             default:
                break;
        }

        // @TODO via RTC : 126147
        //       Review all the scenarios
        // Enable the new data available interrupt
        pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);

    } while(true); // Thread always exists
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void sbeAsyncCommandProcessor_routine(void *arg)
{
    SBE_TRACE("sbeAsyncCommandProcessor Thread started");

    do
    {
        //  @TODO RTC via : 130392
        //        Add infrastructure for host interface

    } while(0);
}
