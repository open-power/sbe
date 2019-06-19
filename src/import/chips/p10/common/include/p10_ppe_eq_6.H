/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_6.H $          */
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

#ifndef __p10_ppe_eq_6_H_
#define __p10_ppe_eq_6_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PPEBAR = 0xc0000500ull;

static const uint32_t QME_PPEBAR_BASE = 8;
static const uint32_t QME_PPEBAR_BASE_LEN = 36;
static const uint32_t QME_PPEBAR_SIZE = 61;
static const uint32_t QME_PPEBAR_SIZE_LEN = 3;
// ppe_eq/reg00006.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCTAR2 = 0xc0001980ull;

static const uint32_t QME_RCTAR2_QME_RCTAR2_DATA = 0;
static const uint32_t QME_RCTAR2_QME_RCTAR2_DATA_LEN = 64;
// ppe_eq/reg00006.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_PBTXTR3 = 0xc0000430ull;

static const uint32_t QME_SCOM_PBTXTR3__VALID = 0;
static const uint32_t QME_SCOM_PBTXTR3__VALID_LEN = 8;
static const uint32_t QME_SCOM_PBTXTR3_0 = 8;
static const uint32_t QME_SCOM_PBTXTR3_0_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_1 = 12;
static const uint32_t QME_SCOM_PBTXTR3_1_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_2 = 16;
static const uint32_t QME_SCOM_PBTXTR3_2_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_3 = 20;
static const uint32_t QME_SCOM_PBTXTR3_3_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_4 = 24;
static const uint32_t QME_SCOM_PBTXTR3_4_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_5 = 28;
static const uint32_t QME_SCOM_PBTXTR3_5_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_6 = 32;
static const uint32_t QME_SCOM_PBTXTR3_6_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR3_7 = 36;
static const uint32_t QME_SCOM_PBTXTR3_7_LEN = 4;
// ppe_eq/reg00006.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIRAMEDR = 0xc0002100ull;

static const uint32_t QME_SCOM_XIRAMEDR_IR = 0;
static const uint32_t QME_SCOM_XIRAMEDR_IR_LEN = 32;
static const uint32_t QME_SCOM_XIRAMEDR_EDR = 32;
static const uint32_t QME_SCOM_XIRAMEDR_EDR_LEN = 32;
// ppe_eq/reg00006.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIVDR2 = 0xc0002840ull;

static const uint32_t QME_SCOM_XIVDR2_2 = 0;
static const uint32_t QME_SCOM_XIVDR2_2_LEN = 32;
static const uint32_t QME_SCOM_XIVDR2_3 = 32;
static const uint32_t QME_SCOM_XIVDR2_3_LEN = 32;
// ppe_eq/reg00006.H

}
}
#ifndef __PPE_HCODE__
    #include "ppe_eq/reg00006.H"
#endif
#endif
