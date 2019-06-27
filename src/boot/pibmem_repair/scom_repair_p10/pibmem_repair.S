/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/pibmem_repair/scom_repair_p10/pibmem_repair.S $      */
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
#include "sbe_link.H"
#include "pk.h"

.set d0 , 0
        .text
        .section .loader_text, "ax", @progbits


_pibmemRepair:

        ### Pibmem Init Sequence
        #R4 and R5 are used to create the pattern to be written
        #R6 and R7 are used to read the double word for comparison.
        #Care should be taken if you add or delete code here.
        #Registers values are sometime reused from the previous block

        #Release PCB Reset
        #SCOMIN   00050130 0x00000002_00000000
        _liw    %r3, 0x50130
        li      r4, 0x2
        li      r5, 0x0
        stvd    d4, 0(r3)

        #Initialize OPCG_ALIGN register with 1:1 scan ratio
        #SCOMIN   01030001 0x50000000_00000020
        _liw    %r3, 0x1030001
        lis     %r4, 0x5000
        li      r5, 0x20
        stvd    d4, 0x0(r3)

        #Set Chiplet Enable
        #SCOMIN   0005012A 0x80000000_00000000
        _liw    %r3, 0x5012A
        lis     r4, 0x8000
        li      r5, 0x0
        stvd    d4, 0(r3)

        #Drop TP Chiplet Fence Enable
        #SCOMIN   0005013A 0x00002000_00000000
        li      r4, 0x2000
        stvd    d4, 0x10(r3)

        #Drop PIB region fence
        #SCOMIN   01000021 0x20000000_00000000
        _liw    %r3, 0x1000021
        lis    r4, 0x2000
        stvd    d4, 0x0(r3)

        #Starting clock for PIB
        #Entering ...
        #Exit flush (set flushmode inhibit)
        #SCOMIN   01000010 0x20000000_00000000
        stvd    d4, -0x11(r3)

        #Clear Scan region type register
        #SCOMIN   01030005 0x00000000_00000000
        _liw    %r3, 0x1030005
        li      r4, 0x0
        stvd    d4, 0x0(r3)

        #Setup all Clock Domains and Clock Types
        #SCOMIN   01030006 0x42000000_0000E000
        lis     r4, 0x4200
        lis     r5, 0x1
        addi    r5, r5, -0x2000
        stvd    d4, 0x1(r3)


        #Note per PPE manual , for instruction cmpwbne , only RA is
        #guaranteed not be modified. So don't change the ordering
        #in the code. R4 and R5 are reused across some blocks

        #Poll OPCG done bit to check for completeness
        #SCOMOUT  01000100 0x00C00000_00000000
        _liw    %r3, 0x1000100
        lis     r4, 0xC0
        li      r5, 0
        lvd     d6, 0x0(r3)
        cmpwbne r4, r6, __compare_failed
        cmpwbne r5, r7, __compare_failed

        #Check for clocks running SL
        #SCOMOUT  01030008 0xF9FFFFFF_FFFFFFFF
        _liw    %r3, 0x1030008
        lis     r4, 0xFA00
        addi    r4, r4, -1
        li      r5, -1
        lvd     d6, 0x0(r3)
        cmpwbne r4, r6, __compare_failed
        cmpwbne r5, r7, __compare_failed

        #Check for clocks running NSLI
        #SCOMOUT  01030009 0xF9FFFFFF_FFFFFFFF
        lvd     d6, 0x1(r3)
        cmpwbne r4, r6, __compare_failed
        cmpwbne r5, r7, __compare_failed

        #Check for clocks running ARY
        #SCOMOUT  0103000A F9FFFFFFFFFFFFFF
        lvd     d6, 0x2(r3)
        cmpwbne r4, r6, __compare_failed
        cmpwbne r5, r7, __compare_failed

        #Clear clock region
        #SCOMIN   01030006 0x00000000_00000000
        _liw    %r3, 0x1030006
        lis     r4, 0x0
        li      r5, 0x0
        stvd    d4, 0x0(r3)

        #Enter flush (clear flushmode inhibit)
        #SCOMIN   01030020 0x20000000_00000000
        lis     r4, 0x2000
        stvd    d4, 0x1A(r3)

        ### Pibmem repair code here
        ## Row structure
        ################################################################
        ### 1B CMD ### 1B END ROW ### 2B PAD ### 4B ADDR ### 8B DATA ###
        ################################################################
        # NOTE - Saving LR into r4 and restore back at the end,
        # DONOT use R4 in any operation below.
        mflr     %r4                            # Store R4
        _liw     %r3, SBE_PIBMEM_REPAIR_SECTION # Pibmem Repair Section Location
        _liw     %r9, SBE_SEEPROM_BASE_ORIGIN
        lwz      r8, 0(r3)                      # offset of pibmem repair section

        # Check if R8 has non-zero offset, This is required if the repair
        # section is not loaded onto to binary, no point trying to parse which
        # is not loaded.
        cmplwi   r8, 0x0                        # compare r8 to 0
        beq      skip_parse_row                 # if offset is zero, skip row parsing
        adde     r8, r8, r9                     # add base address to offset to get absolute address

        # r8 is pointing to the first row of the table
