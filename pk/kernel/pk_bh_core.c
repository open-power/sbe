//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_bh_core.c
/// \brief PK bottom half APIs
///
///  The entry points in this file are considered 'core' routines that will
///  always be present at runtime in any PK application.

#include "pk.h"

/// Statically initialize the bottom half queue
PK_DEQUE_SENTINEL_STATIC_CREATE(_pk_bh_queue);

void _pk_process_bh(void)
{
    PkBottomHalf *bh;
    while((bh = (PkBottomHalf*)pk_deque_pop_front(&_pk_bh_queue)) != 0)
    {
        bh->bh_handler(bh->arg);
    }
    return;
}


#undef __PK_THREAD_CORE_C__
