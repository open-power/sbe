/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/measurement_l1.S $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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

        ## Check if it is called in MPIPL path, if yes then skip calling pibmem
        ## the below function which handles OPGC Align, Pibmem Repair, Pib Init
        lis     r6, 0xc000
        ori     r6, r6, 0x2040
        lvd     d0, 0x0(r6)
        bb1wi   r0, 14, skip_pibmem_repair_in_mpipl
        bl _pibmemRepair

skip_pibmem_repair_in_mpipl:
        # Lock PAU DPLL for faster SBE clock if Scratch Reg6 bit15
        lis     r5,  0x5               # load 0x50000 into r5
        lvd     d0,  0x3D(r5)          # load first 32bits from 5003D into r2
        bb1wi   r0,  15, skip_lock_pau_dpll # if bit15 is clear, then jump to
                                            # configure PAU DPLL, else continue
        bl _lock_pau_dpll

skip_lock_pau_dpll:

        # For Measurement SEEPROM, there is no L2 loader. The L1 loader will
        # directly load the base, which contains the application.
        li      r3, 0 # Dummy Instruction to push the copy_loop code in cache aligned boundary
        li      r3, 0 # Dummy Instruction to push the copy_loop code in cache aligned boundary
        li      r3, 0 # Dummy Instruction to push the copy_loop code in cache aligned boundary
        li      r3, 0 # Dummy Instruction to push the copy_loop code in cache aligned boundary
        _liw    %r3, SBE_MEASUREMENT_BASE_SECTION # Base Loader Section Location
        _liw    %r4, SBE_BASE_ORIGIN  # dest
        _liw    %r9, SBE_MEASUREMENT_BASE_ORIGIN

        lwz     r5, 4(r3)   # size of image in bytes
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
        # initialise empty section of the pibmem
        bl __zeroInitPibmemArea

skip_pibmem_init_memory_in_mpipl:
        # Update 50009 ( sbe status register) to show sbe in TPM HReset
        _liw     %r5, 0x50009
        lvd      d0, 0(r5)
        rlwinm   r0, r0, 0x0, 0x0, 0x1b   # Clear off the bits 28:29:30:31
        ori      r0, r0, SBE_CODE_MEASUREMENT_SEEPROM_TPM_RESET_MSG
        li       r1, 0x00
        stvd     d0, 0(r5)

        // Clear the deconfig bit for TPM for all chips
        // It is possible that PHYP did TPM deconfig. start cbs clears this
        // bit. But in MPIPL this bit is not cleared, so HB IPL will fail.
        // bit 12 of PU_SECURITY_SWITCH_REGISTER_SCOM1 clear TPM deconfig state
        // Reset the TPM using SBE local register 0xC0002020
        // 0xC0002030 is the W_OR Register, 0xC0002038 is W_CLR Register,
        // Set the Bit0 to 1 then to 0
        lis     r6, 0x0001
        ori     r6, r6, 0x0006
        lis     r0, 0x0008
        ori     r0, r0, 0x0
        li      r1, 0x0
        stvd    d0, 0(r6)   // putscom 0x10006 0x00080000_00000000

        lis     r6, 0xc000
        ori     r6, r6, 0x2030
        lis     r0, 0x8000
        ori     r0, r0, 0x0
        li      r1, 0x0
        stvd    d0, 0(r6)   // putscom 0xc0002030 0x80000000_00000000

        // Setup loop counter to delay the transition [2nsec * 0x50 = 160nsec]
        _liw    %r5, 0x50
        mtctr   r5
delay_loop:
        bdnz    delay_loop

        stvd    d0, 0x8(r6) // putscom 0xc0002038 0x8000000000000000

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
# Though this is inefficient, this is quick fix to solve the HW write of data
# which is not 8 byte alligned. All write which are not 8 byte alligned,
# translates to read-modify-write. So if code does a write for 4 byte, first
# HW will read 8 bytes. If there is no data written there earlier and random
# data is present it can cause ecc errors.
# This is only required for the first time write to pibmem, not in mpipl reset
# as well as not in Hreset flows.
# Measurment base will get written into pibmem from fff8_0000 till the len.
# remaining len for zero fill is (fff8_0000 + len) till ffff_ffff

