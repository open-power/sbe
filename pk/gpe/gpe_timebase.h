#ifndef __GPE_TIMEBASE_H__
#define __GPE_TIMEBASE_H__

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

//The timebase register is not yet supported in the OCB model.
//remove this line once it is supported.
#define APPCFG_USE_DEC_FOR_TIMEBASE

#ifndef __ASSEMBLER__

#ifndef APPCFG_USE_DEC_FOR_TIMEBASE
static inline
uint32_t pk_timebase32_get(void)
{
    return in32(OCB_OTBR);
}

#else
//assembly function is defined in ppe42_timebase.S
uint32_t pk_timebase32_get(void);

#endif /* APPCFG_USE_DEC_FOR_TIMEBASE */

#else

    .macro _pk_timebase32_get rT, rA
        _lwzi \rT, \rA, OCB_OTBR
    .endm
#endif  /* __ASSEMBLER__ */

#endif /* __GPE_TIMEBASE_H__ */
