        .nolist
#include "pk.h"
        .list

### ****************************************************************************
### Do the initial setup for base loader
###
### ****************************************************************************

__base_loader_setup:

        ## Set up PowerPC EABI constant registers.

        _liw    %r2, _SDA2_BASE_
        _liw    %r13, _SDA_BASE_

        ## The stack pointer is initialized for use by the remainder of the
        ## initialization. The linker script defines the initial stack area.
        ##
        ## Stacks are always 8-byte aligned.  A '0' is stored at the
        ## stack pointer to indicate the end of the stack chain. Stack frames
        ## always consist of at least 8 bytes - the backchain pointer and the
        ## slot above the backchain pointer for the callee's LR.

        _liw    %r1, _BASE_LOADER_STACK_LIMIT
        _clrfield %r1, %r1, 3, 29 # 8-byte align
        li      %r3, 0
        li      %r4, 0
        stvd    %r3, -8(%r1)

        ## Call the base loader
        bl      base_loader

        .epilogue __base_loader_setup
