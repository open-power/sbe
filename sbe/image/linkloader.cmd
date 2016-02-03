
// Need to do this so that elf32-powerpc is not modified!
#undef powerpc

#ifndef BASE_LOADER_STACK_SIZE
#define BASE_LOADER_STACK_SIZE 128
#endif
#include <sbe_link.H>

OUTPUT_FORMAT(elf32-powerpc);

MEMORY
{
 sram : ORIGIN = SBE_LOADER_BASE_ORIGIN, LENGTH = SBE_LOADER_BASE_LENGTH
}

SECTIONS
{
    . = SBE_LOADER_BASE_ORIGIN;

    ////////////////////////////////
    // Read-only Data
    ////////////////////////////////

    . = ALIGN(8);
    _RODATA_SECTION_BASE = .;

   .text . : { *(.text) } > sram
   .data  . : { *(.data) } > sram

    // SDA2 constant sections .sdata2 and .sbss2 must be adjacent to each
    // other.  Our SDATA sections are small so we'll use strictly positive
    // offsets.

    _SDA2_BASE_ = .;
   .sdata2 . : { *(.sdata2) } > sram
   .sbss2  . : { *(.sbss2) } > sram

   // Other read-only data.

   .rodata . : { *(.rodata*) *(.got2) } > sram

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
    .sdata  . : { *(.sdata)  } > sram
    .sbss   . : { *(.sbss)   } > sram

    // Other read-write data
    // It's not clear why boot.S is generating empty .glink,.iplt

   .rela   . : { *(.rela*) } > sram
   .rwdata . : { *(.data) *(.bss) } > sram

   _BASE_LOADER_STACK_LIMIT = .;
   . = . + BASE_LOADER_STACK_SIZE;
   _BASE_LOADER_STACK_LIMIT = . - 1;

    . = ALIGN(8);
    _loader_end = . - 0;

}
