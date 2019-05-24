/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_c.H $          */
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

#include "proc_scomt.H"

#ifndef __p10_ppe_eq_c_H_
#define __p10_ppe_eq_c_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_BCEBAR1 = 0xc0000640ull;

static const uint32_t QME_BCEBAR1_BASE = 8;
static const uint32_t QME_BCEBAR1_BASE_LEN = 36;
static const uint32_t QME_BCEBAR1_SIZE = 61;
static const uint32_t QME_BCEBAR1_SIZE_LEN = 3;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_FLAGS_RW = 0xc0001200ull;
static const uint32_t QME_FLAGS_WO_CLEAR = 0xc0001230ull;
static const uint32_t QME_FLAGS_WO_OR = 0xc0001220ull;

static const uint32_t QME_FLAGS_QME_FLAGS_DATA = 0;
static const uint32_t QME_FLAGS_QME_FLAGS_DATA_LEN = 64;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PPEDBG_RW = 0xc0002440ull;
static const uint32_t QME_PPEDBG_WO_CLEAR = 0xc0002470ull;
static const uint32_t QME_PPEDBG_WO_OR = 0xc0002460ull;

static const uint32_t QME_PPEDBG_EN_DBG = 0;
static const uint32_t QME_PPEDBG_HALT_ON_XSTOP = 1;
static const uint32_t QME_PPEDBG_HALT_ON_TRIG = 2;
static const uint32_t QME_PPEDBG_EN_COVERAGE_MODE = 3;
static const uint32_t QME_PPEDBG_EN_INTR_ADDR = 4;
static const uint32_t QME_PPEDBG_EN_TRACE_EXTRA = 5;
static const uint32_t QME_PPEDBG_EN_TRACE_STALL = 6;
static const uint32_t QME_PPEDBG_EN_WAIT_CYCLES = 7;
static const uint32_t QME_PPEDBG_EN_FULL_SPEED = 8;
static const uint32_t QME_PPEDBG_DIS_FLOW_CHANGE = 9;
static const uint32_t QME_PPEDBG_TRACE_MODE_SEL = 10;
static const uint32_t QME_PPEDBG_TRACE_MODE_SEL_LEN = 2;
static const uint32_t QME_PPEDBG_FIR_TRIGGER = 16;
static const uint32_t QME_PPEDBG_MIB_GPIO = 17;
static const uint32_t QME_PPEDBG_MIB_GPIO_LEN = 3;
static const uint32_t QME_PPEDBG_TRACE_DATA_SEL = 20;
static const uint32_t QME_PPEDBG_TRACE_DATA_SEL_LEN = 4;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PTSR = 0xc00013c0ull;
// ppe_eq/reg00012.H

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
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCTAR1 = 0xc0001940ull;

static const uint32_t QME_RCTAR1_QME_RCTAR1_DATA = 0;
static const uint32_t QME_RCTAR1_QME_RCTAR1_DATA_LEN = 64;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_PBTXTR0 = 0xc0000400ull;

static const uint32_t QME_SCOM_PBTXTR0__VALID = 0;
static const uint32_t QME_SCOM_PBTXTR0__VALID_LEN = 8;
static const uint32_t QME_SCOM_PBTXTR0_0 = 8;
static const uint32_t QME_SCOM_PBTXTR0_0_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_1 = 12;
static const uint32_t QME_SCOM_PBTXTR0_1_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_2 = 16;
static const uint32_t QME_SCOM_PBTXTR0_2_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_3 = 20;
static const uint32_t QME_SCOM_PBTXTR0_3_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_4 = 24;
static const uint32_t QME_SCOM_PBTXTR0_4_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_5 = 28;
static const uint32_t QME_SCOM_PBTXTR0_5_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_6 = 32;
static const uint32_t QME_SCOM_PBTXTR0_6_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR0_7 = 36;
static const uint32_t QME_SCOM_PBTXTR0_7_LEN = 4;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIRAMDBG = 0xc00020c0ull;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIRAMGA = 0xc0002080ull;

static const uint32_t QME_SCOM_XIRAMGA_QME_SCOM_XIRAMEDR_IR = 0;
static const uint32_t QME_SCOM_XIRAMGA_QME_SCOM_XIRAMEDR_IR_LEN = 32;
// ppe_eq/reg00012.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XISIB = 0xc0002180ull;

static const uint32_t QME_SCOM_XISIB_ADDR = 0;
static const uint32_t QME_SCOM_XISIB_ADDR_LEN = 32;
static const uint32_t QME_SCOM_XISIB_R_NW = 32;
static const uint32_t QME_SCOM_XISIB_BUSY = 33;
static const uint32_t QME_SCOM_XISIB_IMPRECISE_ERROR_PENDING = 34;
static const uint32_t QME_SCOM_XISIB_RSP_INFO = 49;
static const uint32_t QME_SCOM_XISIB_RSP_INFO_LEN = 3;
static const uint32_t QME_SCOM_XISIB_RESET_PENDING = 61;
static const uint32_t QME_SCOM_XISIB_IFETCH_PENDING = 62;
static const uint32_t QME_SCOM_XISIB_DATAOP_PENDING = 63;
// ppe_eq/reg00012.H

}
}
#include "ppe_eq/reg00012.H"
#endif
