//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------
        
/// \file ppe42_exceptions.S
/// \brief PPE42 exception vector area.
///
/// \cond

        .nolist
#include "pk.h"
        .list

## declare and initializes global variables that hold external irq config data
## Each PPE macro type (GPE, CME, and SBE) will have it's own implementation of this macro
## defined in (gpe, cme, sbe)_common.h
        .hwmacro_irq_cfg_bitmaps

### ****************************************************************************
### .vectors - This section contains all ppe42 exception vectors
### 
### ****************************************************************************

        .section .vectors, "ax", @progbits

        .global __vectors

__vectors:

        ############################################################
        # 0x0000 : Machine Check
        ############################################################

        ### Unmaskable interrupts (including program interrupts) are promoted
        ### to machine check interrupts if MSR[UIE] = 0 and MSR[ME] = 1.
        ### If the machine check was caused by a program interrupt it
        ### will be forwarded to the program exception handler.
__machine_check:

        PPE42_MACHINE_CHECK_HANDLER

        ############################################################
        # 0x0040 : System Reset
        ############################################################
        .global __system_reset
        .org __vectors + 0x0040
__system_reset:
        b   __pk_boot

        ############################################################
        # 0x0060 : Data Storage Interrupt
        ############################################################

        .org __vectors + 0x0060
__data_storage:

        PPE42_DATA_STORAGE_HANDLER

        ############################################################
        # 0x0080 : Instruction Storage Interrupt
        ############################################################

        .org __vectors + 0x0080
__instruction_storage:

        PPE42_INSTRUCTION_STORAGE_HANDLER


        ############################################################
        # 0x00A0 : External Interrupt
        ############################################################

        .org __vectors + 0x00A0
__external_interrupt_vector:
        _pk_fast_ctx_push

        b __external_interrupt

        ############################################################
        # 0x00C0 : Alignment Exception
        ############################################################

        .org __vectors + 0x00C0
__alignment_exception:

        PPE42_ALIGNMENT_HANDLER


        ############################################################
        # 0x00E0 : Program Interrupt
        ############################################################

        .org __vectors + 0x00E0

        ### Program exceptions are utilized for emulating the system call
        ### instruction (0x44000002) which is used for doing context
        ### switches between threads.  They can also be used by the code
        ### to signal an exception in an error scenario.
__program_exception:
        _pk_fast_ctx_push

        b       program_exception_handler 


        ############################################################
        # 0x0100 : DEC Interrupts
        ############################################################

        .org __vectors + 0x0100
__dec_interrupt:
        _pk_fast_ctx_push
        li      %r3, PPE42_IRQ_DEC
        b       dec_handler

        ############################################################
        # 0x0120 : FIT Interrupts
        ############################################################
        .org __vectors + 0x0120
__fit_interrupt:        

        #b       fit_handler
        b       .

        ############################################################
        # 0x0140 : Watchdog Interrupts
        ############################################################
        .org __vectors + 0x0140
__watchdog_interrupt:   

        #b       watchdog_handler
        b       .





        .global __pk_irq_fast2full
__pk_irq_fast2full:    
        
        ## Convert a fast-mode to a full-mode interrupt by saving the
        ## (volatile - fast) context, and switching to the appropriate system
        ## stack. 

        ## Entry invariants:
        ## 1. The SP/stack must be exactly as it was when the fast-mode
        ##    handler was entered.
        ## 2. No changes have been made to the MSR - the interrupt level must
        ##    remain disabled.
        ## 3. The handler owns the fast context and has not modified the other
        ##    register context.  This routine can only use the (volatile -
        ##    fast) register context.

        ## 41 (linear) instructions plus alignmenmt

        ## Start by pushing the (volatile - fast) context. Technically we also
        ## need to save the CR as our contract with the handler is not to
        ## disturb any of its register state.
        
        _pk_vol_fast_ctx_push
        mfcr    %r10
        mfsprg0 %r8

        ## At entry here the (volatile - fast) context has been pushed,
        ## R8 has SPRG0 and R10 contains the saved CR.

        ## Note that it would violate a kernel/API invariant if this routine
        ## were entered from outside an interrupt context.

fast2full_noncritical:

        ## switch to the kernel stack if we haven't already done so. (SPRG0[RI] = 0)
        #bb1wi   %r8, RI_BIT, 1f //branches if the RI_BIT is '1'

        extrwi  %r9, %r8, 8, 8
        cmpwi   %r9, 1
        bne     1f

        _stwsd  %r1, __pk_saved_sp_noncritical
        _lwzsd  %r1, __pk_noncritical_stack

