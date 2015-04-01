// Need to do this so that elf32-powerpc is not modified!
#undef powerpc

#ifndef INITIAL_STACK_SIZE
#define INITIAL_STACK_SIZE 256
#endif

OUTPUT_FORMAT(elf32-powerpc);

SECTIONS
{
    . = 0xfff00000;
    _sbe_base_origin = . - 0;
    ////////////////////////////////
    // Header
    ////////////////////////////////
   . = ALIGN(1); _header_origin = .; _header_offset = . - _sbe_base_origin; .header . : { *(.header) } _header_size = . - _header_origin;

    ////////////////////////////////
    // FIXED
    ////////////////////////////////
    . = ALIGN(512); _fixed_origin = .; _fixed_offset = . - _sbe_base_origin; .fixed . : { *(.fixed) }  _fixed_size = . - _fixed_origin;

   ////////////////////////////////
    // FIXED_TOC
    ////////////////////////////////
    . = ALIGN(8); _fixed_toc_origin = .; _fixed_toc_offset = . - _sbe_base_origin; .fixed_toc . : { *(.fixed_toc) } _fixed_toc_size = . - _fixed_toc_origin;

    ////////////////////////////////
    // IPL_TEXT
    ////////////////////////////////
    . = ALIGN(4); _ipl_text_origin = .; _ipl_text_offset = . - _sbe_base_origin; .ipl_text . : { *(.ipl_text) } _ipl_text_size = . - _ipl_text_origin;

    ////////////////////////////////
    // IPL_DATA
    ////////////////////////////////
    . = ALIGN(8); _ipl_data_origin = .; _ipl_data_offset = . - _sbe_base_origin; .ipl_data . : { *(.ipl_data) } _ipl_data_size = . - _ipl_data_origin;

    ////////////////////////////////
    // TEXT
    ////////////////////////////////
    . = ALIGN(4); _text_origin = .; _text_offset = . - _sbe_base_origin; .text . : {. = ALIGN(512); *(.vectors) *(.text) *(.eh_frame) } _text_size = . - _text_origin;

    ////////////////////////////////
    // DATA
    ////////////////////////////////
    . = ALIGN(8); _data_origin = .; _data_offset = . - _sbe_base_origin; .data . : { *(.data) *(.comment) *(.rodata*)} _data_size = . - _data_origin;

   ////////////////////////////////
    // TOC
    ////////////////////////////////
    . = ALIGN(4); _toc_origin = .; _toc_offset = . - _sbe_base_origin; .toc . : { *(.toc) } _toc_size = . - _toc_origin;

   ////////////////////////////////
    // STRING
    ////////////////////////////////
    . = ALIGN(1); _strings_origin = .; _strings_offset = . - _sbe_base_origin; .strings . : { *(.strings) } _strings_size = . - _strings_origin;

//    . = ALIGN(8);
//    _sbe_base_size = . - _sbe_base_origin;
//    _sbe_base_end = . - 0;


    _RODATA_SECTION_BASE = .;


    _SDA2_BASE_ = .;

    _RODATA_SECTION_SIZE = . - _RODATA_SECTION_BASE;

    // SDA sections .sdata and .sbss must be adjacent to each
    // other.  Our SDATA sections are small so we'll use strictly positive
    // offsets. 

    _SDA_BASE_ = .;

    .sbss   . : { *(.sbss)   }
    .sdata   . : { *(.sdata)  }


   _PK_INITIAL_STACK_LIMIT = .;
   . = . + INITIAL_STACK_SIZE;
   _PK_INITIAL_STACK = . - 1;


    . = ALIGN(8);
    _sbe_base_size = . - _sbe_base_origin;
    _sbe_base_end = . - 0;


}
