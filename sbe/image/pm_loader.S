
        .nolist
#include "pk.h"
#include "sbe_link.H"
        .list

### ****************************************************************************
### .loader_text - This section contains pm loader code
###                @TODO via RTC 136315
###                It also contains vector code. We can remove vector code
###                once OTPROM support for simics is in.
### ****************************************************************************

        .section .loader_text, "ax", @progbits

__vectors:

        ############################################################
        # 0x0040 : System Reset
        ############################################################
        .org __vectors + 0x0040

__system_reset:
        b   __pmLoader


__pmLoader:
        _liw     %r3, SBE_LOADER_BASE_SECTION # Base Loader Section Location
        _liw     %r4, SBE_LOADER_BASE_ORIGIN  # dest
        _liw     %r9, SBE_SEEPROM_BASE_ORIGIN
        lwz       r5, 4(r3)   #size of image in bytes
        li     r6, 3
        srw    r5, r5, r6   # Number of double word transfers
        mtctr   r5          # set the counter for loop
        lwz    r8, 0(r3)    # offset of baseloader section
        adde    r8, r8, r9  # add base address to offset to get absolute address

copy_loop:
        lvd    d28, 0(r8)
        stvd    d28, 0(r4)
        addi    r8, r8, 8
        addi    r4, r4, 8
        bdnz    copy_loop

        ############################################################
        # SBE entry function is 4 byte number in image header
        ############################################################

        _liw     %r3,  SBE_SEEPROM_BASE_ORIGIN + SBE_LOADER_ENTRY_HEADER_OFFSET
        lwz      r6, 0(r3)
        mtlr  r6
        blr

        .epilogue __pmLoader

