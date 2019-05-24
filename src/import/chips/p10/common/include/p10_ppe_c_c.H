/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_c.H $           */
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

#ifndef __p10_ppe_c_c_H_
#define __p10_ppe_c_c_H_


namespace scomt
{
namespace ppe_c
{


static const uint32_t CPMS_CGCSR = 0xc001e000ull;
static const uint32_t CPMS_CGCSR_WO_CLEAR = 0xc001e020ull;
static const uint32_t CPMS_CGCSR_WO_OR = 0xc001e030ull;

static const uint32_t CPMS_CGCSR_L3_CLK_SYNC_ENABLE = 0;
static const uint32_t CPMS_CGCSR_CL2_CLK_SYNC_ENABLE = 1;
static const uint32_t CPMS_CGCSR_RESERVED_2_3 = 2;
static const uint32_t CPMS_CGCSR_RESERVED_2_3_LEN = 2;
static const uint32_t CPMS_CGCSR_L3_CLKGLM_ASYNC_RESET = 4;
static const uint32_t CPMS_CGCSR_L3_CLKGLM_SEL = 7;
static const uint32_t CPMS_CGCSR_CL2_CLKGLM_ASYNC_RESET = 8;
static const uint32_t CPMS_CGCSR_CL2_CLKGLM_SEL = 11;
static const uint32_t CPMS_CGCSR_L3_CACHE_SB_CTRL = 12;
static const uint32_t CPMS_CGCSR_L3_CACHE_SB_CTRL_LEN = 4;
static const uint32_t CPMS_CGCSR_L3_CLK_SYNC_DONE = 32;
static const uint32_t CPMS_CGCSR_CL2_CLK_SYNC_DONE = 33;
// ppe_c/reg00012.H

}
}
#include "ppe_c/reg00012.H"
#endif
