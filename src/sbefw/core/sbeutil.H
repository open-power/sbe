/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeutil.H $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
#ifndef SBE_UTIL_H
#define SBE_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "pk.h"
#include "pk_api.h"

#ifdef __cplusplus
}
#endif

#define MASK_ZERO_L32B_UINT64(x)    ((x) & 0xFFFFFFFF00000000ull)
#define MASK_ZERO_H32B_UINT64(x)    ((x) & 0x00000000FFFFFFFFull)
#define SHIFT_RIGHT(x, bits)        ((x) >> bits)

/**
 * @brief Enum of SBE Fifo type.
 */
enum sbeFifoType
{
   SBE_FIFO,
   SBE_HB_FIFO
};

// Macros Defined for Internal RC Check, Break if Error
#define CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc) \
if ((l_rc) != SBE_SEC_OPERATION_SUCCESSFUL) \
{ \
    break; \
}

#define CHECK_SBE_SECURITY_RC_AND_BREAK_IF_NOT_SUCCESS(addr, op, resp, fifotype, mask) \
if (!((fifotype == SBE_HB_FIFO) || (SBE_SECURITY::isAllowed(addr,mask,op))) ) \
{ \
    /* TODO via RTC 180983:Enable this once the BL/WL registers are settled */ \
    if(SbeRegAccess::theSbeRegAccess().disableScomFiltering()) \
    { \
        /* security override possible && scom filtering disabled */ \
        resp->setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED, \
                        SBE_SEC_BLACKLISTED_REG_ACCESS); \
    } \
    else \
    { \
        resp->setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED, \
                        SBE_SEC_BLACKLISTED_REG_ACCESS_BLOCKED); \
        break; \
    } \
}

//TODO:Below macro can be removed once OCMB reg's are supported in .csv file.
#define CHECK_SBE_OCMB_READ_SECURITY_RC_AND_BREAK_IF_NOT_SUCCESS(addr, resp, fifotype) \
if (!((fifotype == SBE_HB_FIFO) || (SBE_SECURITY::isOcmbReadAllowed(addr))) ) \
{ \
    if(SbeRegAccess::theSbeRegAccess().disableScomFiltering()) \
    { \
        /* security override possible && scom filtering disabled */ \
        resp->setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED, \
                        SBE_SEC_BLACKLISTED_REG_ACCESS); \
    } \
    else \
    { \
        resp->setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED, \
                        SBE_SEC_BLACKLISTED_REG_ACCESS_BLOCKED); \
        break; \
    } \
}

#define mfdec()                               \
    ({volatile uint32_t l_dec;                          \
        asm volatile ("mfdec %0" : "=r" (l_dec)); \
        l_dec;})

// To handle unused variables compilation error
static inline void UNUSED(int dummy, ...) {}

// Magic instruction

/** @brief  This is a special assembler instruction that is a nop on
 *  regular hardware, but has special meaning to Simics.  Code that
 *  executes this instruction in Simics will cause a "hap," a
 *  Simics term.  If there is no hap handler registered, and magic
 *  breakpoints have not been enabled with
 *      simics> enable-magic-breakpoint
 *  then this instruction is also a nop in Simics.
 *
 *  If magic breakpoints are enabled, and there is no hap handler, then
 *  when SBE code executes this instruction in Simics, Simics will
 *  stop the simulation. (Prompt changes from running> to simics> )
 *
 *  If a hap is registered, then Simics will call the hap handler.  Hap
 *  handlers are written in Python, and the best place for them is
 *
 *     src/tools/debug//simics-debug-framework.py
 *
 *  Sample code to register the hap handler:
 *        # arg contains the integer parameter n passed to MAGIC_INSTRUCTION(n)
 *        def magic_instruction_callback(user_arg, cpu, arg):
 *            # print to console works...
 *            print "Hit magic instruction ", arg
 *            # Or else stop the simulation...
 *            SIM_break_simulation( "Stopped at magic instruction" )
 *
 *        # Register the magic instruction callback.
 *        SIM_hap_add_callback( "Core_Magic_Instruction",
 *                              magic_instruction_callback, None )
 *
 *        # Better to register the SBE range 8000-8190
 *        # so that PHYP and others won't be affected.
 *        SIM_hap_add_callback_range( "Core_Magic_Instruction",
 *                      magic_instruction_callback, None, 8000, 8190 )
 *
 *  The argument n is an integer from 0..8191 which Simics passes to the hap
 *  handler in parameter 3, or "arg" in the sample code above.
 */
__attribute__((always_inline))
inline void SBE_MAGIC_INSTRUCTION(int _n)
{
    register int n = _n;
    asm volatile("rlwimi %0,%0,0,%1,%2"       \
        :: "i" (((n) >> 8) & 0x1f),           \
        "i" (((n) >> 4) & 0xf),               \
        "i" ((((n) >> 0) & 0xf) | 16));
}

