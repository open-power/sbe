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
    SBE_NONCRITICAL_STACK_SIZE           = 256,
    SBE_THREAD_ASYNC_CMD_PROC_STACK_SIZE = 256,
    SBE_THREAD_CMD_RECV_STACK_SIZE       = 512,
    SBE_THREAD_SYNC_CMD_PROC_STACK_SIZE  = 2048,
};

/**
 * @brief Global semaphore : g_sbeSemCmdRecv
 *
 *   This is used to synchronize between the ISR and
 *   the command receiver thread.
 *
 */
extern PkSemaphore g_sbeSemCmdRecv;

/**
 * @brief Global semaphore : g_sbeSemCmdProcess
 *
 *   This is used to synchronize between command receiver thread
 *   and synchronous command processor thread.
 *
 */
extern PkSemaphore g_sbeSemCmdProcess;


/**
 *  @brief SBE Interface source
 *
 */
typedef enum
{
    SBE_INTERFACE_UNKNOWN = 0x00,
    SBE_INTERFACE_FIFO    = 0x01,
    SBE_INTERFACE_PSU     = 0x02,
    SBE_INTERFACE_FIFO_RESET    = 0x04,
} sbeInterfaceSrc_t;

/**
 *  @brief structure for SBE external Interrupt handling
 *
 */
typedef struct
{
    uint8_t l_intrSource;

    void setIntrSource(const sbeInterfaceSrc_t i_val)
    {
        l_intrSource |= i_val;
    }

    void clearIntrSource(const sbeInterfaceSrc_t i_val)
    {
        l_intrSource &= ~i_val;
    }

    bool isSet (const sbeInterfaceSrc_t i_val)
    {
        return (l_intrSource & i_val);
    }
} sbeIntrHandle_t;
extern sbeIntrHandle_t g_sbeIntrSource ;


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
