/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeirq.C $                                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/*
 * @file: ppe/sbe/sbefw/sbeirq.C
 *
 * @brief This sets up and registers SBE ISRs
 *
 */

#include "sbeexeintf.H"
#include "sbeirq.H"
#include "sbetrace.H"
#include "assert.h"
#include "sbeglobals.H"

////////////////////////////////////////////////////////////////
// @brief:     SBE control loop ISR:
//               - FIFO new data available
//               - FIFO reset request
//               - PSU new data available
//
// @param[in]  i_pArg - Unused
// @param[in]  i_irq  - IRQ number as defined in sbeirq.h
//
////////////////////////////////////////////////////////////////
void sbe_interrupt_handler (void *i_pArg, PkIrqId i_irq)
{
    #define SBE_FUNC " sbe_interrupt_handler "
    SBE_ENTER(SBE_FUNC"i_irq=[0x%02X]",i_irq);

    int l_rc = 0;

    switch (i_irq)
    {
        case SBE_IRQ_HOST_PSU_INTR:
            SBE_GLOBAL->sbeIntrSource.setIntrSource(SBE_INTERRUPT_ROUTINE,
                                            SBE_INTERFACE_PSU);
            break;

        case SBE_IRQ_SBEFIFO_DATA:
            SBE_GLOBAL->sbeIntrSource.setIntrSource(SBE_INTERRUPT_ROUTINE,
                                            SBE_INTERFACE_FIFO);
            pk_irq_disable(SBE_IRQ_SBEFIFO_RESET);
            break;

        case SBE_IRQ_SBEFIFO_RESET:
            SBE_GLOBAL->sbeIntrSource.setIntrSource(SBE_INTERRUPT_ROUTINE,
                                            SBE_INTERFACE_FIFO_RESET);
            pk_irq_disable(SBE_IRQ_SBEFIFO_DATA);
            break;

        default:
            SBE_ERROR(SBE_FUNC"Unknown IRQ, assert");
            assert(0);
            break;
    }
    // Mask the interrupt
    pk_irq_disable(i_irq);

    // Unblock the command receiver thread
    l_rc = pk_semaphore_post(&SBE_GLOBAL->sbeSemCmdRecv);
    if (l_rc)
    {
        // If we received an error while posting the semaphore,
        // unmask the interrupt back and assert
        SBE_ERROR(SBE_FUNC"pk_semaphore_post failed, rc=[%d]", l_rc);
        pk_irq_enable(i_irq);
        assert(!l_rc);
    }
    #undef SBE_FUNC
}

////////////////////////////////////////////////////////////////
// See sbeexeintf.h for more details
////////////////////////////////////////////////////////////////
int sbeIRQSetup (void)
{
    #define SBE_FUNC " sbeIRQSetup "
    int l_rc = 0;
    PkIrqId l_irq;

    // Disable the relevant IRQs while we set them up
    pk_irq_disable(SBE_IRQ_HOST_PSU_INTR);
    pk_irq_disable(SBE_IRQ_SBEFIFO_DATA);
    pk_irq_disable(SBE_IRQ_SBEFIFO_RESET);

    do
    {
        // Register the IRQ handler with PK

        // PSU New data available interrupt
        l_irq = SBE_IRQ_HOST_PSU_INTR;
        l_rc = pk_irq_handler_set(l_irq, sbe_interrupt_handler, NULL);
        if(l_rc)
        {
            break;
        }

        // FIFO New data available interrupt
        l_irq = SBE_IRQ_SBEFIFO_DATA;
        l_rc = pk_irq_handler_set(l_irq, sbe_interrupt_handler, NULL);
        if(l_rc)
        {
            break;
        }

        // FIFO Reset request
        l_irq = SBE_IRQ_SBEFIFO_RESET;
        l_rc = pk_irq_handler_set(l_irq, sbe_interrupt_handler, NULL);
        if(l_rc)
        {
            break;
        }

        // Enable the IRQ
        pk_irq_enable(SBE_IRQ_SBEFIFO_RESET);
        pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
        pk_irq_enable(SBE_IRQ_HOST_PSU_INTR);
    } while(false);

    if (l_rc)
    {
        SBE_ERROR (SBE_FUNC"pk_irq_handler_set failed, IRQ=[0x%02X], "
            "rc=[%d]", l_irq, l_rc);
    }

    return l_rc;
    #undef SBE_FUNC
}

////////////////////////////////////////////////////////////////
// SBE handler for the PPE machine check interrupt
////////////////////////////////////////////////////////////////
// TODO: via RTC 155896 - Change the way bad scoms are handled.
// Once HW375602 is fixed, there will be no need for this
// interrupt handler.
extern "C" void __sbe_machine_check_handler()
{
    asm(
    "# Save r4 to stack, since it is going to be used by\n"
    "# this handler\n"
    "stwu %r1, -8(%r1)\n"
    "stw  %r4, 0(%r1)\n"
    "# Check the MCS bits (29:31) in the ISR to determine the cause for the machine check\n"
    "# For a data machine check, the MCS should be 0x001 to 0x011\n"
    "mfisr %r4\n"
    "andi. %r4, %r4, 0x0007\n"
    "bwz %r4, __halt_sbe\n"
    "cmpwibgt %r4, 0x0003, __halt_sbe\n"
    "# The EDR contains the address that caused the machine check\n"
    "mfedr %r4\n"
    "srawi %r4, %r4, 16\n"
    "# If the address is in the range 0x00000000 - 0x7f000000, we treat it as a\n"
    "# failed scom and jump to __scom_error\n"
    "cmplwi %r4, 0x8000\n"
    "blt __scom_error\n"
    "# Else, halt the SBE\n"
    "__halt_sbe:\n"
    "b pk_halt\n"
    "__scom_error:\n"
    "# The srr0 contains the address of the instruction that caused the machine\n"
    "# check (since the the interrupt is raised *before* the instruction\n"
    "# completed execution). Since we want the code to continue with the next\n"
    "# instruction, we increment srr0 by 4, restore r4, and rfi to branch to srr0\n"
    "mfsrr0 %r4\n"
    "addi %r4, %r4, 4\n"
    "mtsrr0 %r4\n"
    "lwz %r4, 0(%r1)\n"
    "addi %r1, %r1, 8\n"
    "rfi\n"
    );
}

