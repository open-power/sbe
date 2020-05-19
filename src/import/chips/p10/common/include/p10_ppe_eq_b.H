/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_b.H $          */
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

#ifndef __p10_ppe_eq_b_H_
#define __p10_ppe_eq_b_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_RCPTR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCPTR = 0xc0001880ull;

static const uint32_t QME_RCPTR_TARGET_PSTATE = 0;
static const uint32_t QME_RCPTR_TARGET_PSTATE_LEN = 8;
static const uint32_t QME_RCPTR_PSTATE_CHANGE_PENDING = 8;
static const uint32_t QME_RCPTR_TARGET_INDEX = 59;
static const uint32_t QME_RCPTR_TARGET_INDEX_LEN = 5;
//<< [QME_RCPTR]
// ppe_eq/reg00011.H

//>> [QME_PCBQBI]
//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_PCBQBI = 0xc00003c0ull;

static const uint32_t QME_PCBQBI_ENTRY_VALID = 0;
static const uint32_t QME_PCBQBI_MERGING_SCHEME = 2;
static const uint32_t QME_PCBQBI_MERGING_SCHEME_LEN = 3;
static const uint32_t QME_PCBQBI_R_NW = 8;
static const uint32_t QME_PCBQBI_PORT = 12;
static const uint32_t QME_PCBQBI_PORT_LEN = 4;
static const uint32_t QME_PCBQBI_ADDR = 16;
static const uint32_t QME_PCBQBI_ADDR_LEN = 16;
//<< [QME_PCBQBI]
// ppe_eq/reg00011.H

//>> [QME_QHTLAST]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QHTLAST = 0xc0001ac0ull;

static const uint32_t QME_QHTLAST_LAST_ADDRESS = 8;
static const uint32_t QME_QHTLAST_LAST_ADDRESS_LEN = 49;
//<< [QME_QHTLAST]
// ppe_eq/reg00011.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00011.H"
#endif
#endif
