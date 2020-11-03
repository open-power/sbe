/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_5.H $           */
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

#ifndef __p10_ppe_c_5_H_
#define __p10_ppe_c_5_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_FLMR]
static const uint32_t CPMS_FLMR_RW = 0xc000e480ull;
static const uint32_t CPMS_FLMR_WO_CLEAR = 0xc000e4b0ull;
static const uint32_t CPMS_FLMR_WO_OR = 0xc000e4a0ull;

static const uint32_t CPMS_FLMR_LARGE_INITIAL_CYCLES = 0;
static const uint32_t CPMS_FLMR_LARGE_INITIAL_CYCLES_LEN = 6;
static const uint32_t CPMS_FLMR_LARGE_INITIAL_ACTION = 6;
static const uint32_t CPMS_FLMR_LARGE_INITIAL_ACTION_LEN = 5;
static const uint32_t CPMS_FLMR_LARGE_PREFETCH_ACTION = 11;
static const uint32_t CPMS_FLMR_LARGE_PREFETCH_ACTION_LEN = 2;
static const uint32_t CPMS_FLMR_LARGE_RESPONSE_DISABLE = 13;
static const uint32_t CPMS_FLMR_LARGE_FLUSH_TYPE = 14;
static const uint32_t CPMS_FLMR_LARGE_DITHER_WINDOW_SEL = 15;
static const uint32_t CPMS_FLMR_LARGE_DITHER_INITIAL = 16;
static const uint32_t CPMS_FLMR_LARGE_DITHER_INITIAL_LEN = 4;
static const uint32_t CPMS_FLMR_LARGE_DITHER_REPEAT = 20;
static const uint32_t CPMS_FLMR_LARGE_DITHER_REPEAT_LEN = 4;
static const uint32_t CPMS_FLMR_LARGE_DITHER_INCREASE = 24;
static const uint32_t CPMS_FLMR_LARGE_DITHER_INCREASE_LEN = 4;
static const uint32_t CPMS_FLMR_PHASE1_RECOVERY_ACTION = 28;
static const uint32_t CPMS_FLMR_PHASE1_RECOVERY_ACTION_LEN = 3;
static const uint32_t CPMS_FLMR_PHASE1_RECOVERY_PREFETCH = 31;
static const uint32_t CPMS_FLMR_PHASE1_RECOVERY_PREFETCH_LEN = 2;
static const uint32_t CPMS_FLMR_PHASE1_RECOVERY_VECTOR = 33;
static const uint32_t CPMS_FLMR_PHASE1_RECOVERY_VECTOR_LEN = 3;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_ACTION = 36;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_ACTION_LEN = 3;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_PREFETCH = 39;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_PREFETCH_LEN = 2;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_VECTOR = 41;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_VECTOR_LEN = 3;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_CYCLES = 44;
static const uint32_t CPMS_FLMR_PHASE2_RECOVERY_CYCLES_LEN = 8;
static const uint32_t CPMS_FLMR_MMA_DROOP_PROTECT_ENABLE = 52;
static const uint32_t CPMS_FLMR_MMA_DROOP_PREDICT_SELECT = 53;
static const uint32_t CPMS_FLMR_MMA_DROOP_PREDICT_SELECT_LEN = 3;
static const uint32_t CPMS_FLMR_CORE_UCODE_IDLE_DISABLE = 56;
static const uint32_t CPMS_FLMR_CORE_UCODE_GRANT_SUPPRESS = 57;
static const uint32_t CPMS_FLMR_CORE_UCODE_GRANT_SUPPRESS_LEN = 2;
static const uint32_t CPMS_FLMR_CORE_UCODE_GRANT_INJECT = 59;
static const uint32_t CPMS_FLMR_FTC_RESET = 60;
static const uint32_t CPMS_FLMR_IGNORE_IFU_THROTTLE_ACTIVE = 61;
static const uint32_t CPMS_FLMR_PHASE1_DITHER_NTC = 62;
static const uint32_t CPMS_FLMR_CORE_UCODE_EXIT_DISABLE = 63;
//<< [CPMS_FLMR]
// ppe_c/reg00005.H

