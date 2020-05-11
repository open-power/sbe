/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_a.H $          */
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

#ifndef __p10_ppe_eq_a_H_
#define __p10_ppe_eq_a_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_BCEBAR0]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_BCEBAR0 = 0xc0000600ull;

static const uint32_t QME_BCEBAR0_BASE = 8;
static const uint32_t QME_BCEBAR0_BASE_LEN = 36;
static const uint32_t QME_BCEBAR0_SIZE = 61;
static const uint32_t QME_BCEBAR0_SIZE_LEN = 3;
//<< [QME_BCEBAR0]
// ppe_eq/reg00010.H

//>> [QME_QSDR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QSDR = 0xc0000780ull;

static const uint32_t QME_QSDR_SRAM_DATA = 0;
static const uint32_t QME_QSDR_SRAM_DATA_LEN = 64;
//<< [QME_QSDR]
// ppe_eq/reg00010.H

//>> [QME_RCTAR0]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCTAR0 = 0xc0001900ull;

static const uint32_t QME_RCTAR0_DATA = 0;
static const uint32_t QME_RCTAR0_DATA_LEN = 64;
//<< [QME_RCTAR0]
// ppe_eq/reg00010.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00010.H"
#endif
#endif
