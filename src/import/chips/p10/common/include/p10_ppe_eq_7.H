/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_7.H $          */
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

#ifndef __p10_ppe_eq_7_H_
#define __p10_ppe_eq_7_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_ERR = 0xc0000100ull;

static const uint32_t QME_ERR_PPE_INTERNAL_ERROR = 0;
static const uint32_t QME_ERR_PPE_EXTERNAL_ERROR = 1;
static const uint32_t QME_ERR_PPE_PROGRESS_ERROR = 2;
static const uint32_t QME_ERR_PPE_BREAKPOINT_ERROR = 3;
static const uint32_t QME_ERR_RESCLK_FREEZE = 4;
static const uint32_t QME_ERR_RESCLK_FREEZE_LEN = 4;
static const uint32_t QME_ERR_SPECIAL_WKUP_PROTOCOL_ERR = 8;
static const uint32_t QME_ERR_SPECIAL_WKUP_PROTOCOL_ERR_LEN = 4;
static const uint32_t QME_ERR_SPECIAL_WKUP_DONE_WINDOW = 12;
static const uint32_t QME_ERR_SPECIAL_WKUP_DONE_WINDOW_LEN = 4;
static const uint32_t QME_ERR_DISABLED_INTR = 16;
static const uint32_t QME_ERR_DISABLED_INTR_LEN = 4;
static const uint32_t QME_ERR_DECONFIGURED_INTR = 20;
static const uint32_t QME_ERR_DECONFIGURED_INTR_LEN = 4;
static const uint32_t QME_ERR_CTFS_ERR = 24;
static const uint32_t QME_ERR_CTFS_ERR_LEN = 4;
static const uint32_t QME_ERR_CPMS_ERR = 28;
static const uint32_t QME_ERR_CPMS_ERR_LEN = 4;
static const uint32_t QME_ERR_PCB_ENDPOINT_RESET = 32;
static const uint32_t QME_ERR_PCB_NETWORK_RESET = 33;
static const uint32_t QME_ERR_PIG_PENDING_SOURCE_LOST = 34;
static const uint32_t QME_ERR_PIG_PENDING_SOURCE_LOST_LEN = 6;
static const uint32_t QME_ERR_SPARE = 40;
static const uint32_t QME_ERR_SPARE_LEN = 4;
static const uint32_t QME_ERR_LOCAL_ACCESS_PROTECT_ERR = 44;
static const uint32_t QME_ERR_LOCAL_ACCESS_REGION_ERR = 45;
static const uint32_t QME_ERR_LOCAL_ACCESS_PARTIAL_ERR = 46;
// ppe_eq/reg00007.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QHTTRIG = 0xc0001b00ull;

static const uint32_t QME_QHTTRIG_START = 0;
static const uint32_t QME_QHTTRIG_STOP = 1;
static const uint32_t QME_QHTTRIG_PAUSE = 2;
// ppe_eq/reg00007.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_QMCR = 0xc0000880ull;
static const uint32_t QME_QMCR_PPE2 = 0xc00008a0ull;
static const uint32_t QME_QMCR_WO_CLEAR = 0xc00008b0ull;

static const uint32_t QME_QMCR_PMCR_OVERRIDE_EN = 0;
static const uint32_t QME_QMCR_PSCR_OVERRIDE_EN = 1;
static const uint32_t QME_QMCR_CHAR_REGS_READ_ENABLE = 2;
static const uint32_t QME_QMCR_BCECSR_OVERRIDE_EN = 3;
static const uint32_t QME_QMCR_PSREQ_SCOM_READ_EN = 4;
static const uint32_t QME_QMCR_LOCK_QUEUE_ON_PCB_ERR = 5;
static const uint32_t QME_QMCR_STOP_OVERRIDE_MODE = 6;
static const uint32_t QME_QMCR_STOP_ACTIVE_MASK = 7;
static const uint32_t QME_QMCR_AUTO_PMCR_UPDATE = 8;
static const uint32_t QME_QMCR_PMCR_RECENT_MODE = 9;
static const uint32_t QME_QMCR_FUSED_CORE_PAIR_MODE = 10;
static const uint32_t QME_QMCR_CYCLE_REPRO_MODE = 11;
static const uint32_t QME_QMCR_QUEUED_WR_EN = 12;
static const uint32_t QME_QMCR_QUEUED_RD_EN = 13;
static const uint32_t QME_QMCR_RESET_IMPRECISE_QERR = 14;
static const uint32_t QME_QMCR_RESET_PB_RAMP = 15;
static const uint32_t QME_QMCR_LOPRI_STOP_WKUP_SEL = 16;
static const uint32_t QME_QMCR_LOPRI_STOP_WKUP_SEL_LEN = 4;
static const uint32_t QME_QMCR_LOPRI_STOP_ENTRY_SEL = 20;
static const uint32_t QME_QMCR_LOPRI_STOP_ENTRY_SEL_LEN = 4;
static const uint32_t QME_QMCR_WRITE_PROTECT_ENABLE = 24;
static const uint32_t QME_QMCR_TTSR_READ_ENABLE = 25;
static const uint32_t QME_QMCR_PTSR_READ_ENABLE = 26;
static const uint32_t QME_QMCR_RESET_QME_PCB_MASTER_INTERFACE = 27;
static const uint32_t QME_QMCR_OCP_REQUEST_ENABLE = 28;
static const uint32_t QME_QMCR_STOP_SHIFTREG_OVERRIDE_EN = 29;
static const uint32_t QME_QMCR_CPMS_PG_CLKOFF_DISABLE = 30;
static const uint32_t QME_QMCR_CTFS_PG_CLKOFF_DISABLE = 31;
static const uint32_t QME_QMCR_DTC_SEQUENCER_ENABLE = 32;
static const uint32_t QME_QMCR_WOF_AUTO_SEQ_ENABLE = 33;
static const uint32_t QME_QMCR_QOS_AUTO_SEQ_ENABLE = 34;
static const uint32_t QME_QMCR_QOSR_SPR_UPDATE_ENABLE = 35;
static const uint32_t QME_QMCR_SPARE2 = 36;
static const uint32_t QME_QMCR_SPARE2_LEN = 4;
static const uint32_t QME_QMCR_FUSED_CORE_MODE = 47;
static const uint32_t QME_QMCR_CORE_PARTIAL_GOOD = 60;
static const uint32_t QME_QMCR_CORE_PARTIAL_GOOD_LEN = 4;
// ppe_eq/reg00007.H

//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_SWPR1 = 0xc0000940ull;

static const uint32_t QME_SWPR1_BAR = 0;
static const uint32_t QME_SWPR1_BAR_LEN = 27;
static const uint32_t QME_SWPR1_SIZE = 44;
static const uint32_t QME_SWPR1_SIZE_LEN = 15;
// ppe_eq/reg00007.H

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00007.H"
#endif
#endif
