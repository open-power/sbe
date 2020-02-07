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

static const uint32_t QME_FLAGS_DATA = 0;
static const uint32_t QME_FLAGS_DATA_LEN = 64;
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
static const uint32_t QME_PPEDBG_EN_MARK_TRACE = 12;
static const uint32_t QME_PPEDBG_EN_EE_TRACE = 13;
static const uint32_t QME_PPEDBG_TRACE_LOWER_SEL1 = 14;
static const uint32_t QME_PPEDBG_TRACE_LOWER_SEL1_LEN = 2;
static const uint32_t QME_PPEDBG_FIR_TRIGGER = 16;
static const uint32_t QME_PPEDBG_MIB_GPIO = 17;
static const uint32_t QME_PPEDBG_MIB_GPIO_LEN = 3;
static const uint32_t QME_PPEDBG_TRACE_DATA_SEL = 20;
static const uint32_t QME_PPEDBG_TRACE_DATA_SEL_LEN = 4;
static const uint32_t QME_PPEDBG_QHTM_TRACE_SEL = 25;
static const uint32_t QME_PPEDBG_TRACE_EXT_SEL0 = 26;
static const uint32_t QME_PPEDBG_TRACE_MODE_SEL1 = 27;
static const uint32_t QME_PPEDBG_TRACE_DATA_SEL1 = 28;
static const uint32_t QME_PPEDBG_TRACE_DATA_SEL1_LEN = 4;
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

static const uint32_t QME_RCTAR1_DATA = 0;
static const uint32_t QME_RCTAR1_DATA_LEN = 64;
// ppe_eq/reg00012.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00012.H"
#endif
#endif
