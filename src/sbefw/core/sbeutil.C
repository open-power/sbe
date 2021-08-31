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
        // Set the mpiplreset bit and write to WO_CLR Reg to clear
        lfrReg.mpipl = 1;
        PPE_STVD(0xc0002058, lfrReg);
        #undef SBE_FUNC
    }

    bool isMpiplReset(void)
    {
        #define SBE_FUNC "IS_MPIPL_RESET"
        bool isMpipl = false;
        sbe_local_LFR lfrReg;
        PPE_LVD(0xc0002040, lfrReg);
        isMpipl = lfrReg.mpipl;
        return (isMpipl);
        #undef SBE_FUNC
    }

    void runSystemReset(void)
    {
        #define SBE_FUNC "RUN_SYSTEM_RESET"
        SBE_INFO(SBE_FUNC" System is going for manual Reset/Jump to Reset location");
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

    void unlockI2CEngineE(void)
    {
        #define SBE_FUNC "SBE_UNLOCK_I2C_ENGINE_E"
        SBE_INFO(SBE_FUNC "unlock the secure lock on i2c E engine");
        uint64_t i2cEngineELockReset = 0xC000000000000000ULL; // Bit 0 & 1 are set
        // Fetch the I2C Engine E Status
        uint64_t i2cEngineEStatusReg = 0;
        PPE_LVD(0x000A300B, i2cEngineEStatusReg);

        // Check if it's secure operation and SBE is owning the secure lock
        // Bit 52 indicates Secure Operation is going on
        // Bit 48..51 indicates Locked PIB master Id, E is SBE, 9 is ADU/Host
        if( ((i2cEngineEStatusReg >> 11) & 0x1) && // bit 52 set
            ((i2cEngineEStatusReg >> 12) & 0xE) )  // Bit 48..51 as E = SBE
        {
            PPE_STVD(0x000A3001, i2cEngineELockReset);
        }
        #undef SBE_FUNC
    }

    bool isIplReset(void)
    {
        #define SBE_FUNC "IS_IPL_RESET"
        bool isIplReset = false;
        sbe_local_LFR lfrReg;
        PPE_LVD(0xc0002040, lfrReg);
        isIplReset = lfrReg.ipl_reset;
        return (isIplReset);
        #undef SBE_FUNC
    }

    void updateScratchReg16fromLFRinIplReset()
    {
        #define SBE_FUNC "UPDATE_SCRATCH_REG16_FROM_LFR_IPL_RESET"
        sbe_local_LFR lfrReg;
        sbe_scratch_reg16 scratchReg16;
        PPE_LVD(0x50187, scratchReg16);
        PPE_LVD(0xc0002040, lfrReg);
        scratchReg16.spi_clock_divider = lfrReg.spi_clock_divider;
        scratchReg16.spi_clock_delay = lfrReg.round_trip_delay;
        scratchReg16.tpm_spi_clock_freq_Mhz = lfrReg.tpm_spi_clock_freq_Mhz;
        scratchReg16.tpm_spi_clock_delay =  lfrReg.tpm_spi_clock_delay;
        PPE_STVD(0x50187, scratchReg16);
        #undef SBE_FUNC
    }

    void updateLFRfromSrcatchReg16inHReset()
    {
        #define SBE_FUNC "UPDATE_LFR_FROM_SCRATCH_REG16_HRESET"
        SBE_INFO(SBE_FUNC " Hreset path updating LFR from scratch16");
        sbe_local_LFR lfrReg;
        sbe_scratch_reg16 scratchReg16;
        PPE_LVD(0x50187, scratchReg16);
        PPE_LVD(0xc0002040, lfrReg);
        lfrReg.tpm_spi_clock_freq_Mhz = scratchReg16.tpm_spi_clock_freq_Mhz;
        lfrReg.tpm_spi_clock_delay = scratchReg16.tpm_spi_clock_delay;
        PPE_STVD(0xc0002040, lfrReg);
        #undef SBE_FUNC
    }

    void updateLFRfromScratchReg13inHReset()
    {
        #define SBE_FUNC "UPDATE_LFR_FROM_SCRATCH_REG13_HRESET"
        SBE_INFO(SBE_FUNC " Hreset path updating LFR from scratch13");
        sbe_local_LFR lfrReg;
        sbe_scratch_reg13_reuse scratchReg13;
        PPE_LVD(0x50184, scratchReg13);
        PPE_LVD(0xc0002040, lfrReg);
        lfrReg.pau_freq_in_mhz = scratchReg13.pau_freq_in_mhz;
        PPE_STVD(0xc0002040, lfrReg);
        #undef SBE_FUNC
    }

    void updateScratchReg13fromLFRinIplReset()
    {
        #define SBE_FUNC "UPDATE_SCRATCH_REG13_FROM_LFR_IPL_RESET"
        sbe_local_LFR lfrReg;
        sbe_scratch_reg13_reuse scratchReg13;
        PPE_LVD(0x50184, scratchReg13);
        PPE_LVD(0xc0002040, lfrReg);
        scratchReg13.pau_freq_in_mhz = lfrReg.pau_freq_in_mhz;
        PPE_STVD(0x50184, scratchReg13);
        #undef SBE_FUNC
    }

    void enableXscoms(void)
    {
        #define SBE_FUNC "ENABLE_XSCOMS"
        SBE_INFO(SBE_FUNC " Enable Xscoms after an mpipl reset");
        uint64_t data = 0;
        PPE_LVD(0x90010, data);
        data = (data & 0xfffffffffffffffeull);
        PPE_STVD(0x90010, data);
        #undef SBE_FUNC
    }

    void hardResetTPMSpiEngine(void)
    {
        #define SBE_FUNC "HARD_RESET_TPM_SPI_ENGINE"
        uint64_t loadData =0x0008000000000000ULL;
        PPE_STVD(0xc0002010, loadData);
        PPE_STVD(0xc0002018, loadData);
        SBE_INFO( SBE_FUNC " Hard-resetting the TPM SPI engine before Mpipl Reset");
        #undef SBE_FUNC
    }
}

