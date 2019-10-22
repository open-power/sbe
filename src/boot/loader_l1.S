/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/loader_l1.S $                                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
        _liw     %r0, SBE_CODE_SEEPROM_START_MSG
        li       r1, 0x00
        stvd     d0, 0(r5)

        # If hreset of SBE, jump directly to kernel, as all the pibmem contents
        # are already populated
        _liw     %r3, 0x50008
        lvd      d3,0(r3)
        bb1wi    r3,13,jump_to_kernel
        #ifndef SBE_MEASUREMENT_SUPPORT
        bl _pibmemRepair
        #endif

        _liw     %r3, SBE_LOADER_BASE_SECTION # Base Loader Section Location
        _liw     %r4, SBE_LOADER_BASE_ORIGIN  # dest
        _liw     %r9, SBE_SEEPROM_BASE_ORIGIN
        lwz       r5, 4(r3)   #size of image in bytes
        srawi    r5, r5, 3   # Number of double word transfers
        mtctr   r5          # set the counter for loop
        lwz    r8, 0(r3)    # offset of l2loader section
        adde    r8, r8, r9  # add base address to offset to get absolute address

copy_loop:
        lvd    d28, 0(r8)
        stvd    d28, 0(r4)
        addi    r8, r8, 8
        addi    r4, r4, 8
        bdnz    copy_loop

        # initilaise empty section of L1 loader with 0
        bl __initPibmemL1Area
        ############################################################
        # SBE entry function is 4 byte number in image header
        ############################################################

        _liw     %r3,  SBE_SEEPROM_BASE_ORIGIN + SBE_LOADER_ENTRY_HEADER_OFFSET
        lwz      r6, 0(r3)
        mtlr  r6
        blr

        .epilogue __l1Loader

# initialise the PIBMEM section for l1 loader with 0

__initPibmemL1Area:
        _liw     %r3, SBE_LOADER_BASE_SECTION # Base Loader Section Location
        _liw     %r4, SBE_LOADER_BASE_ORIGIN  # dest
        lwz       r5, 4(r3)   #size of image in bytes
        _liw     %r6, SBE_LOADER_BASE_LENGTH   #Max size of loader
        add r4, r4, r5
        subf r5, r5, r6
        srawi    r5, r5, 3   # Number of double word transfers
        mtctr   r5          # set the counter for loop
        li r28, 0
        li r29, 0
copy_loop_init:
        stvd    d28, 0(r4)
        addi    r4, r4, 8
        bdnz    copy_loop_init

        blr

jump_to_kernel:
        # setup IVPR before jumping to kernel
        _liw    %r8, SBE_BASE_ORIGIN
        li      r9,0
        _liw    %r10, 0xC0000160
        stvd    d8,0(r10)
        ############################################################
        # SBE entry function is 4 byte number in image header
        ############################################################

        _liw     %r3,  SBE_SEEPROM_BASE_ORIGIN + SBE_KERNEL_ENTRY_HEADER_OFFSET
        lwz      r6, 0(r3)
        mtlr  r6
        blr

#include "pibmem_repair/scom_repair_p10/pibmem_repair.S"
