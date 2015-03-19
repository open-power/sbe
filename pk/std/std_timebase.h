#ifndef __STD_TIMEBASE_H__
#define __STD_TIMEBASE_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file std_timebase.h
/// \brief support for using the standard PPE 32 bit timebase register
///
/// Each standard PPE has it's own timebase register that runs at a constant
/// frequency. 

#include "pk.h"

#ifndef __ASSEMBLER__

#ifndef APPCFG_USE_EXT_TIMEBASE
static inline
uint32_t pk_timebase32_get(void)
{
    return (uint32_t)((in64(STD_LCL_TBR)) >> 32);
}

#else
//assembly function is defined in ppe42_timebase.S
uint32_t pk_timebase32_get(void);

#endif  /* APPCFG_USE_EXT_TIMEBASE */

#else

    .macro _pk_timebase32_get rT, rA
        lis \rA, STD_LCL_TBR@ha
        lvd \rT, STD_LCL_TBR@l(\rA)
    .endm

#endif  /* __ASSEMBLER__ */

#endif /* __STD_TIMEBASE_H__ */
