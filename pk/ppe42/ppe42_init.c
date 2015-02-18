//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe42_init.c
/// \brief PPE42 initialization routines
///
/// The entry points in this file are routines that are typically used during
/// initialization, and their code space could be deallocated and recovered if
/// no longer needed by the application after initialization.

#include "pk.h"
#include "pk_trace.h"

// Note that __ppe42_system_setup() is called from the PK bootloader early
// in the initialization, at a point before the aplication has enabled
// critical or external interruts.

// This function is expected to be defined by the macro specific code (GPE, CME, SBE)
void __hwmacro_setup(void);


void
__ppe42_system_setup()
{
    //Only do this if the application hasn't provided a static table definition
#ifndef STATIC_IRQ_TABLE
    PkIrqId irq;

    // Initialize the interrupt vectors.
    for (irq = 0; irq < EXTERNAL_IRQS; irq++) {
        __ppe42_irq_handlers[irq].handler = __ppe42_default_irq_handler;      
    }

    //NOTE: EXTERNAL_IRQS is the phantom interrupt assigned irq
    __ppe42_irq_handlers[irq].handler = __ppe42_phantom_irq_handler;

    // Initialize special interrupt handlers
/*
    __ppe42_fit_routine = __ppe42_default_irq_handler;
    __ppe42_fit_arg = 0;

    __ppe42_watchdog_routine = __ppe42_default_irq_handler;
    __ppe42_watchdog_arg = 0;

    __ppe42_debug_routine = __ppe42_default_irq_handler;
    __ppe42_debug_arg = 0;
*/
#endif /*STATIC_IRQ_TABLE*/

    // Set the DEC to decrement on every cycle and enable the DEC interrupt. Clear the status
    // of all timers for good measure.

    //andc_spr(SPRN_TCR, TCR_DS);
    //or_spr(SPRN_TCR, TCR_DIE);

    //Use dec_timer signal for decrementer
    or_spr(SPRN_TCR, TCR_DIE | TCR_DS);
    or_spr(SPRN_TSR, TSR_ENW | TSR_WIS | TSR_DIS | TSR_FIS);

#if PK_TIMER_SUPPORT
#if PK_TRACE_SUPPORT
extern PkTraceBuffer g_pk_trace_buf;
    //set the ppe instance id
    g_pk_trace_buf.instance_id = (uint16_t)(mfspr(SPRN_PIR) & PIR_PPE_INSTANCE_MASK);
#endif  /* PK_TRACE_SUPPORT */
#endif  /* PK_TIMER_SUPPORT */

    //call macro-specific setup
    __hwmacro_setup();
}


