/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_3.H $           */
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

#ifndef __p10_ppe_c_3_H_
#define __p10_ppe_c_3_H_


namespace scomt
{
namespace ppe_c
{


static const uint64_t CPMS_CL2_PFETCNTL = 0x00000006c001e280ull;
static const uint64_t CPMS_CL2_PFETCNTL_WO_CLEAR = 0x00000006c001e2a0ull;
static const uint64_t CPMS_CL2_PFETCNTL_WO_OR = 0x00000006c001e2b0ull;

static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE = 0;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEQ_STATE_LEN = 2;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE = 2;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEQ_STATE_LEN = 2;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_VAL_OVERRIDE = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEL_OVERRIDE = 5;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_VAL_OVERRIDE = 6;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEL_OVERRIDE = 7;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_REGULATION_FINGER_SET = 8;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE = 12;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_ENABLE_VALUE_LEN = 8;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE = 20;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PFET_SEL_VALUE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE = 24;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_ENABLE_VALUE_LEN = 8;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE = 32;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PFET_SEL_VALUE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_STATE = 42;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_STATE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_SEL = 46;
static const uint32_t CPMS_CL2_PFETCNTL_VDD_PG_SEL_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_STATE = 50;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_STATE_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_SEL = 54;
static const uint32_t CPMS_CL2_PFETCNTL_VCS_PG_SEL_LEN = 4;
static const uint32_t CPMS_CL2_PFETCNTL_SRAM_ENABLE = 63;
// ppe_c/reg00003.H

static const uint64_t QME_CDBA = 0x00000006c0018000ull;

static const uint32_t QME_CDBA_QME_CDBA_MESSAGE_NUMBER = 0;
static const uint32_t QME_CDBA_QME_CDBA_MESSAGE_NUMBER_LEN = 8;
// ppe_c/reg00003.H

static const uint64_t QME_CSIR = 0x00000006c00187c0ull;

static const uint32_t QME_CSIR_QME_CSIR_DATA = 0;
static const uint32_t QME_CSIR_QME_CSIR_DATA_LEN = 64;
// ppe_c/reg00003.H

}
}
#include "ppe_c/reg00003.H"
#endif