1:      

        .if     (PK_ERROR_CHECK_KERNEL | PK_ERROR_CHECK_API)
        #bb1wi   %r8, PI_BIT, 2f //branches if PI_BIT is '1'
        cmpwi   %r9, 0
        bne     2f
        _pk_panic PPE42_IRQ_FAST2FULL_INVARIANT
2:      
        .endif  

        mtcr0   %r10    
        blr

        .global __pk_irq_full_mode_exit
__pk_irq_full_mode_exit:       

        ## Exit a full-mode handler.

        ## Entry invariants:
        ## 1. The SP/stack must be in exactly the same state it was left in at
        ##    the  exit of __pk_irq_fast2full.
        ## 2. It is assumed the the preemption rules of PK have been followed
        ##    - in particular that critical handlers have not enabled
        ##    non-critical interrupts.

        ## We can freely modify the volatile context here - the handler is done
        ## and we will restore the interrupted volatile context.

        ## 22 linear instructions

        ## If the critical count is non-zero, then the PK preemption rules
        ## guarantee that we are exiting from a critical interrupt
        ## handler. This test is safe to make even if critical interrupts are
        ## enabled, because the variable is set exactly once in a critical
        ## section. 

        mfsprg0        %r3

        ## Exiting a full-mode non-critical handler is more complex than the
        ## critical case, because the handler may have made a new
        ## highest-priority thread runnable and we may need to go through a
        ## delayed scheduling step.

        ## Note that the idle thread is treated as a special case.  The idle
        ## thread has no permanent register context. To avoid having to
        ## allocate a stack area for the idle thread, the idle thread 
        ## 'uses' the non-critical stack.  When the idle thread is interrupted
        ## the (redundant) context is pushed, but is then effectively lost.
        ## Whenever we restore the idle thread we simply reenter the idle
        ## thread entry point.

        ## At entry:    
        ## 1. R3 holds the value of SPRG0 (__PkKernelContext)

        ## 33 linear instructions.

full_exit_noncritical:  
        
        ## Enter a critical section for the return from interrupt, in the event
        ## that the handler enabled preemption.

        _pk_critical_section_enter     %r4, %r5

        ## If the non-critical count is > 1 then this is a nested interrupt 
        ## and we can simply pop the context and RFI.

        extrwi. %r4, %r3, 8, 8

        ## If SPRG0[RI] = 1 then this is a recursive interrupt 
        ## and we can simply pop the context and RFI.  Note that it would
        ## violate a kernel/API invariant if this routine were entered from
        ## outside an interrupt context (interrupt level == 0). 

        .if     (PK_ERROR_CHECK_KERNEL | PK_ERROR_CHECK_API)
        #bb1wi   %r3, PI_BIT, 1f  //branch if the PI bit is set
        bne     1f
        _pk_panic PPE42_IRQ_FULL_EXIT_INVARIANT
1:      
        .endif
        
        cmpwi   %r4, 1
        bne     exit_noncritical_without_switch
                
        ## Otherwise, restore the saved stack pointer and continue.
        
        _lwzsd  %r1, __pk_saved_sp_noncritical

        ## If we are not in thread mode (i.e., we took an interrupt in an
        ## interupt-only configuration of PK or after pk_initialize() but
        ## before pk_start_threads) simply pop the context and RFI - in this
        ## case we'll most likely be returning to main() or the non-thread-mode
        ## idle thread.

        andi.   %r4, %r3, PPE42_THREAD_MODE
        beq     exit_noncritical_without_switch

        ## Now, check for a delayed context switch.  If none is pending, we can
        ## exit (after a check for the idle thread special case).

        _lwzsd  %r3, __pk_delayed_switch
        cmpwi   %r3, 0
        bne     noncritical_switch

        _lwzsd  %r3, __pk_current_thread
        cmpwi   %r3, 0
        beq     __pk_idle_thread

exit_noncritical_without_switch:
        _pk_vol_fast_ctx_pop
        b       fast_exit_noncritical

        ## The non-critical interrupt activated a delayed context switch.  The
        ## C-level code has taken care of the scheduling decisions - we simply
        ## need to implement them here.

