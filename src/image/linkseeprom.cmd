/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/image/linkseeprom.cmd $                                   */
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
#include <sbe_link.H>
OUTPUT_FORMAT(elf32-powerpc);

#ifndef INITIAL_STACK_SIZE
#define INITIAL_STACK_SIZE 256
#endif

MEMORY
{
    pibmem : ORIGIN = SBE_BASE_ORIGIN, LENGTH = SBE_BASE_LENGTH
    seeprom : ORIGIN = SBE_SEEPROM_BASE_ORIGIN, LENGTH = 0x40000
}

SECTIONS
{
    . = SBE_SEEPROM_BASE_ORIGIN;
      _sbe_image_start_addr = . + SIZEOF_HEADERS;

      // TODO via RTC 149153
      // It seems first section in elf image is 8 byte aligned.
      // We need to verify this assumption.

      // Get 8 byte alligned address
      _sbe_image_start_addr = ( ( _sbe_image_start_addr % 8)  == 0 ) ? _sbe_image_start_addr : ( _sbe_image_start_addr + ( 8 - ( _sbe_image_start_addr % 8)));

     // Get the image offset after elf header
     _sbe_image_start_offset = _sbe_image_start_addr - .;
     _seeprom_origin = . - 0;
     _pibmem_origin = SBE_BASE_ORIGIN;

    ////////////////////////////////
    // Header
    ////////////////////////////////
    .header : {
        _header_origin = .; _header_offset = . - _seeprom_origin; *(.header);
    } > seeprom
    _header_size = . - _header_origin;

    // @TODO via RTC 136215
    // We have used allignment 0x1000 so that it can be found
    // at fixed location. once otprom loader support is enabled.
    // alligment will not be required.

    ////////////////////////////////
    // LOADER_TEXT
    ////////////////////////////////
    .loader_text ALIGN(0x1000): {
      _loader_text_origin = .; _loader_text_offset = . - _seeprom_origin;
      *(.loader_text);
    } > seeprom
     _loader_text_size = . - _loader_text_origin;

    // @TODO via RTC 136215
    // loader_text section should come after fixed and related
    // sections as we want fixed section at knowon offset. Currently we
    // have shared SEEPROM start address ( IVPR register value ) with
    // multiple teams. So keeping loader_text as first section. Once
    // WE have otprom loader support, we will put loader_text at right
    // position

    ////////////////////////////////
    // FIXED
    ////////////////////////////////
    .fixed  ALIGN(0x200) : {
      _fixed_origin = .; _fixed_offset = . - _seeprom_origin;
     *(.fixed)
    } > seeprom
     _fixed_size = . - _fixed_origin;

   ////////////////////////////////
    // FIXED_TOC
    ////////////////////////////////
    .fixed_toc ALIGN(8) : {
        _fixed_toc_origin = .; _fixed_toc_offset = . - _seeprom_origin;  *(.fixed_toc);
    } > seeprom
    _fixed_toc_size = . - _fixed_toc_origin;

   ////////////////////////////////
    // TOC
    ////////////////////////////////
    .toc ALIGN(4): {
        _toc_origin = .; _toc_offset = . - _seeprom_origin;  *(.toc);
    } > seeprom
    _toc_size = . - _toc_origin;

   ////////////////////////////////
    // STRING
    ////////////////////////////////
    .strings : {
         _strings_origin = .; _strings_offset = . - _seeprom_origin; *(.strings);
    } > seeprom
    _strings_size = . - _strings_origin;

    .text ALIGN(8): {
         _text_origin = .; _text_offset = . - _seeprom_origin;
        *\libcore.a:(.text* ) *\libcache.a:(.text* ) *\libperv.a:(.text* )} > seeprom
     _text_size = . - _text_origin;

    _seeprom_size = . - _seeprom_origin;

    // TODO via RTC 149153
    // It seems when we jump across memory region, elf creates 32 byte offset.
    // We need to verify this assumption.

    _seeprom_size_with_elf_hdr = _seeprom_size + _sbe_image_start_offset;
    _seeprom_size = ( ( _seeprom_size_with_elf_hdr % 32)  == 0 ) ? _seeprom_size : ( _seeprom_size + ( 32 - (_seeprom_size_with_elf_hdr % 32)));


    . = _pibmem_origin;
    _base_origin = .;
    _base_offset = . - _base_origin + _seeprom_size;

    .pkVectors ALIGN(32) : {
            *(.vectors)
     } > pibmem

    .base . : {
                *(.text*) *(.eh_frame) *(.dtors*);
             } > pibmem

    . = ALIGN(8);
    _RODATA_SECTION_BASE = .;

    // SDA2 constant sections .sdata2 and .sbss2 must be adjacent to each
    // other.  Our SDATA sections are small so we'll use strictly positive
    // offsets.

    _SDA2_BASE_ = .;
   .sdata2 . : { *(.sdata2*) } > pibmem
   .sbss2  . : { *(.sbss2*) } > pibmem

    . = ALIGN(8);
    .rodata . : {
                  ctor_start_address = .;
                  *(.ctors) *(.ctors.*)
                  ctor_end_address = .;
                  *(.rodata*) *(.got2);
     } > pibmem
    _RODATA_SECTION_SIZE = . - _RODATA_SECTION_BASE;


    . = ALIGN(8); _DATA_SECTION_BASE = .;
    _SDA_BASE_ = .;
    .data . : {
              *(.data*)  *(.comment)
     } > pibmem
    .sdata  . : { *(.sdata*)  } > pibmem
    . = ALIGN(8);

    // We do not want to store bss section in sbe image as laoder will take
    // care of it while loading image on PIBMEM. It will save us space in
    // SEEPROM. So define XIP image related variables here so that SBE image
    // finishes here.

    _base_size = . - _base_origin;
    _pibmem_size = . - _pibmem_origin;
    _sbe_image_size = _seeprom_size  + ( . - _pibmem_origin );

    _sbss_start = .;
    .sbss   . : {
    *(.bss*) *(.sbss*);
    . = ALIGN(8);
   } > pibmem
    _sbss_end = .;

    . = ALIGN(8);
    _sbss_size = SIZEOF(.sbss);

    . = ALIGN(8);
   _PK_INITIAL_STACK_LIMIT = .;
   . = . + INITIAL_STACK_SIZE;
   _PK_INITIAL_STACK = . - 1;
    . = ALIGN(8);

}
