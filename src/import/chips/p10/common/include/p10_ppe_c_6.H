/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_6.H $           */
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

#ifndef __p10_ppe_c_6_H_
#define __p10_ppe_c_6_H_


namespace scomt
{
namespace ppe_c
{


static const uint32_t CPMS_CERR = 0xc000e100ull;

static const uint32_t CPMS_CERR_L3_PFET_SEQ_ERR = 0;
static const uint32_t CPMS_CERR_CL2_PFET_SEQ_ERR = 1;
static const uint32_t CPMS_CERR_MMA_PFET_SEQ_ERR = 2;
static const uint32_t CPMS_CERR_DDS_ERR = 3;
static const uint32_t CPMS_CERR_L3_CLOCK_SYNC_ERR = 4;
static const uint32_t CPMS_CERR_CL2_CLOCK_SYNC_ERR = 5;
static const uint32_t CPMS_CERR_DPT_ACCESS_ERROR = 6;
static const uint32_t CPMS_CERR_SPARE_ERR0 = 7;
static const uint32_t CPMS_CERR_SPARE_ERR1 = 8;
static const uint32_t CPMS_CERR_FTC_DEBUG_EVENT = 9;
static const uint32_t CPMS_CERR_FTC_DEBUG_EVENT_LEN = 3;
// ppe_c/reg00006.H

static const uint32_t CPMS_CUCR = 0xc000e3c0ull;
static const uint32_t CPMS_CUCR_PPE1 = 0xc000e3e0ull;
static const uint32_t CPMS_CUCR_PPE2 = 0xc000e3f0ull;

static const uint32_t CPMS_CUCR_ENABLE_CORE_SHADOW = 0;
static const uint32_t CPMS_CUCR_ENABLE_CORE_SAMPLE = 1;
static const uint32_t CPMS_CUCR_DPT_PER_THREAD_MODE = 2;
static const uint32_t CPMS_CUCR_SPARE = 3;
static const uint32_t CPMS_CUCR_SPARE_LEN = 2;
static const uint32_t CPMS_CUCR_REFRESH_FTC_SHADOW = 9;
static const uint32_t CPMS_CUCR_REFRESH_PP_SHADOW = 10;
static const uint32_t CPMS_CUCR_REFRESH_DPT_SETTINGS = 11;
static const uint32_t CPMS_CUCR_INDUCED_DROOP_COUNT = 12;
static const uint32_t CPMS_CUCR_INDUCED_DROOP_COUNT_LEN = 3;
static const uint32_t CPMS_CUCR_PCB_SKEW_ADJ = 17;
static const uint32_t CPMS_CUCR_PCB_SKEW_ADJ_LEN = 3;
static const uint32_t CPMS_CUCR_FTC_SHADOW_STATE = 33;
static const uint32_t CPMS_CUCR_FTC_SHADOW_STATE_LEN = 3;
static const uint32_t CPMS_CUCR_FTC_SHADOW_PENDING = 36;
static const uint32_t CPMS_CUCR_FTC_SHADOW_PENDING_LEN = 4;
static const uint32_t CPMS_CUCR_PP_SHADOW_STATE = 40;
static const uint32_t CPMS_CUCR_PP_SHADOW_STATE_LEN = 2;
static const uint32_t CPMS_CUCR_PP_SHADOW_PENDING = 42;
static const uint32_t CPMS_CUCR_PP_SHADOW_PENDING_LEN = 3;
static const uint32_t CPMS_CUCR_DPT_UPDATE_STATE = 45;
static const uint32_t CPMS_CUCR_DPT_UPDATE_STATE_LEN = 2;
static const uint32_t CPMS_CUCR_DPT_UPDATE_PENDING = 47;
static const uint32_t CPMS_CUCR_DPT_UPDATE_PENDING_LEN = 3;
static const uint32_t CPMS_CUCR_CORE_REFRESH_ACTIVE = 50;
static const uint32_t CPMS_CUCR_CORE_REFRESH_ACTIVE_LEN = 3;
static const uint32_t CPMS_CUCR_FDCR_UPDATE_INPROGRESS = 56;
static const uint32_t CPMS_CUCR_QME_EVENTS_PENDING = 57;
static const uint32_t CPMS_CUCR_QME_EVENTS_PENDING_LEN = 7;
// ppe_c/reg00006.H

static const uint32_t CPMS_MMA_PFETCNTL = 0xc000e300ull;
static const uint32_t CPMS_MMA_PFETCNTL_WO_CLEAR = 0xc000e320ull;
static const uint32_t CPMS_MMA_PFETCNTL_WO_OR = 0xc000e330ull;

static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_SEQ_STATE = 0;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_SEQ_STATE_LEN = 2;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_VAL_OVERRIDE = 4;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_SEL_OVERRIDE = 5;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_ENABLE_VALUE = 12;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_ENABLE_VALUE_LEN = 8;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_SEL_VALUE = 20;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PFET_SEL_VALUE_LEN = 4;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PG_STATE = 42;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PG_STATE_LEN = 4;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PG_SEL = 46;
static const uint32_t CPMS_MMA_PFETCNTL_VDD_PG_SEL_LEN = 4;
static const uint32_t CPMS_MMA_PFETCNTL_MMA_FORCE_CL2 = 63;
// ppe_c/reg00006.H

}
}
#ifndef __PPE_HCODE__
    #include "ppe_c/reg00006.H"
#endif
#endif
