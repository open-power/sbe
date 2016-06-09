/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/sbefw/sbecmdprocessor.C $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
#include "sbeHostUtils.H"
#include "sbeHostMsg.H"
#include "sbecmdiplcontrol.H"
#include "sberegaccess.H"
#include "sbestates.H"


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sbeHandlePsuResponse (const uint32_t i_rc)
{
    #define SBE_FUNC " sbeHandlePsuResponse "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        uint8_t  l_cnt = 0;
        switch (i_rc)
        {
            case SBE_SEC_COMMAND_CLASS_NOT_SUPPORTED:
            case SBE_SEC_COMMAND_NOT_SUPPORTED:
                // Caller sent an invalid Command class/opcode
                // Set the Ack bit in SBE->PSU DB register
                l_rc = sbeAcknowledgeHost();
                if (SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
                {
                    break;
                }
                // Set primary and secondary status
                g_sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INVALID_COMMAND, i_rc);

                // Now Update SBE->PSU Mbx Reg4 with response
                l_cnt = sizeof(g_sbeSbe2PsuRespHdr)/
                                       sizeof(uint64_t);
                l_rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG4,
                            reinterpret_cast<const uint64_t *>(
                                &g_sbeSbe2PsuRespHdr), l_cnt, true);
                break;

            case SBE_SEC_OS_FAILURE:
                // Set primary and secondary status
                g_sbeSbe2PsuRespHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                               i_rc);

                // Now Update SBE->PSU Mbx Reg4 with response
                l_cnt = sizeof(g_sbeSbe2PsuRespHdr)/
                                       sizeof(uint64_t);
                l_rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG4,
                            reinterpret_cast<const uint64_t *>(
                                &g_sbeSbe2PsuRespHdr), l_cnt, true);
                break;

            case SBE_SEC_OPERATION_SUCCESSFUL:
                // Services code successfully executed the chipOp.
                SBE_INFO(SBE_FUNC"PSU ChipOp Done");
                break;

             default:
                // The only possibility (as of now) to reach till this point
                // is when there was a mbx register access (scom) failure
                // happened. Going to return to the waiting loop.
                break;
        }
    } while(false);

    SBE_DEBUG(SBE_FUNC"l_rc[0x0%08X]", l_rc);
    #undef SBE_FUNC
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sbeHandleFifoResponse (const uint32_t i_rc)
{
    #define SBE_FUNC " sbeHandleFifoResponse "
    SBE_ENTER(SBE_FUNC);

    do
    {
        uint16_t l_primStatus = SBE_PRI_OPERATION_SUCCESSFUL;
        uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

        // Handle FIFO reset case
        if (i_rc == SBE_FIFO_RESET_RECEIVED)
        {
            break;
        }

        if ( (i_rc == SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA) ||
             (i_rc == SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA) )
        {
            l_primStatus = SBE_PRI_INVALID_DATA;
        }

        uint32_t l_len2dequeue            = 0;
        uint32_t l_dist2StatusHdr         = 0;
        uint32_t l_sbeDownFifoRespBuf[4] = {0};
        uint32_t l_secStatus = i_rc;

        switch (i_rc)
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
    } while (false);

    #undef SBE_FUNC
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void sbeSyncCommandProcessor_routine(void *i_pArg)
{
    #define SBE_FUNC " sbeSyncCommandProcessor_routine "
    SBE_ENTER(SBE_FUNC);

    // Check the destination bit at the start
    if(SbeRegAccess::theSbeRegAccess().isDestBitRuntime())
    {
        SBE_DEBUG(SBE_FUNC"Destination bit tells us to go to runtime");
        (void)SbeRegAccess::theSbeRegAccess().
              updateSbeState(SBE_STATE_RUNTIME);
    }
    else if(SbeRegAccess::theSbeRegAccess().isIstepMode())
    {
        SBE_DEBUG(SBE_FUNC"Continuous IPL mode not set, will wait for "
                "commands...");
        (void)SbeRegAccess::theSbeRegAccess().
              updateSbeState(SBE_STATE_ISTEP);
    }
    else
    {
        sbeDoContinuousIpl();
    }

    do
    {
        uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
        uint16_t l_primStatus = SBE_PRI_OPERATION_SUCCESSFUL;

        // Wait for new command processing
        int l_rcPk = pk_semaphore_pend (
                    &g_sbeSemCmdProcess, PK_WAIT_FOREVER);

        do
        {
            // Local Variables
            uint8_t  l_cmdClass  = 0;
            uint8_t  l_cmdOpCode = 0;

            // Check on the Rx Thread Interrupt Bits for Interrupt Status
            if ( g_sbeIntrSource.isSet(SBE_RX_ROUTINE,
                                        SBE_INTERFACE_PSU) )
            {
                l_rc         = g_sbeSbe2PsuRespHdr.secStatus;
                l_cmdClass   = g_sbePsu2SbeCmdReqHdr.cmdClass;
                l_cmdOpCode  = g_sbePsu2SbeCmdReqHdr.command;
                // Set this here, so that during response handling we know which
                // interrupt we are processing, need not check for
                // g_sbeIntrSource again
                g_sbeIntrSource.setIntrSource(SBE_PROC_ROUTINE,
                                               SBE_INTERFACE_PSU);
            }
            else if ( g_sbeIntrSource.isSet(SBE_RX_ROUTINE,
                                             SBE_INTERFACE_FIFO) )
            {
                l_rc         = g_sbeCmdRespHdr.sec_status;
                l_cmdClass   = g_sbeFifoCmdHdr.cmdClass;
                l_cmdOpCode  = g_sbeFifoCmdHdr.command;
                // Set this here, so that during response handling we know which
                // interrupt we are processing, need not check for
                // g_sbeIntrSource again
                g_sbeIntrSource.setIntrSource(SBE_PROC_ROUTINE,
                                               SBE_INTERFACE_FIFO);
            }
            else // SBE_INTERFACE_FIFO_RESET or SBE_INTERFACE_UNKNOWN
            {
                SBE_ERROR(SBE_FUNC"Unexpected interrupt communicated to the "
                   "processor thread. Interrupt source: 0x%02X 0x%02X",
                   g_sbeIntrSource.intrSource, g_sbeIntrSource.rxThrIntrSource);
                assert(false);
                break;
            }

            SBE_DEBUG (SBE_FUNC"New cmd arrived, g_sbeSemCmdProcess.count=%d "
                "l_primStatus=[0x%04X], l_rc=[0x%04X]",
                g_sbeSemCmdProcess.count, l_primStatus, l_rc);

            // PK API failure
            if (l_rcPk != PK_OK)
            {
                SBE_ERROR(SBE_FUNC"pk_semaphore_pend failed, "
                          "l_rcPk=%d, g_sbeSemCmdRecv.count=%d",
                           l_rcPk, g_sbeSemCmdRecv.count);

                // If it's a semphore_pend error then update the same to show
                // internal failure
                l_rc         = SBE_SEC_OS_FAILURE;
            }

            // Check for error which Receiver thread might have set
            if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                break;
            }

            // Get the command function
            sbeCmdFunc_t l_pFuncP = sbeFindCmdFunc (l_cmdClass, l_cmdOpCode);
            assert( l_pFuncP )

            // Call the ChipOp function
            l_rc = l_pFuncP ((uint8_t *)i_pArg);

        } while(false); // Inner do..while loop ends here

        SBE_DEBUG(SBE_FUNC"l_rc=[0x%08X]", l_rc);
        if ( g_sbeIntrSource.isSet(SBE_PROC_ROUTINE, SBE_INTERFACE_PSU) )
        {
            sbeHandlePsuResponse (l_rc);

            // Enable Host interrupt
            g_sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,SBE_INTERFACE_PSU);
            pk_irq_enable(SBE_IRQ_HOST_PSU_INTR);
        }
        else if ( g_sbeIntrSource.isSet(SBE_PROC_ROUTINE, SBE_INTERFACE_FIFO) )
        {
            sbeHandleFifoResponse (l_rc);

            // Enable the new data available interrupt
            g_sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,SBE_INTERFACE_FIFO);
            pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
            pk_irq_enable(SBE_IRQ_SBEFIFO_RESET);
        }
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
