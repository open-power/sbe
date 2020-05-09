/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_5.H $          */
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

#ifndef __p10_ppe_eq_5_H_
#define __p10_ppe_eq_5_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_DB0]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_DB0 = 0xc0001000ull;

static const uint32_t QME_DB0_NUMBER = 0;
static const uint32_t QME_DB0_NUMBER_LEN = 8;
static const uint32_t QME_DB0_DATA = 8;
static const uint32_t QME_DB0_DATA_LEN = 56;
//<< [QME_DB0]
// ppe_eq/reg00005.H

//>> [QME_EITR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_EITR_RW = 0xc00026c0ull;
static const uint32_t QME_EITR_WO_CLEAR = 0xc00026f0ull;
static const uint32_t QME_EITR_WO_OR = 0xc00026e0ull;

static const uint32_t QME_EITR_INTERRUPT_TYPE = 0;
static const uint32_t QME_EITR_INTERRUPT_TYPE_LEN = 64;
//<< [QME_EITR]
// ppe_eq/reg00005.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00005.H"
#endif
#endif
