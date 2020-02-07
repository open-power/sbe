/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_f.H $          */
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

#ifndef __p10_ppe_eq_f_H_
#define __p10_ppe_eq_f_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_PBCR = 0xc0000440ull;
static const uint32_t QME_PBCR_PPE2 = 0xc0000460ull;
static const uint32_t QME_PBCR_WO_CLEAR = 0xc0000470ull;

static const uint32_t QME_PBCR_DISABLE_LN_RD = 0;
static const uint32_t QME_PBCR_DISABLE_NN_RN_RD = 1;
static const uint32_t QME_PBCR_DISABLE_GROUP_RD = 2;
static const uint32_t QME_PBCR_DISABLE_VG_RD = 3;
static const uint32_t QME_PBCR_DISABLE_LN_WR = 4;
static const uint32_t QME_PBCR_DISABLE_NN_RN_WR = 5;
static const uint32_t QME_PBCR_DISABLE_GROUP_WR = 6;
static const uint32_t QME_PBCR_DISABLE_VG_WR = 7;
static const uint32_t QME_PBCR_SKIP_GROUP_SCOPE = 8;
static const uint32_t QME_PBCR_USE_DMA_PR_W = 9;
static const uint32_t QME_PBCR_FREEZE_ON_CRESP_ERR = 10;
static const uint32_t QME_PBCR_SPARE = 11;
static const uint32_t QME_PBCR_SPARE_LEN = 2;
static const uint32_t QME_PBCR_TYPE = 13;
static const uint32_t QME_PBCR_TYPE_LEN = 3;
static const uint32_t QME_PBCR_HANG_POLL_MAX_CNT = 16;
static const uint32_t QME_PBCR_HANG_POLL_MAX_CNT_LEN = 4;
static const uint32_t QME_PBCR_HANG_DATA_MAX_CNT = 20;
static const uint32_t QME_PBCR_HANG_DATA_MAX_CNT_LEN = 4;
static const uint32_t QME_PBCR_DISABLE_PB_CACHE = 24;
static const uint32_t QME_PBCR_INVALIDATE_PB_CACHE = 25;
// ppe_eq/reg00015.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QSAR = 0xc0000740ull;

static const uint32_t QME_QSAR_SRAM_ADDRESS = 16;
static const uint32_t QME_QSAR_SRAM_ADDRESS_LEN = 13;
static const uint32_t QME_QSAR_AUTO_INCR_MODE = 63;
// ppe_eq/reg00015.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00015.H"
#endif
#endif
