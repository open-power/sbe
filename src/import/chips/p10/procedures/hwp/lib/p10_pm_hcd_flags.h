/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/lib/p10_pm_hcd_flags.h $  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2023                        */
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
///
/// @file  p10_pm_hcode_flags.h
/// @brief Hocde defined flag register structures
///
// *HWP HWP Owner           : David Du <daviddu@us.ibm.com>
// *HWP Backup HWP Owner    : Greg Still <stillgs@us.ibm.com>
// *HWP FW Owner            : Prasad BG  <prasadbgr@in.ibm.com>
// *HWP Team: PM
// *HWP Level: 1
// *HWP Consumed by: FSP:HS:HCD
///
///-----------------------------------------------------------------------------

#ifndef _P10_PM_HCDFLAGS_H_
#define _P10_PM_HCDFLAGS_H_

#ifndef __PPE_PLAT
namespace p10hcd
{
#endif

//Enum form of OCC FLAGs.
enum PM_GPE_OCCFLG0_DEFS
{
    PIB_I2C_MASTER_ENGINE_1_LOCK_BIT0       = 16, //BIT0 ored BIT1 gives the field
    PIB_I2C_MASTER_ENGINE_1_LOCK_BIT1       = 17, //BIT0 ored BIT1 gives the field
    PIB_I2C_MASTER_ENGINE_2_LOCK_BIT0       = 18, //BIT0 ored BIT1 gives the field
    PIB_I2C_MASTER_ENGINE_2_LOCK_BIT1       = 19, //BIT0 ored BIT1 gives the field
    PIB_I2C_MASTER_ENGINE_3_LOCK_BIT0       = 20, //BIT0 ored BIT1 gives the field
    PIB_I2C_MASTER_ENGINE_3_LOCK_BIT1       = 21, //BIT0 ored BIT1 gives the field
    GPU0_RESET_STATUS                       = 22,
    GPU1_RESET_STATUS                       = 23,
    GPU2_RESET_STATUS                       = 24,
    PGPE_SAMPLE_DIRTY                       = 25,
    PGPE_SAMPLE_DIRTY_TYPE                  = 26,
    PGPE_PM_RESET_SUPPRESS                  = 27,
    REQUEST_OCC_SAFE_STATE                  = 31
};


//Enum form of OCC FLAGs.
enum PM_GPE_OCCFLG2_DEFS
{
    PGPE_PSTATE_PROTOCOL_STOP               = 0,
    PGPE_PSTATE_PROTOCOL_AUTO_ACTIVATE      = 1,
    PGPE_SAFE_MODE                          = 2,
    PGPE_XSTOP_ON_SAFE_MODE                 = 3,
    PGPE_DEBUG_TRAP_ENABLE                  = 4,
    PGPE_DEBUG_HALT_ENABLE                  = 5,
    PGPE_HCODE_ERROR_INJECT                 = 6,
    PGPE_HCODE_ERROR_INJECT_LEN         = 2,
    PGPE_OP_TRACE_DISABLE                   = 8,
    PGPE_OP_TRACE_MEM_MODE                  = 9,
    PGPE_OP_TRACE_MEM_MODE_LEN          = 2,
    PGPE_HW_ERROR_INJECT                    = 11,
    PGPE_AVS_PROFILE_ENABLE                 = 12,
    PGPE_PROBE_ENABLE                       = 13,
    PGPE_ACTIVE                             = 16,
    PGPE_PSTATE_PROTOCOL_ACTIVE             = 17,
    PGPE_SAFE_MODE_ACTIVE                   = 18,
    PGPE_HCODE_FAULT_STATE                  = 19,
    PGPE_SAFE_MODE_ERROR                    = 20,
    PGPE_CEFFOVR_CONTROL_LOOP               = 22,
    PM_CALLOUT_ACTIVE                       = 31
};

//Enum form of OCC FLAGs.
enum PM_GPE_OCCFLG3_DEFS
{
    XGPE_IODLR_ENABLE                       = 0,
    AUX_THREAD_ACTIVATE                     = 1,
    XGPE_HW_ERROR_INJECT                    = 2,
    XGPE_PM_COMPLEX_SUSPEND                 = 3,
    PM_COMPLEX_SUSPEND                      = 3,  // Leaving a generic for use in p10_pm_suspend.
    XGPE_DEBUG_TRAP_ENABLE                  = 4,
    XGPE_DEBUG_HALT_ENABLE                  = 5,
    XGPE_HCODE_ERROR_INJECT                 = 6,
    XGPE_HCODE_ERROR_INJECT_LEN         = 2,
    XGPE_OP_TRACE_DISABLE                   = 8,
    XGPE_OP_TRACE_MEM_MODE                  = 9,
    XGPE_OP_TRACE_MEM_MODE_LEN          = 2,
    XGPE_IGNORE_STOP_CONTROL                = 11,
    XGPE_IGNORE_STOP_ACTION                 = 12,
    XGPE_IGNORE_STOP_EXITS                  = 13,
    XGPE_IGNORE_STOP_ENTRIES                = 14,
    XGPE_IGNORE_STOP_LEN                = 4,
    XGPE_ACTIVE                             = 16,
    XGPE_IODLR_ACTIVE                       = 17,
    AUX_THREAD_ACTIVE                       = 18,
    XGPE_PM_COMPLEX_SUSPENDED               = 19,
    PM_COMPLEX_SUSPENDED                    = 19,  // Leaving a generic for use in p10_pm_suspend.
    XGPE_ZERO_CORE_CHIP                     = 20,
    XGPE_HCODE_FAULT_STATE                  = 21,
    STOP_RECOVERY_TRIGGER_ENABLE            = 26,
    CORE_THROT_INJECT_RESP                  = 27,
    CORE_THROT_INJECT_RESP_LEN          = 2,
    CORE_THROT_CONTIN_CHANGE_ENABLE         = 29,
    CORE_THROT_SINGLE_EVENT_INJECT          = 30,
    FORCE_LEGACY_THROTTLE                   = 31,
};

//Enum form of OCC FLAG2.
enum PM_GPE_OCCFLG7_DEFS
{
    OCCFLG7_DEAD_CORES_START                = 0,
    OCCFLG7_DEAD_CORES_LEN              = 32,
};


//
//Enum form of QME_FLAGS
//
enum PM_QME_FLAG_DEFS
{

