/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_utils.H $                                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
#ifndef _PLAT_UTILS_H_
#define _PLAT_UTILS_H_

#include <plat_types.H>

namespace fapi2 {
#ifdef __PPE_QME
static const PlatformType PLATFORM = PLAT_QME;
#else
static const PlatformType PLATFORM = PLAT_SBE;
#endif
}

/// @brief Cause a compile time error unless eliminated by dead code elimination
/// Use this if you have nested if statements and would like the compile to fail
/// if a given condition is possible to be hit.
#define STATIC_COMPILE_ERROR(msg)                           \
    {                                                       \
        extern void __attribute__((error(msg))) e();		\
        e();                                                \
    }

/// @brief Static assertion that can be optimized out
#define STATIC_CHECK(cond, msg)                 \
    {                                           \
        if (!(cond))                            \
        {                                       \
            STATIC_COMPILE_ERROR(msg);          \
        }                                       \
    }

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
#ifndef BITS
    #define BITS(b, n) ((ULL(0xffffffffffffffff) << (64 - (n))) >> (b))
#endif

/// Create a single bit mask at bit \a b
#ifndef BIT
    #define BIT(b) BITS((b), 1)
#endif
#endif //_PLAT_UTILS_H_
