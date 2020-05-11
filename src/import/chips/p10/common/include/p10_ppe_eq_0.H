/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_0.H $          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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

#ifndef __PPE_HCODE__
    #include "proc_scomt.H"
#endif

#ifndef __p10_ppe_eq_0_H_
#define __p10_ppe_eq_0_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_BCECSR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_BCECSR = 0xc0000680ull;

static const uint32_t QME_BCECSR_BUSY = 0;
static const uint32_t QME_BCECSR_ERROR = 1;
static const uint32_t QME_BCECSR_START = 2;
static const uint32_t QME_BCECSR_STOP = 3;
static const uint32_t QME_BCECSR_RNW = 4;
static const uint32_t QME_BCECSR_BARSEL = 5;
static const uint32_t QME_BCECSR_PRIORITY = 6;
static const uint32_t QME_BCECSR_INJECT_ERR = 7;
static const uint32_t QME_BCECSR_TYPE = 13;
static const uint32_t QME_BCECSR_TYPE_LEN = 3;
static const uint32_t QME_BCECSR_NUM_BLOCKS = 17;
static const uint32_t QME_BCECSR_NUM_BLOCKS_LEN = 11;
static const uint32_t QME_BCECSR_SBASE = 28;
static const uint32_t QME_BCECSR_SBASE_LEN = 13;
static const uint32_t QME_BCECSR_MBASE = 42;
static const uint32_t QME_BCECSR_MBASE_LEN = 22;
//<< [QME_BCECSR]
// ppe_eq/reg00000.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00000.H"
#endif
#endif
