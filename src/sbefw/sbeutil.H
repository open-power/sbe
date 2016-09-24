/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeutil.H $                                         */
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
#ifndef SBE_UTIL_H
#define SBE_UTIL_H
#include "pk.h"
#include "pk_api.h"

#define MASK_ZERO_L32B_UINT64(x)    ((x) & 0xFFFFFFFF00000000)
#define MASK_ZERO_H32B_UINT64(x)    ((x) & 0x00000000FFFFFFFF)
#define SHIFT_RIGHT(x, bits)        ((x) >> bits)

// Macros Defined for Internal RC Check, Break if Error
#define CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc) \
if ((l_rc) != SBE_SEC_OPERATION_SUCCESSFUL) \
{ \
    break; \
}

// To handle unused variables compilation error
static inline void UNUSED(int dummy, ...) {}

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

void sbeHandleFifoResponse (const uint32_t i_rc);

void sbeHandlePsuResponse (const uint32_t i_rc);

namespace SBE
{

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

} // namespace SBE
#endif //SBE_UTIL_H
