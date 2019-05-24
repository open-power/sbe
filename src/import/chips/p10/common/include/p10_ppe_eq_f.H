/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_f.H $          */
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

#ifndef __p10_ppe_eq_f_H_
#define __p10_ppe_eq_f_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PBCR_RW = 0xc0000440ull;
static const uint32_t QME_PBCR_WO_CLEAR = 0xc0000470ull;
static const uint32_t QME_PBCR_WO_OR = 0xc0000460ull;
// ppe_eq/reg00015.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QSAR = 0xc0000740ull;
// ppe_eq/reg00015.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIVDR6 = 0xc00028c0ull;

static const uint32_t QME_SCOM_XIVDR6_6 = 0;
static const uint32_t QME_SCOM_XIVDR6_6_LEN = 32;
static const uint32_t QME_SCOM_XIVDR6_7 = 32;
static const uint32_t QME_SCOM_XIVDR6_7_LEN = 32;
// ppe_eq/reg00015.H

}
}
#include "ppe_eq/reg00015.H"
#endif
