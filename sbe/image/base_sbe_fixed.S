/// \file sbefixed.S
/// \brief code to generated fixed section in pibmem image
///
#include "p9_xip_image.h"
#include "proc_sbe_fixed.H"


	.macro	..checku, x:req, bits:req, err="Unsigned value too large"

	.if	(((\bits) <= 0) || ((\bits) > 63))
	.error	"The number of bits must be in the range 0 < bits < 64"
	.endif

	.iflt	(\x)
	.error  "An unsigned value is required here"
        .endif

	.ifgt	((\x) - (0xffffffffffffffff >> (64 - (\bits))))
	.error	"\err"
	.endif

	.endm


    .macro  ..check_u16, u16
	..checku (\u16), 16, "Unsigned immediate is larger than 16 bits"
    .endm


    .macro  ..set_default_space, s
	..check_u16 (\s)
    .set    _PGAS_DEFAULT_SPACE, (\s)
    .endm

    .section .fixed, "a", @progbits


    // Create the .fixed section
    .proc_sbe_fixed_system
    .proc_sbe_fixed_proc_chip
    .proc_sbe_fixed_perv
    .proc_sbe_fixed_core
    .proc_sbe_fixed_ex
    .proc_sbe_fixed_eq
