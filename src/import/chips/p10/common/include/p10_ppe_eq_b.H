/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_b.H $          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
// ppe_eq/reg00011.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCPTR = 0xc0001880ull;

static const uint32_t QME_RCPTR_TARGET_PSTATE = 0;
static const uint32_t QME_RCPTR_TARGET_PSTATE_LEN = 8;
static const uint32_t QME_RCPTR_PSTATE_CHANGE_PENDING = 8;
static const uint32_t QME_RCPTR_TARGET_INDEX = 59;
static const uint32_t QME_RCPTR_TARGET_INDEX_LEN = 5;
// ppe_eq/reg00011.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIMEM = 0xc00021c0ull;

static const uint32_t QME_SCOM_XIMEM_ADDR = 0;
static const uint32_t QME_SCOM_XIMEM_ADDR_LEN = 32;
static const uint32_t QME_SCOM_XIMEM_R_NW = 32;
static const uint32_t QME_SCOM_XIMEM_BUSY = 33;
static const uint32_t QME_SCOM_XIMEM_IMPRECISE_ERROR_PENDING = 34;
static const uint32_t QME_SCOM_XIMEM_BYTE_ENABLE = 35;
static const uint32_t QME_SCOM_XIMEM_BYTE_ENABLE_LEN = 8;
static const uint32_t QME_SCOM_XIMEM_LINE_MODE = 43;
static const uint32_t QME_SCOM_XIMEM_ERROR = 49;
static const uint32_t QME_SCOM_XIMEM_ERROR_LEN = 3;
static const uint32_t QME_SCOM_XIMEM_IFETCH_PENDING = 62;
static const uint32_t QME_SCOM_XIMEM_DATAOP_PENDING = 63;
// ppe_eq/reg00011.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIVDR30 = 0xc00029c0ull;

static const uint32_t QME_SCOM_XIVDR30_0 = 0;
static const uint32_t QME_SCOM_XIVDR30_0_LEN = 32;
static const uint32_t QME_SCOM_XIVDR30_1 = 32;
static const uint32_t QME_SCOM_XIVDR30_1_LEN = 32;
// ppe_eq/reg00011.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_TTSR = 0xc0001380ull;

static const uint32_t QME_TTSR_0_DATA = 0;
static const uint32_t QME_TTSR_0_DATA_LEN = 8;
static const uint32_t QME_TTSR_1_DATA = 8;
static const uint32_t QME_TTSR_1_DATA_LEN = 8;
static const uint32_t QME_TTSR_2_DATA = 16;
static const uint32_t QME_TTSR_2_DATA_LEN = 8;
static const uint32_t QME_TTSR_3_DATA = 24;
static const uint32_t QME_TTSR_3_DATA_LEN = 8;
static const uint32_t QME_TTSR_4_DATA = 32;
static const uint32_t QME_TTSR_4_DATA_LEN = 8;
static const uint32_t QME_TTSR_5_DATA = 40;
static const uint32_t QME_TTSR_5_DATA_LEN = 8;
static const uint32_t QME_TTSR_6_DATA = 48;
static const uint32_t QME_TTSR_6_DATA_LEN = 8;
static const uint32_t QME_TTSR_7_DATA = 56;
static const uint32_t QME_TTSR_7_DATA_LEN = 8;
// ppe_eq/reg00011.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00011.H"
#endif
#endif
