/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_e.H $           */
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

#ifndef __p10_ppe_c_e_H_
#define __p10_ppe_c_e_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


static const uint32_t CPMS_ASSR = 0xc000e180ull;

static const uint32_t CPMS_ASSR_CPMS_L3_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 0;
static const uint32_t CPMS_ASSR_CPMS_L3_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 1;
static const uint32_t CPMS_ASSR_CPMS_CL2_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 2;
static const uint32_t CPMS_ASSR_CPMS_CL2_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 3;
static const uint32_t CPMS_ASSR_CPMS_MMA_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 4;
static const uint32_t CPMS_ASSR_CPMS_MMA_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 5;
static const uint32_t CPMS_ASSR_CPMS_RVCSR_RVID_ACTIVE = 6;
static const uint32_t CPMS_ASSR_CPMS_RVCSR_BYPASS_ACTIVE = 7;
static const uint32_t CPMS_ASSR_CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET = 8;
static const uint32_t CPMS_ASSR_CPMS_L3_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 12;
static const uint32_t CPMS_ASSR_CPMS_L3_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 13;
static const uint32_t CPMS_ASSR_CPMS_CL2_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 14;
static const uint32_t CPMS_ASSR_CPMS_CL2_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 15;
static const uint32_t CPMS_ASSR_CPMS_CGCSR_L3_CLKGLM_SEL = 16;
static const uint32_t CPMS_ASSR_CPMS_CGCSR_CL2_CLKGLM_SEL = 17;
static const uint32_t CPMS_ASSR_CPMS_CGCSR_L3_CLK_SYNC_DONE = 18;
static const uint32_t CPMS_ASSR_CPMS_CGCSR_CL2_CLK_SYNC_DONE = 19;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_AVG = 27;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_AVG_LEN = 5;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE0 = 32;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE0_LEN = 5;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE1 = 37;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE1_LEN = 5;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE2 = 42;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE2_LEN = 5;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE3 = 47;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE3_LEN = 5;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE_MAX = 52;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE_MAX_LEN = 5;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE_MIN = 57;
static const uint32_t CPMS_ASSR_CL2_VDD_CURRENT_SENSE_MIN_LEN = 5;
// ppe_c/reg00014.H

static const uint32_t CPMS_DESR = 0xc000e5c0ull;

static const uint32_t CPMS_DESR_COUNTA_FTC_ACTIVITY_SEL = 0;
static const uint32_t CPMS_DESR_COUNTA_FTC_ACTIVITY_SEL_LEN = 3;
static const uint32_t CPMS_DESR_COUNTA_FTC_EVENT_SEL = 3;
static const uint32_t CPMS_DESR_COUNTA_FTC_EVENT_SEL_LEN = 3;
static const uint32_t CPMS_DESR_COUNTB_FTC_ACTIVITY_SEL = 6;
static const uint32_t CPMS_DESR_COUNTB_FTC_ACTIVITY_SEL_LEN = 3;
static const uint32_t CPMS_DESR_COUNTB_FTC_EVENT_SEL = 9;
static const uint32_t CPMS_DESR_COUNTB_FTC_EVENT_SEL_LEN = 3;
static const uint32_t CPMS_DESR_COUNTC_FTC_ACTIVITY_SEL = 12;
static const uint32_t CPMS_DESR_COUNTC_FTC_ACTIVITY_SEL_LEN = 3;
static const uint32_t CPMS_DESR_COUNTC_FTC_EVENT_SEL = 15;
static const uint32_t CPMS_DESR_COUNTC_FTC_EVENT_SEL_LEN = 3;
static const uint32_t CPMS_DESR_COUNTA_FTC_STATE_SELECT = 18;
static const uint32_t CPMS_DESR_COUNTA_FTC_STATE_SELECT_LEN = 3;
static const uint32_t CPMS_DESR_COUNTB_FTC_STATE_SELECT = 21;
static const uint32_t CPMS_DESR_COUNTB_FTC_STATE_SELECT_LEN = 3;
static const uint32_t CPMS_DESR_COUNTC_FTC_STATE_SELECT = 24;
static const uint32_t CPMS_DESR_COUNTC_FTC_STATE_SELECT_LEN = 3;
static const uint32_t CPMS_DESR_FTC_INVERT_SEL = 27;
static const uint32_t CPMS_DESR_FTC_INVERT_SEL_LEN = 3;
static const uint32_t CPMS_DESR_FTC_SPARE = 30;
static const uint32_t CPMS_DESR_FTC_SPARE_LEN = 2;
static const uint32_t CPMS_DESR_FTC_EVENT_INVERT = 32;
static const uint32_t CPMS_DESR_FTC_EVENT_INVERT_LEN = 3;
static const uint32_t CPMS_DESR_STATEW_ENABLE = 35;
static const uint32_t CPMS_DESR_STATEW_ENABLE_LEN = 3;
static const uint32_t CPMS_DESR_STATEZ_ENABLE = 38;
static const uint32_t CPMS_DESR_STATEZ_ENABLE_LEN = 3;
static const uint32_t CPMS_DESR_SPARE_COUNT_SEL = 41;
static const uint32_t CPMS_DESR_SPARE_COUNT_SEL_LEN = 3;
static const uint32_t CPMS_DESR_SECONDARY_EVENT_SEL = 44;
static const uint32_t CPMS_DESR_SECONDARY_EVENT_SEL_LEN = 2;
// ppe_c/reg00014.H

