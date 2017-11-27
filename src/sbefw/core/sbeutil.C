/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeutil.C $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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

    void updatePkFreq()
    {
        #define SBE_FUNC "updatePkFreq "
        using namespace fapi2;
        Target<TARGET_TYPE_SYSTEM> sys;
        uint8_t nestPllBkt = 0;
        FAPI_ATTR_GET( ATTR_NEST_PLL_BUCKET, sys, nestPllBkt );
        assert( nestPllBkt && (nestPllBkt <= NEST_PLL_FREQ_BUCKETS ));
        SBE_GLOBAL->sbefreq = ( NEST_PLL_FREQ_LIST[ nestPllBkt - 1 ] * 1000 * 1000 )/
                                              SBE::SBE_TO_NEST_FREQ_FACTOR;
        SBE_INFO(SBE_FUNC"Setting new frequency:0x%08X", SBE_GLOBAL->sbefreq);
        pk_timebase_freq_set(SBE_GLOBAL->sbefreq);
        #undef SBE_FUNC
    }
}

