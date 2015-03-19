#ifndef __PPE_TIMEBASE_H__
#define __PPE_TIMEBASE_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe_timebase.h
/// \brief support for using the OCB 32 bit timebase register
///

#include "pk.h"

//The timebase register will never be supported in the base ppe model
#ifdef APPCFG_USE_EXT_TIMEBASE
#error "External timebase is not supported on the PPE standalone model"
#endif /* APPCFG_USE_EXT_TIMEBASE */

#ifndef __ASSEMBLER__

//assembly function is defined in ppe42_timebase.S
uint32_t pk_timebase32_get(void);

#endif  /* __ASSEMBLER__ */

#endif /* __PPE_TIMEBASE_H__ */
