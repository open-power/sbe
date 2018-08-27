/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/pibmem_repair/scom_repair_axone/pibmem_repair.S $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2018                        */
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

        ### switch pcb mux settings to intermediate state
        _liw     %r3, 0x50010
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0x00003000             # Set Bit 18 & 19 Mask
        or       r0, r0, r5                  # Read modify, set bit 18/19
        _liw     %r5, 0xFFFF7FFF             # Reset Bit 16 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 16
        stvd     d0, 0(r3)                   # putscom 50010 Set bit 18/19 and
                                             # reset bit 16

        ### switch pcb mux settings to pib2pcb path
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0x00002000             # Set Bit 18 Mask
        or       r0, r0, r5                  # Read modify, set bit 18
        _liw     %r5, 0xFFFF6FFF             # Reset Bit 16 and 19 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 16 and 19
        stvd     d0, 0(r3)                   # putscom 50010 Set bit 18 and
                                             # reset bit 16/19
        
        ### release pcb reset
        lvd      d0, 0(r3)                   # read data from 0x50010
        _liw     %r5, 0xFFFFFFFD             # Reset Bit 30 Mask
        and      r0, r0, r5                  # Read Modify, reset bit 30
        stvd     d0, 0(r3)                   # putscom 50010 reset bit 30
 
        ### Pibmem repair code here
        ###########################
        
        ###########################

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

