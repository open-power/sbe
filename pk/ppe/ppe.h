#ifndef __PPE_H__
#define __PPE_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pgp.h
/// \brief The PPE environment for PK.

// This is a 'circular' reference in PK, but included here to simplify PGAS
// programming. 

#ifndef HWMACRO_PPE
#define HWMACRO_PPE
#include "ppe42.h"
#endif  

#include "ppe_common.h"

#endif  /* __PPE_H__ */