parse_row:
        lvd      d0, 0(r8)                      # Get First 8bytes, r0 points to cmd, r1 points to addr
        srwi     %r3, %r0, 24                   # get the command byte
        addi     r8, r8, 8                      # Point to data offset
        lvd      d6, 0(r8)                      # r6 & r7 have the scom data
        ## Switch case for the operation to perform based on Command Byte
        ## r6/r7 contains the data and r1 contains the address
        cmpwibcl  1, 2, %r3, 0, __getscom_bootloader
        cmpwibcl  0, 2, %r3, 0, __putscom_bootloader
        addi     r8, r8, 8                      # point to the next row
        srwi     %r3, %r0, 16                   # fetch the second byte after command to check for end row
        andi.    %r3, %r3, 0x000000ff
        cmplwi   r3, 0x1                        # compare r3 with 1, if not equal got to parser another row
        bne      parse_row

        # End of Row Parsing
skip_parse_row:
        mtlr     %r4                            # restore LR from r4
        ######################################################

        ### Set chiplet enable
        _liw     %r3, 0x5001a                # Register 5001A
        lvd      d0, 0(r3)                   # read data from 0x5001A
        _liw     %r5, 0x80000000             # Set Bit 0 Mask
        or       r0, r0, r5                  # Read modify, set bit 0
        stvd     d0, 0(r3)                   # putscom 5001A Set bit 0

        ### Drop TP Chiplet Fence Enable
        lvd      d0, 0(r3)                   # read data from 0x5001A
        _liw     %r5, 0xFFFFDFFF             # Reset Bit 18 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 18
        stvd     d0, 0(r3)                   # putscom 5001A reset bit 18

        ### Drop clock region fences for PIB, NET
        _liw     %r3, 0x01000021             # Address wo_clear for 01000001
        _liw     %r4, 0x06000000             # Bit 5 and 6 Set for wo_clear
        _liw     %r5, 0x00000000             # clears bits in r5
        stvd     d4, 0(r3)                   # putscom 0x01000021 Set bit 5&6

        ### Start pervasive bus clocks (PIB & NET)
        _liw     %r3, 0x01030006             # Address
        _liw     %r0, 0x46000000             # load data higher 4 bytes
        _liw     %r1, 0x0000E000             # load data lower 4 bytes
        stvd     d0, 0(r3)                   # putscom 01030006 460000000000E000

        ### Set PCB Reset
        _liw     %r3, 0x50010
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0x2                    # Set Bit 30 Mask
        or       r0, r0, r5                  # Read modify, set bit 30
        stvd     d0, 0(r3)                   # putscom 50010 Set bit 30

        ### switch pcb mux settings to intermediate state
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0x00003000             # Set Bit 18 & 19 Mask
        or       r0, r0, r5                  # Read modify, set bit 18/19
        _liw     %r5, 0xFFFF7FFF             # Reset Bit 16 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 16
        stvd     d0, 0(r3)                   # putscom 50010 Set bit 18/19 and
                                             # reset bit 16

        ### switch pcb mux settings to pcb2pcb path
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0x00001000             # Set Bit 19 Mask
        or       r0, r0, r5                  # Read modify, set bit 19
        _liw     %r5, 0xFFFF5FFF             # Reset Bit 16 & 18 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 16 & 18
        stvd     d0, 0(r3)                   # putscom 50010 reset bit 16,18
                                             # and set bit 19

        ### Release PCB Reset
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0xFFFFFFFD             # Reset Bit 30 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 30
        stvd     d0, 0(r3)                   # putscom 50010 reset bit 30
 
        blr                                  # back to l1_loader
        .epilogue _pibmemRepair


__getscom_bootloader:
        ## r1 has the address and r6/r7 has the data
        lvd      d28, 0(r1)
        ## compare r28 with r6 and r29 with r7, all should match else halt
        cmpwbne r28, r6, __compare_failed
        cmpwbne r29, r7, __compare_failed
        blr
        .epilogue __getscom_bootloader

__putscom_bootloader:
        ## r1 has the address and r6/r7 has the data
        stvd     d6, 0(r1)
        blr
        .epilogue __putscom_bootloader

__compare_failed:
        trap 
        .epilogue __compare_failed
