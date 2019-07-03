/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_d.H $           */
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

#ifndef __p10_ppe_c_d_H_
#define __p10_ppe_c_d_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


static const uint32_t CPMS_DCCR = 0xc000e600ull;

static const uint32_t CPMS_DCCR_0_SELECT = 0;
static const uint32_t CPMS_DCCR_1_SELECT = 1;
static const uint32_t CPMS_DCCR__SMALL_OFFSET = 2;
static const uint32_t CPMS_DCCR__SMALL_OFFSET_LEN = 2;
static const uint32_t CPMS_DCCR_0_THRESH_SCALE = 4;
static const uint32_t CPMS_DCCR_0_THRESH_SCALE_LEN = 4;
static const uint32_t CPMS_DCCR_0_THRESH_VALUE = 8;
static const uint32_t CPMS_DCCR_0_THRESH_VALUE_LEN = 8;
static const uint32_t CPMS_DCCR_1_THRESH_SCALE = 16;
static const uint32_t CPMS_DCCR_1_THRESH_SCALE_LEN = 4;
static const uint32_t CPMS_DCCR_1_THRESH_VALUE = 20;
static const uint32_t CPMS_DCCR_1_THRESH_VALUE_LEN = 8;
static const uint32_t CPMS_DCCR_0_EVENT_SEL = 28;
static const uint32_t CPMS_DCCR_0_EVENT_SEL_LEN = 3;
static const uint32_t CPMS_DCCR_1_EVENT_SEL = 31;
static const uint32_t CPMS_DCCR_1_EVENT_SEL_LEN = 3;
// ppe_c/reg00013.H

static const uint32_t CPMS_MMA_PFETSTAT = 0xc000e340ull;

static const uint32_t CPMS_MMA_PFETSTAT_S_ENABLED_SENSE = 0;
static const uint32_t CPMS_MMA_PFETSTAT_S_DISABLED_SENSE = 1;
static const uint32_t CPMS_MMA_PFETSTAT__ENABLE_ACTUAL = 16;
static const uint32_t CPMS_MMA_PFETSTAT__ENABLE_ACTUAL_LEN = 8;
// ppe_c/reg00013.H

static const uint32_t CPMS_PFETDLY = 0xc000e1c0ull;

static const uint32_t CPMS_PFETDLY_POWDN_DLY = 0;
static const uint32_t CPMS_PFETDLY_POWDN_DLY_LEN = 4;
static const uint32_t CPMS_PFETDLY_L3_POWUP_DLY = 4;
static const uint32_t CPMS_PFETDLY_L3_POWUP_DLY_LEN = 4;
static const uint32_t CPMS_PFETDLY_CL2_POWUP_DLY = 8;
static const uint32_t CPMS_PFETDLY_CL2_POWUP_DLY_LEN = 4;
static const uint32_t CPMS_PFETDLY_MMA_POWUP_DLY = 12;
static const uint32_t CPMS_PFETDLY_MMA_POWUP_DLY_LEN = 4;
// ppe_c/reg00013.H

static const uint32_t QME_PSCRS0 = 0xc0008500ull;

static const uint32_t QME_PSCRS0_DPDES_EXIT_ENABLE = 0;
static const uint32_t QME_PSCRS0_SD = 1;
static const uint32_t QME_PSCRS0_ESL = 2;
static const uint32_t QME_PSCRS0_EC = 3;
static const uint32_t QME_PSCRS0_PSLL = 4;
static const uint32_t QME_PSCRS0_PSLL_LEN = 4;
static const uint32_t QME_PSCRS0_HYP_VIRT_EXIT_ENABLE = 8;
static const uint32_t QME_PSCRS0_UV_DB_EXIT_ENABLE = 9;
static const uint32_t QME_PSCRS0_HYP_DB_EXIT_ENABLE = 10;
static const uint32_t QME_PSCRS0_EXT_EXIT_ENABLE = 11;
static const uint32_t QME_PSCRS0_DEC_EXIT_ENABLE = 12;
static const uint32_t QME_PSCRS0_HMI_EXIT_ENABLE = 13;
static const uint32_t QME_PSCRS0_ = 14;
static const uint32_t QME_PSCRS0__LEN = 2;
static const uint32_t QME_PSCRS0_MTL = 16;
static const uint32_t QME_PSCRS0_MTL_LEN = 4;
static const uint32_t QME_PSCRS0_RL = 20;
static const uint32_t QME_PSCRS0_RL_LEN = 4;
static const uint32_t QME_PSCRS0_PLS = 24;
static const uint32_t QME_PSCRS0_PLS_LEN = 4;
// ppe_c/reg00013.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00013.H"
#endif
#endif
