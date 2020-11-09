/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_6.H $           */
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

#ifndef __p10_ppe_c_6_H_
#define __p10_ppe_c_6_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_CERR]
static const uint32_t CPMS_CERR = 0xc000e100ull;

static const uint32_t CPMS_CERR_L3_PFET_SEQ_ERR = 0;
static const uint32_t CPMS_CERR_CL2_PFET_SEQ_ERR = 1;
static const uint32_t CPMS_CERR_MMA_PFET_SEQ_ERR = 2;
static const uint32_t CPMS_CERR_DDS_ERR = 3;
static const uint32_t CPMS_CERR_L3_CLOCK_SYNC_ERR = 4;
static const uint32_t CPMS_CERR_CL2_CLOCK_SYNC_ERR = 5;
static const uint32_t CPMS_CERR_DPT_ACCESS_ERROR = 6;
static const uint32_t CPMS_CERR_PROXY_UNDERFLOW = 7;
static const uint32_t CPMS_CERR_PITCH_SATURATED = 8;
static const uint32_t CPMS_CERR_FTC_DEBUG_EVENT = 9;
static const uint32_t CPMS_CERR_FTC_DEBUG_EVENT_LEN = 3;
static const uint32_t P10_20_CPMS_CERR_CCFG_PARITY_ERR = 12; // p10:20,
static const uint32_t P10_20_CPMS_CERR_RVRM_DROPOUT = 13; // p10:20,
static const uint32_t P10_20_CPMS_CERR_PITCH_UPDATE_ERR = 14; // p10:20,
static const uint32_t P10_20_CPMS_CERR_SPARE_ERR = 15; // p10:20,
//<< [CPMS_CERR]
// ppe_c/reg00006.H

//>> [CPMS_CUCR]
static const uint32_t CPMS_CUCR = 0xc000e3c0ull;
static const uint32_t CPMS_CUCR_PPE2 = 0xc000e3e0ull;
static const uint32_t CPMS_CUCR_WO_CLEAR = 0xc000e3f0ull;

static const uint32_t CPMS_CUCR_ENABLE_CORE_SHADOW = 0;
static const uint32_t CPMS_CUCR_ENABLE_CORE_SAMPLE = 1;
static const uint32_t CPMS_CUCR_DPT_PER_THREAD_MODE = 2;
static const uint32_t CPMS_CUCR_SPARE = 3;
static const uint32_t CPMS_CUCR_SPARE_LEN = 2;
static const uint32_t CPMS_CUCR_CORE_MODE_SAMPLE_DISABLE = 5;
static const uint32_t CPMS_CUCR_STOP01_SAMPLE_ENABLE = 6;
static const uint32_t CPMS_CUCR_REFRESH_FTC_SHADOW = 9;
static const uint32_t CPMS_CUCR_REFRESH_PP_SHADOW = 10;
static const uint32_t CPMS_CUCR_REFRESH_DPT_SETTINGS = 11;
static const uint32_t CPMS_CUCR_INDUCED_DROOP_COUNT = 12;
static const uint32_t CPMS_CUCR_INDUCED_DROOP_COUNT_LEN = 3;
static const uint32_t CPMS_CUCR_PCB_SKEW_ADJ = 17;
static const uint32_t CPMS_CUCR_PCB_SKEW_ADJ_LEN = 3;
static const uint32_t CPMS_CUCR_FDCR_UPDATE_INPROGRESS = 31;
static const uint32_t CPMS_CUCR_CORE_SAMPLE_ENABLED_LT = 32;
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
static const uint32_t CPMS_CUCR_QME_DATA_TYPE1_PENDING = 53;
static const uint32_t CPMS_CUCR_QME_DATA_TYPE2_PENDING = 54;
static const uint32_t CPMS_CUCR_QME_DATA_TYPE3_PENDING = 55;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTB0_PENDING = 56;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTB1_PENDING = 57;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTB2_PENDING = 58;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTB3_PENDING = 59;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTC0_PENDING = 60;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTC1_PENDING = 61;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTC2_PENDING = 62;
static const uint32_t CPMS_CUCR_QME_TYPE0_EVENTC3_PENDING = 63;
//<< [CPMS_CUCR]
// ppe_c/reg00006.H

