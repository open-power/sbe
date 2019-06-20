/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ram_core_TEMP_DEFINES.H $ */
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
#ifndef _P10_RAM_CORE_DEFINES_H_
#define _P10_RAM_CORE_DEFINES_H_
//FIXME RTC 211536 -- remove this file when scomt supports these definitions

#define EC_PC_PMC_THREAD_INFO_VTID0_V                    0
#define EC_PC_PMC_THREAD_INFO_VTID1_V                    1
#define EC_PC_PMC_THREAD_INFO_VTID2_V                    2
#define EC_PC_PMC_THREAD_INFO_VTID3_V                    3
#define EC_PC_PMC_THREAD_INFO_PTID0_V                    4
#define EC_PC_PMC_THREAD_INFO_PTID1_V                    5
#define EC_PC_PMC_THREAD_INFO_PTID2_V                    6
#define EC_PC_PMC_THREAD_INFO_PTID3_V                    7
#define EC_PC_PMC_THREAD_INFO_SMT_MODE                   8
#define EC_PC_PMC_THREAD_INFO_VTID0_TO_PTID_MAP         10
#define EC_PC_PMC_THREAD_INFO_VTID1_TO_PTID_MAP         12
#define EC_PC_PMC_THREAD_INFO_VTID2_TO_PTID_MAP         14
#define EC_PC_PMC_THREAD_INFO_VTID3_TO_PTID_MAP         16
#define EC_PC_PMC_THREAD_INFO_RAM_THREAD_ACTIVE         18
#define EC_PC_PMC_THREAD_INFO_PSCOM_PURGE               22
#define EC_PC_PMC_THREAD_INFO_THREAD_ACTION_IN_PROGRESS 23

#define EC_PC_FIR_RAM_CTRL_RAM_VTID                     0
#define EC_PC_FIR_RAM_CTRL_RAM_VTID_LEN                 2
#define EC_PC_FIR_RAM_CTRL_PPC_PREDCD                   2
#define EC_PC_FIR_RAM_CTRL_PPC_PREDCD_LEN               4
#define EC_PC_FIR_RAM_CTRL_PPC_INSTR                    8
#define EC_PC_FIR_RAM_CTRL_PPC_INSTR_LEN               32

#define EC_PC_FIR_RAM_STATUS_RAM_CONTROL_ACCESS_DURING_RECOV   0
#define EC_PC_FIR_RAM_STATUS_RAM_COMPLETION                    1
#define EC_PC_FIR_RAM_STATUS_RAM_EXCEPTION                     2
#define EC_PC_FIR_RAM_STATUS_LSU_EMPTY                         3

// register SPR_COMMON.SCOMC , definition for field MODE_CX_SCOMC
// where field value = SCRATCH xxx SPR
#define EC_PC_COMMON_SPR_SCOMC_MODE_CX_SCR0 0

// SCOM register bit field spanning multiple bits LT0 to LT7
#define EC_PC_COMMON_SPR_MODE_SPRC_LTX_SEL_WIDTH 8
#define EC_PC_COMMON_SPR_MODE_SPRC_LTX_SEL_MASK  0xFF

#endif //_P10_RAM_CORE_DEFINES_H_
