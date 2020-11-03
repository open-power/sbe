/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_8.H $           */
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

#ifndef __p10_ppe_c_8_H_
#define __p10_ppe_c_8_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_ACDR]
static const uint32_t CPMS_ACDR = 0xc000e7c0ull;

static const uint32_t CPMS_ACDR_CHAR_COUNTA_SCALE = 0;
static const uint32_t CPMS_ACDR_CHAR_COUNTA_SCALE_LEN = 4;
static const uint32_t CPMS_ACDR_CHAR_COUNTA_VALUE = 4;
static const uint32_t CPMS_ACDR_CHAR_COUNTA_VALUE_LEN = 16;
static const uint32_t CPMS_ACDR_CHAR_COUNTB_SCALE = 20;
static const uint32_t CPMS_ACDR_CHAR_COUNTB_SCALE_LEN = 4;
static const uint32_t CPMS_ACDR_CHAR_COUNTB_VALUE = 24;
static const uint32_t CPMS_ACDR_CHAR_COUNTB_VALUE_LEN = 16;
static const uint32_t CPMS_ACDR_CHAR_COUNTC_SCALE = 40;
static const uint32_t CPMS_ACDR_CHAR_COUNTC_SCALE_LEN = 4;
static const uint32_t CPMS_ACDR_CHAR_COUNTC_VALUE = 44;
static const uint32_t CPMS_ACDR_CHAR_COUNTC_VALUE_LEN = 16;
static const uint32_t CPMS_ACDR_CHAR_COUNTA_MOD = 60;
static const uint32_t CPMS_ACDR_CHAR_COUNTB_MOD = 61;
static const uint32_t CPMS_ACDR_CHAR_COUNTC_MOD = 62;
static const uint32_t CPMS_ACDR_SAMPLE_DISABLE_INDICATOR = 63;
//<< [CPMS_ACDR]
// ppe_c/reg00008.H

//>> [CPMS_CL2_PFETSTAT]
static const uint32_t CPMS_CL2_PFETSTAT = 0xc000e2c0ull;

static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 0;
static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 1;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 2;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 3;
static const uint32_t CPMS_CL2_PFETSTAT_L2_VDD_PFETS_FINGER0_SENSE = 4; // p10:20,
static const uint32_t CPMS_CL2_PFETSTAT_L2_VCS_PFETS_FINGER0_SENSE = 5; // p10:20,
static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFET_ENABLE_ACTUAL = 16;
static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFET_ENABLE_ACTUAL_LEN = 8;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFET_ENABLE_ACTUAL = 24;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFET_ENABLE_ACTUAL_LEN = 8;
static const uint32_t CPMS_CL2_PFETSTAT_SRAM_ENABLE_ACTUAL = 63;
//<< [CPMS_CL2_PFETSTAT]
// ppe_c/reg00008.H

//>> [CPMS_DTCR]
static const uint32_t CPMS_DTCR = 0xc000e840ull;

static const uint32_t CPMS_DTCR_DPT_MODE = 0;
static const uint32_t CPMS_DTCR_DPT_MODE_LEN = 2;
static const uint32_t CPMS_DTCR_DPT_MODE_UPDATE = 2;
static const uint32_t CPMS_DTCR_DPT_RATE_UPDATE = 3;
static const uint32_t CPMS_DTCR_DPT_INFLIGHT_UPDATE = 4;
static const uint32_t CPMS_DTCR_DPT_WINDOW_SEL = 5;
static const uint32_t CPMS_DTCR_DPT_WINDOW_SEL_LEN = 3;
static const uint32_t CPMS_DTCR_CORE_DPT_RATE_LIMIT = 8;
static const uint32_t CPMS_DTCR_CORE_DPT_RATE_LIMIT_LEN = 8;
static const uint32_t CPMS_DTCR_CORE_DPT_INFLIGHT_LIMIT = 16;
static const uint32_t CPMS_DTCR_CORE_DPT_INFLIGHT_LIMIT_LEN = 8;
static const uint32_t CPMS_DTCR_DISABLE_DPT_ON_UPDATE = 24;
static const uint32_t CPMS_DTCR_CORE_SMT_MODE = 34;
static const uint32_t CPMS_DTCR_CORE_SMT_MODE_LEN = 2;
//<< [CPMS_DTCR]
// ppe_c/reg00008.H

//>> [CPMS_ESCR]
static const uint32_t CPMS_ESCR = 0xc000e6c0ull;

