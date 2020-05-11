/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_b.H $           */
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

#ifndef __p10_ppe_c_b_H_
#define __p10_ppe_c_b_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_L3_PFETSTAT]
static const uint32_t CPMS_L3_PFETSTAT = 0xc000e240ull;

static const uint32_t CPMS_L3_PFETSTAT_VDD_PFETS_ENABLED_SENSE = 0;
static const uint32_t CPMS_L3_PFETSTAT_VDD_PFETS_DISABLED_SENSE = 1;
static const uint32_t CPMS_L3_PFETSTAT_VCS_PFETS_ENABLED_SENSE = 2;
static const uint32_t CPMS_L3_PFETSTAT_VCS_PFETS_DISABLED_SENSE = 3;
static const uint32_t CPMS_L3_PFETSTAT_VDD_PFET_ENABLE_ACTUAL = 16;
static const uint32_t CPMS_L3_PFETSTAT_VDD_PFET_ENABLE_ACTUAL_LEN = 8;
static const uint32_t CPMS_L3_PFETSTAT_VCS_PFET_ENABLE_ACTUAL = 24;
static const uint32_t CPMS_L3_PFETSTAT_VCS_PFET_ENABLE_ACTUAL_LEN = 8;
static const uint32_t CPMS_L3_PFETSTAT_SRAM_ENABLE_ACTUAL = 63;
//<< [CPMS_L3_PFETSTAT]
// ppe_c/reg00011.H

//>> [QME_PSCRS1]
static const uint32_t QME_PSCRS1 = 0xc0008540ull;

static const uint32_t QME_PSCRS1_DPDES_EXIT_ENABLE = 0;
static const uint32_t QME_PSCRS1_SD = 1;
static const uint32_t QME_PSCRS1_ESL = 2;
static const uint32_t QME_PSCRS1_EC = 3;
static const uint32_t QME_PSCRS1_PSLL = 4;
static const uint32_t QME_PSCRS1_PSLL_LEN = 4;
static const uint32_t QME_PSCRS1_HYP_VIRT_EXIT_ENABLE = 8;
static const uint32_t QME_PSCRS1_UV_DB_EXIT_ENABLE = 9;
static const uint32_t QME_PSCRS1_HYP_DB_EXIT_ENABLE = 10;
static const uint32_t QME_PSCRS1_EXT_EXIT_ENABLE = 11;
static const uint32_t QME_PSCRS1_DEC_EXIT_ENABLE = 12;
static const uint32_t QME_PSCRS1_HMI_EXIT_ENABLE = 13;
static const uint32_t QME_PSCRS1_TR = 14;
static const uint32_t QME_PSCRS1_TR_LEN = 2;
static const uint32_t QME_PSCRS1_MTL = 16;
static const uint32_t QME_PSCRS1_MTL_LEN = 4;
static const uint32_t QME_PSCRS1_RL = 20;
static const uint32_t QME_PSCRS1_RL_LEN = 4;
static const uint32_t QME_PSCRS1_PLS = 24;
static const uint32_t QME_PSCRS1_PLS_LEN = 4;
//<< [QME_PSCRS1]
// ppe_c/reg00011.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00011.H"
#endif
#endif
