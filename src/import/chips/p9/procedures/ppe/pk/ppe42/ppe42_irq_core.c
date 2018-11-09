/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/ppe/pk/ppe42/ppe42_irq_core.c $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

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

uint32_t G_pib_reset_flag = 0;

#ifdef __PPE__

void
__ppe42_pib_reset_handler()
{
    //PK_TRACE("Entered 1 ppe42_pib_reset_handler");
    uint32_t srr1 = mfspr(SPRN_SRR1);

    // assuming pib is being reset thus give timeout error
    if (((srr1 & MSR_SIBRC) == MSR_SIBRC))
    {
        // if already waited for pib to reset, panic as still fail
        if (G_pib_reset_flag == 10 )
        {
            G_pib_reset_flag = 0;
            PK_PANIC(PPE42_PIB_RESET_NOT_RECOVER);
        }

        // note pib reset is being detected
        // this flag will be cleared by fit timer if pib reset recovers
        G_pib_reset_flag++;

        // DELAY to wait pib reset to complete
        volatile uint32_t loop;

        for(loop = 0; loop < 6400; loop++);

        PK_TRACE_INF("PIB reset flag value %x", G_pib_reset_flag);

    }
    else
    {
        // panic for all other pib return codes
        PK_PANIC(PPE42_MACHINE_CHECK_PANIC);
    }
}
#endif


#ifndef STATIC_IRQ_TABLE
    Ppe42IrqHandler __ppe42_irq_handlers[EXTERNAL_IRQS + 1];
#endif

/// This function is installed by default for interrupts not explicitly set up
/// by the application.  These interrupts should never fire.

void
__ppe42_default_irq_handler(void* arg, PkIrqId irq)
{
    PK_PANIC(PK_DEFAULT_IRQ_HANDLER);
}


/// This function is installed by default to handle the case that the
/// interrupt dispatch code is entered in response to an external
/// interrupt, but no interrupt is found pending in the interrupt
/// controller.  This should never happen, as it would indicate that a
/// 'glitch' occurred on the external interrupt input
/// to the PPE42 core.

void __ppe42_phantom_irq_handler(void* arg, PkIrqId irq)
{
    PK_PANIC(PPE42_PHANTOM_INTERRUPT);
}


#undef __PPE42_IRQ_CORE_C__
