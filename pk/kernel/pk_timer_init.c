//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_timer_init.c
/// \brief PK timer initialization
///
/// The entry points in this file might only be used during initialization of
/// the application.  In this case the code space for these routines could be
/// recovered and reused after initialization.

#include "pk.h"

// Implementation of timer creation

static int
_pk_timer_create(PkTimer         *timer,
                  PkTimerCallback callback,
                  void             *arg,
                  int              options)
{
    if (PK_ERROR_CHECK_API) {
        PK_ERROR_IF((timer == 0), PK_INVALID_TIMER_AT_CREATE);
    }

    pk_deque_element_create((PkDeque*)timer);
    timer->timeout = 0;
    timer->period = 0;
    timer->callback = callback;
    timer->arg = arg;
    timer->options = options;

    return PK_OK;
}


/// Create (initialize) a preemptible timer.
///
/// \param timer The PkTimer to initialize.
///
/// \param callback The timer callback
///
/// \param arg Private data provided to the callback.
///
/// Once created with pk_timer_create() a timer can be scheduled with
/// pk_timer_schedule() or pk_timer_schedule_absolute(), which queues the
/// timer in the kernel time queue.  Timers can be cancelled by a call of
/// pk_timer_cancel(). 
///
/// Timers created with pk_timer_create() are always run as noncritical
/// interrupt handlers with interrupt preemption enabled. Timer callbacks are
/// free to enter critical sections of any priorioty if required, but must
/// always exit with noncritical interrupts enabled.
///
/// Caution: PK has no way to know if an PkTimer structure provided to
/// pk_timer_create() is safe to use as a timer, and will silently modify
/// whatever memory is provided.
///
/// Return values other then PK_OK (0) are errors; see \ref pk_errors
///
/// \retval 0 Successful completion
///
/// \retval -PK_INVALID_TIMER_AT_CREATE The \a timer is a null (0) pointer.

int 
pk_timer_create(PkTimer         *timer,
                 PkTimerCallback callback,
                 void             *arg)
{
    return _pk_timer_create(timer, callback, arg, 
                             PK_TIMER_CALLBACK_PREEMPTIBLE);
}


/// Create (initialize) a nonpreemptible timer.
///
/// \param timer The PkTimer to initialize.
///
/// \param callback The timer callback
///
/// \param arg Private data provided to the callback.
///
/// Once created with pk_timer_create_preemptible() a timer can be scheduled
/// with pk_timer_schedule() or pk_timer_schedule_absolute(), which queues
/// the timer in the kernel time queue.  Timers can be cancelled by a call of
/// pk_timer_cancel().
///
/// Timers created with pk_timer_create_nonpreemptible() are always run as
/// noncritical interrupt handlers with interrupt preemption disabled. Timer
/// callbacks are free to later enable preemption if desired, but must always
/// exit with noncritical interrupts disabled.
///
/// \note The use of pk_timer_create_nonpreemptible() should be rare, and the
/// timer callbacks should be short and sweet to avoid long interrupt
/// latencies for other interrupts. This API was initially introduced for use
/// by the PK kernel itself when scheduling thread-timer callbacks to avoid
/// potential race conditions with other interrupts that may modify thread
/// state or the state of the time queue. Applications may also require this
/// facility to guarantee a consistent state in the event that other
/// interrupts may cancel the timer.
///
/// Caution: PK has no way to know if an PkTimer structure provided to
/// pk_timer_create() is safe to use as a timer, and will silently modify
/// whatever memory is provided.
///
/// Return values other then PK_OK (0) are errors; see \ref pk_errors
///
/// \retval 0 Successful completion
///
/// \retval -PK_INVALID_TIMER_AT_CREATE The \a timer is a null (0) pointer.

int 
pk_timer_create_nonpreemptible(PkTimer         *timer,
                                PkTimerCallback callback,
                                void             *arg)
{
    return _pk_timer_create(timer, callback, arg, 0);
}


