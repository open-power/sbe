/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_f.H $           */
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

#ifndef __p10_ppe_c_f_H_
#define __p10_ppe_c_f_H_


namespace scomt
{
namespace ppe_c
{


static const uint32_t CPMS_MMAR_RW = 0xc001e380ull;
static const uint32_t CPMS_MMAR_WO_CLEAR = 0xc001e3a0ull;
static const uint32_t CPMS_MMAR_WO_OR = 0xc001e3b0ull;

static const uint32_t CPMS_MMAR_AVAILABLE = 0;
static const uint32_t CPMS_MMAR_FUNC_RESET = 1;
// ppe_c/reg00015.H

}
}
#include "ppe_c/reg00015.H"
#endif
