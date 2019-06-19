/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_1.H $           */
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

#ifndef __p10_ppe_c_1_H_
#define __p10_ppe_c_1_H_


namespace scomt
{
namespace ppe_c
{


static const uint32_t QME_DCSR = 0xc0008700ull;

static const uint32_t QME_DCSR_PLS_T0 = 0;
static const uint32_t QME_DCSR_PLS_T0_LEN = 4;
static const uint32_t QME_DCSR_PLS_T1 = 4;
static const uint32_t QME_DCSR_PLS_T1_LEN = 4;
static const uint32_t QME_DCSR_PLS_T2 = 8;
static const uint32_t QME_DCSR_PLS_T2_LEN = 4;
static const uint32_t QME_DCSR_PLS_T3 = 12;
static const uint32_t QME_DCSR_PLS_T3_LEN = 4;
static const uint32_t QME_DCSR_SL_T0 = 16;
static const uint32_t QME_DCSR_SL_T0_LEN = 2;
static const uint32_t QME_DCSR_SL_T1 = 18;
static const uint32_t QME_DCSR_SL_T1_LEN = 2;
static const uint32_t QME_DCSR_SL_T2 = 20;
static const uint32_t QME_DCSR_SL_T2_LEN = 2;
static const uint32_t QME_DCSR_SL_T3 = 22;
static const uint32_t QME_DCSR_SL_T3_LEN = 2;
static const uint32_t QME_DCSR_SRESET_MSR_S = 24;
// ppe_c/reg00001.H

static const uint32_t QME_PMCRS = 0xc0008600ull;

static const uint32_t QME_PMCRS_QME_PMCRS_DATA = 0;
static const uint32_t QME_PMCRS_QME_PMCRS_DATA_LEN = 64;
// ppe_c/reg00001.H

static const uint32_t QME_PMSRS = 0xc0008640ull;

static const uint32_t QME_PMSRS_PSTATE_DATA = 0;
static const uint32_t QME_PMSRS_PSTATE_DATA_LEN = 36;
static const uint32_t QME_PMSRS_VERSION = 59;
static const uint32_t QME_PMSRS_VERSION_LEN = 5;
// ppe_c/reg00001.H

static const uint32_t QME_PSCRS2 = 0xc0008580ull;

static const uint32_t QME_PSCRS2_DPDES_EXIT_ENABLE = 0;
static const uint32_t QME_PSCRS2_SD = 1;
static const uint32_t QME_PSCRS2_ESL = 2;
static const uint32_t QME_PSCRS2_EC = 3;
static const uint32_t QME_PSCRS2_PSLL = 4;
static const uint32_t QME_PSCRS2_PSLL_LEN = 4;
static const uint32_t QME_PSCRS2_HYP_VIRT_EXIT_ENABLE = 8;
static const uint32_t QME_PSCRS2_UV_DB_EXIT_ENABLE = 9;
static const uint32_t QME_PSCRS2_HYP_DB_EXIT_ENABLE = 10;
static const uint32_t QME_PSCRS2_EXT_EXIT_ENABLE = 11;
static const uint32_t QME_PSCRS2_DEC_EXIT_ENABLE = 12;
static const uint32_t QME_PSCRS2_HMI_EXIT_ENABLE = 13;
static const uint32_t QME_PSCRS2_ = 14;
static const uint32_t QME_PSCRS2__LEN = 2;
static const uint32_t QME_PSCRS2_MTL = 16;
static const uint32_t QME_PSCRS2_MTL_LEN = 4;
static const uint32_t QME_PSCRS2_RL = 20;
static const uint32_t QME_PSCRS2_RL_LEN = 4;
static const uint32_t QME_PSCRS2_PLS = 24;
static const uint32_t QME_PSCRS2_PLS_LEN = 4;
// ppe_c/reg00001.H

}
}
#ifndef __PPE_HCODE__
    #include "ppe_c/reg00001.H"
#endif
#endif
