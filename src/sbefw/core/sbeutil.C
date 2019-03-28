/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeutil.C $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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

#include "plat_hw_access.H"
#include "assert.h"

// Nest frequency array
#include "p9_frequency_buckets.H"

#define SBE_PERV_SB_CS_SCOM 0x00050008
#define SBCS_VEC_1_CLEAR(sbcsreg) (sbcsreg & ~0x0004000000000000ull)
#define SBCS_VEC_1_GET(sbcsreg)   (sbcsreg & 0x0004000000000000ull)

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
#if 0
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
#endif
    }

    bool isHreset(void)
    {
        #define SBE_FUNC "IS_HRESET"
        using namespace fapi2;
        bool isHreset = false;
        uint64_t sbcsreg = 0;
        ReturnCode rc = FAPI2_RC_SUCCESS;
        plat_target_handle_t tgtHndl;
        rc = getscom_abs_wrap (&tgtHndl,
                                SBE_PERV_SB_CS_SCOM,
                                &sbcsreg);
        if (rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" could not read sbcs register, halting.");
            pk_halt();
        }
        isHreset = SBCS_VEC_1_GET(sbcsreg);
        SBE_INFO(SBE_FUNC" [%d]", isHreset);
        return (isHreset);
        #undef SBE_FUNC
    }

    void clearHresetBit(void)
    {
        #define SBE_FUNC "CLEAR_HRESET_BIT"
        using namespace fapi2;
        // clear the hreset bit
        uint64_t sbcsreg = 0;
        plat_target_handle_t tgtHndl;
        ReturnCode rc = getscom_abs_wrap (&tgtHndl,
                                          SBE_PERV_SB_CS_SCOM,
                                          &sbcsreg);
        if (rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" could not read sbcs register, halting.");
            pk_halt();
        }

        sbcsreg = SBCS_VEC_1_CLEAR(sbcsreg);
        rc = putscom_abs_wrap (&tgtHndl,
                                SBE_PERV_SB_CS_SCOM,
                                sbcsreg);
        if (rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" could not write sbcs register, halting.");
            pk_halt();
        }
        #undef SBE_FUNC
    }
}

