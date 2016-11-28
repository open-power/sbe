/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/src/plat/plat_utils.C $                              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
 *  @file plat_utils.C
 *  @brief Implements fapi2 common utilities
 */

#include <stdint.h>
#include <fapi2_attribute_service.H>
#include <attribute_ids.H>
#include <return_code.H>
#include <plat_trace.H>
#include <target.H>
#include <sbeutil.H>

namespace fapi2
{

    // Define own function rather than using PK function
    // This is required as PK function does not scale well for low
    // frequency till istep 2.7
    inline uint64_t delayCycles(uint64_t i_nanoSeconds )
    {
        return ( i_nanoSeconds/1000) * ( g_sbefreq /(1000*1000));
    }
    /// @brief Delay this thread.
    ///
    ReturnCode delay(uint64_t i_nanoSeconds, uint64_t i_simCycles, bool i_fixed /* = false*/)
    {
        // void statements to keep the compiler from complaining
        // about unused variables.
        static_cast<void>(i_nanoSeconds);
        static_cast<void>(i_simCycles);


#ifndef __FAPI_DELAY_SIM__

        PkTimebase  target_time;
        PkTimebase  current_time;
        PkMachineContext  ctx;


        // Only execute if nanoSeconds is non-zero (eg a real wait)
        if (i_nanoSeconds)
        {
            // The critical section enter/exit set is done to ensure the timebase
            // operations are non-interrupible.

            pk_critical_section_enter(&ctx);

            target_time = pk_timebase_get() + delayCycles( i_nanoSeconds);

            do
            {
                current_time = pk_timebase_get();
            } while (target_time > current_time);

            pk_critical_section_exit(&ctx);
        }
#else

        // Execute a tight loop that simply counts down the i_simCycles
        // value.

        // @todo This can might be optimized with a fused compare branch loop
        //    Note, though, that subwibnz instruction is optimized for word
        //      operations.   i_simCycles are uint64_t values so the upper
        //      word values needs to be accounted for.
        //
        //  Need to determine if this optimization is worth the effort.

#ifndef __FAPI_DELAY_PPE_SIM_CYCLES__
#define __FAPI_DELAY_PPE_SIM_CYCLES__ 8
#endif

        static const uint8_t NUM_OVERHEAD_INSTRS = 15;
        static const uint8_t NUM_LOOP_INSTRS = 4;
        static const uint64_t MIN_DELAY_CYCLES =
                ((NUM_OVERHEAD_INSTRS + NUM_LOOP_INSTRS) * __FAPI_DELAY_PPE_SIM_CYCLES__);

        uint64_t l_adjusted_simcycles;

        if (i_simCycles < MIN_DELAY_CYCLES)
            l_adjusted_simcycles = MIN_DELAY_CYCLES;
        else
            l_adjusted_simcycles = i_simCycles;

        uint64_t delay_loop_count =
            ((l_adjusted_simcycles - (NUM_OVERHEAD_INSTRS * __FAPI_DELAY_PPE_SIM_CYCLES__)) /
                        (NUM_LOOP_INSTRS * __FAPI_DELAY_PPE_SIM_CYCLES__));

        for (auto i = delay_loop_count; i > 0; --i) {
            // Force compiler not to optimize for loop
             asm("");
        }

#endif

        // replace with platform specific implementation
        return FAPI2_RC_SUCCESS;
    }

    ///
    /// @brief Queries the ATTR_NAME and ATTR_EC attributes
    ///
    void queryChipEcAndName(
        const Target < fapi2::TARGET_TYPE_PROC_CHIP > & i_target,
        fapi2::ATTR_NAME_Type& o_chipName, fapi2::ATTR_EC_Type& o_chipEc )
    {

        FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, i_target, o_chipName);

        FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, i_target, o_chipEc);
    }
};

#ifndef _BIG_ENDIAN

/// Byte-reverse a 16-bit integer if on a little-endian machine

uint16_t
revle16(uint16_t i_x)
{
    uint16_t rx;
    uint8_t *pix = (uint8_t*)(&i_x);
    uint8_t *prx = (uint8_t*)(&rx);

    prx[0] = pix[1];
    prx[1] = pix[0];

    return rx;
}

/// Byte-reverse a 32-bit integer if on a little-endian machine

uint32_t
revle32(uint32_t i_x)
{
    uint32_t rx;
    uint8_t *pix = (uint8_t*)(&i_x);
    uint8_t *prx = (uint8_t*)(&rx);

    prx[0] = pix[3];
    prx[1] = pix[2];
    prx[2] = pix[1];
    prx[3] = pix[0];

    return rx;
}


/// Byte-reverse a 64-bit integer if on a little-endian machine

uint64_t
revle64(const uint64_t i_x)
{
    uint64_t rx;
    uint8_t *pix = (uint8_t*)(&i_x);
    uint8_t *prx = (uint8_t*)(&rx);

    prx[0] = pix[7];
    prx[1] = pix[6];
    prx[2] = pix[5];
    prx[3] = pix[4];
    prx[4] = pix[3];
    prx[5] = pix[2];
    prx[6] = pix[1];
    prx[7] = pix[0];

    return rx;
}
#endif