//>> [CPMS_DMSR]
static const uint32_t CPMS_DMSR = 0xc000e640ull;

static const uint32_t CPMS_DMSR_0_COUNT = 0;
static const uint32_t CPMS_DMSR_0_COUNT_LEN = 24;
static const uint32_t CPMS_DMSR_1_COUNT = 32;
static const uint32_t CPMS_DMSR_1_COUNT_LEN = 24;
//<< [CPMS_DMSR]
// ppe_c/reg00006.H

//>> [CPMS_DPPR]
static const uint32_t CPMS_DPPR = 0xc000ea80ull;
static const uint32_t CPMS_DPPR_WO_CLEAR = 0xc000eab0ull;
static const uint32_t CPMS_DPPR_WO_OR = 0xc000eaa0ull;

static const uint32_t CPMS_DPPR_PITCH_ABORT_INTERRUPT_ENABLE = 0;
static const uint32_t CPMS_DPPR_PTSR_EVENT_SELECT = 1;
static const uint32_t CPMS_DPPR_ENERGY_PROXY_RESET_MODE = 2;
static const uint32_t CPMS_DPPR_PIL_THROTTLE_ENABLE = 3;
static const uint32_t CPMS_DPPR_OCP_THROTTLE_ENABLE = 4;
static const uint32_t CPMS_DPPR_OCP_INTERRUPT_ENABLE = 5;
static const uint32_t CPMS_DPPR_OCP_ENABLE = 6;
static const uint32_t CPMS_DPPR_SPARE = 7;
static const uint32_t CPMS_DPPR_SPARE_LEN = 5;
static const uint32_t CPMS_DPPR_PP0_THRESH_SCALE = 16;
static const uint32_t CPMS_DPPR_PP0_THRESH_SCALE_LEN = 4;
static const uint32_t CPMS_DPPR_PP0_THRESH_VALUE = 20;
static const uint32_t CPMS_DPPR_PP0_THRESH_VALUE_LEN = 4;
static const uint32_t CPMS_DPPR_PP1_THRESH_SCALE = 24;
static const uint32_t CPMS_DPPR_PP1_THRESH_SCALE_LEN = 4;
static const uint32_t CPMS_DPPR_PP1_THRESH_VALUE = 28;
static const uint32_t CPMS_DPPR_PP1_THRESH_VALUE_LEN = 4;
static const uint32_t CPMS_DPPR_OCP_WAIT_FOR_GRANT = 32;
static const uint32_t CPMS_DPPR_OCP_LOCAL_MSB_MAX = 33;
static const uint32_t CPMS_DPPR_OCP_LOCAL_MSB_MAX_LEN = 2;
static const uint32_t CPMS_DPPR_OCP_LOCAL_OVERAGE = 35;
static const uint32_t CPMS_DPPR_OCP_LOCAL_OVERAGE_LEN = 5;
static const uint32_t CPMS_DPPR_OCP_LOCAL_TANK_VALUE = 40;
static const uint32_t CPMS_DPPR_OCP_LOCAL_TANK_VALUE_LEN = 10;
static const uint32_t CPMS_DPPR_OCP_PITCH_OVERRIDE = 50;
static const uint32_t CPMS_DPPR_OCP_OVERRIDE_HISTORY = 51;
//<< [CPMS_DPPR]
// ppe_c/reg00006.H

//>> [CPMS_MMA_PFETCNTL]
static const uint32_t CPMS_MMA_PFETCNTL = 0xc000e300ull;
static const uint32_t CPMS_MMA_PFETCNTL_WO_CLEAR = 0xc000e330ull;
static const uint32_t CPMS_MMA_PFETCNTL_WO_OR = 0xc000e320ull;

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
//<< [CPMS_MMA_PFETCNTL]
// ppe_c/reg00006.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00006.H"
#endif
#endif
