/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_9.H $           */
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

#ifndef __p10_ppe_c_9_H_
#define __p10_ppe_c_9_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


static const uint32_t CPMS_DDSR = 0xc000e500ull;

static const uint32_t CPMS_DDSR_DDS_DATA = 3;
static const uint32_t CPMS_DDSR_DDS_DATA_LEN = 5;
static const uint32_t CPMS_DDSR_DDS_MIN = 11;
static const uint32_t CPMS_DDSR_DDS_MIN_LEN = 5;
static const uint32_t CPMS_DDSR_DDS_MAX = 19;
static const uint32_t CPMS_DDSR_DDS_MAX_LEN = 5;
static const uint32_t CPMS_DDSR_DPLL_ENCODED_DATA = 33;
static const uint32_t CPMS_DDSR_DPLL_ENCODED_DATA_LEN = 3;
static const uint32_t CPMS_DDSR_DPLL_ENCODED_MIN = 37;
static const uint32_t CPMS_DDSR_DPLL_ENCODED_MIN_LEN = 3;
static const uint32_t CPMS_DDSR_DPLL_ENCODED_MAX = 41;
static const uint32_t CPMS_DDSR_DPLL_ENCODED_MAX_LEN = 3;
static const uint32_t CPMS_DDSR_PDSR_RESET_OCCURRED = 59;
static const uint32_t CPMS_DDSR_STOP_STATE_ACTIVE = 60;
static const uint32_t CPMS_DDSR_CORE_SAMPLE_DISABLED = 61;
static const uint32_t CPMS_DDSR_STOP_STATE_OCCURRED = 62;
static const uint32_t CPMS_DDSR_SAMPLE_DISABLE_OCCURRED = 63;
// ppe_c/reg00009.H

static const uint32_t CPMS_FDIR = 0xc000e440ull;
static const uint32_t CPMS_FDIR_WO_CLEAR = 0xc000e470ull;
static const uint32_t CPMS_FDIR_WO_OR = 0xc000e460ull;

static const uint32_t CPMS_FDIR_INJECT_ENABLE = 0;
static const uint32_t CPMS_FDIR_HALT_INJECT = 1;
static const uint32_t CPMS_FDIR_HALT_TOGGLE_INJECT = 2;
static const uint32_t CPMS_FDIR_FETCH_THROTTLE10_INJECT = 3;
static const uint32_t CPMS_FDIR_FETCH_THROTTLE01_INJECT = 4;
static const uint32_t CPMS_FDIR_ISSUE_NTC_INJECT = 5;
static const uint32_t CPMS_FDIR_PREFETCH_THROTTLE_INJECT = 6;
static const uint32_t CPMS_FDIR_PREFETCH_THROTTLE_INJECT_LEN = 2;
static const uint32_t CPMS_FDIR_SLOW_FETCH_INJECT = 8;
static const uint32_t CPMS_FDIR_VECTOR_LIMIT_INJECT = 9;
static const uint32_t CPMS_FDIR_VECTOR_LIMIT_INJECT_LEN = 3;
static const uint32_t CPMS_FDIR_SLOW_COMPLETION_INJECT = 12;
static const uint32_t CPMS_FDIR_MMA_UARCH_ENABLE_INJECT = 13;
static const uint32_t CPMS_FDIR_MMA_LIMIT = 14;
static const uint32_t CPMS_FDIR_MMA_LIMIT_LEN = 2;
static const uint32_t CPMS_FDIR_TOGGLE_HALT_AMOUNT = 16;
static const uint32_t CPMS_FDIR_TOGGLE_HALT_AMOUNT_LEN = 4;
static const uint32_t CPMS_FDIR_IRRITATE_INJECT_RESPONSE = 20;
static const uint32_t CPMS_FDIR_IRRITATE_INJECT_RESPONSE_LEN = 2;
static const uint32_t CPMS_FDIR_IRRITATE_LFSR_SELECT = 22;
static const uint32_t CPMS_FDIR_IRRITATE_LFSR_SELECT_LEN = 2;
static const uint32_t CPMS_FDIR_IRRITATE_SMALL_SELECT = 24;
static const uint32_t CPMS_FDIR_IRRITATE_SMALL_SELECT_LEN = 2;
static const uint32_t CPMS_FDIR_IRRITATE_LARGE_SELECT = 26;
static const uint32_t CPMS_FDIR_IRRITATE_LARGE_SELECT_LEN = 2;
static const uint32_t CPMS_FDIR_EXCLUDE_LSU_HALT = 28;
static const uint32_t CPMS_FDIR_IRRITATE_WEIGHT_SELECT = 29;
static const uint32_t CPMS_FDIR_IRRITATE_WEIGHT_SELECT_LEN = 2;
static const uint32_t CPMS_FDIR_INJECT_DPT_SUSPEND = 31;
// ppe_c/reg00009.H

static const uint32_t QME_CDBB = 0xc0008040ull;

static const uint32_t QME_CDBB_MESSAGE_NUMBER = 0;
static const uint32_t QME_CDBB_MESSAGE_NUMBER_LEN = 8;
// ppe_c/reg00009.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00009.H"
#endif
#endif
