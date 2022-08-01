/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeexeintf.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2022                        */
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
 * @file: ppe/sbe/sbefw/sbeexeintf.H
 *
 * @brief This file contains the SBE control loop firmware details like
 *        - Thread priority enums
 *        - Thread stack size and space enums
 *        - Thread sub-rountine declarations
 *        - IRQ setup and ISR declarations
 *        - Other Common declaration among all the threads
 */

#ifndef __SBEFW_SBE_H
#define __SBEFW_SBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk.h"

#ifdef __cplusplus
}
#endif
/**
 * @brief enums for priorities for thread creation
 *
 */
typedef enum
{
    THREAD_PRIORITY_MAX_0,
    THREAD_PRIORITY_1,
    THREAD_PRIORITY_2,
    THREAD_PRIORITY_3,
    THREAD_PRIORITY_4,
    THREAD_PRIORITY_5,
    THREAD_PRIORITY_6,
    THREAD_PRIORITY_7,
    THREAD_PRIORITY_8,
    THREAD_PRIORITY_MIN_30 = 30,
} sbeThreadPriorities ;

/**
 * @brief enums for thread stack sizes
 *  - Non-Critical Stack used by non-critical interrupt handlers
 *  - Critical Stack used for critical interrupts
 *  - Stacks for each thread
 *
 * @TODO via RTC : 128657
 *  - Measure the actual thread stack utilization
 *  - This will be a continuous activity
 */
enum sbeThreadStackSize
{
    SBE_NONCRITICAL_STACK_SIZE           = 512,
    SBE_THREAD_ASYNC_CMD_PROC_STACK_SIZE = 256,
    SBE_THREAD_CMD_RECV_STACK_SIZE       = 512,
    SBE_THREAD_SYNC_CMD_PROC_STACK_SIZE  = 12288, //12K of Stack to handle large rings
};

/**
 *  @brief SBE Interface source
 *
 */
typedef enum
{
    SBE_INTERFACE_UNKNOWN        = 0x00,
    SBE_INTERFACE_FIFO           = 0x01,
    SBE_INTERFACE_PSU            = 0x02,
    SBE_INTERFACE_FIFO_RESET     = 0x04,
#ifdef _S0_
    SBE_INTERFACE_S0             = 0x08,
#endif
    SBE_INTERFACE_SBEHFIFO_RESET = 0x10,
    SBE_INTERFACE_SBEHFIFO       = 0x20,
    SBE_INTERFACE_S1             = 0x40,
} sbeInterfaceSrc_t;

/*
 * @brief Enum for Handler, handling the interrupt and setting/clearing the
 * interrupt variable
 */
typedef enum
{
    SBE_ALL_HANDLER        = 0x0,
    SBE_INTERRUPT_ROUTINE  = 0x1,
    SBE_RX_ROUTINE         = 0x2,
    SBE_PROC_ROUTINE       = 0x3,
} sbeHandler_t;

/**
 *  @brief structure for SBE external Interrupt handling
 *
 */
