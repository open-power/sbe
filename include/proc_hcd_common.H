#ifndef __PROC_HCD_COMMON_H
#define __PROC_HCD_COMMON_H

/// \file proc_hcd_common.H
/// \brief Standard bit-manipulation macros (C and Assembler) for PORE code

/// \defgroup be64_bits Bit manipulation for 64-bit Big-Endian values
///
/// \note These macros only work in the assembler context because we build our
/// assemblers to do 64-bit arithmetic, which is required for PORE assembly.
///
/// @{

/// Create a multi-bit mask of \a n bits starting at bit \a b
#define BITS(b, n) ((ULL(0xffffffffffffffff) << (64 - (n))) >> (b))

/// Create a single bit mask at bit \a b
#define BIT(b) BITS((b), 1)

#endif  // __PROC_HCD_COMMON_H
