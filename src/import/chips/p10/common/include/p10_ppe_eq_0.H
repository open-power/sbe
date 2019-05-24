/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_0.H $          */
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

#ifndef __p10_ppe_eq_0_H_
#define __p10_ppe_eq_0_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_BCECSR = 0xc0000680ull;

static const uint32_t QME_BCECSR_BUSY = 0;
static const uint32_t QME_BCECSR_ERROR = 1;
static const uint32_t QME_BCECSR_RNW = 4;
static const uint32_t QME_BCECSR_BARSEL = 5;
static const uint32_t QME_BCECSR_PRIORITY = 6;
static const uint32_t QME_BCECSR_INJECT_ERR = 7;
static const uint32_t QME_BCECSR_TYPE = 13;
static const uint32_t QME_BCECSR_TYPE_LEN = 3;
static const uint32_t QME_BCECSR_NUM_BLOCKS = 17;
static const uint32_t QME_BCECSR_NUM_BLOCKS_LEN = 11;
static const uint32_t QME_BCECSR_SBASE = 28;
static const uint32_t QME_BCECSR_SBASE_LEN = 12;
static const uint32_t QME_BCECSR_MBASE = 42;
static const uint32_t QME_BCECSR_MBASE_LEN = 22;
// ppe_eq/reg00000.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PSREQ = 0xc0001300ull;

static const uint32_t QME_PSREQ_CORE_PARTIAL_GOOD = 48;
static const uint32_t QME_PSREQ_CORE_PARTIAL_GOOD_LEN = 4;
static const uint32_t QME_PSREQ_AUTO_PMCR_UPDATE = 52;
static const uint32_t QME_PSREQ_PMCR_RECENT_MODE = 53;
// ppe_eq/reg00000.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCOCR = 0xc00017c0ull;

static const uint32_t QME_RCOCR_0_RESCLK_CONTROL = 0;
static const uint32_t QME_RCOCR_0_RESCLK_CONTROL_LEN = 8;
static const uint32_t QME_RCOCR_1_RESCLK_CONTROL = 8;
static const uint32_t QME_RCOCR_1_RESCLK_CONTROL_LEN = 8;
static const uint32_t QME_RCOCR_2_RESCLK_CONTROL = 16;
static const uint32_t QME_RCOCR_2_RESCLK_CONTROL_LEN = 8;
static const uint32_t QME_RCOCR_3_RESCLK_CONTROL = 24;
static const uint32_t QME_RCOCR_3_RESCLK_CONTROL_LEN = 8;
static const uint32_t QME_RCOCR_0_RESCLK_ACTUAL = 32;
static const uint32_t QME_RCOCR_0_RESCLK_ACTUAL_LEN = 8;
static const uint32_t QME_RCOCR_1_RESCLK_ACTUAL = 40;
static const uint32_t QME_RCOCR_1_RESCLK_ACTUAL_LEN = 8;
static const uint32_t QME_RCOCR_2_RESCLK_ACTUAL = 48;
static const uint32_t QME_RCOCR_2_RESCLK_ACTUAL_LEN = 8;
static const uint32_t QME_RCOCR_3_RESCLK_ACTUAL = 56;
static const uint32_t QME_RCOCR_3_RESCLK_ACTUAL_LEN = 8;
// ppe_eq/reg00000.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RCTAR3 = 0xc00019c0ull;

static const uint32_t QME_RCTAR3_QME_RCTAR3_DATA = 0;
static const uint32_t QME_RCTAR3_QME_RCTAR3_DATA_LEN = 64;
// ppe_eq/reg00000.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_RS4CSR = 0xc0001680ull;

static const uint32_t QME_RS4CSR_BUSY = 0;
static const uint32_t QME_RS4CSR_ERROR = 1;
static const uint32_t QME_RS4CSR_SCAN_MODE_OVERRIDE = 5;
static const uint32_t QME_RS4CSR_SCAN_MODE_OVERRIDE_LEN = 2;
static const uint32_t QME_RS4CSR_INJECT_ERR = 7;
static const uint32_t QME_RS4CSR_ERR_CARE_MASK_INVALID = 9;
static const uint32_t QME_RS4CSR_ERR_CHECKWORD_MISMATCH = 10;
static const uint32_t QME_RS4CSR_ERR_SCAN_MODE_INVALID = 11;
static const uint32_t QME_RS4CSR_ERR_ROTATE_AMOUNT = 12;
static const uint32_t QME_RS4CSR_BAD_SCOM_RC = 13;
static const uint32_t QME_RS4CSR_BAD_SCOM_RC_LEN = 3;
// ppe_eq/reg00000.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_PBTXTR2 = 0xc0000420ull;

static const uint32_t QME_SCOM_PBTXTR2__VALID = 0;
static const uint32_t QME_SCOM_PBTXTR2__VALID_LEN = 8;
static const uint32_t QME_SCOM_PBTXTR2_0 = 8;
static const uint32_t QME_SCOM_PBTXTR2_0_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_1 = 12;
static const uint32_t QME_SCOM_PBTXTR2_1_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_2 = 16;
static const uint32_t QME_SCOM_PBTXTR2_2_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_3 = 20;
static const uint32_t QME_SCOM_PBTXTR2_3_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_4 = 24;
static const uint32_t QME_SCOM_PBTXTR2_4_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_5 = 28;
static const uint32_t QME_SCOM_PBTXTR2_5_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_6 = 32;
static const uint32_t QME_SCOM_PBTXTR2_6_LEN = 4;
static const uint32_t QME_SCOM_PBTXTR2_7 = 36;
static const uint32_t QME_SCOM_PBTXTR2_7_LEN = 4;
// ppe_eq/reg00000.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCOM_XIVDRX = 0xc0002940ull;

static const uint32_t QME_SCOM_XIVDRX_0 = 0;
static const uint32_t QME_SCOM_XIVDRX_0_LEN = 32;
static const uint32_t QME_SCOM_XIVDRX_3 = 32;
static const uint32_t QME_SCOM_XIVDRX_3_LEN = 32;
// ppe_eq/reg00000.H

}
}
#include "ppe_eq/reg00000.H"
#endif
