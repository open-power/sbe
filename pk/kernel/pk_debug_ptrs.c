//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_debug_ptrs.c
/// \brief Defines a table of pointers to important kernel debug data.
///
/// This table is placed in a special section named .debug_ptrs which can be
/// placed at a well-known memory location for tools to find.
///

#include "pk.h"
#include "pk_trace.h"
#include "pk_debug_ptrs.h"

extern PkTimebase  ppe42_64bit_timebase;

#if PK_TRACE_SUPPORT
extern PkTraceBuffer g_pk_trace_buf;
#endif

pk_debug_ptrs_t pk_debug_ptrs SECTION_ATTRIBUTE(".debug_ptrs") =
{
    .debug_ptrs_size            = sizeof(pk_debug_ptrs),
    .debug_ptrs_version         = PK_DEBUG_PTRS_VERSION,

#if PK_TRACE_SUPPORT
    .debug_trace_ptr            = &g_pk_trace_buf,
    .debug_trace_size           = sizeof(g_pk_trace_buf),
#else
    .debug_trace_ptr            = 0,
    .debug_trace_size           = 0,
#endif /* PK_TRACE_SUPPORT */

#if PK_THREAD_SUPPORT
    .debug_thread_table_ptr     = &__pk_priority_map,
    .debug_thread_table_size    = sizeof(__pk_priority_map),
    .debug_thread_runq_ptr      = (void*)&__pk_run_queue,
    .debug_thread_runq_size     = sizeof(__pk_run_queue),
#else
    .debug_thread_table_ptr     = 0,
    .debug_thread_table_size    = 0,
    .debug_thread_runq_ptr      = 0,
    .debug_thread_runq_size     = 0,
#endif /* PK_THREAD_SUPPORT */

    .debug_timebase_ptr         = &ppe42_64bit_timebase,
    .debug_timebase_size        = sizeof(ppe42_64bit_timebase),

};

