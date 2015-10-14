
/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: $                                                             */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2011,2014              */
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
#include <plat_trace.H>
#include <return_code.H>

#ifndef __PPE__
#include <error_info.H>
#endif

namespace fapi2
{

#ifndef __PPE__
    ///
    /// @brief Log an error.
    ///
    void logError(
        fapi2::ReturnCode & io_rc,
        fapi2::errlSeverity_t i_sev = fapi2::FAPI2_ERRL_SEV_UNRECOVERABLE,
        bool i_unitTestError = false )
    {
        // To keep the compiler from complaing about i_sevbeing unused.
        static_cast<void>(i_sev);
        static_cast<void>(i_unitTestError);

        FAPI_DBG("logging 0x%lx.", uint64_t(io_rc));

        // Iterate over the vectors and output what is in them.
        const ErrorInfo* ei = io_rc.getErrorInfo();

        FAPI_DBG("ffdcs: %lu", ei->iv_ffdcs.size());
        for( auto i = ei->iv_ffdcs.begin(); i != ei->iv_ffdcs.end(); ++i )
        {
            uint32_t sz;
            (*i)->getData(sz);
            FAPI_DBG("\tid: 0x%x size %d", (*i)->getFfdcId(), sz);
        }

        FAPI_DBG("hwCallouts: %lu", ei->iv_hwCallouts.size());
        for( auto i = ei->iv_hwCallouts.begin(); i != ei->iv_hwCallouts.end();
             ++i )
        {
            FAPI_DBG("\thw: %d pri %d target: 0x%lx",
                     (*i)->iv_hw, (*i)->iv_calloutPriority,
                     (*i)->iv_refTarget.get());
        }

        FAPI_DBG("procedureCallouts: %lu", ei->iv_procedureCallouts.size());
        for( auto i = ei->iv_procedureCallouts.begin();
             i != ei->iv_procedureCallouts.end(); ++i )
        {
            FAPI_DBG("\tprocedure: %d pri %d",
                     (*i)->iv_procedure, (*i)->iv_calloutPriority);
        }

e         FAPI_DBG("busCallouts: %lu", ei->iv_busCallouts.size());
        for( auto i = ei->iv_busCallouts.begin(); i != ei->iv_busCallouts.end();
             ++i )
        {
            FAPI_DBG("\tbus: t1: 0x%lx t2: 0x%lx pri: %d",
                     (*i)->iv_target1.get(), (*i)->iv_target2.get(),
                     (*i)->iv_calloutPriority);
        }


        FAPI_DBG("cdgs: %lu", ei->iv_CDGs.size());
        for( auto i = ei->iv_CDGs.begin(); i != ei->iv_CDGs.end(); ++i )
        {
            FAPI_DBG("\ttarget: 0x%lx co: %d dc: %d gard: %d pri: %d",
                     (*i)->iv_target.get(),
                     (*i)->iv_callout,
                     (*i)->iv_deconfigure,
                     (*i)->iv_gard,
                     (*i)->iv_calloutPriority);

        }

        FAPI_DBG("childrenCDGs: %lu", ei->iv_childrenCDGs.size());
        for( auto i = ei->iv_childrenCDGs.begin();
             i != ei->iv_childrenCDGs.end(); ++i )
        {
            FAPI_DBG("\tchildren: parent 0x%lx co: %d dc: %d gard: %d pri: %d",
                     (*i)->iv_parent.get(),
                     (*i)->iv_callout,
                     (*i)->iv_deconfigure,
                     (*i)->iv_gard,
                     (*i)->iv_calloutPriority);
        }

        FAPI_DBG("traces: %lu", ei->iv_traces.size());
        for( auto i = ei->iv_traces.begin(); i != ei->iv_traces.end(); ++i )
        {
            FAPI_DBG("\ttraces: 0x%x", (*i)->iv_eiTraceId);
        }

        // Release the ffdc information now that we're done with it.
        io_rc.forgetData();

    }
#endif

    ///
    /// @brief Delay this thread.
    ///
    ReturnCode delay(uint64_t i_nanoSeconds, uint64_t i_simCycles, bool i_fixed = false)
    {
        // void statements to keep the compiler from complaining
        // about unused variables.
        static_cast<void>(i_nanoSeconds);
        static_cast<void>(i_simCycles);
        

#ifndef __FAPI_DELAY_SIM__

#define PK_NANOSECONDS_SBE(n) ((PkInterval)((PK_BASE_FREQ_HZ * (PkInterval)(n)) / (1024*1024*1024)))

        PkTimebase  target_time;
        PkTimebase  current_time;
        PkMachineContext  ctx;


        // Only execute if nanoSeconds is non-zero (eg a real wait)
        if (i_nanoSeconds)
        {
            // @todo For SBE applications, the time accuracy can be traded off
            // for space with the PK_NANOSECONDS_SBE implemenation as the compiler
            // use shift operations for the unit normalizing division.
            
            // The critical section enter/exit set is done to ensure the timebase 
            // operations are non-interrupible.
            
            pk_critical_section_enter(&ctx);
            //
            // The "accurate" version is the next line.
            // target_time = pk_timebase32_get() + PK_INTERVAL_SCALE(PK_NANOSECONDS(i_nanoSeconds));

            target_time = pk_timebase32_get() + PK_INTERVAL_SCALE(PK_NANOSECONDS_SBE(i_nanoSeconds));

            do
            {
                current_time = pk_timebase32_get();                                
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
        

        for (auto i = delay_loop_count; i > 0; --i) {}
        
#endif

        // replace with platform specific implementation
        return FAPI2_RC_SUCCESS;
    }
};


/// Byte-reverse a 16-bit integer if on a little-endian machine

uint16_t
revle16(uint16_t i_x)
{
    uint16_t rx;

#ifndef _BIG_ENDIAN
    uint8_t *pix = (uint8_t*)(&i_x);
    uint8_t *prx = (uint8_t*)(&rx);

    prx[0] = pix[1];
    prx[1] = pix[0];
#else
    rx = i_x;
#endif

    return rx;
}

/// Byte-reverse a 32-bit integer if on a little-endian machine

uint32_t
revle32(uint32_t i_x)
{
    uint32_t rx;

#ifndef _BIG_ENDIAN
    uint8_t *pix = (uint8_t*)(&i_x);
    uint8_t *prx = (uint8_t*)(&rx);

    prx[0] = pix[3];
    prx[1] = pix[2];
    prx[2] = pix[1];
    prx[3] = pix[0];
#else
    rx = i_x;
#endif

    return rx;
}


/// Byte-reverse a 64-bit integer if on a little-endian machine

uint64_t
revle64(const uint64_t i_x)
{
    uint64_t rx;

#ifndef _BIG_ENDIAN
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
#else
    rx = i_x;
#endif

    return rx;
}


