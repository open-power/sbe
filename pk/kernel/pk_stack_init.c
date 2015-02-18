//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file pk_stack_init.c
/// \brief PK stack initialization
///
/// The entry points in this file are initialization routines - they are never
/// needed after PK initialization and their code space could be reclaimed by
/// the application after initialization if required.
///
/// This code was split out from "pk_init.c" because it may be needed in a
/// thread configuration if threads are being created dynamically. in an
/// interrupt-only configuration it is not needed after \c pk_initialize().

#include "pk.h"

/// Initialize a stack area.  
/// 
/// \param stack A pointer to the smallest legal address of the stack.  The
/// stack address is modified as the stack is aligned and initialized.
///
/// \param size A pointer to the size of the stack (in bytes).  The size is
/// modified as the stack is aligned and initialized.  At exit this is the
/// final usable stack area size aligned to the size of the PK_STACK_TYPE.
///
/// PK makes no assumptions about size or alignment of the area provided as a
/// stack, and carefully aligns and initializes the stack.  Regardless of how
/// the stack grows, the \a stack parameter is considered to be the lowest
/// legal address of the stack.  

int
__pk_stack_init(PkAddress *stack,
                 size_t     *size)
{
    PkAddress mask;
    size_t excess, i, count;
    PK_STACK_TYPE *p;

    if (PK_STACK_DIRECTION < 0) {

        // Stacks grow down.  The initial stack pointer is set to just above
        // the last allocated stack address.  This is legal for pre-decrement
        // stacks, otherwise the initial address is first brought into range
        // before alignment.  The stack is aligned downward, then the size is
        // adjusted to a multiple of the stack type.  Stacks are optionally
        // prepatterned. Alignment is assumed to be a power of 2.

        *stack += *size;

        if (!PK_STACK_PRE_DECREMENT) {
            *stack -= sizeof(PK_STACK_TYPE);
            *size -= sizeof(PK_STACK_TYPE);
        }

        mask = PK_STACK_ALIGNMENT - 1;
        excess = *stack & mask;
        *stack -= excess;
        *size -= excess;
        *size = (*size / sizeof(PK_STACK_TYPE)) * sizeof(PK_STACK_TYPE);

        if (PK_STACK_CHECK) {
            p = (PK_STACK_TYPE *)(*stack);
            count = *size / sizeof(PK_STACK_TYPE);
            for (i = 0; i < count; i++) {
                if (PK_STACK_PRE_DECREMENT) {
                    *(--p) = PK_STACK_PATTERN;
                } else {
                    *(p--) = PK_STACK_PATTERN;
                }
            }
        }

        __pk_stack_create_initial_frame(stack, size);

    } else {

        PK_PANIC(PK_UNIMPLEMENTED);
    }

    return PK_OK;
}

