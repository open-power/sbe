/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/measurement_l1.S $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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

        .nolist
#include "pk.h"
#include "sbe_link.H"
        .list

### ****************************************************************************
### .loader_text - This section contains l1 loader code
###                @TODO via RTC 136315
###                It also contains vector code. We can remove vector code
###                once OTPROM support for simics is in.
### ****************************************************************************

        .section .loader_text, "ax", @progbits

        .global _pibmemRepair

__vectors:

        ############################################################
        # 0x0040 : System Reset
        ############################################################
        .org __vectors + 0x0040

__system_reset:
        b   __l1Loader


__l1Loader:
        # Update 50009 ( sbe status register) to show sbe in seeprom loader
        _liw     %r5, 0x50009
        lvd      d0, 0(r5)
        rlwinm   r0, r0, 0x0, 0x0, 0x1b   # Clear off the bits 28:29:30:31
        ori      r0, r0, SBE_CODE_MEASUREMENT_SEEPROM_START_MSG
        li       r1, 0x00
        stvd     d0, 0(r5)

        # If hreset of SBE, jump directly to kernel, as all the pibmem contents
        # are already populated
        ## TODO - Comeback on this later
        #_liw     %r3, 0x50008
        #lvd      d3,0(r3)
        #bb1wi    r3,13,jump_to_kernel

        ## Check if it is called in MPIPL path, if yes then skip calling pibmem
        ## the below function which handles OPGC Align, Pibmem Repair, Pib Init
        lis     r6, 0xc000
        ori     r6, r6, 0x2040
        lvd     d0, 0x0(r6)
        bb1wi   r0, 14, skip_pibmem_repair_in_mpipl
        bl _pibmemRepair

skip_pibmem_repair_in_mpipl:
        # For Measurement SEEPROM, there is no L2 loader. The L1 loader will
        # directly load the base, which contains the application.

        _liw    %r3, SBE_MEASUREMENT_BASE_SECTION # Base Loader Section Location
        _liw    %r4, SBE_BASE_ORIGIN  # dest
        _liw    %r9, SBE_MEASUREMENT_BASE_ORIGIN

        lwz     r5, 4(r3)   #size of image in bytes
        srawi   r5, r5, 3   # Number of double word transfers
        mtctr   r5          # set the counter for loop
        lwz     r8, 0(r3)   # offset of base section
        adde    r8, r8, r9  # add base address to offset to get absolute address

copy_loop:
        lvd    d28, 0(r8)
        stvd   d28, 0(r4)
        addi   r8, r8, 8
        addi   r4, r4, 8
        bdnz   copy_loop

        ## Check if it is called in MPIPL path, if yes then skip calling pibmem
        ## memory initialise
        lis     r6, 0xc000
        ori     r6, r6, 0x2040
        lvd     d0, 0x0(r6)
        bb1wi   r0, 14, skip_pibmem_init_memory_in_mpipl
        # initilaise empty section of the pibmem
        bl __initPibmemL1Area

skip_pibmem_init_memory_in_mpipl:
        ############################################################
        # SBE entry function is 4 byte number in image header
        ############################################################

jump_to_kernel:
        # Set the IVPR register. This is required so that interrupt vector table
        # points to pk interfaces. *(0xC0000160) = SBE_BASE_ORIGIN
        ###setup IVPR before jumping to kernel
        _liw    %r8, SBE_BASE_ORIGIN
        li      r9,0
        _liw    %r10, 0xC0000160
        stvd    d8,0(r10)

        ############################################################
        # SBE entry function is 4 byte number in image header
        ############################################################
        #Note we will jump to the kernel entry point in the base section
        _liw   %r3, SBE_MEASUREMENT_BASE_ORIGIN + SBE_KERNEL_ENTRY_HEADER_OFFSET
        lwz    r6, 0(r3)
        mtlr   r6
        blr

        .epilogue __l1Loader

# initialise the PIBMEM section after base with 0s.

__initPibmemL1Area:
        _liw   %r3, SBE_MEASUREMENT_BASE_SECTION # Loader Section Location
        _liw   %r4, SBE_BASE_ORIGIN  # dest
        lwz    r5, 4(r3)   #size of image in bytes
        _liw   %r6, SBE_MEASUREMENT_PIBMEM_LENGTH   #Max size of pibmem
        add    r4, r4, r5
        subf   r5, r5, r6
        srawi  r5, r5, 3   # Number of double word transfers
        mtctr  r5          # set the counter for loop
        li     r28, 0
        li     r29, 0
copy_loop_init:
        stvd   d28, 0(r4)
        addi   r4, r4, 8
        bdnz   copy_loop_init
        blr

#include "pibmem_repair/scom_repair_p10/pibmem_repair.S"
