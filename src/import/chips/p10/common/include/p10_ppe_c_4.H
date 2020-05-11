/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_4.H $           */
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

#ifndef __p10_ppe_c_4_H_
#define __p10_ppe_c_4_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_RVCSR]
static const uint32_t CPMS_RVCSR = 0xc000e040ull;
static const uint32_t CPMS_RVCSR_WO_CLEAR = 0xc000e070ull;
static const uint32_t CPMS_RVCSR_WO_OR = 0xc000e060ull;

static const uint32_t CPMS_RVCSR_RVID_ENABLE = 0;
static const uint32_t CPMS_RVCSR_RVID_OVERRIDE = 1;
static const uint32_t CPMS_RVCSR_ACS_CALIBRATE = 2;
static const uint32_t CPMS_RVCSR_RVRM_TUNE = 6;
static const uint32_t CPMS_RVCSR_RVRM_TUNE_LEN = 6;
static const uint32_t CPMS_RVCSR_RVID_ACTIVE = 32;
static const uint32_t CPMS_RVCSR_BYPASS_ACTIVE = 33;
static const uint32_t CPMS_RVCSR_RVID_ENABLED = 34;
//<< [CPMS_RVCSR]
// ppe_c/reg00004.H

//>> [QME_SPWU_FSP]
static const uint32_t QME_SPWU_FSP = 0xc0008340ull;

static const uint32_t QME_SPWU_FSP_REQ = 0;
static const uint32_t QME_SPWU_FSP_DONE = 4;
//<< [QME_SPWU_FSP]
// ppe_c/reg00004.H

//>> [QME_SPWU_HYP]
static const uint32_t QME_SPWU_HYP = 0xc00083c0ull;

static const uint32_t QME_SPWU_HYP_REQ = 0;
static const uint32_t QME_SPWU_HYP_DONE = 4;
//<< [QME_SPWU_HYP]
// ppe_c/reg00004.H

//>> [QME_SPWU_OCC]
static const uint32_t QME_SPWU_OCC = 0xc0008380ull;

static const uint32_t QME_SPWU_OCC_REQ = 0;
static const uint32_t QME_SPWU_OCC_DONE = 4;
//<< [QME_SPWU_OCC]
// ppe_c/reg00004.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00004.H"
#endif
#endif
