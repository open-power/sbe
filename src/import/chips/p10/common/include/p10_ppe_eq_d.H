/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_d.H $          */
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

#ifndef __p10_ppe_eq_d_H_
#define __p10_ppe_eq_d_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_EIMR_RW = 0xc0002640ull;
static const uint32_t QME_EIMR_WO_CLEAR = 0xc0002670ull;
static const uint32_t QME_EIMR_WO_OR = 0xc0002660ull;

static const uint32_t QME_EIMR_QME_EIMR_INTERRUPT_MASK = 0;
static const uint32_t QME_EIMR_QME_EIMR_INTERRUPT_MASK_LEN = 64;
// ppe_eq/reg00013.H

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
// ppe_eq/reg00013.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QSCR_RW = 0xc0000700ull;
static const uint32_t QME_QSCR_WO_CLEAR = 0xc0000730ull;
static const uint32_t QME_QSCR_WO_OR = 0xc0000720ull;
// ppe_eq/reg00013.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCMR = 0xc0001800ull;
static const uint32_t QME_RCMR_WO_CLEAR = 0xc0001830ull;
static const uint32_t QME_RCMR_WO_OR = 0xc0001820ull;

static const uint32_t QME_RCMR_MR_STEP_ENABLE = 0;
static const uint32_t QME_RCMR_MR_AUTO_ACK_ENABLE = 1;
static const uint32_t QME_RCMR_MR_AUTO_TARGET_DISABLE = 2;
static const uint32_t QME_RCMR_MR_AUTO_DONE_DISABLE = 3;
static const uint32_t QME_RCMR_MR_STEP_DELAY = 4;
static const uint32_t QME_RCMR_MR_STEP_DELAY_LEN = 4;
static const uint32_t QME_RCMR_SCR_CORE_CHANGE_ERROR = 8;
static const uint32_t QME_RCMR_SCR_CORE_CHANGE_ERROR_LEN = 4;
static const uint32_t QME_RCMR_MR_CORE0_CONTROL_INDEX = 35;
static const uint32_t QME_RCMR_MR_CORE0_CONTROL_INDEX_LEN = 5;
static const uint32_t QME_RCMR_MR_CORE1_CONTROL_INDEX = 43;
static const uint32_t QME_RCMR_MR_CORE1_CONTROL_INDEX_LEN = 5;
static const uint32_t QME_RCMR_MR_CORE2_CONTROL_INDEX = 51;
static const uint32_t QME_RCMR_MR_CORE2_CONTROL_INDEX_LEN = 5;
static const uint32_t QME_RCMR_MR_CORE3_CONTROL_INDEX = 59;
static const uint32_t QME_RCMR_MR_CORE3_CONTROL_INDEX_LEN = 5;
// ppe_eq/reg00013.H

}
}
#ifndef __PPE_HCODE__
    #include "ppe_eq/reg00013.H"
#endif
#endif