    QME_FLAGS_CORE_WKUP_ERR_INJECT          = 0,
    QME_FLAGS_FATAL_FAULT_ERR_INJECT        = 1,
    QME_FLAGS_STOP11_ENTRY_REQUESTED        = 2,
    QME_FLAGS_HALT_ON_CHECKSTOP             = 3,
    QME_FLAGS_DEBUG_TRAP_ENABLE             = 4,
    QME_FLAGS_DEBUG_HALT_ENABLE             = 5,
    QME_FLAGS_RVRM_ENABLE                   = 6,
    QME_FLAGS_TOD_SETUP_COMPLETE            = 8,
    QME_FLAGS_SMF_DISABLE_MODE              = 9,
    QME_FLAGS_RUNTIME_WAKEUP_MODE           = 10,
    QME_FLAGS_SPWU_CHECK_ENABLE             = 11,
    QME_FLAGS_RUNN_MODE                     = 13,
    QME_FLAGS_CONTAINED_MODE                = 14,
    QME_FLAGS_STOP_READY                    = 16,
    QME_FLAGS_PMCR_READY                    = 17,
    QME_FLAGS_PGPE_HB_LOSS_SAFE_MODE        = 18,
    QME_FLAGS_RCLK_OPERABLE                 = 19,
    QME_FLAGS_DDS_OPERABLE                  = 20,
    QME_FLAGS_SRAM_SBE_MODE                 = 21,
    QME_FLAGS_SRAM_GPE_MODE                 = 22,
    QME_FLAGS_RUNTIME_MODE                  = 23,
    QME_FLAGS_QUIESCE_MODE                  = 24,
    QME_FLAGS_RUNNING_EPM                   = 62,
    QME_FLAGS_RUNNING_SIMICS                = 63
};

//
//Enum form of QME_SCRATCH_1
//
enum PM_QME_SCRA_DEFS
{
    QME_SCRA_STOP_CALLOUT_VECTOR_BASE      = 0,
    QME_SCRA_STOP_CALLOUT_VECTOR_SIZE      = 4  // 0,1,2,3
};

enum PM_QME_SCRB_DEFS
{
    QME_SCRB_STOP_BLOCK_EXIT_VECTOR_BASE   = 0,
    QME_SCRB_STOP_BLOCK_EXIT_VECTOR_SIZE   = 4, // 0,1,2,3
    QME_SCRB_STOP_BLOCK_ENTRY_VECTOR_BASE  = 4,
    QME_SCRB_STOP_BLOCK_ENTRY_VECTOR_SIZE  = 4, // 4,5,6,7
    QME_SCRB_STOP_EXIT_BLOCKED_VECTOR_BASE = 8,
    QME_SCRB_STOP_EXIT_BLOCKED_VECTOR_SIZE = 4, // 8,9,10,11
    QME_SCRB_STOP_ENTRY_BLOCKED_VECTOR_BASE = 12,
    QME_SCRB_STOP_ENTRY_BLOCKED_VECTOR_SIZE = 4, // 12,13,14,15
    QME_SCRB_STOP2_TO_STOP0                = 16,
    QME_SCRB_STOP3_TO_STOP2                = 17,
    QME_SCRB_STOP11_TO_STOP5               = 18,
    QME_SCRB_USE_LPAR_ATTRIBUTE            = 19,
    QME_SCRB_CORE_LPAR_MODE_VECTOR_BASE    = 20, // 20,21,22,23
    QME_SCRB_CORE_LPAR_MODE_VECTOR_SIZE    = 4,
    QME_SCRB_STOP11_ENTRY_REQUESTED_BASE   = 24, // 24,25,26,27
    QME_SCRB_STOP11_ENTRY_REQUESTED_SIZE   = 4,
    QME_SCRB_CACHE_ONLY_MODE_VECTOR_BASE   = 28, // 28,29,30,31
    QME_SCRB_CACHE_ONLY_MODE_VECTOR_SIZE   = 4
};

enum PM_XGPE_FLAGS
{
    XGPE_IODLR_ENABLE_FLAG               = 0x8000,
    XGPE_OCC_PM_SUSPEND_IMMEDIATE_MODE   = 0x4000,
    XGPE_PM_SUSPEND_MODE                 = 0x2000,
};
//
//Enum form of per core QME_PCSCR
//
enum PM_QME_PCSCR_DEFS
{
    QME_PCSCR_CRIT_ELOG_OVERFLOW            = 1,
    QME_PCSCR_STOP23_ENTRY_FAULT_INJECT     = 2,
    QME_PCSCR_STOP11_ENTRY_FAULT_INJECT     = 3,
    QME_PCSCR_STOP2_EXIT_FAULT_INJECT       = 4,
    QME_PCSCR_STOP3_RVRM_POWON_FAULT_INJECT = 5,
    QME_PCSCR_STOP11_POWON_FAULT_INJECT     = 6,
    QME_PCSCR_SELF_RESTORE_FAULT_INJECT     = 7
// backup bits in case we need to enable them
//    QME_PCSCR_BAD_SCOM_FAULT_INJECT         = 8,
//    QME_PCSCR_BAD_LOCAL_FAULT_INJECT        = 9,
};

#ifndef __PPE_PLAT
} //End p10hcd namespace
#endif

#endif // _P10_PM_HCDFLAGS_H_
