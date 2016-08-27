/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/loader_l2_setup.S $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
