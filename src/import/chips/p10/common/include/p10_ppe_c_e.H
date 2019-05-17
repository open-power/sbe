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

#include "proc_scomt.H"

#ifndef __p10_ppe_c_e_H_
#define __p10_ppe_c_e_H_


namespace scomt
{
namespace ppe_c
{


static const uint64_t CPMS_ASSR = 0x00000006c001e180ull;

static const uint32_t CPMS_ASSR_L3_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 0;
static const uint32_t CPMS_ASSR_L3_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 1;
static const uint32_t CPMS_ASSR_CL2_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 2;
static const uint32_t CPMS_ASSR_CL2_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 3;
static const uint32_t CPMS_ASSR_MMA_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 4;
static const uint32_t CPMS_ASSR_MMA_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 5;
static const uint32_t CPMS_ASSR_RVCSR_RVID_ACTIVE = 6;
static const uint32_t CPMS_ASSR_RVCSR_BYPASS_ACTIVE = 7;
static const uint32_t CPMS_ASSR_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET = 8;
static const uint32_t CPMS_ASSR_L3_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 12;
static const uint32_t CPMS_ASSR_L3_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 13;
static const uint32_t CPMS_ASSR_CL2_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 14;
static const uint32_t CPMS_ASSR_CL2_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 15;
static const uint32_t CPMS_ASSR_CGCSR_L3_CLKGLM_SEL = 16;
static const uint32_t CPMS_ASSR_CGCSR_CL2_CLKGLM_SEL = 17;
static const uint32_t CPMS_ASSR_CGCSR_L3_CLK_SYNC_DONE = 18;
static const uint32_t CPMS_ASSR_CGCSR_CL2_CLK_SYNC_DONE = 19;
// ppe_c/reg00014.H

static const uint64_t QME_CISR = 0x00000006c0018780ull;

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

}
}
#include "ppe_c/reg00014.H"
#endif
