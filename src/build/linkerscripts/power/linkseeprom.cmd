/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/build/linkerscripts/power/linkseeprom.cmd $               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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
// Need to do this so that elf32-powerpc is not modified!
#undef powerpc
#include "sbe_link.H"
OUTPUT_FORMAT(elf32-powerpc);

#ifndef INITIAL_STACK_SIZE
#define INITIAL_STACK_SIZE 256
#endif

MEMORY
{
    pibmem : ORIGIN = SBE_BASE_ORIGIN, LENGTH = SBE_BASE_LENGTH
    #ifndef PIBMEM_ONLY_IMAGE
    seeprom : ORIGIN = SBE_SEEPROM_BASE_ORIGIN, LENGTH = 0x80000
    #endif
}

//MEMORY_REGION is a substitute constant used for pibmem or seeprom. Its value
//would be pibmem for pibmem_only image. Else it would be seeprom.

#ifdef PIBMEM_ONLY_IMAGE
//Everything should get in to pibmem
#define MEMORY_REGION pibmem
BASE_ORIGIN = SBE_BASE_ORIGIN;

#else
#define MEMORY_REGION seeprom
BASE_ORIGIN = SBE_SEEPROM_BASE_ORIGIN;
#endif

SECTIONS
{
    . = BASE_ORIGIN;
      _sbe_image_start_addr = . + SIZEOF_HEADERS;

      // TODO via RTC 149153
      // It seems first section in elf image is 8 byte aligned.
      // We need to verify this assumption.

      // Get 8 byte alligned address
      _sbe_image_start_addr = ( ( _sbe_image_start_addr % 8)  == 0 ) ? _sbe_image_start_addr : ( _sbe_image_start_addr + ( 8 - ( _sbe_image_start_addr % 8)));

     // Get the image offset after elf header
     _sbe_image_start_offset = _sbe_image_start_addr - .;
     _origin = . - 0;
     _pibmem_origin = SBE_BASE_ORIGIN;

    ////////////////////////////////
    // Header
    ////////////////////////////////
    .header : {
        _header_origin = .; _header_offset = . - _origin; KEEP(*(.header));
    } > MEMORY_REGION
    _header_size = . - _header_origin;
 
    #ifdef PIBMEM_ONLY_IMAGE
    //pkVector should be at top of pibmem memory after XIP header.
    .pkVectors ALIGN(0x200) : {
            *(.vectors)
    } > pibmem
    #endif

    // @TODO via RTC 136215
    // We have used allignment 0x200 so that it can be found
    // at fixed location. once otprom loader support is enabled.
    // alligment will not be required.

    ////////////////////////////////
    // LOADER_TEXT
    ////////////////////////////////
    .loader_text ALIGN(0x200): {
      _loader_text_origin = .; _loader_text_offset = . - _origin;
      //Loader text should not be loaded for pibmem only image
      #ifndef PIBMEM_ONLY_IMAGE
      KEEP(*(.loader_text));
      #endif
    } > MEMORY_REGION
     _loader_text_size = . - _loader_text_origin;

    // @TODO via RTC 136215
    // loader_text section should come after fixed and related
    // sections as we want fixed section at knowon offset. Currently we
    // have shared MEMORY_REGION start address ( IVPR register value ) with
    // multiple teams. So keeping loader_text as first section. Once
    // WE have otprom loader support, we will put loader_text at right
    // position

    ////////////////////////////////
    // FIXED
    ////////////////////////////////
    .fixed  ALIGN(0x200) : {
      _fixed_origin = .; _fixed_offset = . - _origin;
     KEEP(*(.fixed))
    } > MEMORY_REGION
     _fixed_size = . - _fixed_origin;

    ////////////////////////////////
    // text
    ////////////////////////////////
    .text ALIGN(8): {
         _text_origin = .; _text_offset = . - _origin;
         } > MEMORY_REGION
     _text_size = . - _text_origin;

   ////////////////////////////////
    // FIXED_TOC
    ////////////////////////////////
    .fixed_toc ALIGN(8) : {
        _fixed_toc_origin = .; _fixed_toc_offset = . - _origin;  KEEP(*(.fixed_toc));
    } > MEMORY_REGION
    _fixed_toc_size = . - _fixed_toc_origin;

   ////////////////////////////////
    // TOC
    ////////////////////////////////
    .toc ALIGN(4): {
        _toc_origin = .; _toc_offset = . - _origin;  KEEP(*(.toc));
    } > MEMORY_REGION
    _toc_size = . - _toc_origin;

   ////////////////////////////////
    // STRING
    ////////////////////////////////
    .strings : {
         _strings_origin = .; _strings_offset = . - _origin; KEEP(*(.strings));
    } > MEMORY_REGION
    _strings_size = . - _strings_origin;
    . = ALIGN(8);

    //No need to calculate seeprom size, if pibmem only image is being built
    #ifndef PIBMEM_ONLY_IMAGE
    _seeprom_origin = _origin;
    _seeprom_size = . - _origin;

    // TODO via RTC 149153
    // It seems when we jump across memory region, elf creates 32 byte offset.
    // We need to verify this assumption.

    _seeprom_size_with_elf_hdr = _seeprom_size + _sbe_image_start_offset;
    _seeprom_size = ( ( _seeprom_size_with_elf_hdr % 32)  == 0 ) ? _seeprom_size : ( _seeprom_size + ( 32 - (_seeprom_size_with_elf_hdr % 32)));


    . = _pibmem_origin;

    #else
    _seeprom_size = 0;
    . = ALIGN(4);
    #endif

    _base_origin = .;
    _base_offset = . - _base_origin + _seeprom_size;

    //We are at the beginning of the pibmem memory if seeprom image is being
    //built. So add .pkVector section here.
    #ifndef PIBMEM_ONLY_IMAGE
    .pkVectors ALIGN(32) : {
            *(.vectors)
     } > pibmem
     #endif

    .base . : {
                *(.text*) *(.eh_frame) *(.dtors*);
             } > pibmem

    . = ALIGN(8);
    // End of base section, which contains the Text Section
    _base_size = . - _base_origin;


    // Start of the data section
    _data_origin = .;
    _data_offset = . - _data_origin + _seeprom_size + _base_size;

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
                  KEEP(*(.ctors)) KEEP(*(.ctors.*))
                  ctor_end_address = .;
                  KEEP(*(.rodata*)) KEEP(*(.got2));
     } > pibmem
    _RODATA_SECTION_SIZE = . - _RODATA_SECTION_BASE;


    . = ALIGN(8); _DATA_SECTION_BASE = .;
    _SDA_BASE_ = .;
    .data . : {
              *(.data*)  *(.comment)
     } > pibmem
    .sdata  . : { *(.sdata*)  } > pibmem
    . = ALIGN(8);

    // End of data section
    _data_size = . - _data_origin;

    // We do not want to store bss section in sbe image as loader will take
    // care of it while loading image on PIBMEM. It will save us space in
    // SEEPROM. So define XIP image related variables here so that SBE image
    // finishes here.

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
