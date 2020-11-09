/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_d.H $          */
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

#ifndef __p10_ppe_eq_d_H_
#define __p10_ppe_eq_d_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_EIMR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_EIMR_RW = 0xc0002640ull;
static const uint32_t QME_EIMR_WO_CLEAR = 0xc0002670ull;
static const uint32_t QME_EIMR_WO_OR = 0xc0002660ull;

static const uint32_t QME_EIMR_INTERRUPT_MASK = 0;
static const uint32_t QME_EIMR_INTERRUPT_MASK_LEN = 64;
//<< [QME_EIMR]
// ppe_eq/reg00013.H

//>> [QME_PIG]
//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_PIG = 0xc0000300ull;

static const uint32_t QME_PIG_REQ_INTR_TYPE = 1;
static const uint32_t QME_PIG_REQ_INTR_TYPE_LEN = 4;
static const uint32_t QME_PIG_REQ_INTR_PAYLOAD = 5;
static const uint32_t QME_PIG_REQ_INTR_PAYLOAD_LEN = 19;
static const uint32_t QME_PIG_PENDING_SOURCE = 24;
static const uint32_t QME_PIG_PENDING_SOURCE_LEN = 6;
static const uint32_t QME_PIG_INTR_GRANTED = 32;
static const uint32_t QME_PIG_GRANTED_SOURCE = 33;
static const uint32_t QME_PIG_GRANTED_SOURCE_LEN = 3;
static const uint32_t QME_PIG_QUAD_ID = 37;
static const uint32_t QME_PIG_QUAD_ID_LEN = 3;
static const uint32_t QME_PIG_GRANTED_PACKET = 40;
static const uint32_t QME_PIG_GRANTED_PACKET_LEN = 24;
//<< [QME_PIG]
// ppe_eq/reg00013.H

//>> [QME_QSCR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QSCR_RW = 0xc0000700ull;
static const uint32_t QME_QSCR_WO_CLEAR = 0xc0000730ull;
static const uint32_t QME_QSCR_WO_OR = 0xc0000720ull;

static const uint32_t QME_QSCR_SRAM_ACCESS_MODE = 0;
static const uint32_t P10_20_QME_QSCR_SRAM_ACCESS_MODE = 0; // p10:20,
static const uint32_t QME_QSCR_SRAM_SCRUB_ENABLE = 1;
static const uint32_t P10_20_QME_QSCR_SRAM_SCRUB_ENABLE = 1; // p10:20,
static const uint32_t QME_QSCR_ECC_CORRECT_DIS = 2;
static const uint32_t P10_20_QME_QSCR_ECC_CORRECT_DIS = 2; // p10:20,
static const uint32_t QME_QSCR_ECC_DETECT_DIS = 3;
static const uint32_t P10_20_QME_QSCR_ECC_DETECT_DIS = 3; // p10:20,
static const uint32_t QME_QSCR_ECC_INJECT_TYPE = 4;
static const uint32_t P10_20_QME_QSCR_ECC_INJECT_TYPE = 4; // p10:20,
static const uint32_t QME_QSCR_ECC_INJECT_ERR = 5;
static const uint32_t P10_20_QME_QSCR_ECC_INJECT_ERR = 5; // p10:20,
static const uint32_t QME_QSCR_SPARE_6_7 = 6;
static const uint32_t QME_QSCR_SPARE_6_7_LEN = 2;
static const uint32_t P10_20_QME_QSCR_SPARE_6_7 = 6; // p10:20,
static const uint32_t P10_20_QME_QSCR_SPARE_6_7_LEN = 2;
static const uint32_t QME_QSCR_DTCBASE = 32;
static const uint32_t QME_QSCR_DTCBASE_LEN = 8;
static const uint32_t P10_20_QME_QSCR_DTCBASE = 32; // p10:20,
static const uint32_t P10_20_QME_QSCR_DTCBASE_LEN = 8;
static const uint32_t QME_QSCR_SRAM_SCRUB_INDEX = 47;
static const uint32_t QME_QSCR_SRAM_SCRUB_INDEX_LEN = 13;
static const uint32_t P10_20_QME_QSCR_SRAM_SCRUB_INDEX = 47; // p10:20,
static const uint32_t P10_20_QME_QSCR_SRAM_SCRUB_INDEX_LEN = 13;
//<< [QME_QSCR]
// ppe_eq/reg00013.H

//>> [QME_RCMR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCMR = 0xc0001800ull;
static const uint32_t QME_RCMR_WO_CLEAR = 0xc0001830ull;
static const uint32_t QME_RCMR_WO_OR = 0xc0001820ull;

static const uint32_t QME_RCMR_STEP_ENABLE = 0;
static const uint32_t QME_RCMR_AUTO_ACK_ENABLE = 1;
static const uint32_t QME_RCMR_AUTO_TARGET_DISABLE = 2;
static const uint32_t QME_RCMR_AUTO_DONE_DISABLE = 3;
static const uint32_t QME_RCMR_STEP_DELAY = 4;
static const uint32_t QME_RCMR_STEP_DELAY_LEN = 4;
static const uint32_t QME_RCMR_CORE0_CONTROL_INDEX = 35;
static const uint32_t QME_RCMR_CORE0_CONTROL_INDEX_LEN = 5;
static const uint32_t QME_RCMR_CORE1_CONTROL_INDEX = 43;
static const uint32_t QME_RCMR_CORE1_CONTROL_INDEX_LEN = 5;
static const uint32_t QME_RCMR_CORE2_CONTROL_INDEX = 51;
static const uint32_t QME_RCMR_CORE2_CONTROL_INDEX_LEN = 5;
static const uint32_t QME_RCMR_CORE3_CONTROL_INDEX = 59;
static const uint32_t QME_RCMR_CORE3_CONTROL_INDEX_LEN = 5;
//<< [QME_RCMR]
// ppe_eq/reg00013.H

//>> [QME_WCOR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_WCOR = 0xc0001740ull;

static const uint32_t QME_WCOR_0_OVER_THROTTLED = 1;
static const uint32_t QME_WCOR_0_THROTTLE_INDEX = 2;
static const uint32_t QME_WCOR_0_THROTTLE_INDEX_LEN = 6;
static const uint32_t QME_WCOR_1_OVER_THROTTLED = 9;
static const uint32_t QME_WCOR_1_THROTTLE_INDEX = 10;
static const uint32_t QME_WCOR_1_THROTTLE_INDEX_LEN = 6;
static const uint32_t QME_WCOR_2_OVER_THROTTLED = 17;
static const uint32_t QME_WCOR_2_THROTTLE_INDEX = 18;
static const uint32_t QME_WCOR_2_THROTTLE_INDEX_LEN = 6;
static const uint32_t QME_WCOR_3_OVER_THROTTLED = 25;
static const uint32_t QME_WCOR_3_THROTTLE_INDEX = 26;
static const uint32_t QME_WCOR_3_THROTTLE_INDEX_LEN = 6;
//<< [QME_WCOR]
// ppe_eq/reg00013.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00013.H"
#endif
#endif
