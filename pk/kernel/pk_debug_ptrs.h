#ifndef __PK_DEBUG_PTRS_H__
#define __PK_DEBUG_PTRS_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_debug_ptrs.h
/// \brief Structure for a table of pointers to kernel debug data
///

#define PK_DEBUG_PTRS_VERSION  1

typedef struct
{
    // The size and version of this structure
    unsigned short   debug_ptrs_size;
    unsigned short   debug_ptrs_version;

    // Trace buffer location and size
    void*           debug_trace_ptr;
    unsigned long   debug_trace_size;

    // Thread table location and size
    void*           debug_thread_table_ptr;
    unsigned long   debug_thread_table_size;

    // Thread run queue location and size
    void*           debug_thread_runq_ptr;
    unsigned long   debug_thread_runq_size;

    // Emulated timebase location and size
    void*           debug_timebase_ptr;
    unsigned long   debug_timebase_size;

} pk_debug_ptrs_t;

#endif /*__PK_DEBUG_PTRS_H__*/
