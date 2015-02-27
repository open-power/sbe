#ifndef __PPE42_IRQ_H__
#define __PPE42_IRQ_H__
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file ppe42_irq.h
/// \brief PPE42 interrupt handling for PK
///
/// Interrupt handling protocols and interrupt controller programming are
/// inherently non-portable, however PK defines APIs that may be useful among
/// different machines.
///


// Define pseudo-IRQ numbers for PPE42 built-in interrupts.  These numbers
// will appear in bits 16:23 of SPRG0 (__PkKernelContext) when the handlers
// are active

#define PPE42_EXC_MACHINE_CHECK         0x50
#define PPE42_EXC_DATA_STORAGE          0x53
#define PPE42_EXC_INSTRUCTION_STORAGE   0x54
#define PPE42_EXC_ALIGNMENT             0x56
#define PPE42_EXC_PROGRAM               0x57
#define PPE42_IRQ_DEC                   0x58
#define PPE42_IRQ_FIT                   0x59
#define PPE42_IRQ_WATCHDOG              0x5A


//  Unhandled exceptions default to a kernel panic, but the application can
//  override these definition.  Note that the exception area only allocates 32
//  bytes (8 instructions) to an unhandled exception, so any redefinition
//  would most likely be a branch to an application-defined handler.

#ifndef PPE42_MACHINE_CHECK_HANDLER
#define PPE42_MACHINE_CHECK_HANDLER        PK_PANIC(0x0200)
#endif

#ifndef PPE42_DATA_STORAGE_HANDLER
#define PPE42_DATA_STORAGE_HANDLER         PK_PANIC(0x0300)
#endif

#ifndef PPE42_INSTRUCTION_STORAGE_HANDLER
#define PPE42_INSTRUCTION_STORAGE_HANDLER  PK_PANIC(0x0400)
#endif

#ifndef PPE42_ALIGNMENT_HANDLER
#define PPE42_ALIGNMENT_HANDLER            PK_PANIC(0x0600)
#endif


////////////////////////////////////////////////////////////////////////////
//  PK API
////////////////////////////////////////////////////////////////////////////

#ifndef __ASSEMBLER__

/// An IRQ handler takes 2 arguments:
/// \arg \c arg - Private handler data installed by \c ssx_irq_setup() or
///                   \c ssx_irq_handler_set().
/// \arg \c irq     - The IRQ id; to enable a generic handler to manipulate
///                   its own interrupt status .

typedef void (*PkIrqHandler)(void* arg, PkIrqId irq);

/// Declare a subroutine as an IRQ handler

#define PK_IRQ_HANDLER(f) void f(void* arg, PkIrqId irq)

int pk_irq_setup(PkIrqId irq,
                  int      polarity,
                  int      trigger);

int pk_irq_handler_set(PkIrqId      irq,
                        PkIrqHandler handler,
                        void*         arg);

void pk_irq_enable(PkIrqId irq);
void pk_irq_disable(PkIrqId irq);
void pk_irq_statusclear(PkIrqId irq);

PK_IRQ_HANDLER(__ppe42_default_irq_handler);
PK_IRQ_HANDLER(__ppe42_phantom_irq_handler);


int
ppe42_fit_setup(int tcr_fp, PkIrqHandler handler, void* arg);


///  The address of the optional FIT interrupt handler

UNLESS__PPE42_IRQ_CORE_C__(extern)
volatile
PkIrqHandler __ppe42_fit_routine;


/// The private data of the optional FIT interrupt handler

UNLESS__PPE42_IRQ_CORE_C__(extern)
volatile
void* __ppe42_fit_arg;


int
ppe42_watchdog_setup(int tcr_wp, int tcr_wrc, 
                      PkIrqHandler handler, void* arg);


///  The address of the optional Watchdog interrupt handler

UNLESS__PPE42_IRQ_CORE_C__(extern)
volatile
PkIrqHandler __ppe42_watchdog_routine;


/// The private data of the optional Watchdog interrupt handler

UNLESS__PPE42_IRQ_CORE_C__(extern)
volatile
void* __ppe42_watchdog_arg;


int
ppe42_debug_setup(PkIrqHandler handler, void* arg);


///  The address of the optional Debug interrupt handler

UNLESS__PPE42_IRQ_CORE_C__(extern)
volatile
PkIrqHandler __ppe42_debug_routine;


/// The private data of the optional Watchdog interrupt handler

UNLESS__PPE42_IRQ_CORE_C__(extern)
volatile
void* __ppe42_debug_arg;


// Note: Why PK_IRQ_FAST2FULL (below) is implemented so strangely.
//
// I am adamant that I want to have a a macro in the 'C' environment to create
// these bridge functions. However the limitations of the C preprocessor and
// the intelligence of the GCC 'asm' facility consipre against a
// straightforward solution.  The only way that I was able to find to get
// naked assembly code into the output stream is to use 'asm' with simple
// strings - I couldn't make it work with any kind of argument, as 'asm' would
// reinterpret the arguments and resulting assembler code in various ways.  
//
// There is another alternative that I tried wherby I created a subroutine
// call and then filled in the subroutine body with 'asm' code.  However, the
// subroutine wrapper that GCC creates only works for PowerPC fast-mode
// handlers if GCC is invoked with optimization, which ensures that the
// wrapper doesn't touch the stack pointer or other registers. True, we'll
// always use optimization, but I did not want to have to make this
// requirement for using this macro.

