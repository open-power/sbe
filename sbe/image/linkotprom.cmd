
// Need to do this so that elf32-powerpc is not modified!
#undef powerpc

#include <sbe_link.H>

OUTPUT_FORMAT(elf32-powerpc);

MEMORY
{
 sram : ORIGIN = OTPROM_ORIGIN, LENGTH = OTPROM_BLOCK_SIZE
}

SECTIONS
{
    . = OTPROM_ORIGIN;
    .text  . : { *(.text)}
    . = OTPROM_FIXED_SIZE;
    .fixed . : { *(.fixed)}
}
