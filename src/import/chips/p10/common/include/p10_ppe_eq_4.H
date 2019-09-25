/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_4.H $          */
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

#ifndef __p10_ppe_eq_4_H_
#define __p10_ppe_eq_4_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_EISTR = 0xc0002700ull;

static const uint32_t QME_EISTR_INTERRUPT_STATUS = 0;
static const uint32_t QME_EISTR_INTERRUPT_STATUS_LEN = 64;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_FTXR = 0xc0001780ull;

static const uint32_t QME_FTXR_C0_DTC_INDEX = 2;
static const uint32_t QME_FTXR_C0_DTC_INDEX_LEN = 6;
static const uint32_t QME_FTXR_C1_DTC_INDEX = 10;
static const uint32_t QME_FTXR_C1_DTC_INDEX_LEN = 6;
static const uint32_t QME_FTXR_C2_DTC_INDEX = 18;
static const uint32_t QME_FTXR_C2_DTC_INDEX_LEN = 6;
static const uint32_t QME_FTXR_C3_DTC_INDEX = 26;
static const uint32_t QME_FTXR_C3_DTC_INDEX_LEN = 6;
static const uint32_t QME_FTXR_FTX_UPDATE_PENDING = 32;
static const uint32_t QME_FTXR_FTX_UPDATE_PENDING_LEN = 4;
static const uint32_t QME_FTXR_QOS_UPDATE_PENDING = 36;
static const uint32_t QME_FTXR_QOS_UPDATE_PENDING_LEN = 4;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_LFIRACT0 = 0xc0000080ull;

static const uint32_t QME_LFIRACT0_FIR_ACTION0 = 0;
static const uint32_t QME_LFIRACT0_FIR_ACTION0_LEN = 34;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PLSR = 0xc0001480ull;

static const uint32_t QME_PLSR_0_T0 = 0;
static const uint32_t QME_PLSR_0_T0_LEN = 4;
static const uint32_t QME_PLSR_0_T1 = 4;
static const uint32_t QME_PLSR_0_T1_LEN = 4;
static const uint32_t QME_PLSR_0_T2 = 8;
static const uint32_t QME_PLSR_0_T2_LEN = 4;
static const uint32_t QME_PLSR_0_T3 = 12;
static const uint32_t QME_PLSR_0_T3_LEN = 4;
static const uint32_t QME_PLSR_1_T0 = 16;
static const uint32_t QME_PLSR_1_T0_LEN = 4;
static const uint32_t QME_PLSR_1_T1 = 20;
static const uint32_t QME_PLSR_1_T1_LEN = 4;
static const uint32_t QME_PLSR_1_T2 = 24;
static const uint32_t QME_PLSR_1_T2_LEN = 4;
static const uint32_t QME_PLSR_1_T3 = 28;
static const uint32_t QME_PLSR_1_T3_LEN = 4;
static const uint32_t QME_PLSR_2_T0 = 32;
static const uint32_t QME_PLSR_2_T0_LEN = 4;
static const uint32_t QME_PLSR_2_T1 = 36;
static const uint32_t QME_PLSR_2_T1_LEN = 4;
static const uint32_t QME_PLSR_2_T2 = 40;
static const uint32_t QME_PLSR_2_T2_LEN = 4;
static const uint32_t QME_PLSR_2_T3 = 44;
static const uint32_t QME_PLSR_2_T3_LEN = 4;
static const uint32_t QME_PLSR_3_T0 = 48;
static const uint32_t QME_PLSR_3_T0_LEN = 4;
static const uint32_t QME_PLSR_3_T1 = 52;
static const uint32_t QME_PLSR_3_T1_LEN = 4;
static const uint32_t QME_PLSR_3_T2 = 56;
static const uint32_t QME_PLSR_3_T2_LEN = 4;
static const uint32_t QME_PLSR_3_T3 = 60;
static const uint32_t QME_PLSR_3_T3_LEN = 4;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QHB = 0xc0000800ull;

static const uint32_t QME_QHB_COUNT = 0;
static const uint32_t QME_QHB_COUNT_LEN = 16;
static const uint32_t QME_QHB_ENABLE = 16;
static const uint32_t QME_QHB_LOSS_THROTTLE_ENABLE = 17;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCSCR = 0xc0001840ull;
static const uint32_t QME_RCSCR_WO_CLEAR = 0xc0001870ull;
static const uint32_t QME_RCSCR_WO_OR = 0xc0001860ull;

static const uint32_t QME_RCSCR_OFF_REQ = 0;
static const uint32_t QME_RCSCR_OFF_REQ_LEN = 4;
static const uint32_t QME_RCSCR_CHANGE_DONE = 4;
static const uint32_t QME_RCSCR_CHANGE_DONE_LEN = 4;
static const uint32_t QME_RCSCR_CHANGE_ACTIVE = 8;
static const uint32_t QME_RCSCR_CHANGE_ACTIVE_LEN = 4;
static const uint32_t QME_RCSCR_PARTIAL_GOOD = 12;
static const uint32_t QME_RCSCR_PARTIAL_GOOD_LEN = 4;
static const uint32_t QME_RCSCR_CHANGE_ERROR = 16;
static const uint32_t QME_RCSCR_CHANGE_ERROR_LEN = 4;
static const uint32_t QME_RCSCR_0_TARGET_INDEX = 35;
static const uint32_t QME_RCSCR_0_TARGET_INDEX_LEN = 5;
static const uint32_t QME_RCSCR_1_TARGET_INDEX = 43;
static const uint32_t QME_RCSCR_1_TARGET_INDEX_LEN = 5;
static const uint32_t QME_RCSCR_2_TARGET_INDEX = 51;
static const uint32_t QME_RCSCR_2_TARGET_INDEX_LEN = 5;
static const uint32_t QME_RCSCR_3_TARGET_INDEX = 59;
static const uint32_t QME_RCSCR_3_TARGET_INDEX_LEN = 5;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RS4BAR = 0xc0001600ull;

static const uint32_t QME_RS4BAR_BASE = 0;
static const uint32_t QME_RS4BAR_BASE_LEN = 32;
// ppe_eq/reg00004.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RS4DBGD = 0xc0001580ull;

static const uint32_t QME_RS4DBGD_SCAN_DATA_IN = 0;
static const uint32_t QME_RS4DBGD_SCAN_DATA_IN_LEN = 64;
// ppe_eq/reg00004.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00004.H"
#endif
#endif
