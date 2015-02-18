#ifndef __PPE42_CONTEXT_H__
#define __PPE42_CONTEXT_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe42_context.h
/// \brief PPE42 Machine and Thread context for PK

/// \page ppe42_machine_context PPE42 Assembler Macros for PK Machine
/// Context (Critical Sections)
///
/// \section _pk_enter_critical \b _pk_critical_section_enter/exit
///
/// These macro encapsulates the instruction sequences required to enter and
/// exit critical sections, along with the machine context save for later
/// exiting the critical section.
///
/// \arg \c ctxreg A register that will hold (holds) the machine context (MSR)
/// prior to entering the critical section (to be restored) for \c
/// _pk_critical_section_enter (\c _pk_critical_section_exit).
///
/// \arg \c scrreg A scratch register required for the computation of
/// \c _pk_critical_section_enter. 
///
/// Forms:
/// 
/// \b _pk_critical_section_enter \a priority, \a ctxreg, \a scrreg - Enter a
/// critical section \n
/// \b _pk_critical_section_exit \a ctxreg - Exit a critical section

#ifdef __ASSEMBLER__

        .set    _msr_ee_bit, MSR_EE_BIT

        .macro  _pk_critical_section_enter ctxreg, scrreg
        mfmsr   \ctxreg
        wrteei  0
        .endm

        .macro  _pk_critical_section_exit ctxreg
        mtmsr   \ctxreg
        .endm
        
// ****************************************************************************
// PK context save/restore macros for 32-bit Embedded PowerPC
// ****************************************************************************

// All stack frames are 8-byte aligned in conformance with the EABI.  PK
// never saves or restores GPR2 or GPR13.  GPR13 is constant in (E)ABI
// applications - the base of the read-write small data area.  GPR2 is
// system-reserved in ABI applications, and is the base for read-only small data
// in EABI applications.

// A fair amount of complexity is involved in handling the non-critical and
// critical interrupt levels, and the emphasis on performance of fast-mode
// interrupt handlers. Several different approaches and philosophies could
// have been implemented - this is only one.  In this implementation
// critical/non-critical interrupt levels are treated more or less the same,
// and the interrupt priority is just that - a kind of preemption priority.
// Critical interrupt handling does have a little less overhead because it
// does not have a thread scheduling step at the end.

// A full context save takes place in 3 or 4 steps.  Thread switches always do
// steps 1, 2 and 3.
// 1. The fast context that is always saved in response to every interrupt;
// 1a. The optional save/update of the kernel context for interrupts. 
// 2. The (volatile - fast) context that is saved if an interrupt handler
//    switches from fast-mode to full-mode.
// 3. The non-volatile context that is saved when a thread is switched out.

// USPRG0 holds the __PkKernelContext structure (defined in ppe42.h) that
// represents the current kernel context.  The layout is as follows:
//
// Bits   Meaning
// ==============
// 0:7   The critical interrupt count
// 8:15  The non-critical interrupt count
// 16:23 The IRQ currently being processed
// 24    The 'thread_mode' flag
// 25:31 The thread priority of the running thread
//
// When PK is initialized USPRG0 is initialized to 0.  When thread-mode is
// entered (by pk_start_threads()) bit 24 is set to 1.  In order to support
// PgP/OCC firmware, once initialized (with pk_initialize()) PK can simply
// handle interrupts, reverting back to the non-thread-mode idle loop when
// there's nothing to do.
//      
// Note that it would require a serious error for the interrupt counts to ever
// equal or exceed 2**8 as this would imply runaway reentrancy and stack
// overflow. In fact it is most likely an error if an interrupt handler is
// ever re-entered while active.

// Registers SRR2 and SRR3 are always saved in IRQ context because
// __pk_irq_fast2full must save the (volatile - fast) context to provide
// working registers before it can look at USPRG0 to determine critical
// vs. non-critical context.  However, when restoring a non-critical interrupt
// or thread these registers need not be restored. SRR2 and SRR3 are never
// saved or restored for thread context switches, because threads always
// operate at noncritical level.

