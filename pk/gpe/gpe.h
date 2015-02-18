#ifndef __GPE_H__
#define __GPE_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pgp.h
/// \brief The GPE environment for PK.

// This is a 'circular' reference in PK, but included here to simplify PGAS
// programming. 

#ifndef HWMACRO_GPE
#define HWMACRO_GPE
#include "ppe42.h"
#endif  

#include "ocb_register_addresses.h"
#include "gpe_common.h"

/*
#include "pcbs_register_addresses.h"
#include "pcbs_firmware_registers.h"

#include "tod_register_addresses.h"
#include "tod_firmware_registers.h"

#include "plb_arbiter_register_addresses.h"
#include "plb_arbiter_firmware_registers.h"

*/

#endif  /* __GPE_H__ */