static const uint32_t CPMS_ESCR_DURATION_MODE = 0;
static const uint32_t CPMS_ESCR_DURATION_MODE_LEN = 2;
static const uint32_t CPMS_ESCR_DURATION_EVENT_INVERT = 2;
static const uint32_t CPMS_ESCR_DURATION_EVENT_INVERT_LEN = 2;
static const uint32_t CPMS_ESCR_DURATION_RANGE = 4;
static const uint32_t CPMS_ESCR_DURATION_RANGE_LEN = 3;
static const uint32_t CPMS_ESCR_DURATION_START_SEL = 7;
static const uint32_t CPMS_ESCR_DURATION_START_SEL_LEN = 5;
static const uint32_t CPMS_ESCR_DURATION_TIMEOUT_SEL = 12;
static const uint32_t CPMS_ESCR_DURATION_TIMEOUT_SEL_LEN = 3;
static const uint32_t CPMS_ESCR_DURATION_STOP_SEL = 15;
static const uint32_t CPMS_ESCR_DURATION_STOP_SEL_LEN = 5;
static const uint32_t CPMS_ESCR_STATEW_SELECT = 20;
static const uint32_t CPMS_ESCR_STATEW_SELECT_LEN = 3;
static const uint32_t CPMS_ESCR_STATEW_INVERT = 23;
static const uint32_t CPMS_ESCR_STATEZ_SELECT = 24;
static const uint32_t CPMS_ESCR_STATEZ_SELECT_LEN = 3;
static const uint32_t CPMS_ESCR_STATEZ_PULSE = 27;
static const uint32_t CPMS_ESCR_EVENTX_PULSE = 28;
static const uint32_t CPMS_ESCR_EVENTY_PULSE = 29;
static const uint32_t CPMS_ESCR_STATEX_PULSE = 30;
static const uint32_t CPMS_ESCR_STATEY_PULSE = 31;
static const uint32_t CPMS_ESCR_STATEX_COMPARE = 32;
static const uint32_t CPMS_ESCR_STATEX_COMPARE_LEN = 4;
static const uint32_t CPMS_ESCR_STATEX_DONTCARE = 36;
static const uint32_t CPMS_ESCR_STATEX_DONTCARE_LEN = 4;
static const uint32_t CPMS_ESCR_STATEY_COMPARE = 40;
static const uint32_t CPMS_ESCR_STATEY_COMPARE_LEN = 4;
static const uint32_t CPMS_ESCR_STATEY_DONTCARE = 44;
static const uint32_t CPMS_ESCR_STATEY_DONTCARE_LEN = 4;
static const uint32_t CPMS_ESCR_EVENTX_COMPARE = 48;
static const uint32_t CPMS_ESCR_EVENTX_COMPARE_LEN = 4;
static const uint32_t CPMS_ESCR_EVENTX_DONTCARE = 52;
static const uint32_t CPMS_ESCR_EVENTX_DONTCARE_LEN = 4;
static const uint32_t CPMS_ESCR_EVENTY_COMPARE = 56;
static const uint32_t CPMS_ESCR_EVENTY_COMPARE_LEN = 4;
static const uint32_t CPMS_ESCR_EVENTY_DONTCARE = 60;
static const uint32_t CPMS_ESCR_EVENTY_DONTCARE_LEN = 4;
//<< [CPMS_ESCR]
// ppe_c/reg00008.H

//>> [QME_QOSR]
static const uint32_t QME_QOSR = 0xc0008a00ull;

static const uint32_t QME_QOSR_0_QOS_INDEX = 2;
static const uint32_t QME_QOSR_0_QOS_INDEX_LEN = 6;
static const uint32_t QME_QOSR_1_QOS_INDEX = 10;
static const uint32_t QME_QOSR_1_QOS_INDEX_LEN = 6;
static const uint32_t QME_QOSR_2_QOS_INDEX = 18;
static const uint32_t QME_QOSR_2_QOS_INDEX_LEN = 6;
static const uint32_t QME_QOSR_3_QOS_INDEX = 26;
static const uint32_t QME_QOSR_3_QOS_INDEX_LEN = 6;
static const uint32_t QME_QOSR_0_DTC_INDEX = 34;
static const uint32_t QME_QOSR_0_DTC_INDEX_LEN = 6;
static const uint32_t QME_QOSR_1_DTC_INDEX = 42;
static const uint32_t QME_QOSR_1_DTC_INDEX_LEN = 6;
static const uint32_t QME_QOSR_2_DTC_INDEX = 50;
static const uint32_t QME_QOSR_2_DTC_INDEX_LEN = 6;
static const uint32_t QME_QOSR_3_DTC_INDEX = 58;
static const uint32_t QME_QOSR_3_DTC_INDEX_LEN = 6;
//<< [QME_QOSR]
// ppe_c/reg00008.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00008.H"
#endif
#endif