///  This macro creates a 'bridge' handler that converts the initial fast-mode
///  IRQ dispatch into a call of a full-mode IRQ handler. The full-mode
///  handler is defined by the user (presumably as a \c C subroutine) and has
///  the same prototype (type PkIrqHandler) as the fast handler.
///
///  \param fast_handler This will be the global function name of the fast
///                      IRQ handler created by this macro. This is the symbol
///                      that should be passed in as the \a handler argument
///                      of \c pk_irq_setup() and \c pk_irq_handler_set().
///
///  \param full_handler This is the name of the user-defined full-mode
///                      handler which is invoked through this bridge.
///
/// \e BUG \e ALERT : Beware of passing the \c full_handler to IRQ setup
/// APIs. This won't be caught by the compiler (because the \c full_handler
/// has the correct prototype) and will lead to nasty bugs.  Always pass in
/// the \c fast_handler symbol to IRQ setup APIS.
///
/// The code stream injected into the GCC assembler output in response to
///
/// PK_IRQ_FAST2FULL(fast_handler, full_handler)
/// 
/// is (comments added for clarification) :
///
/// \code
/// .text 
/// .global fast_handler 
/// .align 5                   # Hard-coded PPE42 cache-line alignment
/// fast_handler = .           # Can't macro expand LABEL: - this is equivalent 
/// bl __pk_irq_fast2full     # The fast-mode to full-mode conversion sequence 
/// bl full_handler 
/// b  __pk_irq_full_mode_exit 
/// \endcode
///
/// The macro also declares the prototype of the fast handler:
///
/// \code
/// PK_IRQ_HANDLER(fast_handler);
/// \endcode
///

#define PK_IRQ_FAST2FULL(fast_handler, full_handler) \
    PK_IRQ_HANDLER(fast_handler); \
    __PK_IRQ_FAST2FULL(.global fast_handler, fast_handler = ., bl full_handler)

#define __PK_IRQ_FAST2FULL(global, label, call) \
asm(".text"); \
asm(#global); \
asm(".align 5"); \
asm(#label); \
asm("bl __pk_irq_fast2full"); \
asm(#call); \
asm("b __pk_irq_full_mode_exit");

#endif  /* __ASSEMBLER__ */

//  It's hard to be portable and get all of the definitions and headers in the
//  correct order.  We need to bring in the system IRQ header here.

#ifdef HWMACRO_GPE
#include "gpe_irq.h"
#else
#ifdef HWMACRO_STD
#include "std_irq.h"
#endif
#endif

/// \page ppe42_irq_macros_page PPE42 PK IRQ Assembler Macros
///
///
/// \section fast2full_asm Fast-Mode to Full-Mode Handler Conversion
///
/// This macro produces the calling sequence required to convert a
/// fast-mode interrupt handler to a full-mode interrupt handler. The
/// full-mode handler is implemented by another subroutine.  The
/// requirements for invoking this macro are:
///
/// \li The stack pointer and stack must be exactly as they were when the
/// fast-mode handler was entered.
///
/// \li No changes have been made to the MSR - the interrupt level must
///     remain disabled.
///
/// \li The handler owns the fast context and has not modified the other
///     register context.  The conversion process will not modify any
///     register in the fast context (other than the LR used for
///     subroutine linkage).
///
/// The final condition above means that the \a full_handler will
/// begin with the fast-mode context exactly as it was (save for LR)
/// at conversion, including the contents of GPR3-7 (the first 5
/// PowerPC ABI paramater passing registers) and the entire CR.
///
/// Forms:
///
/// \c _pk_irq_fast2full \a full_handler
/// \cond

#ifdef __ASSEMBLER__

        .macro  _pk_irq_fast2full full_handler
        bl      __pk_irq_fast2full
        bl      \full_handler
        b       __pk_irq_full_mode_exit
        .endm     

#endif  /* __ASSEMBLER__ */

/// \endcond
        
#ifndef __ASSEMBLER__


///  This structure holds the interrupt handler routine addresses and private
///  data.  Assembler code assumes the given structure layout, so any changes
///  to this structure will need to be reflected down into the interrupt
///  dispatch assembler code.

typedef struct {
    PkIrqHandler handler;
    void         *arg;
} Ppe42IrqHandler;


#ifdef STATIC_IRQ_TABLE

#define IRQ_HANDLER(func, arg) \
    {func, arg},

#define IRQ_HANDLER_DEFAULT \
    {__ppe42_default_irq_handler, 0},

#define EXTERNAL_IRQ_TABLE_END \
    {__ppe42_phantom_irq_handler, 0}\
};

#define EXTERNAL_IRQ_TABLE_START \
    Ppe42IrqHandler __ppe42_irq_handlers[EXTERNAL_IRQS + 1] = \
{

#else

#define EXTERNAL_IRQ_TABLE_START

#define IRQ_HANDLER(func, arg)

#define IRQ_HANDLER_DEFAULT

#define EXTERNAL_IRQ_TABLE_END

#endif /*STATIC_IRQ_TABLE*/

/// Interrupt handlers for real (implemented interrupts) plus one for the phantom interrupt handler
extern Ppe42IrqHandler __ppe42_irq_handlers[EXTERNAL_IRQS + 1];


/// The 'phantom interrupt' handler
///
/// A 'phantom' interrupt occurs when the interrupt handling code in the
/// kernel is entered, but no interrupt is found pending in the controller.
/// This is considered a serious bug, as it indictates a short window
/// condition where a level-sensitive interrupt has been asserted and then
/// quickly deasserted before it can be handled.

UNLESS__PPE42_IRQ_CORE_C__(extern)
Ppe42IrqHandler __ppe42_phantom_irq;
    
#endif  /* __ASSEMBLER__ */

#endif  /* __PPE42_IRQ_H__ */
