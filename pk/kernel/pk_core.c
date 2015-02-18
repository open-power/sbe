//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_core.c
/// \brief Core routines for the PK kernel.
///
///  The entry points in this file are routines that are expected to be needed
///  at runtime by all PK applications.  This file also serves as a place for
///  kernel global variables to be realized.

#define __PK_CORE_C__

#include "pk.h"

#if !PK_TIMER_SUPPORT

/// If there is no timer support, then any call of the timer interrupt handler
/// is considered a fatal error.

void
__pk_timer_handler()
{
    PK_PANIC(PK_NO_TIMER_SUPPORT);
}

#endif  /* PK_TIMER_SUPPORT */


/// Initialize an PkDeque sentinel node
///
/// \param deque The sentinel node of the deque
///
/// PK has no way of knowing whether the \a deque is currently in use, so
/// this API must only be called on unitialized or otherwise unused sentinel
/// nodes.
///
/// \retval 0 success
///
/// \retval -PK_INVALID_DEQUE_SENTINEL The \a deque pointer was null

int
pk_deque_sentinel_create(PkDeque *deque)
{
    if (PK_ERROR_CHECK_API) {
        PK_ERROR_IF(deque == 0, PK_INVALID_DEQUE_SENTINEL);
    }

    deque->next = deque->previous = deque;
    return 0;
}


/// Initialize an PkDeque element
///
/// \param element Typically the PkDeque object of a queable structure
///
/// PK has no way of knowing whether the \a element is currently in use, so
/// this API must only be called on unitialized or otherwise unused deque
/// elements. 
///
/// \retval 0 success
///
/// \retval -PK_INVALID_DEQUE_ELEMENT The \a element pointer was null

int
pk_deque_element_create(PkDeque *element)
{
    if (PK_ERROR_CHECK_API) {
        PK_ERROR_IF(element == 0, PK_INVALID_DEQUE_ELEMENT);
    }

    element->next = 0;
    return 0;
}

#undef __PK_CORE_C__
