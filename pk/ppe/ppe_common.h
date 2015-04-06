#ifndef __PPE_COMMON_H__
#define __PPE_COMMON_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe_common.h
/// \brief Common header for PPE
///
/// This header is maintained as part of the PK port for PPE, but needs to be
/// physically present in the PMX area to allow dropping PMX code as a whole
/// to other teams.

// -*- WARNING: This file is maintained as part of PK.  Do not edit in -*-
// -*- the PMX area as your edits will be lost.                         -*-

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif

/// This constant is used to define the size of the table of interrupt handler
/// structures as well as a limit for error checking. 
/// NOTE: This can be specific to each PPE type (SBE, PPE, GPE)
#define EXTERNAL_IRQS 64

#ifdef __ASSEMBLER__
/// This macro contains PPE specific code.
/// Since standalone models of the PPE do not support external interrupts
/// we just set the code to 64 (phantom interrupt)
    .macro hwmacro_get_ext_irq
        
        li  %r4, 64

    .endm

/// Redirect the .hwmacro_irq_cfg_bitmaps macro to call our ppe specific implementation
/// This is called from the ppe42_exceptions.S file.
/// NOTE: The standalone version of PPE doesn't support external interrupts so this
///       does nothing.
    .macro .hwmacro_irq_cfg_bitmaps
    .endm


#endif /* __ASSEMBLER__ */

#endif  /* __PPE_COMMON_H__ */
