/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_a.H $          */
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

#ifndef __p10_ppe_eq_a_H_
#define __p10_ppe_eq_a_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_BCEBAR0 = 0xc0000600ull;

static const uint32_t QME_BCEBAR0_BASE = 8;
static const uint32_t QME_BCEBAR0_BASE_LEN = 36;
static const uint32_t QME_BCEBAR0_SIZE = 61;
static const uint32_t QME_BCEBAR0_SIZE_LEN = 3;
// ppe_eq/reg00010.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QSDR = 0xc0000780ull;

static const uint32_t QME_QSDR_QME_QSDR_SRAM_DATA = 0;
static const uint32_t QME_QSDR_QME_QSDR_SRAM_DATA_LEN = 64;
// ppe_eq/reg00010.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCTAR0 = 0xc0001900ull;

static const uint32_t QME_RCTAR0_QME_RCTAR0_DATA = 0;
static const uint32_t QME_RCTAR0_QME_RCTAR0_DATA_LEN = 64;
// ppe_eq/reg00010.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_PBTXTR1 = 0xc0000410ull;

static const uint32_t QME_SCOM_PBTXTR1__VALID = 0;
static const uint32_t QME_SCOM_PBTXTR1__VALID_LEN = 8;
static const uint32_t QME_SCOM_PBTXTR1_0 = 8;
static const uint32_t QME_SCOM_PBTXTR1_0_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_1 = 12;
static const uint32_t QME_SCOM_PBTXTR1_1_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_2 = 16;
static const uint32_t QME_SCOM_PBTXTR1_2_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_3 = 20;
static const uint32_t QME_SCOM_PBTXTR1_3_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_4 = 24;
static const uint32_t QME_SCOM_PBTXTR1_4_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_5 = 28;
static const uint32_t QME_SCOM_PBTXTR1_5_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_6 = 32;
static const uint32_t QME_SCOM_PBTXTR1_6_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR1_7 = 36;
static const uint32_t QME_SCOM_PBTXTR1_7_LEN = 4;
// ppe_eq/reg00010.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIVDR0 = 0xc0002800ull;

static const uint32_t QME_SCOM_XIVDR0_0 = 0;
static const uint32_t QME_SCOM_XIVDR0_0_LEN = 32;
static const uint32_t QME_SCOM_XIVDR0_1 = 32;
static const uint32_t QME_SCOM_XIVDR0_1_LEN = 32;
// ppe_eq/reg00010.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_TSEL = 0xc0002400ull;

static const uint32_t QME_TSEL_WATCHDOG_SEL = 0;
static const uint32_t QME_TSEL_WATCHDOG_SEL_LEN = 4;
static const uint32_t QME_TSEL_FIT_SEL = 4;
static const uint32_t QME_TSEL_FIT_SEL_LEN = 4;
// ppe_eq/reg00010.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00010.H"
#endif
#endif
