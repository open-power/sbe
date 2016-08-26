/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/boot/pibmem_repair.S $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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

        _liw     %r3, SBE_FIXED_SECTION                  # Fixed Section Location
        _liw     %r9, SBE_SEEPROM_BASE_ORIGIN
        lwz      r8, 0(r3)                               # offset of Fixed section
        adde     r8, r8, r9                              # add base address to offset to get absolute address
        lis      r5, 0x8
        ori      r5, r5 , 0x800B                         #point to repair reg 0 addr
        lis      r0, 0x4000                              # load repair #0 with 0x4000_0000 --> enable fence and repr_mux_sel is disabled
        stvd     d0, 0(r5)
        lvd      d0, 0(r8)                               # r8 was pointing to attributes
        stvd     d0, 1(r5)                               # store repair reg 1
        lvd      d0, 8(r8)                               # r8 was pointing to attributes
        stvd     d0, 2(r5)                               # store repair reg 2
        lvd      d0, 16(r8)                               # r8 was pointing to attributes
        stvd     d0, 3(r5)                               # store repair reg 3
        lis      r0, 0x8000                              # load repair #0 with 0x8000_0000 --> disable fence and repr_mux_sel is enabled
        stvd d0 , 0(r5)

###################################################################
########### Update DBG_LCL_REG to enable trace ####################
###################################################################

        lis      r5, 0xc000                              # local_reg_addr (R5 = C0000120)
        ori      r5, r5, 0x0120
        andi.    r0, r0, 0x0
        andi.    r1, r1, 0x0
        oris     r0, r0, 0x8A00                 # set the bit 0 to enable the debug blot-on
        stvd     d0, 0(r5)                              # DBG_LCL_REG

        blr                                             # back to pm_loader

