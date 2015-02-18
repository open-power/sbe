#ifndef __CME_H__
#define __CME_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pgp.h
/// \brief The CME environment for PK.

// This is a 'circular' reference in PK, but included here to simplify PGAS
// programming. 

#ifndef HWMACRO_CME
#define HWMACRO_CME
#include "ppe42.h"
#endif  

#include "cme_common.h"

#endif  /* __CME_H__ */