__zeroInitPibmemArea:
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

_lock_pau_dpll:

        ########################################################################
        ######################## Putring perv_dpll_time ########################
        # Scan region & type
        #SCOMIN 01030005 000C000000000100
        _liw  %r3, 0x1030005
        lis   r4,  0xC
        li    r5,  0x100
        stvd  d4,  0x0(r3)

        # insert header
        # SCOMIN 0103F040 A5A5A5A5A5A5A5A5
        _liw  %r3, 0x103F000
        _liw  %r4, 0xA5A5A5A5
        _liw  %r5, 0xA5A5A5A5
        stvd  d4,  0x40(r3)

        # insert scan data 64bits
        # SCOMIN 0103F040 0000000000480014
        _liw  %r4, 0x0
        _liw  %r5, 0x00480014
        stvd  d4,  0x40(r3)

        # insert scan data 64bits
        # SCOMIN 0103F040 3101554000012488
        _liw  %r4, 0x31015540
        _liw  %r5, 0x00012488
        stvd  d4,  0x40(r3)

        # insert scan data 64bits
        # SCOMIN 0103F040 0000002000000000
        _liw  %r4, 0x00000020
        _liw  %r5, 0x0
        stvd  d4,  0x40(r3)

        # insert scan data 64bits
        # SCOMIN 0103F040 0009000286202AA8
        _liw  %r4, 0x00090002
        _liw  %r5, 0x86202AA8
        stvd  d4,  0x40(r3)

        # insert scan data 64bits
        # SCOMIN 0103F040 0000249100000004
        _liw  %r4, 0x00002491
        _liw  %r5, 0x00000004
        stvd  d4,  0x40(r3)

        # insert scan data 16bits
        # SCOMIN 0103F010 0000000000000000
        _liw  %r4, 0x0
        _liw  %r5, 0x0
        stvd  d4,  0x10(r3)

        # extract/read the header out and compare
        # SCOMOUT 0103F000 A5A5A5A5A5A5A5A5
        lvd   d0,  0x0(r3)
        _liw  %r4, 0xA5A5A5A5
        cmpwbne r0, r4, __pau_dpll_scan_hdr_compare_failed
        cmpwbne r1, r4, __pau_dpll_scan_hdr_compare_failed

        # clear scan region & type
        #SCOMIN 01030005 0000000000000000
        _liw  %r3, 0x1030005
        _liw  %r4, 0x0
        _liw  %r5, 0x0
        stvd  d4,  0x0(r3)
        ########################################################################

        # Calculate the Clock divider from PAU Freq which is 0x7B0 (1968MHz),
        # default frequency set in the sbe boot seeprom, we don't need to
        # change this basis any boot seeprom update. This will remain hard-coded
        # in the measurement seeprom.
        # (7B0/40 - 1) = 48 = 0x30 -> Clock Divider, in R0 Register
        # Receive clock delay is 0x40, in R1 Register
        li    r0,  0x30
        slwi  r0,  r0, 0x14        # We need clock divider in 0..11th bits
        li    r1,  0x40
        slwi  r1,  r1, 0xC         # We need round trip delay in 12..19th bits
        _liw  %r3, 0xc0000
        ########################################################################

        ########################## SPI CLOCK CONFIG ############################
        # SPI0, update clock config register
        lvd   d4,  0x3(r3)
        rlwinm r4, r4, 0x0, 0x14, 0x1F # Zero the first 20bits (12 + 8)
        or     r4, r4, r0              # Put the clock divider bits
        or     r4, r4, r1              # Put the round trip delay bits
        stvd   r4,  0x3(r3)            # Store the new values back into reg

        # SPI1, update clock config register
        lvd   d4,  0x23(r3)
        rlwinm r4, r4, 0x0, 0x14, 0x1F # Zero the first 20bits (12 + 8)
        or     r4, r4, r0              # Put the clock divider bits
        or     r4, r4, r1              # Put the round trip delay bits
        stvd   r4,  0x23(r3)            # Store the new values back into reg

        # SPI2, update clock config register
        lvd   d4,  0x43(r3)
        rlwinm r4, r4, 0x0, 0x14, 0x1F # Zero the first 20bits (12 + 8)
        or     r4, r4, r0              # Put the clock divider bits
        or     r4, r4, r1              # Put the round trip delay bits
        stvd   r4,  0x43(r3)            # Store the new values back into reg

        # SPI3, update clock config register
        lvd   d4,  0x63(r3)
        rlwinm r4, r4, 0x0, 0x14, 0x1F # Zero the first 20bits (12 + 8)
        or     r4, r4, r0              # Put the clock divider bits
        or     r4, r4, r1              # Put the round trip delay bits
        stvd   r4,  0x63(r3)            # Store the new values back into reg
        
        # SPI4, update clock config register
        lvd   d4,  0x83(r3)
        rlwinm r4, r4, 0x0, 0x14, 0x1F # Zero the first 20bits (12 + 8)
        or     r4, r4, r0              # Put the clock divider bits
        or     r4, r4, r1              # Put the round trip delay bits
        stvd   r4,  0x83(r3)            # Store the new values back into reg
        ########################################################################

        ########################################################################
        # PAU DPLL: Initialize to mode1
        # SCOMIN 01060052 A000000000000000
        _liw  %r3, 0x1060052
        _liw  %r4, 0xA0000000
        _liw  %r5, 0x0
        stvd  d4,  0x0(r3)

        # PAU DPLL: Write frequency settings
        # frquency_calculated = ((7B0 + 1) * 2)/25 = 0x9D
        # SCOMIN 01060051 09D009D009D00000
        _liw  %r3, 0x1060051
        _liw  %r4, 0x09D009D0
        _liw  %r5, 0x09D00000
        stvd  d4,  0x0(r3)

        # PAU DPLL: Switch to internal clocks (Bit 27)
        # SCOMIN 00050133 0000001000000000
        _liw  %r3, 0x50133
        _liw  %r4, 0x00000010
        _liw  %r5, 0x0
        stvd  d4,  0x0(r3)

        # PAU DPLL : Release reset (Bit 24)
        # SCOMIN 00050133 0000008000000000
        _liw  %r4, 0x00000080
        _liw  %r5, 0x0
        stvd  d4,  0x0(r3)
        ########################################################################

        ########################### START CLOCKS ###############################
        # Startclocks for PAU DPLL regions
        # Exit flush (set flushmode inhibit) (set bit 2)
        # SCOMIN 01000010 2000000000000000
        _liw  %r3, 0x1000010
        _liw  %r4, 0x20000000
        _liw  %r5, 0x0
        stvd  d4,  0x0(r3)

        # Clear Scan region type register
        # SCOMIN 01030005 0000000000000000
        _liw  %r3, 0x1030005
        _liw  %r4, 0x0
        #_liw  %r5, 0x0   # already at zero value, commenting this out to make opgc loop to cache align
        stvd  d4,  0x0(r3)

        # Setup all Clock Domains and Clock Types
        # SCOMIN  01030006 400800000000E000
        _liw  %r3, 0x1030006
        _liw  %r4, 0x40080000
        _liw  %r5, 0x0000E000
        stvd  d4,  0x0(r3)

        # Poll OPCG done bit to check for completeness
        # SCOMOUT 01000100 00C0000000000000, see if bit8 is set
        _liw  %r3, 0x1000100
        # Initialise a counter which decrements to 0, then break
        li    r0,  0x100
        mtctr r0              # set the counter for loop