// Macro to execute HWP
#ifdef SEEPROM_IMAGE
#define SBE_EXEC_HWP_NOARG(...) SBE_EXEC_HWP(__VA_ARGS__)
#define SBE_EXEC_HWP(fapiRc, hwp, ...) \
{ \
    fapiRc = hwp(__VA_ARGS__); \
}
#else
#define SBE_EXEC_HWP_NOARG(fapiRc, hwp) \
{ \
    SBE_INFO("Procedure not present in the image:No-Op"); \
    fapiRc = FAPI2_RC_SUCCESS; \
}
#define SBE_EXEC_HWP(fapiRc, hwp, ...) \
{ \
    /* handling unused variables */ \
    UNUSED(0, __VA_ARGS__); \
    SBE_INFO("Procedure not present in the image:No-Op"); \
    fapiRc = FAPI2_RC_SUCCESS; \
}
#endif

namespace SBE
{

    // enum for magic instructions
    enum
    {
        MAGIC_SIMICS_CHECK = 8000, // check if code is running on simics
    };
    // Nest to SBE frequency ratio
    static const uint8_t  SBE_TO_NEST_FREQ_FACTOR = 4;

    // Currently PK does not define start range for app
    // specifc panic code as enum. It is implicit understanding
    // through code comments. Expectation is 0x1cxx range is for
    // non-pk code.
    static const uint32_t PK_APP_OFFSET_SBE_START = 0x1c00;
    enum
    {
        // For defining new panic codes refer to pk/ppe42/pk_panic_codes.h
        PANIC_ASSERT = PK_APP_OFFSET_SBE_START
    };

    /*@brief - Get higher 32bit number from uint64
     *
     * @param[in] - i_lWord - 64bit long word
     *
     * @return - uint32_t word
     */
    inline uint32_t higher32BWord(uint64_t i_lWord)
    {
        return (uint32_t)(SHIFT_RIGHT(MASK_ZERO_L32B_UINT64(i_lWord), 32));
    }

    /*@brief - Get lower 32bit number from uint64
     *
     * @param[in] - i_lWord - 64bit long word
     *
     * @return - uint32_t word
     */
    inline uint32_t lower32BWord(uint64_t i_lWord)
    {
        return (uint32_t)(MASK_ZERO_H32B_UINT64(i_lWord));
    }

    /*@brief - checks if sbe is running on simics
     *
     * @return - True if sbe is running on simics , false otherwise
     */
    bool isSimicsRunning();

    /*@brief - Update the pk frequcy as per NEST PLL bucket
     *
     * @return - void
     */
    void updatePkFreqToPauDpll();

    /*@brief - Update the pk frequcy to Ref Clock
     *
     * @return - void
     */
    void updatePkFreqToRefClk();

    /* @brief - Check if the boot is on HRESET of SBE
     *
     * @return - True if HRESET, false otherwise
     */
    bool isHreset(void);

    /* @brief - Set the HReset Done bit on the LFR
     *
     * @return - void
     */
    void setHResetDoneBit(void);

    /* @brief - Set the Mpipl Reset Done bit on the LFR
     *
     * @return - void
     */
    void setMpiplResetDoneBit(void);

    /* @brief - Set the Mpipl Reset bit on the LFR
     *
     * @return - void
     */
    void setMpiplReset(void);

    /* @brief - Clear the Mpipl Reset bit on the LFR
     *
     * @return - void
     */
    void clearMpiplReset(void);

    /* @brief - Check if the boot is on Mpipl Reset Done of SBE
     *
     * @return - True if Mpipl Reset Done, false otherwise
     */
    bool isMpiplResetDone(void);

    /* @brief - Check if the boot is on Mpipl Reset of SBE
     *
     * @return - True if Mpipl Reset, false otherwise
     */
    bool isMpiplReset(void);

    /* @brief - Set OTPROM_ORIGIN value to g_ivprLoc and
     *          jump to 0x00018040 address to reset of SBE
     *
     * @return - void
     */
    void runSystemReset(void);

    /* @brief - Unlock the i2c engine E in secure operation mode
     *
     *  @return - void
     */
    void unlockI2CEngineE(void);

    /* @brief - Check if the boot is IPL Reset of SBE
     *
     * @return - True if IPL Reset, false otherwise
     */
    bool isIplReset(void);

    /* @brief - Update Scratch Reg 16 from LFR in IPL Reset flow
     *
     * @return - void
     */
    void updateScratchReg16fromLFRinIplReset(void);

    /* @brief - Update LFR from scratch reg 16 in HReset flow
     *
     * @return - void
     */
    void updateLFRfromSrcatchReg16inHReset(void);

    /* @brief - Update LFR from scratch reg 13 in HReset flow
     *
     * @return - void
     */
    void updateLFRfromScratchReg13inHReset(void);

    /* @brief - Update Scratch Reg 13 from LFR in IPL Reset flow
     *
     * @return - void
     */
    void updateScratchReg13fromLFRinIplReset(void);

    /* @brief - Enable Xscoms in MPIPL flow
     *
     * @return - void
     */
    void enableXscoms(void);

    /* @brief - Hard reset the TPM SPI Engine
     *
     * @return - void
     */
    void hardResetTPMSpiEngine(void);

} // namespace SBE
#endif //SBE_UTIL_H
