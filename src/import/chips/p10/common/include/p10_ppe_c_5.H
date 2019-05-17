/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_5.H $           */
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

#ifndef __p10_ppe_c_5_H_
#define __p10_ppe_c_5_H_


namespace scomt
{
namespace ppe_c
{


static const uint64_t QME_CIIR = 0x00000006c0018740ull;

static const uint32_t QME_CIIR__INTR_INJECT = 28;
static const uint32_t QME_CIIR__INTR_INJECT_LEN = 4;
static const uint32_t QME_CIIR_U_INTR_INJECT = 60;
static const uint32_t QME_CIIR_U_INTR_INJECT_LEN = 4;
// ppe_c/reg00005.H

static const uint64_t QME_RMOR = 0x00000006c0018680ull;

static const uint32_t QME_RMOR_DATA = 0;
static const uint32_t QME_RMOR_DATA_LEN = 63;
static const uint32_t QME_RMOR_URMOR_SELECT = 63;
// ppe_c/reg00005.H

static const uint64_t QME_SCR_RW = 0x00000006c00180c0ull;
static const uint64_t QME_SCR_WO_CLEAR = 0x00000006c00180f0ull;
static const uint64_t QME_SCR_WO_OR = 0x00000006c00180e0ull;

static const uint32_t QME_SCR_QME_SCR_DATA = 0;
static const uint32_t QME_SCR_QME_SCR_DATA_LEN = 64;
// ppe_c/reg00005.H

}
}
#include "ppe_c/reg00005.H"
#endif
