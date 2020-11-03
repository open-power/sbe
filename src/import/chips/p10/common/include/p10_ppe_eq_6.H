/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_6.H $          */
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

#ifndef __p10_ppe_eq_6_H_
#define __p10_ppe_eq_6_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_PPEBAR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PPEBAR = 0xc0000500ull;

static const uint32_t QME_PPEBAR_BASE = 8;
static const uint32_t QME_PPEBAR_BASE_LEN = 36;
static const uint32_t QME_PPEBAR_SIZE = 61;
static const uint32_t QME_PPEBAR_SIZE_LEN = 3;
//<< [QME_PPEBAR]
// ppe_eq/reg00006.H

//>> [QME_QHTMEM]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QHTMEM = 0xc0001a40ull;

static const uint32_t QME_QHTMEM_ALLOC = 0;
static const uint32_t QME_QHTMEM_SCOPE = 1;
static const uint32_t QME_QHTMEM_SCOPE_LEN = 3;
static const uint32_t QME_QHTMEM_PRIORITY = 4;
static const uint32_t QME_QHTMEM_SIZE_SMALL = 5; // p10:20,
static const uint32_t QME_QHTMEM_BASE = 8;
static const uint32_t QME_QHTMEM_BASE_LEN = 32;
static const uint32_t QME_QHTMEM_SIZE = 40;
static const uint32_t QME_QHTMEM_SIZE_LEN = 9;
//<< [QME_QHTMEM]
// ppe_eq/reg00006.H

//>> [QME_RCTAR2]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCTAR2 = 0xc0001980ull;

static const uint32_t QME_RCTAR2_DATA = 0;
static const uint32_t QME_RCTAR2_DATA_LEN = 64;
//<< [QME_RCTAR2]
// ppe_eq/reg00006.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00006.H"
#endif
#endif