//>> [CPMS_WPCR]
static const uint32_t CPMS_WPCR = 0xc000e940ull;

static const uint32_t CPMS_WPCR_SPARE = 0;
static const uint32_t CPMS_WPCR_SPARE_LEN = 2;
static const uint32_t CPMS_WPCR_BASE_THROTTLE_INDEX = 2;
static const uint32_t CPMS_WPCR_BASE_THROTTLE_INDEX_LEN = 6;
static const uint32_t CPMS_WPCR_SPARE2 = 8; // p10:20,
static const uint32_t CPMS_WPCR_SPARE2_LEN = 4;
static const uint32_t CPMS_WPCR_INTERVAL_PROXY_TARGET = 12;
static const uint32_t CPMS_WPCR_INTERVAL_PROXY_TARGET_LEN = 20;
static const uint32_t CPMS_WPCR_WOF_INTERVAL_TARGET = 12; // p10:20,
static const uint32_t CPMS_WPCR_WOF_INTERVAL_TARGET_LEN = 20;
// ERROR Duplicate Dial         static const uint32_t CPMS_WPCR_SPARE2 = 32;
static const uint32_t CPMS_WPCR_SPARE3 = 32; // p10:20,
static const uint32_t CPMS_WPCR_PROXY_THRESH_SELECT = 33;
static const uint32_t CPMS_WPCR_PROXY_THRESH_SELECT_LEN = 3;
static const uint32_t CPMS_WPCR_PROXY_CARRYOUT_SEL = 36;
static const uint32_t CPMS_WPCR_PROXY_CARRYOUT_SEL_LEN = 4;
static const uint32_t CPMS_WPCR_PITCH_FTX_SAT_MAX = 58;
static const uint32_t CPMS_WPCR_PITCH_FTX_SAT_MIN = 59;
static const uint32_t CPMS_WPCR_PITCH_ACCERR_SAT_MAX = 60;
static const uint32_t CPMS_WPCR_PITCH_ACCERR_SAT_MIN = 61;
static const uint32_t CPMS_WPCR_PITCH_APP_SATURATED = 62;
static const uint32_t CPMS_WPCR_STOP_STATE_OCCURRED = 63;
//<< [CPMS_WPCR]
// ppe_c/reg00005.H

//>> [QME_CIIR]
static const uint32_t QME_CIIR = 0xc0008740ull;

static const uint32_t QME_CIIR_MSGSND_INTR_INJECT = 28;
static const uint32_t QME_CIIR_MSGSND_INTR_INJECT_LEN = 4;
static const uint32_t QME_CIIR_MSGSNDU_INTR_INJECT = 60;
static const uint32_t QME_CIIR_MSGSNDU_INTR_INJECT_LEN = 4;
//<< [QME_CIIR]
// ppe_c/reg00005.H

//>> [QME_RMOR]
static const uint32_t QME_RMOR = 0xc0008680ull;

static const uint32_t QME_RMOR_DATA = 0;
static const uint32_t QME_RMOR_DATA_LEN = 63;
static const uint32_t QME_RMOR_URMOR_SELECT = 63;
//<< [QME_RMOR]
// ppe_c/reg00005.H

//>> [QME_SCR]
static const uint32_t QME_SCR_RW = 0xc00080c0ull;
static const uint32_t QME_SCR_WO_CLEAR = 0xc00080f0ull;
static const uint32_t QME_SCR_WO_OR = 0xc00080e0ull;

static const uint32_t QME_SCR_DATA = 0;
static const uint32_t QME_SCR_DATA_LEN = 64;
// ERROR Duplicate Dial         static const uint32_t QME_SCR_DATA = 0; // p10:20,
// ERROR Duplicate Dial         static const uint32_t QME_SCR_DATA_LEN = 16;
//<< [QME_SCR]
// ppe_c/reg00005.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00005.H"
#endif
#endif
