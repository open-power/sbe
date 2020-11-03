/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_1.H $          */
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

#ifndef __p10_ppe_eq_1_H_
#define __p10_ppe_eq_1_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_EIPR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_EIPR_RW = 0xc0002680ull;
static const uint32_t QME_EIPR_WO_CLEAR = 0xc00026b0ull;
static const uint32_t QME_EIPR_WO_OR = 0xc00026a0ull;

static const uint32_t QME_EIPR_INTERRUPT_POLARITY = 0;
static const uint32_t QME_EIPR_INTERRUPT_POLARITY_LEN = 64;
//<< [QME_EIPR]
// ppe_eq/reg00001.H

//>> [QME_LFIRMASK]
//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_LFIRMASK_RW = 0xc0000040ull;
static const uint32_t QME_LFIRMASK_WO_AND = 0xc0000050ull;
static const uint32_t QME_LFIRMASK_WO_OR = 0xc0000060ull;

static const uint32_t QME_LFIRMASK_FIR_MASK = 0;
static const uint32_t QME_LFIRMASK_FIR_MASK_LEN = 36;
//<< [QME_LFIRMASK]
// ppe_eq/reg00001.H

//>> [QME_SCDR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SCDR = 0xc0001440ull;

static const uint32_t QME_SCDR_STOP_GATED = 0;
static const uint32_t QME_SCDR_STOP_GATED_LEN = 4;
static const uint32_t QME_SCDR_STOP_TRANSITION_C0 = 4;
static const uint32_t QME_SCDR_STOP_TRANSITION_C0_LEN = 2;
static const uint32_t QME_SCDR_STOP_TRANSITION_C1 = 6;
static const uint32_t QME_SCDR_STOP_TRANSITION_C1_LEN = 2;
static const uint32_t QME_SCDR_STOP_TRANSITION_C2 = 8;
static const uint32_t QME_SCDR_STOP_TRANSITION_C2_LEN = 2;
static const uint32_t QME_SCDR_STOP_TRANSITION_C3 = 10;
static const uint32_t QME_SCDR_STOP_TRANSITION_C3_LEN = 2;
static const uint32_t QME_SCDR_SPC_WKUP_DONE = 12;
static const uint32_t QME_SCDR_SPC_WKUP_DONE_LEN = 4;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C0 = 16;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C0_LEN = 4;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C1 = 20;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C1_LEN = 4;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C2 = 24;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C2_LEN = 4;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C3 = 28;
static const uint32_t QME_SCDR_REQ_STOP_LEVEL_C3_LEN = 4;
static const uint32_t QME_SCDR_REG_WKUP_REQUESTED = 32;
static const uint32_t QME_SCDR_REG_WKUP_REQUESTED_LEN = 4;
static const uint32_t QME_SCDR_SPC_WKUP_REQUESTED = 36;
static const uint32_t QME_SCDR_SPC_WKUP_REQUESTED_LEN = 4;
static const uint32_t QME_SCDR_SELF_SAVE_ACTIVE = 40;
static const uint32_t QME_SCDR_SELF_SAVE_ACTIVE_LEN = 4;
static const uint32_t QME_SCDR_SELF_RESTORE_ACTIVE = 44;
static const uint32_t QME_SCDR_SELF_RESTORE_ACTIVE_LEN = 4;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C0 = 48;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C0_LEN = 4;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C1 = 52;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C1_LEN = 4;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C2 = 56;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C2_LEN = 4;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C3 = 60;
static const uint32_t QME_SCDR_ACT_STOP_LEVEL_C3_LEN = 4;
//<< [QME_SCDR]
// ppe_eq/reg00001.H

//>> [QME_SSDR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SSDR = 0xc0001400ull;

