//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe42_irq_core.c
/// \brief Core IRQ routines required of any PPE42 configuration of PK
///
///  This file is mostly only a placeholder - where 'extern inline' API
///  functions and 'extern' variables are realized.  A couple of default
///  handlers are also installed here.  The entry points in this file are
///  considered 'core' routines that will always be present at runtime in any
///  PK application.

#define __PPE42_IRQ_CORE_C__

#include "pk.h"

#ifndef STATIC_IRQ_TABLE
Ppe42IrqHandler __ppe42_irq_handlers[EXTERNAL_IRQS + 1];
#endif

/// This function is installed by default for interrupts not explicitly set up
/// by the application.  These interrupts should never fire.

void 
__ppe42_default_irq_handler(void* arg, PkIrqId irq)
{
    PK_PANIC(PPE42_DEFAULT_IRQ_HANDLER);
}


/// This function is installed by default to handle the case that the
/// interrupt dispatch code is entered in response to an external critical or
/// non-critical interrupt, but no interrupt is found pending in the interrupt
/// controller.  This should never happen, as it would indicate that a
/// 'glitch' occurred on the external interrupt input
/// to the PPE42 core.

void __ppe42_phantom_irq_handler(void* arg, PkIrqId irq)
{
    PK_PANIC(PPE42_PHANTOM_INTERRUPT);
}
    

#undef __PPE42_IRQ_CORE_C__