// When MMU protection is enabled, relocation/protection is re-established
// immediately upon entry to the interrupt handler, before any memory
// operations (load/store) take place.  This requires using SPRG0 and SPGR4
// for temporary storage for noncritical/critical handlers respectively in
// accordance with the PK conventions for SPRGn usage by fast-mode
// interrupts. 

        ## ------------------------------------------------------------
        ## Unused registers for embedded PPE42`
        ## ------------------------------------------------------------

        ## Registers GPR2 and GPR13 are never saved or restored.  In ABI and 
        ## EABI applications these registers are constant.

        .set    UNUSED_GPR2,  0x2 # Dedicated; EABI read-only small data area
        .set    UNUSED_GPR13, 0xd # Dedicated; (E)ABI read-write small data area

        ## ------------------------------------------------------------
        ## Flags for context push/pop
        ## ------------------------------------------------------------

        .set    PK_THREAD_CONTEXT, 0
        .set    PK_IRQ_CONTEXT, 1

        ## ------------------------------------------------------------
        ## The PK fast context layout for Embedded PPE42
        ## ------------------------------------------------------------

        .set    PK_FAST_CTX_GPR1,       0x00 # Dedicated; Stack pointer
        .set    PK_FAST_CTX_HANDLER_LR, 0x04 # Slot for handler to store LR 
        .set    PK_FAST_CTX_GPR3,       0x08 # Volatile;  Parameter; Return Value
        .set    PK_FAST_CTX_GPR4,       0x0c # Volatile;  Parameter
        .set    PK_FAST_CTX_GPR5,       0x10 # Volatile;  Parameter
        .set    PK_FAST_CTX_GPR6,       0x14 # Volatile;  Parameter
        .set    PK_FAST_CTX_CR,         0x18 # Condition register 
        .set    PK_FAST_CTX_LR,         0x1c # Link register      SPRN 0x008 
        .set    PK_FAST_CTX_KERNEL_CTX, 0x20 # Saved __PkKernelContext for IRQ

        .set    PK_FAST_CTX_SIZE,  0x28  # Must be 8-byte aligned

        ## ------------------------------------------------------------
        ## The PK (volatile - fast) context layout for Embedded PPE42
        ## ------------------------------------------------------------

        .set    PK_VOL_FAST_CTX_GPR1,       0x00 # Dedicated; Stack pointer
        .set    PK_VOL_FAST_CTX_HANDLER_LR, 0x04 # Slot for handler to store LR
        .set    PK_VOL_FAST_CTX_GPR7,       0x08 # Volatile;  Parameter
        .set    PK_VOL_FAST_CTX_GPR8,       0x0c # Volatile;  Parameter
        .set    PK_VOL_FAST_CTX_GPR9,       0x10 # Volatile;  Parameter
        .set    PK_VOL_FAST_CTX_GPR10,      0x14 # Volatile;  Parameter
        .set    PK_VOL_FAST_CTX_XER,        0x18 # Fixed-point exception register  SPRN 0x001
        .set    PK_VOL_FAST_CTX_CTR,        0x1c # Count register                  SPRN 0x009
        .set    PK_VOL_FAST_CTX_SRR0,       0x20 # Save/restore register 0         SPRN 0x01a
        .set    PK_VOL_FAST_CTX_SRR1,       0x24 # Save/restore register 1         SPRN 0x01b
        .set    PK_VOL_FAST_CTX_GPR0,       0x28 # Volatile;  Language specific

        .set    PK_VOL_FAST_CTX_SIZE,  0x30  # Must be 8-byte aligned

        ## ------------------------------------------------------------
        ## The PK non-volatile context layout for Embedded PowerPC
        ## ------------------------------------------------------------

        ## The 'preferred form' for stmw is for the LSB of R31 to fall into the
        ## end of a 16-byte aligned block.

        .set    PK_NON_VOL_CTX_GPR1,       0x00  # Dedicated; Stack Pointer
        .set    PK_NON_VOL_CTX_HANDLER_LR, 0x04  # Slot for handler to store LR
        .set    PK_NON_VOL_CTX_GPR28,      0x08  # Non-volatile
        .set    PK_NON_VOL_CTX_GPR29,      0x0c  # Non-volatile
        .set    PK_NON_VOL_CTX_GPR30,      0x10  # Non-volatile
        .set    PK_NON_VOL_CTX_GPR31,      0x14  # Non-volatile

        .set    PK_NON_VOL_CTX_SIZE,  0x18 # Must be 8-byte aligned

        ## ------------------------------------------------------------
        ## Save/restore the fast context
        ## 
        ## 11 Instructions, 8 Loads/Stores : If MMU is disabled
        ## 17 Instructions, 8 Loads/Stores : If MMU is enabled
        ## ------------------------------------------------------------
        ##
        ## Without MMU support, an EIEIO is always executed at the entry point
        ## to gauarantee that all memory operations (especially MMIO
        ## operations) have completed prior to execution of the interrupt
        ## handler.
        ##
        ## If MMU support is enabled, address translation is re-established
        ## immediately at the entry of each interrupt, prior to performing any
        ## loads or stores. PK currently only supports using the MMU for
        ## protection, not for address translation.  Therfore it is 'legal'
        ## to change translation modes a with an MTMSR followed by an
        ## ISYNC. This is much simpler then the complex instruction sequence
        ## that would be required if we had to set up RFI/RFCI sequences to
        ## change the execution context at this point.
        ##
        ## Note that since we are not really doing address translation, it
        ## would also be in keeping with the 'fast interrupt' idea to defer
        ## reenabling translation (protection) until the fast-to-full sequence
        ## was executed for full-mode interrupts, and run fast-mode interrupts
        ## unprotected. However here we chose to run all interrupts with MMU
        ## protection. 
        ##
        ## Unfortunately the simple MTMSR;ISYNC sequence exposes a serious bug
        ## in the PPE42 core that causes the stack-pointer store instruction
        ## to generate a seemingly random, *real-mode* address in certain cases
        ## when this instruction in a noncritical interrupt prologue is
        ## interrupted by a critical interrupt. This bug is described in
        ## HW239446. The workaround is to follow the ISYNC sith a SYNC - which
        ## eliminates the problem for reasons still unknown. On the bright side
        ## this SYNC might also serve the same purpose as the EIEIO in the
        ## non-MMU case, guaranteeing that all MMIO has completed prior to the
        ## interrupt handler. However without the initial EIEIO we still
        ## experience failures, so this seemingly redundant instruction also
        ## remains in place. This requirement is assumed to be related to the
        ## HW239446 issue.       

        .macro  _pk_fast_ctx_push

        stwu    %r1, -PK_FAST_CTX_SIZE(%r1)

        stvd    %d3,  PK_FAST_CTX_GPR3(%r1)
        stvd    %d5,  PK_FAST_CTX_GPR5(%r1)

        mfcr    %r3
        mflr    %r4

        stvd    %d3,  PK_FAST_CTX_CR(%r1)

        .endm


        .macro  _pk_fast_ctx_pop
                
        lvd     %d3, PK_FAST_CTX_CR(%r1)

        mtcr0   %r3
        mtlr    %r4

        lvd     %d3,  PK_FAST_CTX_GPR3(%r1)
        lvd     %d5,  PK_FAST_CTX_GPR5(%r1)

        lwz     %r1, 0(%r1)

        .endm

        ## ------------------------------------------------------------
        ## Save/update the kernel context in response to an interrupt.  This is
        ## not part of the fast context save because for external interupts the
        ## IRQ is not determined until later.
        ## ------------------------------------------------------------

        ## The kernel context is saved, then updated with the currently active
        ## IRQ in bits 16:23.  The correct interrupt count is incremented and
        ## the context is returned to SPRG0.

        .macro  _save_update_kernel_context irqreg, ctxreg

        #PK_TRACE_NONCRITICAL_IRQ_ENTRY \irqreg, \ctxreg

        mfsprg0 \ctxreg
        stw     \ctxreg, PK_FAST_CTX_KERNEL_CTX(%r1)
        #rlwimi  \ctxreg, \irqreg, 24, 9, 15 //set the irq #
        rlwimi  \ctxreg, \irqreg, 8, 16, 23 //set the irq #
        #oris    \ctxreg, \ctxreg, 0x4000 //set the 'processing interrupt' PI bit
        addis   \ctxreg, \ctxreg, 0x0001 //increment the irq count
        mtsprg0 \ctxreg

        .endm

        ## ------------------------------------------------------------
        ## Fast-mode context pop and RF(C)I.  This is only used by
        ## interrupt handlers - the thread context switch has its own
        ## code to handle updating USPRG0 for thread mode.
        ## ------------------------------------------------------------

        .macro  _pk_fast_ctx_pop_exit

        .if     PK_KERNEL_TRACE_ENABLE
        bl      __pk_trace_noncritical_irq_exit
        .endif

        lwz     %r3, PK_FAST_CTX_KERNEL_CTX(%r1)
        mtsprg0 %r3
        _pk_fast_ctx_pop
        rfi

        .endm

        ## ------------------------------------------------------------
        ## Save/restore the (volatile - fast) context
        ## 
        ## Thread - 15     Instructions, 11 Loads/Stores
        ## IRQ    - 19(15) Instructions, 13(11) Loads/Stores
        ## ------------------------------------------------------------

        .macro  _pk_vol_fast_ctx_push

        stwu    %r1, -PK_VOL_FAST_CTX_SIZE(%r1)

        stw     %r0,  PK_VOL_FAST_CTX_GPR0(%r1)
        stvd    %d7,  PK_VOL_FAST_CTX_GPR7(%r1)
        stvd    %d9,  PK_VOL_FAST_CTX_GPR9(%r1)

        mfxer   %r7
        mfctr   %r8
        mfsrr0  %r9
        mfsrr1  %r10

        stvd     %d7,  PK_VOL_FAST_CTX_XER(%r1)
        stvd     %d9,  PK_VOL_FAST_CTX_SRR0(%r1)

        .endm


        .macro  _pk_vol_fast_ctx_pop

        lvd     %d7,  PK_VOL_FAST_CTX_XER(%r1)
        lvd     %d9,  PK_VOL_FAST_CTX_SRR0(%r1)

        mtxer   %r7
        mtctr   %r8
        mtsrr0  %r9
        mtsrr1  %r10

        lwz     %r0,  PK_VOL_FAST_CTX_GPR0(%r1)
        lvd     %d7,  PK_VOL_FAST_CTX_GPR7(%r1)
        lvd     %d9,  PK_VOL_FAST_CTX_GPR9(%r1)

        lwz     %r1, 0(%r1)

        .endm

        ## ------------------------------------------------------------
        ## Save/restore the non-volatile context on the stack
        ##
        ## 2 Instructions, 19 Loads/Stores
        ## ------------------------------------------------------------

        .macro  _pk_non_vol_ctx_push

        stwu    %r1, -PK_NON_VOL_CTX_SIZE(%r1)
        stvd    %d28, PK_NON_VOL_CTX_GPR28(%r1)
        stvd    %d30, PK_NON_VOL_CTX_GPR30(%r1)
        
        .endm


        .macro  _pk_non_vol_ctx_pop

        lvd     %d28, PK_NON_VOL_CTX_GPR28(%r1)
        lvd     %d30, PK_NON_VOL_CTX_GPR30(%r1)
        lwz     %r1, 0(%r1)

        .endm