static const uint32_t QME_SSDR_PC_UNMASKED_ATTN = 0;
static const uint32_t QME_SSDR_PC_UNMASKED_ATTN_LEN = 4;
static const uint32_t QME_SSDR_INSTR_RUNNING = 4;
static const uint32_t QME_SSDR_INSTR_RUNNING_LEN = 4;
static const uint32_t QME_SSDR_QME_SCSR_PM_EXIT = 8;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_PM_EXIT = 9;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_PM_EXIT = 10;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_PM_EXIT = 11;
static const uint32_t QME_SSDR_PM_STATE_ACTIVE = 12;
static const uint32_t QME_SSDR_PM_STATE_ACTIVE_LEN = 4;
static const uint32_t QME_SSDR_PM_STATE_C0 = 16;
static const uint32_t QME_SSDR_PM_STATE_C0_LEN = 4;
static const uint32_t QME_SSDR_PM_STATE_C1 = 20;
static const uint32_t QME_SSDR_PM_STATE_C1_LEN = 4;
static const uint32_t QME_SSDR_PM_STATE_C2 = 24;
static const uint32_t QME_SSDR_PM_STATE_C2_LEN = 4;
static const uint32_t QME_SSDR_PM_STATE_C3 = 28;
static const uint32_t QME_SSDR_PM_STATE_C3_LEN = 4;
static const uint32_t QME_SSDR_QME_SCSR_BLOCK_INTR_OUTPUTS = 32;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_BLOCK_INTR_OUTPUTS = 33;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_BLOCK_INTR_OUTPUTS = 34;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_BLOCK_INTR_OUTPUTS = 35;
static const uint32_t QME_SSDR_QME_SCSR_PM_BLOCK_INTR = 36;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_PM_BLOCK_INTR = 37;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_PM_BLOCK_INTR = 38;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SCSR_PM_BLOCK_INTR = 39;
static const uint32_t QME_SSDR_PC_DPES_INTR_PEND = 40;
static const uint32_t QME_SSDR_PC_DPES_INTR_PEND_LEN = 4;
static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 44;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 45;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 46;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_ACTIVE = 47;
static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE = 48;
static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE = 52;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE = 56;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE = 60;
// ERROR Duplicate Dial         static const uint32_t QME_SSDR_QME_SSH_SRC_SAMPLED_STOP_STATE_LEN = 4;
//<< [QME_SSDR]
// ppe_eq/reg00001.H

//>> [QME_SWHR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SWHR = 0xc00014c0ull;

static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C0 = 0;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C0_LEN = 4;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 4;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 6;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 8;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 10;
static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
static const uint32_t QME_SWHR_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 12;
static const uint32_t QME_SWHR_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 13;
static const uint32_t QME_SWHR_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 14;
static const uint32_t QME_SWHR_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 15;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C1 = 16;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C1_LEN = 4;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 20;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 22;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 24;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 26;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 28;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 29;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 30;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 31;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C2 = 32;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C2_LEN = 4;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 36;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 38;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 40;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 42;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 44;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 45;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 46;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 47;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C3 = 48;
static const uint32_t QME_SWHR_SPC_WKUP_HISTORY_C3_LEN = 4;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET = 52;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_SET_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR = 54;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_CLR_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT = 56;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_ACT_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP = 58;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SSH_SRC_SPC_WKUP_RECENT_DRP_LEN = 2;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_OTR_SPECIAL_WKUP_REQ = 60;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_FSP_SPECIAL_WKUP_REQ = 61;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_OCC_SPECIAL_WKUP_REQ = 62;
// ERROR Duplicate Dial         static const uint32_t QME_SWHR_QME_SPWU_HYP_SPECIAL_WKUP_REQ = 63;
//<< [QME_SWHR]
// ppe_eq/reg00001.H

//>> [QME_SWPR0]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SWPR0 = 0xc0000900ull;

static const uint32_t QME_SWPR0_BAR = 0;
static const uint32_t QME_SWPR0_BAR_LEN = 27;
static const uint32_t QME_SWPR0_SIZE = 44;
static const uint32_t QME_SWPR0_SIZE_LEN = 15;
//<< [QME_SWPR0]
// ppe_eq/reg00001.H

//>> [QME_TBR]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_TBR = 0xc0002480ull;

static const uint32_t QME_TBR_TIMEBASE = 0;
static const uint32_t QME_TBR_TIMEBASE_LEN = 32;
static const uint32_t QME_TBR_CYCLES = 32;
static const uint32_t QME_TBR_CYCLES_LEN = 32;
//<< [QME_TBR]
// ppe_eq/reg00001.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00001.H"
#endif
#endif
