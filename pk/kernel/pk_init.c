//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_init.c
/// \brief PK initialization
///
/// The entry points in this file are initialization routines - they are never
/// needed after PK initialization and their code space could be reclaimed by
/// the application after initialization if required.

#include "pk.h"
#include "pk_trace.h"

uint32_t __pk_timebase_frequency_hz;
uint32_t __pk_timebase_frequency_khz;
uint32_t __pk_timebase_frequency_mhz;

/// Initialize PK.  
///
/// \param noncritical_stack A stack area for noncritical interrupt handlers.
///
/// \param noncritical_stack_size The size (in bytes) of the stack area for
/// noncritical interrupt handlers. 
///
/// \param critical_stack A stack area for critical interrupt handlers.
///
/// \param critical_stack_size The size (in bytes) of the stack area for
/// critical interrupt handlers. 
///
/// \param initial_timebase The initial value of the PK timebase.  If this
/// argument is given as the special value \c PK_TIMEBASE_CONTINUE, then the
/// timebase is not reset.
///
/// \param timebase_frequency_hz The frequency of the PK timebase in Hz.
///
/// This routine \e must be called before any other PK / routines, and \e
/// should be called before any interrupts are enabled.
///
/// Return values other than PK_OK (0) are errors; see \ref pk_errors
///
/// \retval 0 Successful completion
///
/// \retval -PK_INVALID_ARGUMENT_INIT A stack pointer is 0 or is given 
/// a 0 size.
///
/// \retval -PK_STACK_OVERFLOW One or both stacks are not large enough to
/// support a minimum context save in the event of an interrupt.

// Note that PK does not rely on any static initialization of dynamic
// variables. In debugging sessions using RAM-resident PK images it is
// assumed that the processor may be reset at any time, so we always need to
// reset everything at initialization.

int
pk_initialize(PkAddress  noncritical_stack,
               size_t      noncritical_stack_size,
               PkTimebase initial_timebase,
               uint32_t    timebase_frequency_hz)
{
    int rc;

    if (PK_ERROR_CHECK_API) {
        PK_ERROR_IF((noncritical_stack == 0) ||
                     (noncritical_stack_size == 0),
                     PK_INVALID_ARGUMENT_INIT);
    }

    __pk_timebase_frequency_hz = timebase_frequency_hz;
    __pk_timebase_frequency_khz = timebase_frequency_hz / 1000;
    __pk_timebase_frequency_mhz = timebase_frequency_hz / 1000000;

    __pk_thread_machine_context_default = PK_THREAD_MACHINE_CONTEXT_DEFAULT;

    rc = __pk_stack_init(&noncritical_stack, &noncritical_stack_size);
    if (rc) {
        return rc;
    }

    __pk_noncritical_stack = noncritical_stack;
    __pk_noncritical_stack_size = noncritical_stack_size;

#if PK_TIMER_SUPPORT

    // Initialize the time queue sentinel as a circular queue, set the next
    // timeout and clear the cursor.

    pk_deque_sentinel_create((PkDeque*)&__pk_time_queue);
    __pk_time_queue.cursor = 0;
    __pk_time_queue.next_timeout = PK_TIMEBASE_MAX;

#if PK_TRACE_SUPPORT
extern PkTimer       g_pk_trace_timer;
extern PkTraceBuffer g_pk_trace_buf;

    // Schedule the timer that puts a 64bit timestamp in the trace buffer
    // periodically.  This allows us to use 32bit timestamps.
    pk_timer_schedule(&g_pk_trace_timer,
                      PK_TRACE_TIMER_PERIOD,
                      0);

    //set the trace timebase HZ
    g_pk_trace_buf.hz = timebase_frequency_hz;

    //set the timebase ajdustment for trace synchronization
    pk_trace_set_timebase(initial_timebase);

#endif  /* PK_TRACE_SUPPORT */

#endif  /* PK_TIMER_SUPPORT */

#if PK_THREAD_SUPPORT
    
    // Clear the priority map. The final entry [PK_THREADS] is for the idle
    // thread.

    int i;
    for (i = 0; i <= PK_THREADS; i++) {
        __pk_priority_map[i] = 0;
    }

    // Initialize the thread scheduler

    __pk_thread_queue_clear(&__pk_run_queue);
    __pk_current_thread = 0;   
    __pk_next_thread    = 0;
    __pk_delayed_switch = 0;

#endif  /* PK_THREAD_SUPPORT */

   return PK_OK;
}


/// Call the application main()
///
/// __pk_main() is called from the bootloader.  It's only purpose is to
/// provide a place for the PK_MAIN_HOOK to be called before main() is
/// called.

void
__pk_main(int argc, char **argv)
{
    PK_MAIN_HOOK;

    int main(int argc, char **argv);
    main(argc, argv);
}






        
        
        