#else /* __ASSEMBLER__ */

/// PK thread context layout as a C structure.
///
/// This is the structure of the stack area pointed to by
/// thread->saved_stack_pointer when a thread is fully context-switched out.

typedef struct {

    uint32_t r1_nv;
    uint32_t link_nv;
    uint32_t r28;
    uint32_t r29;
    uint32_t r30;
    uint32_t r31;
    uint32_t r1_vf;
    uint32_t link_vf;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t xer;
    uint32_t ctr;
    uint32_t srr0;
    uint32_t srr1;
    uint32_t r0;
    uint32_t pad;
    uint32_t r1;
    uint32_t link_fast;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t cr;
    uint32_t lr;
    uint32_t sprg0;

} PkThreadContext;

/// PK thread context of an interrupted thread (full-mode handler)
///
/// When a thread is interrupted by a full-mode interrupt handler, this is the
/// layout of the stack area pointed to by either __pk_saved_sp_noncritical
/// or __pk_saved_sp_critical.

typedef struct {

    uint32_t r1_vf;
    uint32_t link_vf;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t xer;
    uint32_t ctr;
    uint32_t srr0;
    uint32_t srr1;
    uint32_t r0;
    uint32_t pad;
    uint32_t r1;
    uint32_t link_fast;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t cr;
    uint32_t lr;
    uint32_t sprg0;

} PkThreadContextFullIrq;


/// PK thread context of an interrupted thread (fast-mode handler)
///
/// When a thread is interrupted by a fast-mode interrupt handler, this is the
/// layout of the stack area pointed to by R1 - unless the fast-mode interrupt
/// handler extends the stack.

typedef struct {

    uint32_t r1;
    uint32_t link_fast;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t cr;
    uint32_t lr;
    uint32_t sprg0;

} PkThreadContextFastIrq;

#endif /* __ASSEMBLER__ */

#endif /* __PPE42_CONTEXT_H__ */


