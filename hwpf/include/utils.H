/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: $                                                             */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2014                        */
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
/**
 *  @file utils.H
 *
 *  @brief Defines common utility elements for FAPI2 use.
 */

#ifndef FAPI2_UTILS_H_
#define FAPI2_UTILS_H_

#include <return_code.H>

#ifdef __ASSEMBLER__

#ifndef ULL
#define ULL(x) x
#endif

#else

#ifndef ULL
#define ULL(x) x##ull

#endif

#endif // __ASSEMBLER

/// Create a multi-bit mask of \a n bits starting at bit \a b
#define BITS(b, n) ((ULL(0xffffffffffffffff) << (64 - (n))) >> (b))

/// Create a single bit mask at bit \a b
#define BIT(b) BITS((b), 1)

#ifdef _BIG_ENDIAN

#define revle16(x) x
#define revle32(x) x
#define revle64(x) x

#else

uint16_t revle16(uint16_t i_x);
uint32_t revle32(uint32_t i_x);
uint64_t revle64(uint64_t i_x);

#endif

namespace fapi2
{
///
/// @brief Delay this thread. Hostboot will use the nanoseconds parameter
/// and make a syscall to nanosleep. While in the syscall, the hostboot
/// kernel will continue to consume CPU cycles as it looks for a runnable
/// task.  When the delay time expires, the task becomes runnable and will soon
/// return from the syscall.  Callers of delay() in the hostboot environment
/// will likely have to know the mHz clock speed they are running on and
/// compute a non-zero value for i_nanoSeconds.
///
/// On the FSP, it was sometimes acceptable to just provide zero for the
/// sleep delay time, causing the task to yield its time slice. By the
/// time the calling task could run again, it was pretty certain enough
/// host cycles had past.  This is probably not acceptable in
/// the hostboot environment. Callers should calculate and provide a
/// sleep value in nanoseconds relative to host clock speed.
///
/// On FSP when VBU is the target, then the i_simCycles parameter will be
/// used instead.  The FSP needs to use the simdispatcher client/server
/// API and issue a command to the awan to advance the simulation the
/// specified number of cycles.
///
/// On SBE when __FAPI_DELAY_SIM__ is defined, then the i_simCycles parameter
/// will be used instead and will use the number passed.  The build parameter
/// __FAPI_DELAY_SIM_CYCLES__ allows the delay to adjust for the number of 
/// simulation cycles that the PPE engine is running at.  The delay algorithm
/// takes the i_simCycles parameter, subtracts the loop overhead instructions
/// times __FAPI_DELAY_SIM_CYCLES__ and then divides the remainder by the
/// number of loop instructions times __FAPI_DELAY_SIM_CYCLES__.
///
/// On SBE when __FAPI_DELAY_SIM__ is NOT defined, the nanoseconds parameter
/// will bus used to loop on the a call to pk_timebase32_get() function to 
/// determine the elapsed time.  pk_sleep() is NOT used as there are not
/// other threads to dispatch.
///
/// @param[in] i_nanoSeconds    nanoseconds to sleep
/// @param[in] i_simCycles      count of Awan cycles to advance
/// @param[in] i_fixed          Determination, for DFT, if this time is
///                             fixed or not. Defaults to non-fixed
///
/// @return ReturnCode. Zero on success, else platform specified error.
///
ReturnCode delay(uint64_t i_nanoSeconds,
                 uint64_t i_simCycles,
                 bool i_fixed = false);
}

#endif // FAPI2_UTILS_H_
