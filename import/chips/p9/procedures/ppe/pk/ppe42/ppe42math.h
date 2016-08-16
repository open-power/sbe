/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/ppe/pk/ppe42/ppe42math.h $         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
#ifndef _MATH_H
#define _MATH_H

#ifdef __cplusplus
extern "C"
{
#endif
// These names are emitted by the ppe42 compiler.
// Implement the ones that will be used.

// 64 bit unsigned divide. Implement if needed
// unsigned long long __udivdi3(unsigned long long a, unsigned long long b);

/** 32 bit unsigned divide
 *  @param[in] Dividend
 *  @param[in] Divisor
 *  @return quotient
 */
unsigned long __udivsi3(unsigned long a, unsigned long b);

/** 32 bit signed divide
 * @param[in] Dividend
 * @param[in] Divisor
 * @return quotient
 */
int __divsi3(int _a, int _b);

/** 32 bit unsigned modulus
 * @param[in] Dividend
 * @param[in] Divisor
 * @return modulus
 */
unsigned long __umodsi3(unsigned long a, unsigned long b);

/** 32 bit unsigned multiply
 * @param[in] multiplier
 * @param[in] multiplier
 * @return product
 */
unsigned long __umulsi3(unsigned long _a, unsigned long _b);

/** 32 bit signed multiply
 * @param[in] multiplier
 * @param[in] multiplier
 * @return product
 */
unsigned int __mulsi3(unsigned int _a, unsigned int _b);

/** 64 bit signed multiply
 * @param[in] multiplier
 * @param[in] multiplier
 * @return product
 */
unsigned long long __muldi3(unsigned long long _a, unsigned long long _b);

#ifdef __cplusplus
};
#endif

#endif
