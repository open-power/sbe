//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file base_ppe_demo.c
/// \brief example code to include into fixed section
///
/// demo function to include into fixed section. Can be replaced as soon as 
/// real functions are available

#include "base_ppe_demo.h"
#include "pk.h"
#include "pk_trace.h"

void base_ppe_demo_func(const char* str)
{
    PK_TRACE("TESTFUNCTION");
}