noncritical_switch:
                
        ## Clear the delayed switch flag and go to the context switch code to
        ## finish the switch. 

        li      %r3, 0
        _stwsd  %r3, __pk_delayed_switch
        
        b       thread_save_non_volatile_and_switch





        ## The idle thread has no permanent register context.  The idle thread
        ## entry point is re-entered whenever the idle thread is scheduled.

        .global __pk_idle_thread
        .global __pk_idle_thread_from_bootloader

__pk_idle_thread:

        ## The idle thread 'uses' the non-critical stack.  Any register context
        ## pushed here is redundant and is wiped out/ignored every time the
        ## idle thread is re-scheduled. 
        
        ## The idle thread simply establishes a default machine context and
        ## enters the wait-enable state.  The idle thread is always entered
        ## with non-critical interrupts disabled.  
        ##
        ## The kernel context is initialized to indicate that the idle thread
        ## is running - the idle thread priority is PK_THREADS, and the
        ## 'thread-mode' bit is asserted as well.
        ##
        ## This loop can also be called from the PK bootloader if main()
        ## returns - in which case we don't muck with the SPRG0 or the stack
        ## pointer. 

        li      %r3, (PK_THREADS | PPE42_THREAD_MODE)
        mtsprg0        %r3
        _lwzsd  %r1, __pk_noncritical_stack
                
__pk_idle_thread_from_bootloader:      

        #li      %r3, PK_THREADS
        #PK_TRACE_THREAD_SWITCH %r3, %r4
        _lwzsd  %r3, __pk_thread_machine_context_default
        _oriwa  %r3, %r3, MSR_WE
        mtmsr   %r3
        b       .

        ## pk_halt() is implemented on the ppe42 by writing a value of 0x3 to
        ## the RST field of the DBCR.
        .global pk_halt
pk_halt:
        lis     %r31, 0x3000
        mtdbcr  %r31
        .long   0


dec_handler:            

        ## The portable timer handler of PK is a full-mode handler with the prototype:
        ## void (*pk_timer_handler)(void).
        ##
        ## To support the portable specification, the kernel clears the
        ## interrupt by writing the DIS back into the TSR before calling the
        ## handler.  The timer handler does not take any arguments.

        _save_update_kernel_context %r3, %r4
        
        _liwa   %r3, TSR_DIS
        mttsr   %r3

        _pk_irq_fast2full      __pk_timer_handler





        ## Exit traces are moved here because the code area (0x100 bytes)
        ## reserved for individual interrupts is overflowing when tracing is
        ## enabled.  This is kind of a hack: We know that this trace only
        ## occurs when we're about to exit the fast context, at a place
        ## where we can use any of the fast registers.
#if 0
__pk_trace_noncritical_irq_exit:       
        #PK_TRACE_NONCRITICAL_IRQ_EXIT %r3, %r4
        blr
#endif

program_exception_handler:
        ## first check if exception was caused by an illegal 'sc' instruction
        mfspr       %r3, SPRN_EDR
        _liw        %r4, PPE42_SC_INST
        cmpwbeq     %r3, %r4, __sc_helper
        _pk_panic   PPE42_ILLEGAL_INSTRUCTION

        ## SRR0 is currently pointing to the 'sc' instruction.  We need to advance it
        ## to the next instruction so that we don't end up in an endless loop (something
        ## that the ppc sc instruction does automatically).
__sc_helper:
        mfsrr0      %r3
        addi        %r3, %r3, 4
        mtsrr0      %r3
        
__system_call:  

        ## The program exception is used by PK as a handy way to start a
        ## context switch, as the continuation address and MSR of the thread to
        ## be swapped out are saved in SRR0 and SRR1.

        ## Non-critical interrupts are disabled at entry.

        ## Begin by saving the volatile context of the current thread.
        ## NOTE: fast context has already been saved prior to branching here.

        _pk_vol_fast_ctx_push

thread_save_non_volatile_and_switch:    

        ## Finish the thread context save by pushing the non-volatile context
        ## and saving the resulting stack pointer in the thread structure.  If
        ## the current thread is the idle thread this step is bypassed.
        
        ## This symbol is also used as an entry point by the non-critical
        ## interrupt handler - non-critical interrupts are disabled here.
        
        _lwzsd  %r3, __pk_current_thread
        cmpwi   %r3, 0
        beq     __pk_next_thread_resume
        
        _pk_non_vol_ctx_push
        stw     %r1, PK_THREAD_OFFSET_SAVED_STACK_POINTER(%r3)

        ## The next thread becomes the current thread, and we switch to its
        ## stack - unless the new thread is the idle thread, in which case it
        ## (the idle thread) is simply resumed.  

        .global __pk_next_thread_resume
