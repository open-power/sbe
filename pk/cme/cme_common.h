#ifndef __CME_COMMON_H__
#define __CME_COMMON_H__

//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file cme_common.h
/// \brief Common header for CME
///
/// This header is maintained as part of the PK port for CME, but needs to be
/// physically present in the PMX area to allow dropping PMX code as a whole
/// to other teams.

// -*- WARNING: This file is maintained as part of PK.  Do not edit in -*-
// -*- the PMX area as your edits will be lost.                         -*-

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif

/// This constant is used to define the size of the table of interrupt handler
/// structures as well as a limit for error checking. 
/// NOTE: This can be specific to each PPE type (SBE, CME, GPE)
#define EXTERNAL_IRQS 64

#ifdef __ASSEMBLER__
/// This macro contains CME specific code for determining what IRQ caused the
/// external exception handler to be invoked by the PPE

/// Load noncritical status 0 and the handler array base address.  Check
/// for interrupts pending in status register 0 while the IRQ is
/// computed.  The IRQ is expected to be stored in r3.
    .macro hwmacro_get_ext_irq
        
#_lwzi       %r4, %r4, OCB_ONISR0
        cntlzw      %r3, %r4
        cmpwible    %r3, 31, external_irq_found   #branch if irq is lt or eq to 31
        
        ## No IRQ pending in interrupt set 0.  Try set 1.
        
#_lwzi   %r4, %r4, OCB_ONISR1
        cntlzw  %r3, %r4
        addi    %r3, %r3, 32

    .endm

/// Redirect the .hwmacro_irq_cfg_bitmaps macro to call our cme specific implementation
/// This is called from the ppe42_exceptions.S file.
    .macro .hwmacro_irq_cfg_bitmaps
    .endm

#endif /* __ASSEMBLER__ */

#endif  /* __CME_COMMON_H__ */
