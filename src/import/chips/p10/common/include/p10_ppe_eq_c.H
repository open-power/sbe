/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_c.H $          */
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

#ifndef __p10_ppe_eq_c_H_
#define __p10_ppe_eq_c_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_BCEBAR1]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_BCEBAR1 = 0xc0000640ull;

static const uint32_t QME_BCEBAR1_BASE = 8;
static const uint32_t QME_BCEBAR1_BASE_LEN = 36;
static const uint32_t QME_BCEBAR1_SIZE = 61;
static const uint32_t QME_BCEBAR1_SIZE_LEN = 3;
//<< [QME_BCEBAR1]
// ppe_eq/reg00012.H

//>> [QME_FLAGS]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_FLAGS_RW = 0xc0001200ull;
static const uint32_t QME_FLAGS_WO_CLEAR = 0xc0001230ull;
static const uint32_t QME_FLAGS_WO_OR = 0xc0001220ull;

static const uint32_t QME_FLAGS_DATA = 0;
static const uint32_t QME_FLAGS_DATA_LEN = 64;
//<< [QME_FLAGS]
// ppe_eq/reg00012.H

//>> [QME_RCIMR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCIMR = 0xc00018c0ull;

static const uint32_t QME_RCIMR_P0 = 0;
static const uint32_t QME_RCIMR_P0_LEN = 8;
static const uint32_t QME_RCIMR_X0 = 11;
static const uint32_t QME_RCIMR_X0_LEN = 5;
static const uint32_t QME_RCIMR_P1 = 16;
static const uint32_t QME_RCIMR_P1_LEN = 8;
static const uint32_t QME_RCIMR_X1 = 27;
static const uint32_t QME_RCIMR_X1_LEN = 5;
static const uint32_t QME_RCIMR_P2 = 32;
static const uint32_t QME_RCIMR_P2_LEN = 8;
static const uint32_t QME_RCIMR_X2 = 43;
static const uint32_t QME_RCIMR_X2_LEN = 5;
static const uint32_t QME_RCIMR_P3 = 48;
static const uint32_t QME_RCIMR_P3_LEN = 8;
static const uint32_t QME_RCIMR_X3 = 59;
static const uint32_t QME_RCIMR_X3_LEN = 5;
//<< [QME_RCIMR]
// ppe_eq/reg00012.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00012.H"
#endif
#endif
