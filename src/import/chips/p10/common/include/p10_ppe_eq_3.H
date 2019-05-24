/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_3.H $          */
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

#ifndef __p10_ppe_eq_3_H_
#define __p10_ppe_eq_3_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_DB1 = 0xc0001040ull;

static const uint32_t QME_DB1_NUMBER = 0;
static const uint32_t QME_DB1_NUMBER_LEN = 8;
static const uint32_t QME_DB1_DATA = 8;
static const uint32_t QME_DB1_DATA_LEN = 56;
// ppe_eq/reg00003.H

//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_PCBSID = 0xc0000340ull;
// ppe_eq/reg00003.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RS4DBGS = 0xc00015c0ull;

static const uint32_t QME_RS4DBGS_ACTUAL_BITS = 16;
static const uint32_t QME_RS4DBGS_ACTUAL_BITS_LEN = 16;
static const uint32_t QME_RS4DBGS_MACHINE_STATE = 32;
static const uint32_t QME_RS4DBGS_MACHINE_STATE_LEN = 4;
static const uint32_t QME_RS4DBGS_STATE_COUNT = 36;
static const uint32_t QME_RS4DBGS_STATE_COUNT_LEN = 4;
static const uint32_t QME_RS4DBGS_CURRENT_NIBBLE = 40;
static const uint32_t QME_RS4DBGS_CURRENT_NIBBLE_LEN = 4;
static const uint32_t QME_RS4DBGS_CARE_MASK = 44;
static const uint32_t QME_RS4DBGS_CARE_MASK_LEN = 4;
static const uint32_t QME_RS4DBGS_TERMINATE_COUNT = 48;
static const uint32_t QME_RS4DBGS_TERMINATE_COUNT_LEN = 4;
static const uint32_t QME_RS4DBGS_SCAN_MODE = 52;
static const uint32_t QME_RS4DBGS_SCAN_COUNT = 58;
static const uint32_t QME_RS4DBGS_SCAN_COUNT_LEN = 6;
// ppe_eq/reg00003.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIVDR4 = 0xc0002880ull;

static const uint32_t QME_SCOM_XIVDR4_4 = 0;
static const uint32_t QME_SCOM_XIVDR4_4_LEN = 32;
static const uint32_t QME_SCOM_XIVDR4_5 = 32;
static const uint32_t QME_SCOM_XIVDR4_5_LEN = 32;
// ppe_eq/reg00003.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIXCR = 0xc0002000ull;

static const uint32_t QME_SCOM_XIXCR_QME_SCOM_XIXCR_CTR = 32;
static const uint32_t QME_SCOM_XIXCR_QME_SCOM_XIXCR_CTR_LEN = 32;
// ppe_eq/reg00003.H

}
}
#include "ppe_eq/reg00003.H"
#endif
