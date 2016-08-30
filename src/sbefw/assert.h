/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/assert.h $                                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
/*
 *  @file assert.h
 *
 *  @brief This file contains the assert macro for SBE
 */
#ifndef SBE_ASSERT_H
#define SBE_ASSERT_H
#include "sbetrace.H"
#include "sbeutil.H"

//@TODO via RTC 129166
//inject exception to halt SBE. Also see if we can use some
//PK kernel API.
#ifndef NDEBUG
#define assert(expr) \
    if( !(expr ))  \
    {  \
        SBE_ERROR("assertion failed: "#expr); \
        PK_PANIC(SBE::PANIC_ASSERT); \
    } \

#else
#define assert(expr)
#endif  //NDEBUG

#endif // SBE_ASSERT_H
