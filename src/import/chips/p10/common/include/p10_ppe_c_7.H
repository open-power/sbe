/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_c_7.H $           */
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

#ifndef __p10_ppe_c_7_H_
#define __p10_ppe_c_7_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_c
{
#endif


//>> [CPMS_CERRMSK]
static const uint32_t CPMS_CERRMSK = 0xc000e140ull;

static const uint32_t CPMS_CERRMSK_PFET_SEQ_ERR_MASK = 0;
static const uint32_t CPMS_CERRMSK_DDS_ERR_MASK = 1;
static const uint32_t CPMS_CERRMSK_CLK_SYNC_ERR_MASK = 2;
static const uint32_t CPMS_CERRMSK_DPT_ACCESS_MASK = 3;
static const uint32_t CPMS_CERRMSK_PROXY_MASK = 4;
static const uint32_t CPMS_CERRMSK_FTC_EVENT_MASK = 5;
static const uint32_t CPMS_CERRMSK_FTC_EVENT_MASK_LEN = 3;
static const uint32_t CPMS_CERRMSK_PITCH_MASK = 8;
//<< [CPMS_CERRMSK]
// ppe_c/reg00007.H

//>> [QME_PSCRS3]
static const uint32_t QME_PSCRS3 = 0xc00085c0ull;

static const uint32_t QME_PSCRS3_DPDES_EXIT_ENABLE = 0;
static const uint32_t QME_PSCRS3_SD = 1;
static const uint32_t QME_PSCRS3_ESL = 2;
static const uint32_t QME_PSCRS3_EC = 3;
static const uint32_t QME_PSCRS3_PSLL = 4;
static const uint32_t QME_PSCRS3_PSLL_LEN = 4;
static const uint32_t QME_PSCRS3_HYP_VIRT_EXIT_ENABLE = 8;
static const uint32_t QME_PSCRS3_UV_DB_EXIT_ENABLE = 9;
static const uint32_t QME_PSCRS3_HYP_DB_EXIT_ENABLE = 10;
static const uint32_t QME_PSCRS3_EXT_EXIT_ENABLE = 11;
static const uint32_t QME_PSCRS3_DEC_EXIT_ENABLE = 12;
static const uint32_t QME_PSCRS3_HMI_EXIT_ENABLE = 13;
static const uint32_t QME_PSCRS3_TR = 14;
static const uint32_t QME_PSCRS3_TR_LEN = 2;
static const uint32_t QME_PSCRS3_MTL = 16;
static const uint32_t QME_PSCRS3_MTL_LEN = 4;
static const uint32_t QME_PSCRS3_RL = 20;
static const uint32_t QME_PSCRS3_RL_LEN = 4;
static const uint32_t QME_PSCRS3_PLS = 24;
static const uint32_t QME_PSCRS3_PLS_LEN = 4;
//<< [QME_PSCRS3]
// ppe_c/reg00007.H

//>> [QME_SCSR]
static const uint32_t QME_SCSR = 0xc0008400ull;
static const uint32_t QME_SCSR_PPE2 = 0xc0008420ull;
static const uint32_t QME_SCSR_WO_CLEAR = 0xc0008430ull;

static const uint32_t QME_SCSR_ASSERT_PM_BLOCK_INTR = 0;
static const uint32_t QME_SCSR_ASSERT_PM_EXIT = 1;
static const uint32_t QME_SCSR_ASSERT_PM_ENTRY_LIMIT = 2;
static const uint32_t QME_SCSR_PC_SPARE_OUT = 3;
static const uint32_t QME_SCSR_HBUS_DISABLE = 4;
static const uint32_t QME_SCSR_L2_PURGE_REQ = 5;
static const uint32_t QME_SCSR_L2_PURGE_ABORT = 6;
static const uint32_t QME_SCSR_L2RCMD_INTF_QUIESCE = 7;
static const uint32_t QME_SCSR_NCU_TLBIE_QUIESCE = 8;
static const uint32_t QME_SCSR_NCU_PURGE_REQ = 9;
static const uint32_t QME_SCSR_NCU_PURGE_ABORT = 10;
static const uint32_t QME_SCSR_CHTM_PURGE_REQ = 11;
static const uint32_t QME_SCSR_PB_PURGE_REQ = 12;
static const uint32_t QME_SCSR_CORE_QUIESCE = 13;
static const uint32_t QME_SCSR_SPARE_MODE = 14;
static const uint32_t QME_SCSR_SRESET_REQ = 15;
static const uint32_t QME_SCSR_ASSERT_SPECIAL_WKUP_DONE = 16;
static const uint32_t QME_SCSR_SPARE_OUT = 17;
static const uint32_t QME_SCSR_CORE_INTR_SAMPLE = 18;
static const uint32_t QME_SCSR_IGNORE_PMCR_RECENT = 19;
static const uint32_t QME_SCSR_AUTO_SPECIAL_WAKEUP_DISABLE = 20;
static const uint32_t QME_SCSR_HALTED_STOP_OVERRIDE_DISABLE = 21;
static const uint32_t QME_SCSR_AUTO_PMSR_SHIFT_DIS = 22;
static const uint32_t QME_SCSR_REFRESH_PMSR = 23;
static const uint32_t QME_SCSR_BLOCK_INTR_OUTPUTS = 24;
static const uint32_t QME_SCSR_BLOCK_INTR_INPUTS = 25;
static const uint32_t QME_SCSR_ENABLE_PECE = 26;
static const uint32_t QME_SCSR_CTFS_WKUP_ENABLE = 27;
static const uint32_t QME_SCSR_PM_BLOCK_INTR = 32;
static const uint32_t QME_SCSR_PM_EXIT = 33;
static const uint32_t QME_SCSR_PM_ENTRY_LIMIT = 34;
static const uint32_t QME_SCSR_PC_SPARE_IN = 35;
static const uint32_t QME_SCSR_HBUS_INACTIVE = 36;
static const uint32_t QME_SCSR_L2_PURGE_DONE = 37;
static const uint32_t QME_SCSR_NCU_PURGE_DONE = 41;
static const uint32_t QME_SCSR_CHTM_PURGE_DONE = 43;
static const uint32_t QME_SCSR_PB_PURGE_DONE = 44;
static const uint32_t QME_SCSR_CORE_QUIESCED = 45;
static const uint32_t QME_SCSR_PC_INTR_PENDING = 46;
static const uint32_t QME_SCSR_STOP_SHIFT_ACTIVE = 47;
static const uint32_t QME_SCSR_SPECIAL_WKUP_DONE = 48;
static const uint32_t QME_SCSR_SPARE_IN = 49;
static const uint32_t QME_SCSR_PSSCR_POWEROFF_ALLOWED = 50;
static const uint32_t QME_SCSR_PSSCR_POWEROFF_ALLOWED_LEN = 2;
static const uint32_t QME_SCSR_PC_THREAD_INSTR_RUNNING = 52;
static const uint32_t QME_SCSR_PC_THREAD_INSTR_RUNNING_LEN = 4;
static const uint32_t QME_SCSR_PMSR_SHIFT_INACTIVE = 56;
static const uint32_t QME_SCSR_OTHER_DPDES_INTR_PENDING = 57;
static const uint32_t QME_SCSR_PC_UNMASKED_ATTN = 58;
static const uint32_t QME_SCSR_PM_STATE_ACTIVE = 59;
static const uint32_t QME_SCSR_PM_STATE = 60;
static const uint32_t QME_SCSR_PM_STATE_LEN = 4;
//<< [QME_SCSR]
// ppe_c/reg00007.H

//>> [QME_TFCSR]
static const uint32_t QME_TFCSR = 0xc0008800ull;
static const uint32_t QME_TFCSR_PPE2 = 0xc0008820ull;
static const uint32_t QME_TFCSR_WO_CLEAR = 0xc0008830ull;

static const uint32_t QME_TFCSR_XFER_START = 0;
static const uint32_t QME_TFCSR_TFAC_RESET = 1;
static const uint32_t QME_TFCSR_CORE_NUM = 30;
static const uint32_t QME_TFCSR_CORE_NUM_LEN = 2;
static const uint32_t QME_TFCSR_XFER_RECEIVE_DONE = 32;
static const uint32_t QME_TFCSR_XFER_SENT_DONE = 33;
static const uint32_t QME_TFCSR_INCOMING_ERR = 34;
static const uint32_t QME_TFCSR_RUNTIME_ERR = 35;
static const uint32_t QME_TFCSR_STATE_ERR = 36;
static const uint32_t QME_TFCSR_HDEC_MSB = 59;
static const uint32_t QME_TFCSR_DEC_MSB = 60;
static const uint32_t QME_TFCSR_DEC_MSB_LEN = 4;
//<< [QME_TFCSR]
// ppe_c/reg00007.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_c/reg00007.H"
#endif
#endif
