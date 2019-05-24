/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_9.H $          */
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

#ifndef __p10_ppe_eq_9_H_
#define __p10_ppe_eq_9_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_DB2 = 0xc0001080ull;

static const uint32_t QME_DB2_QME_DB2_MESSAGE_NUMBER = 0;
static const uint32_t QME_DB2_QME_DB2_MESSAGE_NUMBER_LEN = 8;
// ppe_eq/reg00009.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PSSUM = 0xc0001340ull;

static const uint32_t QME_PSSUM_CORE_PARTIAL_GOOD = 48;
static const uint32_t QME_PSSUM_CORE_PARTIAL_GOOD_LEN = 4;
static const uint32_t QME_PSSUM_AUTO_PMCR_UPDATE = 52;
static const uint32_t QME_PSSUM_PMCR_RECENT_MODE = 53;
// ppe_eq/reg00009.H

}
}
#include "ppe_eq/reg00009.H"
#endif
