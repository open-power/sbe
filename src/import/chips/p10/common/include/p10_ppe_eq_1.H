/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_1.H $          */
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

#ifndef __p10_ppe_eq_1_H_
#define __p10_ppe_eq_1_H_


namespace scomt
{
namespace ppe_eq
{


//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_EIPR_RW = 0x00000006c0002680ull;
static const uint64_t QME_EIPR_WO_CLEAR = 0x00000006c00026b0ull;
static const uint64_t QME_EIPR_WO_OR = 0x00000006c00026a0ull;

static const uint32_t QME_EIPR_QME_EIPR_INTERRUPT_POLARITY = 0;
static const uint32_t QME_EIPR_QME_EIPR_INTERRUPT_POLARITY_LEN = 64;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint64_t QME_LFIRMASK_RW = 0x00000006c0000040ull;
static const uint64_t QME_LFIRMASK_WO_AND = 0x00000006c0000050ull;
static const uint64_t QME_LFIRMASK_WO_OR = 0x00000006c0000060ull;

static const uint32_t QME_LFIRMASK_QME_LFIRMASK_FIR_MASK = 0;
static const uint32_t QME_LFIRMASK_QME_LFIRMASK_FIR_MASK_LEN = 28;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_SCDR = 0x00000006c0001440ull;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_SCOM_XIDBGPRO = 0x00000006c0002140ull;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_SSDR = 0x00000006c0001400ull;

static const uint32_t QME_SSDR_0_QME_SCSR_PM_EXIT = 8;
static const uint32_t QME_SSDR_1_QME_SCSR_PM_EXIT = 9;
static const uint32_t QME_SSDR_2_QME_SCSR_PM_EXIT = 10;
static const uint32_t QME_SSDR_3_QME_SCSR_PM_EXIT = 11;
static const uint32_t QME_SSDR_0_QME_SCSR_BLOCK_INTR_OUTPUTS = 32;
static const uint32_t QME_SSDR_1_QME_SCSR_BLOCK_INTR_OUTPUTS = 33;
static const uint32_t QME_SSDR_2_QME_SCSR_BLOCK_INTR_OUTPUTS = 34;
static const uint32_t QME_SSDR_3_QME_SCSR_BLOCK_INTR_OUTPUTS = 35;
static const uint32_t QME_SSDR_0_QME_SCSR_PM_BLOCK_INTR = 36;
static const uint32_t QME_SSDR_1_QME_SCSR_PM_BLOCK_INTR = 37;
static const uint32_t QME_SSDR_2_QME_SCSR_PM_BLOCK_INTR = 38;
static const uint32_t QME_SSDR_3_QME_SCSR_PM_BLOCK_INTR = 39;
static const uint32_t QME_SSDR_0_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 44;
static const uint32_t QME_SSDR_1_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 45;
static const uint32_t QME_SSDR_2_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 46;
static const uint32_t QME_SSDR_3_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 47;
static const uint32_t QME_SSDR_0_QME_SSH_SRC_SAMPLED_STOP_STATE = 48;
static const uint32_t QME_SSDR_0_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
static const uint32_t QME_SSDR_1_QME_SSH_SRC_SAMPLED_STOP_STATE = 52;
static const uint32_t QME_SSDR_1_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
static const uint32_t QME_SSDR_2_QME_SSH_SRC_SAMPLED_STOP_STATE = 56;
static const uint32_t QME_SSDR_2_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
static const uint32_t QME_SSDR_3_QME_SSH_SRC_SAMPLED_STOP_STATE = 60;
static const uint32_t QME_SSDR_3_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_SWHR = 0x00000006c00014c0ull;

static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 4;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 6;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 8;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 10;
static const uint32_t QME_SWHR_0_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
static const uint32_t QME_SWHR_0_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 12;
static const uint32_t QME_SWHR_0_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 13;
static const uint32_t QME_SWHR_0_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 14;
static const uint32_t QME_SWHR_0_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 15;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 20;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 22;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 24;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 26;
static const uint32_t QME_SWHR_1_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
static const uint32_t QME_SWHR_1_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 28;
static const uint32_t QME_SWHR_1_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 29;
static const uint32_t QME_SWHR_1_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 30;
static const uint32_t QME_SWHR_1_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 31;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 36;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 38;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 40;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 42;
static const uint32_t QME_SWHR_2_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
static const uint32_t QME_SWHR_2_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 44;
static const uint32_t QME_SWHR_2_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 45;
static const uint32_t QME_SWHR_2_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 46;
static const uint32_t QME_SWHR_2_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 47;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 52;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 54;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 56;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 58;
static const uint32_t QME_SWHR_3_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
static const uint32_t QME_SWHR_3_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 60;
static const uint32_t QME_SWHR_3_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 61;
static const uint32_t QME_SWHR_3_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 62;
static const uint32_t QME_SWHR_3_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 63;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_SWPR0 = 0x00000006c0000900ull;

static const uint32_t QME_SWPR0_BAR = 0;
static const uint32_t QME_SWPR0_BAR_LEN = 27;
static const uint32_t QME_SWPR0_SIZE = 44;
static const uint32_t QME_SWPR0_SIZE_LEN = 15;
// ppe_eq/reg00001.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint64_t QME_TBR = 0x00000006c0002480ull;

static const uint32_t QME_TBR_TIMEBASE = 0;
static const uint32_t QME_TBR_TIMEBASE_LEN = 32;
static const uint32_t QME_TBR_CYCLES = 32;
static const uint32_t QME_TBR_CYCLES_LEN = 32;
// ppe_eq/reg00001.H

}
}
#include "ppe_eq/reg00001.H"
#endif