typedef struct
{
    uint8_t intrSource;
    uint8_t rxThrIntrSource;
    uint8_t procThrIntrSource;

    void setIntrSource(const sbeHandler_t i_handler,
                       const sbeInterfaceSrc_t i_val)
    {
        PkMachineContext  ctx;
        pk_critical_section_enter(&ctx);
        switch(i_handler)
        {
            case SBE_INTERRUPT_ROUTINE: intrSource |= i_val; break;
            case SBE_RX_ROUTINE: rxThrIntrSource |= i_val; break;
            case SBE_PROC_ROUTINE: procThrIntrSource |= i_val; break;
            case SBE_ALL_HANDLER: break;
        }
        pk_critical_section_exit(&ctx);
    }

    void clearIntrSource(const sbeHandler_t i_handler,
                         const sbeInterfaceSrc_t i_val)
    {
        PkMachineContext  ctx;
        pk_critical_section_enter(&ctx);
        switch(i_handler)
        {
            case SBE_INTERRUPT_ROUTINE: intrSource &= ~i_val; break;
            case SBE_RX_ROUTINE: rxThrIntrSource &= ~i_val; break;
            case SBE_PROC_ROUTINE: procThrIntrSource &= ~i_val; break;
            case SBE_ALL_HANDLER:
            {
                intrSource &= ~i_val;
                rxThrIntrSource &= ~i_val;
                procThrIntrSource &= ~i_val;
                break;
            }
        }
        pk_critical_section_exit(&ctx);
    }

    bool isSet (const sbeHandler_t i_handler, const sbeInterfaceSrc_t i_val)
    {
        bool l_ret = false;
        switch(i_handler)
        {
            case SBE_INTERRUPT_ROUTINE: l_ret = (intrSource & i_val); break;
            case SBE_RX_ROUTINE: l_ret = (rxThrIntrSource & i_val); break;
            case SBE_PROC_ROUTINE: l_ret = (procThrIntrSource & i_val); break;
            case SBE_ALL_HANDLER: break;
        }
        return l_ret;
    }
} sbeIntrHandle_t;

/**
 * @TODO via RTC : 128658
 *   Mutex protect the critical data
 *   e.g., add Mutex g_sbeMutCmdReqBuf etc.
 */

/**
 * @brief sbeCommandReceiver_routine
 *    The major responsibilities of this thread are :
 *        - Determine the reason for the interrupt
 *             - FIFO New data
 *             - FIFO reset
 *             - Host services
 *        - Dequeue the mandatory 2 entry header from upstream FIFO
 *        - Command input data validation
 *        - SBE State and pre-requirements validation
 *        - FFDC collection and FIFO flush upon validation failure
 *        - Unblock SBE command processor thread
 *        - Perform FIFO reset upon request from SP
 *
 *  @param[in]  i_pArg - Any buffer needed to be passed to the thread routine
 */
void sbeCommandReceiver_routine(void *i_pArg);

/**
 * @brief sbeSyncCommandProcessor_routine
 *    The major responsibilities of this thread are :
 *        - Dequeue data payload from upstream FIFO
 *        - Un-marshalling of the command request data
 *        - Blacklist validation
 *        - FFDC collection upon validation failure
 *        - Invoke the corresponding Hardware access utility
 *          or the HWP corresponding to the chipOp request
 *        - FFDC collection and FIFO flush upon hardware access / HWP failure
 *        - Build the response buffer with the data and the header
 *        - Enqueue the response into the Downstream FIFO
 *        - Un-mask the new data available interrupt
 *
 *  @param[in]  i_pArg - Any buffer needed to be passed to the thread routine
 */
void sbeSyncCommandProcessor_routine(void *i_pArg);

/**
 * @brief sbeAsyncCommandProcessor_routine
 *        @TODO RTC via : 130392
 *               Add infrastructure for host interface
 *
 *  @param[in]  i_pArg - Any buffer needed to be passed to the thread routine
 */
void sbeAsyncCommandProcessor_routine(void *i_pArg);


/* @brief  ISR for all SBE External Interrupts
 *         - FIFO : New data available
 *         - FIFO : Reset Request
 *         - PSU  : New data available
 *
 * @param[in/out]  i_pArg - Any buffer needed to be passed to the handler
 * @param[in]      i_irq  - IRQ number as defined in the SBE PPE spec
 */
void sbe_interrupt_handler(void* i_pArg, PkIrqId i_irq);


/* brief : Register SBE interrupt handlers and enable the IRQs
 *
 * @return int   PK_OK - Success (IRQ Setup was successful)
 *               PK_INVALID_ARGUMENT_IRQ_HANDLER - Invalid argument passed
 *                                                 (Code bug)
 *
 */
int sbeIRQSetup (void);


#endif /* __SBEFW_SBE_H  */
