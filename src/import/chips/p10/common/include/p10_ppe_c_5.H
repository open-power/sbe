/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_5.H $           */
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

#ifndef __p10_ppe_c_5_H_
#define __p10_ppe_c_5_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [QME_CIIR]
static const uint32_t QME_CIIR = 0xc0008740ull;

static const uint32_t QME_CIIR_MSGSND_INTR_INJECT = 28;
static const uint32_t QME_CIIR_MSGSND_INTR_INJECT_LEN = 4;
static const uint32_t QME_CIIR_MSGSNDU_INTR_INJECT = 60;
static const uint32_t QME_CIIR_MSGSNDU_INTR_INJECT_LEN = 4;
//<< [QME_CIIR]
// ppe_c/reg00005.H

//>> [QME_RMOR]
static const uint32_t QME_RMOR = 0xc0008680ull;

static const uint32_t QME_RMOR_DATA = 0;
static const uint32_t QME_RMOR_DATA_LEN = 63;
static const uint32_t QME_RMOR_URMOR_SELECT = 63;
//<< [QME_RMOR]
// ppe_c/reg00005.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00005.H"
#endif
#endif