static const uint32_t CPMS_DTTR = 0xc000e880ull;

static const uint32_t CPMS_DTTR_CPMS_DTCR_CORE_DPT_RATE_LIMIT = 0;
static const uint32_t CPMS_DTTR_CPMS_DTCR_CORE_DPT_RATE_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_T1_DPT_RATE_LIMIT = 8;
static const uint32_t CPMS_DTTR_T1_DPT_RATE_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_T2_DPT_RATE_LIMIT = 16;
static const uint32_t CPMS_DTTR_T2_DPT_RATE_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_T3_DPT_RATE_LIMIT = 24;
static const uint32_t CPMS_DTTR_T3_DPT_RATE_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_CPMS_DTCR_CORE_DPT_INFLIGHT_LIMIT = 32;
static const uint32_t CPMS_DTTR_CPMS_DTCR_CORE_DPT_INFLIGHT_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_T1_DPT_INFLIGHT_LIMIT = 40;
static const uint32_t CPMS_DTTR_T1_DPT_INFLIGHT_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_T2_DPT_INFLIGHT_LIMIT = 48;
static const uint32_t CPMS_DTTR_T2_DPT_INFLIGHT_LIMIT_LEN = 8;
static const uint32_t CPMS_DTTR_T3_DPT_INFLIGHT_LIMIT = 56;
static const uint32_t CPMS_DTTR_T3_DPT_INFLIGHT_LIMIT_LEN = 8;
// ppe_c/reg00014.H

static const uint32_t QME_CISR = 0xc0008780ull;

static const uint32_t QME_CISR_HYP_INTR_PRESENT = 0;
static const uint32_t QME_CISR_HYP_INTR_PRESENT_LEN = 4;
static const uint32_t QME_CISR_OS_INTR_PRESENT = 4;
static const uint32_t QME_CISR_OS_INTR_PRESENT_LEN = 4;
static const uint32_t QME_CISR_MSGSND_INTR_PRESENT = 8;
static const uint32_t QME_CISR_MSGSND_INTR_PRESENT_LEN = 4;
static const uint32_t QME_CISR_EBB_INTR_PRESENT = 12;
static const uint32_t QME_CISR_EBB_INTR_PRESENT_LEN = 4;
static const uint32_t QME_CISR_HYP_INTR_REQUESTED = 16;
static const uint32_t QME_CISR_HYP_INTR_REQUESTED_LEN = 4;
static const uint32_t QME_CISR_OS_INTR_REQUESTED = 20;
static const uint32_t QME_CISR_OS_INTR_REQUESTED_LEN = 4;
static const uint32_t QME_CISR_MSGSND_INTR_REQUESTED = 24;
static const uint32_t QME_CISR_MSGSND_INTR_REQUESTED_LEN = 4;
static const uint32_t QME_CISR_MSGSND_INTR_SAMPLE = 28;
static const uint32_t QME_CISR_MSGSND_INTR_SAMPLE_LEN = 4;
static const uint32_t QME_CISR_MSGSND_ACK = 32;
static const uint32_t QME_CISR_MALF_ALERT_PRESENT = 33;
static const uint32_t QME_CISR_MALF_ALERT_REQUESTED = 34;
static const uint32_t QME_CISR_MSGSNDU_INTR_PRESENT = 36;
static const uint32_t QME_CISR_MSGSNDU_INTR_PRESENT_LEN = 4;
static const uint32_t QME_CISR_MSGSNDU_INTR_REQUESTED = 40;
static const uint32_t QME_CISR_MSGSNDU_INTR_REQUESTED_LEN = 4;
static const uint32_t QME_CISR_MSGSNDU_INTR_SAMPLE = 44;
static const uint32_t QME_CISR_MSGSNDU_INTR_SAMPLE_LEN = 4;
static const uint32_t QME_CISR_TFCS_DEC_PRESENT = 48;
static const uint32_t QME_CISR_TFCS_DEC_PRESENT_LEN = 4;
static const uint32_t QME_CISR_TFCS_DEC_REQUESTED = 52;
static const uint32_t QME_CISR_TFCS_DEC_REQUESTED_LEN = 4;
static const uint32_t QME_CISR_TFCS_HDEC_PRESENT = 56;
static const uint32_t QME_CISR_TFCS_HDEC_REQUESTED = 57;
static const uint32_t QME_CISR_DPDES_INTR_PRESENT = 58;
static const uint32_t QME_CISR_DPDES_INTR_REQUESTED = 59;
static const uint32_t QME_CISR_REG_WKUP_PRESENT = 62;
static const uint32_t QME_CISR_REG_WKUP_REQUESTED = 63;
// ppe_c/reg00014.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00014.H"
#endif
#endif
