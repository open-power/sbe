/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_4.H $           */
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

#ifndef __p10_ppe_c_4_H_
#define __p10_ppe_c_4_H_


namespace scomt
{
namespace ppe_c
{


static const uint64_t CPMS_RVCSR = 0x00000006c001e040ull;
static const uint64_t CPMS_RVCSR_WO_CLEAR = 0x00000006c001e060ull;
static const uint64_t CPMS_RVCSR_WO_OR = 0x00000006c001e070ull;

static const uint32_t CPMS_RVCSR_RVID_ENABLE = 0;
static const uint32_t CPMS_RVCSR_RVID_OVERRIDE = 1;
static const uint32_t CPMS_RVCSR_RVRM_TUNE = 6;
static const uint32_t CPMS_RVCSR_RVRM_TUNE_LEN = 6;
static const uint32_t CPMS_RVCSR_RVID_ACTIVE = 32;
static const uint32_t CPMS_RVCSR_BYPASS_ACTIVE = 33;
// ppe_c/reg00004.H

static const uint64_t QME_SPWU_FSP = 0x00000006c0018340ull;

static const uint32_t QME_SPWU_FSP_REQ = 0;
static const uint32_t QME_SPWU_FSP_DONE = 4;
// ppe_c/reg00004.H

static const uint64_t QME_SPWU_HYP = 0x00000006c00183c0ull;

static const uint32_t QME_SPWU_HYP_REQ = 0;
static const uint32_t QME_SPWU_HYP_DONE = 4;
// ppe_c/reg00004.H

static const uint64_t QME_SPWU_OCC = 0x00000006c0018380ull;

static const uint32_t QME_SPWU_OCC_REQ = 0;
static const uint32_t QME_SPWU_OCC_DONE = 4;
// ppe_c/reg00004.H

static const uint64_t QME_SSH_FSP = 0x00000006c0018240ull;

static const uint32_t QME_SSH_FSP_STOP_GATED = 0;
static const uint32_t QME_SSH_FSP_SPECIAL_WKUP_DONE = 1;
static const uint32_t QME_SSH_FSP_STOP_TRANSITION = 2;
static const uint32_t QME_SSH_FSP_STOP_TRANSITION_LEN = 2;
static const uint32_t QME_SSH_FSP_REQ_STOP_LEVEL = 4;
static const uint32_t QME_SSH_FSP_REQ_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_FSP_ACT_STOP_LEVEL = 8;
static const uint32_t QME_SSH_FSP_ACT_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_FSP_DEEPEST_REQ_STOP_LEVEL = 12;
static const uint32_t QME_SSH_FSP_DEEPEST_REQ_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_FSP_DEEPEST_ACT_STOP_LEVEL = 16;
static const uint32_t QME_SSH_FSP_DEEPEST_ACT_STOP_LEVEL_LEN = 4;
// ppe_c/reg00004.H

static const uint64_t QME_SSH_HYP = 0x00000006c00182c0ull;

static const uint32_t QME_SSH_HYP_STOP_GATED = 0;
static const uint32_t QME_SSH_HYP_SPECIAL_WKUP_DONE = 1;
static const uint32_t QME_SSH_HYP_STOP_TRANSITION = 2;
static const uint32_t QME_SSH_HYP_STOP_TRANSITION_LEN = 2;
static const uint32_t QME_SSH_HYP_REQ_STOP_LEVEL = 4;
static const uint32_t QME_SSH_HYP_REQ_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_HYP_ACT_STOP_LEVEL = 8;
static const uint32_t QME_SSH_HYP_ACT_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_HYP_DEEPEST_REQ_STOP_LEVEL = 12;
static const uint32_t QME_SSH_HYP_DEEPEST_REQ_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_HYP_DEEPEST_ACT_STOP_LEVEL = 16;
static const uint32_t QME_SSH_HYP_DEEPEST_ACT_STOP_LEVEL_LEN = 4;
// ppe_c/reg00004.H

static const uint64_t QME_SSH_OCC = 0x00000006c0018280ull;

static const uint32_t QME_SSH_OCC_STOP_GATED = 0;
static const uint32_t QME_SSH_OCC_SPECIAL_WKUP_DONE = 1;
static const uint32_t QME_SSH_OCC_STOP_TRANSITION = 2;
static const uint32_t QME_SSH_OCC_STOP_TRANSITION_LEN = 2;
static const uint32_t QME_SSH_OCC_REQ_STOP_LEVEL = 4;
static const uint32_t QME_SSH_OCC_REQ_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_OCC_ACT_STOP_LEVEL = 8;
static const uint32_t QME_SSH_OCC_ACT_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_OCC_DEEPEST_REQ_STOP_LEVEL = 12;
static const uint32_t QME_SSH_OCC_DEEPEST_REQ_STOP_LEVEL_LEN = 4;
static const uint32_t QME_SSH_OCC_DEEPEST_ACT_STOP_LEVEL = 16;
static const uint32_t QME_SSH_OCC_DEEPEST_ACT_STOP_LEVEL_LEN = 4;
// ppe_c/reg00004.H

}
}
#include "ppe_c/reg00004.H"
#endif
