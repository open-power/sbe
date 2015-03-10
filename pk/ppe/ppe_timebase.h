#ifndef __PPE_TIMEBASE_H__
#define __PPE_TIMEBASE_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file gpe_timebase.h
/// \brief support for using the OCB 32 bit timebase register
///
/// The OCB timebase register is a 32 bit count-up register that is shared
/// by all GPE's in the OCC complex.

#include "pk.h"

//The timebase register will never be supported in the base ppe model
#define APPCFG_USE_DEC_FOR_TIMEBASE

#ifndef __ASSEMBLER__

//assembly function is defined in ppe42_timebase.S
uint32_t pk_timebase32_get(void);

#endif  /* __ASSEMBLER__ */

#endif /* __PPE_TIMEBASE_H__ */
