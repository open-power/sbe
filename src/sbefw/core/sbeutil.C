/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeutil.C $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
#include "sbeutil.H"
#include "fapi2.H"
#include "sbetrace.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"
#include "sbeglobals.H"

#include "plat_hw_access.H"
#include "assert.h"

// Nest frequency array
#include "p9_frequency_buckets.H"

namespace SBE
{
    bool isSimics() __attribute__((alias("__isSimicsRunning")));
    extern "C" void __isSimicsRunning() __attribute__ ((noinline));

    void __isSimicsRunning()
    {
        asm volatile("li 3, 0");
        SBE_MAGIC_INSTRUCTION(MAGIC_SIMICS_CHECK);
    }

    bool isSimicsRunning()
    {
        static bool simics = isSimics();
        return simics;
    }

    void updatePkFreqToPauDpll()
    {
        #define SBE_FUNC "updatePkFreqPauDpll "
        using namespace fapi2;
        Target<TARGET_TYPE_SYSTEM> sys;
        uint32_t pauFreqMhz = 0;
        FAPI_ATTR_GET( ATTR_FREQ_PAU_MHZ, sys, pauFreqMhz );
        SBE_INFO(SBE_FUNC "Attr PAU Frequency in MHz [0x%08X]", pauFreqMhz);
        assert( pauFreqMhz );
        SBE_GLOBAL->sbefreq = 
            ( pauFreqMhz * 1000 * 1000 ) / SBE::SBE_TO_NEST_FREQ_FACTOR;
        SBE_INFO(SBE_FUNC"Setting new frequency:0x%08X", SBE_GLOBAL->sbefreq);
        pk_timebase_freq_set(SBE_GLOBAL->sbefreq);
        #undef SBE_FUNC
    }

    void updatePkFreqToRefClk()
    {
        #define SBE_FUNC "updatePkFreqRefClk "
        using namespace fapi2;
        SBE_GLOBAL->sbefreq = SBE_REF_BASE_FREQ_HZ; 
        SBE_INFO(SBE_FUNC"Setting new frequency:0x%08X", SBE_GLOBAL->sbefreq);
        pk_timebase_freq_set(SBE_GLOBAL->sbefreq);
        #undef SBE_FUNC
    }

    bool isHreset(void)
    {
        #define SBE_FUNC "IS_HRESET"
        bool isHreset = false;
        sbe_local_LFR lfrReg;
        PPE_LVD(0xc0002040, lfrReg);
        isHreset = lfrReg.runtime_reset;
        //SBE_INFO(SBE_FUNC" [%d]", isHreset);
        return (isHreset);
        #undef SBE_FUNC
    }

    void setHResetDoneBit(void)
    {
        #define SBE_FUNC "HRESET_DONE"
        sbe_local_LFR lfrReg;
        // Set the hreset_done bit and write to WO_OR Reg to set
        lfrReg.hreset_done = 1;
        PPE_STVD(0xc0002050, lfrReg);
        #undef SBE_FUNC
    }

    void setMpiplResetDoneBit(void)
    {
        #define SBE_FUNC "MPIPL_RESET_DONE"
        sbe_local_LFR lfrReg;
        // Set the mpiplreset_done bit and write to WO_OR Reg to set
        lfrReg.mpipl_reset_done = 1;
        PPE_STVD(0xc0002050, lfrReg);
        #undef SBE_FUNC
    }

    bool isMpiplResetDone(void)
    {
        #define SBE_FUNC "IS_MPIPL_RESET_DONE"
        bool isMpiplDone = false;
        sbe_local_LFR lfrReg;
        PPE_LVD(0xc0002040, lfrReg);
        isMpiplDone = lfrReg.mpipl_reset_done;
        //SBE_INFO(SBE_FUNC" [%d]", isMpiplDone);
        return (isMpiplDone);
        #undef SBE_FUNC
    }

    void setMpiplReset(void)
    {
        #define SBE_FUNC "SET_MPIPL_RESET"
        sbe_local_LFR lfrReg;
        // Set the mpiplreset bit and write to WO_OR Reg to set
        lfrReg.mpipl = 1;
        PPE_STVD(0xc0002050, lfrReg);
        #undef SBE_FUNC
    }

    void clearMpiplReset(void)
    {
        #define SBE_FUNC "CLEAR_MPIPL_RESET"
        sbe_local_LFR lfrReg;
        // Set the mpiplreset bit and write to WO_OR Reg to clear
        lfrReg.mpipl = 1;
        PPE_STVD(0xc0002058, lfrReg);
        #undef SBE_FUNC
    }

    bool isMpiplReset(void)
    {
        #define SBE_FUNC "IS_MPIPL"
        bool isMpipl = false;
        sbe_local_LFR lfrReg;
        PPE_LVD(0xc0002040, lfrReg);
        isMpipl = lfrReg.mpipl;
        //SBE_INFO(SBE_FUNC" [%d]", isMpipl);
        return (isMpipl);
        #undef SBE_FUNC
    }

    void runSystemReset(void)
    {
        #define SBE_FUNC "RUN_SYSTEM_RESET"
        SBE_INFO(SBE_FUNC" System is going for manual Reset");
        uint32_t reset_msr = 0;
        mtmsr(reset_msr);
        uint64_t data = (uint64_t)(OTPROM_ORIGIN) << 32;
        PPE_STVD(g_ivprLoc, data);
        if(SBE::isSimicsRunning())
        {
            JUMP_TO_ADDR(0x00018040);
        }
        else //HW
        {
            JUMP_TO_ADDR(0x000C0040);
        }
        #undef SBE_FUNC
    }
}