poll_start_clock_opcg:
        lvd   d4,  0x0(r3)
        bb1wi r4,  8, start_clock_opcg_done   #if 1, then break
        bdnz  poll_start_clock_opcg           #decrement ctr and compare with 0
        bb0wi r4,  8, __pau_dpll_start_clock_opcg_failed  # trap is set

start_clock_opcg_done:
        ########################################################################
        # status of region 8 - dpllpau, bit 12 
        _liw  %r4, 0x00800000
        ###################### CLOCK RUNNING STATUS ############################
        # OPGC Done, check clock status SL, NSL, ARY
        # Check for clocks running SL
        # SCOMOUT  01030008 F9F7FFFFFFFFFFFF
        _liw  %r3, 0x1030008
        lvd   d0,  0x0(r3)
        and.  r8,  r4, r0        # if and of r4 & r0 is zero then fail
        beq   __pau_dpll_clock_running_status_fail       # trap is set

        # Check for clocks running NSL
        # SCOMOUT  01030009 F9F7FFFFFFFFFFFF
        _liw  %r3, 0x1030009
        lvd   d0,  0x0(r3)
        and.  r8,  r4, r0        # if and of r4 & r0 is zero then fail
        beq   __pau_dpll_clock_running_status_fail       # trap is set

        # Check for clocks running ARY
        # SCOMOUT  0103000A F9F7FFFFFFFFFFFF
        _liw  %r3, 0x103000A
        lvd   d0,  0x0(r3)
        and.  r8,  r4, r0        # if and of r4 & r0 is zero then fail
        beq   __pau_dpll_clock_running_status_fail       # trap is set

        # Clear clock region
        # SCOMIN   01030006 0000000000000000
        _liw  %r3, 0x1030006
        _liw  %r0, 0x0
        _liw  %r1, 0x0
        stvd  d0,  0x0(r3)

        # Enter flush (clear flushmode inhibit)
        # SCOMIN   01000024 0008000000000000
        _liw  %r3, 0x1000024
        _liw  %r0, 0x00080000
        _liw  %r1, 0x0
        stvd  d0,  0x0(r3)
        ########################################################################

        # Drop clock region fences for PAU DPLL
        # SCOMIN   01000021 0008000000000000
        _liw  %r3, 0x1000021
        _liw  %r0, 0x00080000
        _liw  %r1, 0x0
        stvd  d0,  0x0(r3)
        ########################################################################

        ########################### PAU DPLL LOCK ##############################
        # Check for PAU DPLL lock, check if bit 63 is set
        # SCOMOUT  01060055 0AA0000000000009
        _liw  %r3, 0x1060055
        # Initialise a counter which decrements to 0, then break
        li    r0,  0x2750     # Counter length at minimum to keep 10000 loops
        mtctr r0              # set the counter for loop

