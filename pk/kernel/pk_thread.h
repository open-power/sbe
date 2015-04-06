#ifndef __PK_THREAD_H__
#define __PK_THREAD_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_thread.h
/// \brief Contains private declarations and definitions needed for threads
///

void
__pk_thread_map(PkThread* thread);

void
__pk_thread_unmap(PkThread *thread);


// Interrupts must be disabled at entry.

static inline int
__pk_thread_is_active(PkThread *thread)
{
    return ((thread->state != PK_THREAD_STATE_COMPLETED) &&
            (thread->state != PK_THREAD_STATE_DELETED));
}


// Interrupts must be disabled at entry.

static inline int
__pk_thread_is_mapped(PkThread *thread)
{
    return (thread->state == PK_THREAD_STATE_MAPPED);
}


// Interrupts must be disabled at entry. This is only called on mapped threads.

static inline int
__pk_thread_is_runnable(PkThread *thread)
{
    return __pk_thread_queue_member(&__pk_run_queue, thread->priority);
}


// Interrupts must be disabled at entry.

static inline PkThread*
__pk_thread_at_priority(PkThreadPriority priority)
{
    return (PkThread*)__pk_priority_map[priority];
}

#endif /* __PK_THREAD_H__ */