__pk_next_thread_resume:
        
        _lwzsd  %r3, __pk_next_thread
        _stwsd  %r3, __pk_current_thread

        cmpwi   %r3, 0
        beq     __pk_idle_thread
        
        lwz     %r1, PK_THREAD_OFFSET_SAVED_STACK_POINTER(%r3)

        ## Restore the thread context and resume the new thread.  The kernel
        ## context in thread mode is simply the thread priority OR'ed with the
        ## thread-mode flag. All other fields are cleared.

        _pk_non_vol_ctx_pop
        _pk_vol_fast_ctx_pop
        
        _lbzsd  %r3, __pk_next_priority
        #PK_TRACE_THREAD_SWITCH %r3, %r4
        ori     %r3, %r3, PPE42_THREAD_MODE            
        mtsprg0 %r3

        _pk_fast_ctx_pop
        rfi

fit_handler:
        
        ## The FIT handler is user defined, and is a fast-mode handler. By
        ## convention the kernel clears the interrupt by writing the FIS back
        ## into the TSR.
        
        _pk_fast_ctx_push
        
        _lwzsd  %r3, __ppe42_fit_arg
        li      %r4, PPE42_IRQ_FIT
                
        _save_update_kernel_context %r4, %r6

        _liwa   %r6, TSR_FIS
        mttsr   %r6

        _lwzsd  %r6, __ppe42_fit_routine
        mtlr    %r6
        blrl
        
        b       fast_exit_noncritical

watchdog_handler:
        ## Watchdog setup is described in the PK Specification. 
        ## The kernel clears TSR[WIS] prior to calling the handler.  
        ## The watchdog handler is a critical, fast-mode handler.

        _pk_fast_ctx_push
        
        li      %r3, PPE42_IRQ_WATCHDOG
        
        _save_update_kernel_context %r3, %r6

        _liwa   %r6, TSR_WIS
        mttsr   %r6

        _lwzsd  %r6, __ppe42_watchdog_routine
        mtlr    %r6
        blrl

        b       .


#if 0
debug_handler:          

        ## PK does nothing upon reception of the debug interrupt other
        ## than calling the handler (if non-0). The debug handler is a
        ## fast-mode handler.

        _pk_fast_ctx_push

        li      %r3, PPE42_IRQ_DEBUG
        
        _save_update_kernel_context %r3, %r6

        _lwzsd  %r6, __ppe42_debug_routine
        cmpwi   %r6, 0
        mtlr    %r6
        beq     debug_exit
        blrl
        
debug_exit:     
        b       fast_exit_critical
#endif

__external_interrupt:   

        ## The non-critical interrupt handler entry point is re-entrant - A
        ## handler may allow preemption, which could cause another entry here.
        
        ## Entry invariants:
        ## 1. Non-critical interupts are disabled;
        ## 2. The SP points to a thread stack or the non-critical stack.

        ## Since fast-mode handlers can not use PK services or alter the
        ## machine context, the exit of a fast mode handler is a simple RF(C)I.
        
        ## Begin by pushing the fast context on the current stack.

        ## _pk_fast_ctx_push was called prior to branching here. No need to call it here.

        ## Load the base address for the external interrupt table

        ## TODO: This is HW Macro specific code that is responsible for finding the
        ## IRQ # and storing it in r4 (phantom IRQ's are assigned a value of EXTERNAL_IRQS).
        
        hwmacro_get_ext_irq        

        ## An active or phantom IRQ was found. 
        ## R4 has the IRQ number.
        ## The IRQ is converted into a pointer to an 8-byte handler
        ## structure, and the handler is dispatched. The call is made with the
        ## parameters:

        ## R3 = private
        ## R4 = irq

external_irq_found:  

        _save_update_kernel_context %r4, %r5
        slwi    %r3, %r4, 3                 //multiply the irq# by 8
        _liw    %r6, __ppe42_irq_handlers
        lwzx    %r5, %r6, %r3
        addi    %r3, %r3, 4
        lwzx    %r3, %r6, %r3
        mtlr    %r5
        blrl
                
        ## Pop the stack/RFI when (if) it returns here. 

fast_exit_noncritical:
        
        _pk_fast_ctx_pop_exit



/// \endcond