poll_pau_dpll_lock:
        lvd   d4,  0x0(r3)
        bb1wi r5,  31, pau_dpll_lock_done   #if 1, then break
        bdnz  poll_pau_dpll_lock  # decrement ctr and compare with 0
        bb0wi r5,  31, __pau_dpll_lock_failed  # trap is set

pau_dpll_lock_done:
        ########################################################################
        # PAU DPLL: Release test_enable and bypass
        # SCOMIN 00050133 0000006000000000
        _liw  %r3, 0x50133
        _liw  %r0, 0x00000060
        _liw  %r1, 0x0
        stvd  d0,  0x0(r3)

        # Raise clock region fences for PAU DPLL
        # SCOMIN 01000011 0008000000000000
        _liw  %r3, 0x1000011
        _liw  %r0, 0x00080000
        _liw  %r1, 0x0
        stvd  d0,  0x0(r3)
        ########################################################################

        blr                               # Back to the caller function

__pau_dpll_scan_hdr_compare_failed:
        li  r0, PAU_DPLL_SCAN_HDR_COMPARE_FAIL 
        b __panic
        .epilogue __pau_dpll_scan_hdr_compare_failed

__pau_dpll_start_clock_opcg_failed:
        li  r0, PAU_DPLL_START_CLOCK_OPCG_FAIL
        b __panic
        .epilogue __pau_dpll_start_clock_opcg_failed

__pau_dpll_clock_running_status_fail:
        li  r0, PAU_DPLL_CLOCK_RUNNING_STATUS_FAIL
        b __panic
        .epilogue __pau_dpll_clock_running_status_fail

__pau_dpll_lock_failed:
        li  r0, PAU_DPLL_LOCK_FAIL
        b __panic
        .epilogue __pau_dpll_lock_failed

__panic:
        mtedr  r0
        trap
        .epilogue __panic

#include "pibmem_repair/scom_repair_p10/pibmem_repair.S"
