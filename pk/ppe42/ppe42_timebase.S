/// \file ppe42_timebase.S
/// \brief PPE42-specific 64 bit timebase emulation
///
        .nolist
#include "pk.h"
        .list
        
/// \fn PkTimebase pk_timebase_get(void)
/// \brief Returns a 64 bit timebase
///
#ifdef DOXYGEN_ONLY
PkTimebase 
pk_timebase_get(void);
#endif
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------
        
/// \cond
        .global ppe42_64bit_timebase
        .global ppe42_tb_data
        .global_function pk_timebase_get

//Use the DEC for our timebase until we have a real timebase register (uses 9 instructions)
pk_timebase_get:

        //load the decrementer start time and change tag
        lvd     %r5, ppe42_tb_data@sda21(0)

        //load 64bit timebase accumulator
        lvd     %r3, ppe42_64bit_timebase@sda21(0)


        //load the current decrementer value
        mfdec   %r0

        //load the change tag again (should already be in the cache)
        lwz     %r7, ppe42_tb_data+4@sda21(0)

        //loop until the change tag is the same
        //cmplwbne %r6, %r7, pk_timebase_get
        cmplw   %r6, %r7
        bne     pk_timebase_get

        //calculate how much time has passed since the decrementer was started and store in r6
        subf    %r6, %r0, %r5

        //add the 32bit difference to the 64bit timebase accumulator
        addc    %r4, %r6, %r4
        addze   %r3, %r3

        blr

//enable this once we have a local timebase register in the model
#if 0

// use the local timebase register to keep more accurate time with just 6 instructions
// in the common case and 7 otherwise.
pk_timebase_get:

        //load the timebase local 32bit register address
        lwz     r5, timebase_local_reg@sda21(0)

        //load the 64bit timebase accumulator
        lvd	r3, ppe42_64bit_timebase@sda21(0)

        //read the local timebase register
        lwz	r5, 0(r5)

        //increment the upper 32 bits if the lower 32 bits have flipped
        cmplwbgt r5, r4, update_lower_32
        addi	r3, 1	//update upper 32

update_lower_32:
        //replace the lower 32bits with what we read from the local timebase register
        mr	r4, r6

        blr
#endif
/// \endcond
