/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_8.H $           */
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

#ifndef __p10_ppe_c_8_H_
#define __p10_ppe_c_8_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


static const uint32_t CPMS_ACDR = 0xc000e7c0ull;

static const uint32_t CPMS_ACDR_A_SCALE = 0;
static const uint32_t CPMS_ACDR_A_SCALE_LEN = 4;
static const uint32_t CPMS_ACDR_A_VALUE = 4;
static const uint32_t CPMS_ACDR_A_VALUE_LEN = 16;
static const uint32_t CPMS_ACDR_B_SCALE = 20;
static const uint32_t CPMS_ACDR_B_SCALE_LEN = 4;
static const uint32_t CPMS_ACDR_B_VALUE = 24;
static const uint32_t CPMS_ACDR_B_VALUE_LEN = 16;
static const uint32_t CPMS_ACDR_C_SCALE = 40;
static const uint32_t CPMS_ACDR_C_SCALE_LEN = 4;
static const uint32_t CPMS_ACDR_C_VALUE = 44;
static const uint32_t CPMS_ACDR_C_VALUE_LEN = 16;
static const uint32_t CPMS_ACDR_A_MOD = 60;
static const uint32_t CPMS_ACDR_B_MOD = 61;
static const uint32_t CPMS_ACDR_C_MOD = 62;
// ppe_c/reg00008.H

static const uint32_t CPMS_CL2_PFETSTAT = 0xc000e2c0ull;

static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 0;
static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 1;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 2;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 3;
static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFET_ENABLE_ACTUAL = 16;
static const uint32_t CPMS_CL2_PFETSTAT_VDD_PFET_ENABLE_ACTUAL_LEN = 8;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFET_ENABLE_ACTUAL = 24;
static const uint32_t CPMS_CL2_PFETSTAT_VCS_PFET_ENABLE_ACTUAL_LEN = 8;
static const uint32_t CPMS_CL2_PFETSTAT_SRAM_ENABLE_ACTUAL = 63;
// ppe_c/reg00008.H

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
// ppe_c/reg00008.H

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
// ppe_c/reg00008.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00008.H"
#endif
#endif
