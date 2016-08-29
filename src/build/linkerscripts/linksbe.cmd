/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/build/linkerscripts/linksbe.cmd $                         */
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

// Need to do this so that elf32-powerpc is not modified!
#undef powerpc

#ifndef INITIAL_STACK_SIZE
#define INITIAL_STACK_SIZE 256
#endif
#include <sbe_link.H>

OUTPUT_FORMAT(elf32-powerpc);

MEMORY
{
 // increasing ram size as workaround so that pibmem image compilation
 // doe not fail.
 // TODO: Reducing the SBE_BASE_ORIGIN by 0x1F000 to reduce the size of
 // generated sbe_pibmem.bin. Just a workaround to use pibmem for poweron
 sram : ORIGIN = SBE_BASE_ORIGIN - 0x1F000, LENGTH = SBE_BASE_LENGTH + 0x1F000
}

SECTIONS
{
    . = SBE_BASE_ORIGIN - 0x1F000;

    .vectors : {. = ALIGN(512); *(.vectors)} > sram
    .fixed . : {. = ALIGN(512); *(.fixed) } > sram
    .text . : {. = ALIGN(512); *(.text)} > sram

    ////////////////////////////////
    // Read-only Data
    ////////////////////////////////

    . = ALIGN(8);
    _RODATA_SECTION_BASE = .;

    // SDA2 constant sections .sdata2 and .sbss2 must be adjacent to each
    // other.  Our SDATA sections are small so we'll use strictly positive
    // offsets.

    _SDA2_BASE_ = .;
   .sdata2 . : { *(.sdata2*) } > sram
   .sbss2  . : { *(.sbss2*) } > sram

   // Other read-only data.

    . = ALIGN(8);
    .rodata . : { ctor_start_address = .;
                  *(.ctors) *(.ctors.*)
                  ctor_end_address = .;
                  *(rodata*) *(.got2) } > sram

    _RODATA_SECTION_SIZE = . - _RODATA_SECTION_BASE;

    ////////////////////////////////
    // Read-write Data
    ////////////////////////////////

    . = ALIGN(8);
    _DATA_SECTION_BASE = .;

    // SDA sections .sdata and .sbss must be adjacent to each
    // other.  Our SDATA sections are small so we'll use strictly positive
    // offsets.

    _SDA_BASE_ = .;
    .sdata  . : { *(.sdata*)  } > sram
    _sbss_start = .;
    .sbss   . : { *(.sbss*)   } > sram
    _sbss_end = .;

    // Other read-write data
    // It's not clear why boot.S is generating empty .glink,.iplt

   .rela   . : { *(.rela*) } > sram
   .rwdata . : { *(.data*) *(.bss*) } > sram

    . = ALIGN(8);
   _PK_INITIAL_STACK_LIMIT = .;
   . = . + INITIAL_STACK_SIZE;
   _PK_INITIAL_STACK = . - 1;

    . = ALIGN(8);
    _sbe_end = . - 0;

}
