//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file fapi_sbe_fixed.H
/// \brief definition of functions, attributes and rings going into fixed 
///
/// Contains struct ProcSbeFixed which contains functions, rings and
/// attributes whose pointers are stored in the fixed and fixed_toc section

#ifndef __PROC_SBE_FIXED_H__
#define __PROC_SBE_FIXED_H__



#ifdef __ASSEMBLER__

	.macro	proc_sbe_fixed_uint64, symbol:req, elements=1
   	.global \symbol
         //.weak \symbol
__\symbol\():
	.rept	(\elements)
	.quad 0
	.endr
	
	.xip_toc \symbol, SBE_XIP_UINT64, __\symbol, (\elements)

	.pushsection .fixed_toc
        .space	 8
	.popsection

	.endm

	.macro remove_from_mc_group dx: req, addr:req, px:req
        // Current multicast group is in bits 3:5
        ldandi  \dx, \addr, \px, BITS(3,3)
        // Need to write 0xFC and previous group (bits 19:21) to current group
        rotrdi  \dx, \dx, 16
        ori     \dx, \dx, BITS(0,6)
        std     \dx, \addr, \px
        .endm


#define PROC_SBE_FIXED_UINT64(symbol) proc_sbe_fixed_uint64 symbol

#define PROC_SBE_FIXED_UINT64_VECTOR(symbol, elements) \
             proc_sbe_fixed_uint64 symbol, elements

#else  // __ASSEMBLER__

#define PROC_SBE_FIXED_UINT64(symbol) uint64_t symbol
#define PROC_SBE_FIXED_UINT64_VECTOR(symbol, elements) \
		uint64_t symbol[elements]

#endif // __ASSEMBLER__

#ifdef __ASSEMBLER__
         .macro	.proc_sbe_fixed
	     .section   .fixed, "a", @progbits
	     .balign    8
#else

typedef struct {

#endif

    // Procedure control variables listed in istep order

    //*************************************************************//
    //*                         READ THIS!!!                      *//
    //*                                                           *//
    //*   The locations in this file are "fixed", meaning that    *//
    //*   nothing may be deleted, and new things may only be      *//
    //*   added to the very END of the file. This means that      *//
    //*   some entries are not going to be in the correct order   *//
    //*                                                           *//
    //*************************************************************//

    PROC_SBE_FIXED_UINT64(base_ppe_demo_func);
    PROC_SBE_FIXED_UINT64(pk_trace_wrap3);
    PROC_SBE_FIXED_UINT64(combined_good);
    PROC_SBE_FIXED_UINT64(combined_good_l3);
    //const char *pk_trace_wrap;


#ifdef __ASSEMBLER__
	.endm
#else
} ProcSbeFixed;
#endif


#endif  // __PROC_SBE_FIXED_H__
