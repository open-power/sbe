/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeirq.C $                                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
#include "ppe42_scom.h"
#include "p9_misc_scom_addresses.H"
#include "sbeirqregistersave.H"

////////////////////////////////////////////////////////////////
// @brief:     SBE control loop ISR:
//               - FIFO new data available
//               - FIFO reset request
//               - PSU new data available
//               - S0 Interrupt
//               - S1 Interrupt
//
// @param[in]  i_pArg - Unused
// @param[in]  i_irq  - IRQ number as defined in sbeirq.h
//
////////////////////////////////////////////////////////////////
void sbe_interrupt_handler (void *i_pArg, PkIrqId i_irq)
{
    #define SBE_FUNC " sbe_interrupt_handler "
    SBE_INFO(SBE_FUNC"i_irq=[0x%02X]",i_irq);

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

        case SBE_IRQ_SBEHFIFO_RESET:
            SBE_GLOBAL->sbeIntrSource.setIntrSource(SBE_INTERRUPT_ROUTINE,
                                            SBE_INTERFACE_SBEHFIFO_RESET);
            pk_irq_disable(SBE_IRQ_SBEHFIFO_DATA);
            break;

        case SBE_IRQ_SBEHFIFO_DATA:
            SBE_GLOBAL->sbeIntrSource.setIntrSource(SBE_INTERRUPT_ROUTINE,
                                            SBE_INTERFACE_SBEHFIFO);
            pk_irq_disable(SBE_IRQ_SBEHFIFO_RESET);
            break; 

#ifdef _S0_
        case SBE_IRQ_INTR0:
            SBE_GLOBAL->sbeIntrSource.setIntrSource(SBE_INTERRUPT_ROUTINE,
                                            SBE_INTERFACE_S0);
            break;
#endif
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

// Interrupts which are available in SBE, in an array
static uint32_t G_supported_irqs[] = {
                                        SBE_IRQ_HOST_PSU_INTR,
                                        SBE_IRQ_SBEFIFO_DATA,
                                        SBE_IRQ_SBEFIFO_RESET,
                                        SBE_IRQ_SBEHFIFO_RESET,
                                        SBE_IRQ_SBEHFIFO_DATA,
#ifdef _S0_
                                        SBE_IRQ_INTR0,
#endif
                                     };

// Create the Vector mask for all the interrupts in SBE,
// required to call disable/enable with this vector mask
constexpr uint64_t getVectorMask ( size_t index )
{
    return (index >= (sizeof(G_supported_irqs)/sizeof(uint32_t))?
        0:STD_IRQ_MASK64(G_supported_irqs[index])|getVectorMask(index + 1));
}

////////////////////////////////////////////////////////////////
// See sbeexeintf.h for more details
////////////////////////////////////////////////////////////////
int sbeIRQSetup (void)
{
    #define SBE_FUNC " sbeIRQSetup "
    int rc = 0;
    uint64_t vector_mask = getVectorMask(0);

    // Disable the relevant IRQs while we set them up
    pk_irq_vec_disable(vector_mask);

    for(uint32_t cnt=0; cnt<sizeof(G_supported_irqs)/sizeof(uint32_t); cnt++)
    {
        // Register the IRQ handler with PK
        rc = pk_irq_handler_set(G_supported_irqs[cnt], sbe_interrupt_handler, NULL);
        if(rc)
        {
            SBE_ERROR (SBE_FUNC "pk_irq_handler_set failed, IRQ=[0x%02X], "
                "rc=[%d]", G_supported_irqs[cnt], rc);
            break;
        }
    }

    if(!rc)
    {
        // Enable the IRQ
        pk_irq_vec_enable(vector_mask);
    }

    return rc;
    #undef SBE_FUNC
}

#if !defined(__SBEMFW_MEASUREMENT__) && !defined(__SBEVFW_VERIFICATION__)
///////////////////////////////////////////////////////////////////
// SBE handler to save off specific registers
///////////////////////////////////////////////////////////////////
registersave_t __g_register_ffdc __attribute__((aligned(8), section (".sbss")));
//registersave_t __g_register_ffdc __attribute__ ((aligned(8)));
uint64_t __g_address_for_register_ffdc = (uint32_t)&__g_register_ffdc;
uint32_t __g_isParityError __attribute__((section (".sbss"))) = 0;

extern "C" void __sbe_register_saveoff()
{
    asm(
    "# Save r4, r5, r6 to stack, since it is going to be used\n"
    "# inside here, move the stack by 12\n"
    "stwu %r1, -12(%r1)\n"
    "# Store r4 into stack\n"
    "stw  %r4, 0(%r1)\n"
    "# Store r5 into stack\n"
    "stw  %r5, 4(%r1)\n"
    "# Store r6 into stack\n"
    "stw  %r6, 8(%r1)\n"
    "# Write zero to r4,r5,r6 to clear off the content\n"
    "lis %r4, 0\n"
    "lis %r5, 0\n"
    "lis %r6, 0\n"

    "# Check for loop condition if this branch is happening repetitively\n"
    "# load first 4bytes of __g_register_ffdc, it should be initialized\n"
    "# with zero, if not zero than this is repetitive entry, exit now\n"
    "lvd %d5, __g_register_ffdc@sda21(0)\n"
    "# if r5 and r4 not equal, then it's loop inside loop, exit condition\n"
    "cmplwbne %r4, %r5, __goto_exit\n"

    "# Start gathering registers into FFDC\n"
    "# Read SRR0 into r4\n"
    "mfsrr0  %r4\n"
    "stw %r4, __g_register_ffdc+4@sda21(0)\n"

    "# Read SRR1 into r4\n"
    "mfsrr1  %r4\n"
    "stw %r4, __g_register_ffdc+8@sda21(0)\n"

    "# Read ISR into r4\n"
    "mfisr  %r4\n"
    "stw %r4, __g_register_ffdc+12@sda21(0)\n"

    "# Read LR into r4\n"
    "mflr  %r4\n"
    "stw %r4, __g_register_ffdc+16@sda21(0)\n"

    "# Read LFR into r4\n"
    "lis %r6, 0xc000\n"
    "lvd %d4, 0x2040(%r6)\n"
    "stw %r4, __g_register_ffdc+20@sda21(0)\n"

    "# Read Spi0 Status Register into r4 and r5\n"
    "lis %r6, 0xc\n"
    "lvd %d4, 0x8(%r6)\n"
    "stw %r4, __g_register_ffdc+24@sda21(0)\n"
    "stw %r5, __g_register_ffdc+28@sda21(0)\n"

    "# Read Spi0 Config Register into r4 and r5\n"
    "lvd %d4, 0x2(%r6)\n"
    "stw %r4, __g_register_ffdc+32@sda21(0)\n"

    "# Read Spi0 Clock Config Register into r4 and r5\n"
    "lvd %d4, 0x3(%r6)\n"
    "stw %r4, __g_register_ffdc+36@sda21(0)\n"

    "# Read Spi1 Status Register into r4 and r5\n"
    "lis %r6, 0xc\n"
    "lvd %d4, 0x28(%r6)\n"
    "stw %r4, __g_register_ffdc+40@sda21(0)\n"
    "stw %r5, __g_register_ffdc+44@sda21(0)\n"

    "# Read Spi1 Config Register into r4 and r5\n"
    "lvd %d4, 0x22(%r6)\n"
    "stw %r4, __g_register_ffdc+48@sda21(0)\n"

    "# Read Spi1 Clock Config Register into r4 and r5\n"
    "lvd %d4, 0x23(%r6)\n"
    "stw %r4, __g_register_ffdc+52@sda21(0)\n"
    
    "# Read Spi2 Status Register into r4 and r5\n"
    "lis %r6, 0xc\n"
    "lvd %d4, 0x68(%r6)\n"
    "stw %r4, __g_register_ffdc+56@sda21(0)\n"
    "stw %r5, __g_register_ffdc+60@sda21(0)\n"

    "# Read Spi2 Config Register into r4 and r5\n"
    "lvd %d4, 0x62(%r6)\n"
    "stw %r4, __g_register_ffdc+64@sda21(0)\n"

    "# Read Spi2 Clock Config Register into r4 and r5\n"
    "lvd %d4, 0x63(%r6)\n"
    "stw %r4, __g_register_ffdc+68@sda21(0)\n"

    "# Read Spi3 Status Register into r4 and r5\n"
    "lis %r6, 0xc\n"
    "lvd %d4, 0x48(%r6)\n"
    "stw %r4, __g_register_ffdc+72@sda21(0)\n"
    "stw %r5, __g_register_ffdc+76@sda21(0)\n"

    "# Read Spi3 Config Register into r4 and r5\n"
    "lvd %d4, 0x42(%r6)\n"
    "stw %r4, __g_register_ffdc+80@sda21(0)\n"

    "# Read Spi3 Clock Config Register into r4 and r5\n"
    "lvd %d4, 0x43(%r6)\n"
    "stw %r4, __g_register_ffdc+84@sda21(0)\n"

    "# TODO\n"
    "# Read Spi parity errors\n"
    "lvd %d4, __g_isParityError@sda21(0)\n"
    "stw %r4, __g_register_ffdc+88@sda21(0)\n"

    "# Write versioning into the lower 32bit of d5 i.e. r6\n"
    "#lvd %d5, __g_sbe_register_save_version@sda21(0)\n"
    "# Directly load Versioninfo into upper 16bits in r4 \n"
    "# RTC:183752, Remove this hard-coding of versioning\n"
    "lis %r4, 0x11\n"
    "#rlwinm %r4, %r6, 16, 0, 0x1F\n"
    "# Write magicbyte and validbyte in the lower 16bits\n"
    "ori %r4, %r4, 0xA501\n"
    "stw %r4, __g_register_ffdc@sda21(0)\n"

    "# Get the address of the register ffdc struct and store it in sprg0\n"
    "lvd %d5, __g_address_for_register_ffdc@sda21(0)\n"
    "# Store the content of r6 read above into sprg0\n"
    "mtsprg0 %r6\n"

    "__goto_exit:\n"
    "# Get back the original value of the registers stored in stack\n"
    "lwz %r6, 8(%r1)\n"
    "lwz %r5, 4(%r1)\n"
    "lwz %r4, 0(%r1)\n"
    "addi %r1, %r1, 12\n"
    "b pk_halt\n"
    );
}

////////////////////////////////////////////////////////////////
// SBE handler for the PPE machine check interrupt
// This is valid for both measurement fw and boot fw
////////////////////////////////////////////////////////////////
extern "C" void __sbe_machine_check_handler()
{
    asm(
    "# reclaim function callstack\n"
    "#lwz     %r0,12(%r1)\n"
    "#mtlr    %r0\n"
    "#addi    %r1,%r1,8\n"

    "# Save r0, r1, r2, r3,r4, r5, r6, r7, r8, r9, r10, r13, r28, r29, r30, r31\n"
    "# lr to stack, since it is going to be used by\n"
    "# this handler\n"
    "stwu %r1, -80(%r1)\n"
    "stw  %r0, 0(%r1)\n"
    "stw  %r2, 4(%r1)\n"
    "stw  %r3, 8(%r1)\n"
    "stw  %r4, 12(%r1)\n"
    "stw  %r5, 16(%r1)\n"
    "stw  %r6, 20(%r1)\n"
    "stw  %r7, 24(%r1)\n"
    "stw  %r8, 28(%r1)\n"
    "stw  %r9, 32(%r1)\n"
    "stw  %r10, 36(%r1)\n"
    "stw  %r13, 40(%r1)\n"
    "stw  %r28, 44(%r1)\n"
    "stw  %r29, 48(%r1)\n"
    "stw  %r30, 52(%r1)\n"
    "stw  %r31, 56(%r1)\n"
    "mflr %r0\n"
    "stw  %r0, 60(%r1)\n"

    "# Check the MCS bits (29:31) in the ISR to determine the cause for the machine check\n"
    "# For a data machine check, the MCS should be 0x001 to 0x011\n"
    "# 0x01 -> Data Load MC\n"
    "# 0x02 -> Data Store MC\n"
    "# 0x03 -> Imprecise Data Store MC\n"
    "mfisr %r4\n"
    "andi. %r4, %r4, 0x0007\n"

    "# branch to Instr_MC if MCS is zero\n"
    "bwz %r4, __instruction_machine_check\n"

    "# branch to Other_MC if MCS is greater than 3\n"
    "cmpwibgt %r4, 0x0003, __other_error_machine_check\n"

    "# This is the Data_MC path, EDR contains the Data Addr causing the MC\n"
    "mfedr %r4\n"
    "srawi %r4, %r4, 16\n"
    "# If the address is in the range 0x00000000 - 0x7f000000, we treat it as a\n"
    "# failed scom and jump to __scom_error\n"
    "cmplwi %r4, 0x8000\n"
    "blt __scom_error\n"
    "# Else, save-off and halt the SBE\n"
    "# Save-off Register FFDC and Halt\n"

    "__other_error_machine_check:\n"
    "__instruction_machine_check:\n"
    "# The srr0 contains the address that caused the instruction machine check\n"
    "# Save-off Register FFDC and Halt\n"
    "b __sbe_register_saveoff\n"

    "__scom_error:\n"
    "# The srr0 contains the address of the instruction that caused the machine\n"
    "# check (since the the interrupt is raised *before* the instruction\n"
    "# completed execution). Since we want the code to continue with the next\n"
    "# instruction, we increment srr0 by 4, restore r4, and rfi to branch to srr0\n"
    "mfsrr0 %r4\n"
    "addi %r4, %r4, 4\n"
    "mtsrr0 %r4\n"

    "#lwz %r0, 0(%r1)\n"
    "lwz %r2, 4(%r1)\n"
    "lwz %r3, 8(%r1)\n"
    "lwz %r4, 12(%r1)\n"
    "lwz %r5, 16(%r1)\n"
    "lwz %r6, 20(%r1)\n"
    "lwz %r7, 24(%r1)\n"
    "lwz %r8, 28(%r1)\n"
    "lwz %r9, 32(%r1)\n"
    "lwz %r10, 36(%r1)\n"
    "lwz %r13, 40(%r1)\n"
    "lwz %r28, 44(%r1)\n"
    "lwz %r29, 48(%r1)\n"
    "lwz %r30, 52(%r1)\n"
    "lwz %r31, 56(%r1)\n"
    "lwz %r0, 60(%r1)\n"
    "mtlr %r0\n"
    "lwz %r0, 0(%r1)\n"
    "addi %r1, %r1, 80\n"

    "rfi\n"
    );
}
#endif
