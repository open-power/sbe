/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbe_sp_intf.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2022                        */
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
/*
 *  @file sbe_sp_intf.H
 *
 *  @brief This file contains the SP - SBE interface protocol common details
 */
#ifndef __SBEFW_SBE_SP_INTF_H
#define __SBEFW_SBE_SP_INTF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * $Version: Conforms to SP-SBE Interface Spec v0.8e
 */

/*
 * Constants for SBE FW major and minor number
 */
static const uint16_t SBE_FW_MAJOR_VERSION = 5;
static const uint16_t SBE_FW_MINOR_VERSION = 2;

/**
 * @brief enums for SBE command classes
 *
*/
enum sbeCommandClass
{
    SBE_CMD_CLASS_UNKNOWN                = 0,
    SBE_CMD_CLASS_IPL_CONTROL            = 0xA1,
    SBE_CMD_CLASS_SCOM_ACCESS            = 0xA2,
    SBE_CMD_CLASS_RING_ACCESS            = 0xA3,
    SBE_CMD_CLASS_MEMORY_ACCESS          = 0xA4,
    SBE_CMD_CLASS_REGISTER_ACCESS        = 0xA5,
    SBE_CMD_CLASS_ARRAY_ACCESS           = 0xA6,
    SBE_CMD_CLASS_INSTRUCTION_CONTROL    = 0xA7,
    SBE_CMD_CLASS_GENERIC_MESSAGE        = 0xA8,
    SBE_CMD_CLASS_MPIPL_COMMANDS         = 0xA9,
    SBE_CMD_CLASS_DUMP_COMMANDS          = 0xAA,
    SBE_CMD_CLASS_HOST_FIFO_COMMANDS     = 0xAB,
    SBE_CMD_CLASS_PMIC_TELEMETRY         = 0xAC,
};

static const uint8_t SBE_CMD_UNKNOWN = 0x00;

/**
 * @brief enums for SBE Istep Control command class
 *
*/
enum sbeIplControlCommands
{
    SBE_CMD_EXECUTE_ISTEP      = 0x01, /* Execute istep */
    SBE_CMD_SUSPEND_IO         = 0x02, /* Suspend IO */
    SBE_CMD_FLUSH_NVDIMM       = 0x03, /* Flush NVDIMM */
};

/**
 * @brief enums for SCOM Access Messages
 *
*/
enum sbeScomAccessCommands
{
    SBE_CMD_GETSCOM          = 0x01, /* Get SCOM */
    SBE_CMD_PUTSCOM          = 0x02, /* Put SCOM */
    SBE_CMD_MODIFYSCOM       = 0x03, /* Modify SCOM */
    SBE_CMD_PUTSCOM_MASK     = 0x04, /* Put SCOM under mask */
    SBE_CMD_MULTISCOM        = 0x05, /* Execute Multi SCOM */
};

/**
 *  @brief enum for modify mode
 *
*/
enum sbeChipOpModifyMode
{
    SBE_MODIFY_MODE_OR      = 0x01,
    SBE_MODIFY_MODE_AND     = 0x02,
    SBE_MODIFY_MODE_XOR     = 0x03,
};

/**
 * @brief enums for Ring Access Messages
 *
*/
enum sbeRingAccessCommands
{
    SBE_CMD_GETRING          = 0x01, /* Get Ring */
    SBE_CMD_PUTRING          = 0x02, /* Put Ring */
};

/**
 * @brief enums for Memory Access Messages
 *
*/
enum sbeMemoryAccesCommands
{
    SBE_CMD_GETMEM           = 0x01, /* Get Memory (Proc/ADU) */
    SBE_CMD_PUTMEM           = 0x02, /* Put Memory (Proc/PBA) */
    SBE_CMD_GETSRAM          = 0x03, /* Get Memory (SRAM-IOPPE,OCC,QME) TODO:RTC:252306 Enable PCI XRAM access */
    SBE_CMD_PUTSRAM          = 0x04, /* Put Memory (SRAM-IOPPE,OCC,QME) TODO:RTC:252306 Enable PCI XRAM access */
};

/**
 * @brief enums for GPR/SPR/FPR Access Messages
 *
*/
enum sbeRegisterAccessCommands
{
    SBE_CMD_GETREG           = 0x01, /* Get Register (GPR,SPR,FPR) */
    SBE_CMD_PUTREG           = 0x02, /* Put Register (GPR,SPR,FPR) */
    SBE_CMD_GETHWREG         = 0x03, /* Get HW Register (Scoms, OCMB Scoms) */
    SBE_CMD_PUTHWREG         = 0x04, /* Put HW Register (Scoms, OCMB Scoms) */
};

/**
 * @brief enums for Trace Array Access Messages
 *
*/
enum sbeArrayAccessCommands
{
    SBE_CMD_CONTROL_FAST_ARRAY    = 0x01, /* Control Fast Array */
    SBE_CMD_CONTROL_TRACE_ARRAY   = 0x02, /* Control Trace Array */
};

/**
 * @brief enums for Instruction Control Messages
 *
*/
enum sbeInstructionControlCommands
{
    SBE_CMD_CONTROL_INSTRUCTIONS  = 0x01, /* Control Instructions */
};

/**
 * @brief enums for Generic Messages
 *
*/
enum sbeGenericMessageCommands
{
    SBE_CMD_GET_SBE_FFDC            = 0x01, /* Get FFDC */
    SBE_CMD_GET_SBE_CAPABILITIES    = 0x02, /* GET SBE capabilities */
    SBE_CMD_GET_FREQ_SUPPORTED      = 0x03, /* Get Supported frequencies */
    SBE_CMD_QUIESCE                 = 0x06, /* Sbe Quiesce */
    SBE_CMD_GET_SBE_CAPABILITIES_2  = 0x07, /* GET SBE capabilities including new additions */
    SBE_CMD_GET_LPC_STATUS          = 0x08, /* GET LPC Alive Status */
};

enum sbeMpIplCommands
{
    SBE_CMD_MPIPL_ENTER      = 0x01, /* Enter MPIPL */
    SBE_CMD_MPIPL_CONTINUE   = 0x02, /* Continue MPIPL */
    SBE_CMD_MPIPL_STOPCLOCKS = 0x03, /* Stop Clocks */
    SBE_CMD_GET_TI_INFO      = 0x04, /* Get TI Info */
};


/**
 * @brief enums for SBE Dump.
 *
*/
enum sbeDumpCommands
{
    SBE_CMD_GET_DUMP = 0x01, /* Get Dump */
};

/**
 * @brief enums for Generic Host Commands
 *
*/
enum sbeHostFifoGenericCommands
{
    SBE_CMD_HALT = 0x01, /* Halt request from Host Fifo */
};

/**
 * @brief enums for PMIC telemetry Commands
 *
*/
enum sbePmicTelemetryCommands
{
    SBE_CMD_PMIC_HEALTH_CHECK     = 0x01, /* Analyse and return PMIC health status */
};

/**
 * @brief enum for sbe supported dump types
 *
*/
enum sbeSupportedDumpTypes
{
    SBE_DUMP_TYPE_SCS   = 0x01, /* System Check Stop */
    SBE_DUMP_TYPE_MPIPL = 0x02, /* Memory Preserving IPl */
    SBE_DUMP_TYPE_PERF  = 0x03, /* Performance */
    SBE_DUMP_TYPE_CCS   = 0x04, /* Core Check Stop */
    SBE_DUMP_TYPE_HB    = 0x05, /* Host Boot */
};

/**
 * @brief enum for sbe Dump clock state's
 *
*/
enum sbeDumpClockStates
{
    SBE_DUMP_CLOCK_ON   = 0x01, /* Clock's ON  */
    SBE_DUMP_CLOCK_OFF  = 0x02, /* Clock's OFF */
};

/**
 * @brief enum for sbe dump fastarray collection info
 *
*/
enum sbeDumpFastArrayCollectionInfo
{
    SBE_DUMP_FASTARRY_COLLECTION_OFF   = 0x00, /* Fastarray collection ON  */
    SBE_DUMP_FASTARRY_COLLECTION_ON    = 0x01, /* Fastarray collection OFF */
};

/**
 * @brief enums for primary SBE response
 *
*/
enum sbePrimResponse
{
    SBE_PRI_OPERATION_SUCCESSFUL        = 0x00,
    SBE_PRI_INVALID_COMMAND             = 0x01,
    SBE_PRI_INVALID_DATA                = 0x02,
    SBE_PRI_USER_ERROR                  = 0x03,
    SBE_PRI_INTERNAL_ERROR              = 0x04,
    SBE_PRI_UNSECURE_ACCESS_DENIED      = 0x05,
    SBE_PRI_GENERIC_EXECUTION_FAILURE   = 0xFE,
};

/**
 * @brief enums for secondary SBE response
 *   @TODO via RTC: 129763
 *         Discuss on SBE_SEC_INVALID_TARGET_ID_PASSED
 *
*/
enum sbeSecondaryResponse
{
    SBE_SEC_OPERATION_SUCCESSFUL              = 0x00,
    SBE_SEC_COMMAND_CLASS_NOT_SUPPORTED       = 0x01,
    SBE_SEC_COMMAND_NOT_SUPPORTED             = 0x02,
    SBE_SEC_INVALID_ADDRESS_PASSED            = 0x03,
    SBE_SEC_INVALID_TARGET_TYPE_PASSED        = 0x04,
    SBE_SEC_INVALID_CHIPLET_ID_PASSED         = 0x05,
    SBE_SEC_SPECIFIED_TARGET_NOT_PRESENT      = 0x06,
    SBE_SEC_SPECIFIED_TARGET_NOT_FUNCTIONAL   = 0x07,
    SBE_SEC_COMMAND_NOT_ALLOWED_IN_THIS_STATE = 0x08,
    SBE_SEC_FUNCTIONALITY_NOT_SUPPORTED       = 0x09,
    SBE_SEC_GENERIC_FAILURE_IN_EXECUTION      = 0x0A,
    SBE_SEC_BLACKLISTED_REG_ACCESS            = 0x0B,
    SBE_SEC_OS_FAILURE                        = 0x0C,
    SBE_SEC_FIFO_ACCESS_FAILURE               = 0x0D,
    SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA  = 0x0E,
    SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA        = 0x0F,
    SBE_SEC_HW_OP_TIMEOUT                     = 0x10,
    SBE_SEC_PCB_PIB_ERR                       = 0x11,
    SBE_SEC_FIFO_PARITY_ERROR                 = 0x12,
    SBE_SEC_TIMER_ALREADY_STARTED             = 0x13,
    SBE_SEC_BLACKLISTED_MEM_ACCESS            = 0x14,
    SBE_SEC_MEM_REGION_NOT_FOUND              = 0x15,
    SBE_SEC_MAXIMUM_MEM_REGION_EXCEEDED       = 0x16,
    SBE_SEC_MEM_REGION_AMEND_ATTEMPTED        = 0x17,
    SBE_SEC_INPUT_BUFFER_OVERFLOW             = 0x18,
    SBE_SEC_INVALID_PARAMS                    = 0x19,
    SBE_SEC_BLACKLISTED_CHIPOP_ACCESS         = 0x20,
    SBE_SEC_DMT_TIMEOUT                       = 0x21,
    SBE_SEC_SYSTEM_CHECKSTOP                  = 0x22,
    SBE_SEC_BLACKLISTED_REG_ACCESS_BLOCKED    = 0x23,
    SBE_SEC_S0_START_MPIPL_FAILED             = 0x24,
    SBE_SEC_S0_STOP_CLOCK_FAILED              = 0x25,
    SBE_SEC_S0_CONTINUE_MPIPL_FAILED          = 0x26,
    SBE_SEC_PERIODIC_IO_TOGGLE_FAILED         = 0x27,
    SBE_SEC_SPECIAL_WAKEUP_TIMEOUT            = 0x28,
    SBE_SEC_SPECIAL_WAKEUP_SCOM_FAILURE       = 0x29,
    SBE_SEC_S0_ARCH_REG_DUMP_FAILED           = 0x2A,
    SBE_SEC_LPC_ACCESS_FAILED                 = 0x2B,
    SBE_SEC_HWP_FAILURE                       = 0x2C,
    SBE_SEC_TI_CORE_SCRATCH_READ_FAILED       = 0x2D,
    SBE_SEC_TI_DATA_READ_FAILED               = 0x2E,
    SBE_SEC_PUT_SRAM_FAILED                   = 0x2F,
    SBE_SEC_GET_SRAM_FAILED                   = 0x30,
    SBE_SEC_THREAD_CONTROL_INSTR_FAILED       = 0x31,
    SBE_SEC_RAM_CORE_SETUP_FAILED             = 0x32,
    SBE_SEC_RAM_CORE_ACCESS_FAILED            = 0x33,
    SBE_SEC_RAM_CORE_CLEANUP_FAILED           = 0x34,
    SBE_SEC_SUSPEND_IO_FAILED                 = 0x35,
    SBE_SEC_ENTER_MPIPL_FAILED                = 0X36,
    SBE_SEC_STOP_CLOCK_FAILED                 = 0x37,
    SBE_SEC_OCMB_SCOM_FAILED                  = 0x38,
    SBE_SEC_GET_DUMP_FAILED                   = 0x39,
    SBE_SEC_INVALID_DUMP_TYPE                 = 0x40,
    SBE_SEC_INVALID_OCMB_INSTANCE             = 0x41,
    SBE_SEC_FAILED_TO_CHECKSTOP               = 0x42,
    SBE_SEC_GET_DUMP_STREAM_FAILED            = 0x43,
    SBE_SEC_MPIPL_DUMP_INVALID_PARAMS         = 0x44,
    SBE_SEC_OCMB_TARGET_NOT_PRESENT           = 0x45,
    SBE_SEC_OCMB_TARGET_NOT_FUNCTIONAL        = 0x46,
    SBE_SEC_INVALID_FASTARRAY_COLLECTION_INFO = 0x47,
    SBE_SEC_PMIC_HEALTH_CHECK_FAILED          = 0x48,
    SBE_SEC_INVALID_I2C_CONFIG_VERSION        = 0x49,
    SBE_SEC_INVALID_I2C_TARGET_SUPPORT        = 0x4A,
    SBE_SEC_LPC_ERROR                         = 0x4B,
    SBE_SEC_NOT_ALLOWED_VIA_FIFO_1            = 0x4C,
    SBE_SEC_HOST_ALIVE_TIMER_TIMEOUT          = 0x4D,
};

/**
 * @brief enums for SBE command timeout values
 *
*/
enum sbeCmdRespTimeout
{
    SBE_CMD_TIMEOUT_SHORT_IN_MSEC = 100,
    SBE_CMD_TIMEOUT_LONG_IN_MSEC  = 30000,
};

/**
  * @brief capabilities index values.
  *        Get Capability response will return 18 capabilities. This
  *        enum tells the index for each capability. Currently each generic
  *        functionality( scom, IPL ) etc span across two capabilities.
  */
enum
{
    GENERIC_CAPABILTITY_START_IDX = 0,
    IPL_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 2,
    SCOM_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 4,
    RING_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 6,
    MEMORY_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 8,
    REGISTER_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 10,
    ARRAY_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 12,
    INSTRUCTION_CTRL_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 14,
    GENERIC_CHIPOP_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 16,
    MPIPL_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 18,
    DUMP_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 20,
    HOST_FIFO_CAPABILITY_START_IDX = GENERIC_CAPABILTITY_START_IDX + 22,

    // Keep in sync with the spec
    CAPABILITIES_LAST_INDEX_FIFO = 23,

    // Capabilities which will go into GET_SBE_CAPABILITIES_2
    GENERIC_CAPABILTITY2_START_IDX = 24,

    PMIC_TELEMETRY_CAPABILITY_START_IDX = GENERIC_CAPABILTITY2_START_IDX + 0,

    // Keep in sync with the spec
    CAPABILITIES_2_LAST_INDEX_FIFO = 25,
};
/**
  * @brief capabilities enum values.
  *
  */
enum
{
    HWP_FFDC_COLLECTION_SUPPPORTED      = 0xC0000001,
    SBE_FFDC_COLLECTION_SUPPPORTED      = 0xC0000002,
    ADDRESS_BLACKLISTING_SUPPPORTED     = 0xC0000004,
    FIFO_RESET_SUPPPORTED               = 0xC0000008,
    HOST_CMD_INTERFACE_SUPPORTED        = 0xC0000010,
    SP_LESS_MPIPL_SUPPORTED             = 0xC0000020,
    RESERVED_GENERIC_CAPABILITIES       = 0xC8000000,

    EXECUTE_ISTEP_SUPPPORTED            = 0xA1000001,
    SUSPEND_IO_SUPPPORTED               = 0xA1000002,
    FLUSH_NVDIMM_SUPPPORTED             = 0xA1000004,
    RESERVED_A1_CAPABILITIES            = 0xA1800000,

    GET_SCOM_SUPPPORTED                 = 0xA2000001,
    PUT_SCOM_SUPPPORTED                 = 0xA2000002,
    MODIFY_SCOM_SUPPPORTED              = 0xA2000004,
    PUT_SCOM_UNDER_MASK_SUPPPORTED      = 0xA2000008,
    MULTI_SCOM_SUPPPORTED               = 0xA2000010,
    RESERVED_A2_CAPABILITIES            = 0xA2800000,

    GET_RING_SUPPPORTED                 = 0xA3000001,
    PUT_RING_SUPPPORTED                 = 0xA3000002,
    RESERVED_A3_CAPABILITIES            = 0xA3800000,

    GET_MEMORY_SUPPPORTED               = 0xA4000001,
    PUT_MEMORY_SUPPPORTED               = 0xA4000002,
    GET_SRAM_SUPPPORTED                 = 0xA4000004,
    PUT_SRAM_SUPPPORTED                 = 0xA4000008,
    RESERVED_A4_CAPABILITIES            = 0xA4800000,

    GET_REGISTER_SUPPPORTED             = 0xA5000001,
    PUT_REGISTER_SUPPPORTED             = 0xA5000002,
    GETHWREG_SUPPPORTED                 = 0xA5000004,
    PUTHWREG_SUPPPORTED                 = 0xA5000008,
    RESERVED_A5_CAPABILITIES            = 0xA5800000,

    CONTROL_FAST_ARRAY_SUPPPORTED       = 0xA6000001,
    CONTROL_TRACE_ARRAY_SUPPPORTED      = 0xA6000002,
    RESERVED_A6_CAPABILITIES            = 0xA6800000,

    CONTROL_INSTRUCTIONS_SUPPPORTED     = 0xA7000001,
    RESERVED_A7_CAPABILITIES            = 0xA7800000,

    GET_SBE_FFDC_SUPPPORTED             = 0xA8000001,
    GET_SBE_CAPABILITIES_SUPPORTED      = 0xA8000002,
    SBE_QUIESCE_SUPPPORTED              = 0xA8000004,
    GET_SBE_CAPABILITIES_2_SUPPORTED    = 0xA8000008,
    GET_SBE_LPC_STATUS_SUPPORTED        = 0xA8000010,
    RESERVED_A8_CAPABILITIES            = 0xA8800000,

    ENTER_MPIPL_SUPPORTED               = 0xA9000001,
    CONTINUE_MPIPL_SUPPORTED            = 0xA9000002,
    STOP_CLOCKS_MPIPL_SUPPORTED         = 0xA9000004,
    GET_TI_INFO_SUPPORTED               = 0xA9000008,
    RESERVED_A9_CAPABILITIES            = 0xA9800000,

    GET_DUMP_SUPPORTED                  = 0xAA000001,
    RESERVED_AA_CAPABILITIES            = 0xAA800000,

    SBE_CMD_HALT_SUPPORTED              = 0xAB000001,
    RESERVED_AB_CAPABILITIES            = 0xAB800000,

    SBE_CMD_PMIC_HEALTH_CHECK_SUPPORTED = 0xAC000001,
    RESERVED_AC_CAPABILITIES            = 0xAC800000,
};

/**
 *  * @brief enums for SBE Memory Access command flags
 *
 */
enum sbeMemoryAccessFlags
{
    SBE_MEM_ACCESS_FLAGS_TARGET_PROC        = 0x00000001, //required in ADU
    SBE_MEM_ACCESS_FLAGS_TARGET_PBA         = 0x00000002, //required in PBA
    SBE_MEM_ACCESS_FLAGS_AUTO_INCR_ON       = 0x00000004,
    SBE_MEM_ACCESS_FLAGS_ECC_REQUIRED       = 0x00000008, //required only in ADU-GET
    SBE_MEM_ACCESS_FLAGS_ECC_OVERRIDE       = 0x00000008, //required only in ADU-PUT
    SBE_MEM_ACCESS_FLAGS_ITAG               = 0x00000010, //ITAG Mode in ADU
    SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON       = 0x00000020,
    SBE_MEM_ACCESS_FLAGS_LCO_ENABLED        = 0x00000040, //required only in PBA-PUT
    SBE_MEM_ACCESS_FLAGS_CACHE_INHIBIT      = 0x00000080, //required in I/O oper ADU
    SBE_MEM_ACCESS_FLAGS_HOST_PASS_THROUGH  = 0x00000100, // Host pass through mode (PBA)
    SBE_MEM_ACCESS_FLAGS_INJECT_ON          = 0x00000200, // Inject mode ( PBA put )
    SBE_MEM_ACCESS_FLAGS_PB_DIS             = 0x00000400,
    SBE_MEM_ACCESS_FLAGS_SWITCH             = 0x00000800,
    SBE_MEM_ACCESS_FLAGS_PB_INIT            = 0x00001000,
    SBE_MEM_ACCESS_FLAGS_PRESWITCH_CD       = 0x00002000,
    SBE_MEM_ACCESS_FLAGS_PRESWITCH_AB       = 0x00004000,
    SBE_MEM_ACCESS_FLAGS_POSTSWITCH         = 0x00008000,
};

/*
 * Constants for maximum number of register supported in reg access chipop.
 */
static const uint32_t SBE_MAX_REG_ACCESS_REGS = 64;

/**
 * @brief Error Mode enum - bitmapped fields
 */
enum sbeErrorMode
{
    EXIT_ON_FIRST_ERROR = 0x00,   // Bail out on first error
    IGNORE_HW_ERRORS    = 0x01,   // Attempt best case
};

/**
 * @brief Core Id Enum
 *   User needs to define the coreIDs as input in the sbeCntlInstMsgHdr
 *   structure, similar modification would be there for PBA/ADU, GET/PUT Reg,
 *   Stop clocks etc. There are no Pervasive chiplet Id for core, just coreId,
 *   SBE will generate the right core target basis the CoreId.
 */
enum sbeCoreId
{
    SMT4_CORE0_ID       = 0x00,
    SMT4_CORE_ID_LAST   = 0x1F,
    SMT4_ALL_CORES      = 0xFF,
};

/**
 * @brief Thread Num Enum
 */
enum sbeThreadNum
{
    SMT4_THREAD0     = 0x0,
    SMT4_THREAD1     = 0x1,
    SMT4_THREAD2     = 0x2,
    SMT4_THREAD3     = 0x3,
    SMT4_THREAD_MAX  = 0x4,
    SMT4_THREAD_ALL  = 0xF,
};

/**
 * @brief Thread Operation Enum
 */
enum sbeThreadOps
{
    THREAD_START_INS       = 0x0,
    THREAD_STOP_INS        = 0x1,
    THREAD_STEP_INS        = 0x2,
    THREAD_SRESET_INS      = 0x3,
};


/**
 * @brief LPC Access Timeout Flag
 */
enum LPCAccessTimeoutFlag
{
    LPC_ACCESS_TIMEOUT_FALSE  = 0x0,
    LPC_ACCESS_TIMEOUT_TRUE = 0x1,
};

/**
 *  @brief enums for Reg access register type
 *
 */
enum sbeRegAccesRegType
{
    SBE_REG_ACCESS_GPR  = 0x00,
    SBE_REG_ACCESS_SPR  = 0x01,
    SBE_REG_ACCESS_FPR  = 0x02,
};


// Base Target Type offset maintined by SBE to be used across different chip-ops
// Need to avoid any duplicate target type since it is used to convert into FAPI
// target via file sbefapiutil.C

/*
 * @brief enums for target types used in ring access chip op, getHwReg, putHwReg
 * and stop clocks chip ops
*/
typedef enum
{
    TARGET_PROC_CHIP    = 0x0000,
    TARGET_PERV         = 0x0001,
    TARGET_EQ           = 0x0002,
    TARGET_CORE         = 0x0003,
    TARGET_OCMB_CHIP    = 0x0004,
    TARGET_CACHE        = 0x0005,
} sbeTargetTypes_t;

/**
 * @brief Pervasive Chiplet Id Enum
 */
enum sbePervChipletId
{
    PERV_CHIPLET = 0x01,
};

/**
 * @brief Nest Chiplet Id Enum
 */
enum sbeNestChipletId
{
    NEST_ID_0       = 0x02,
    NEST_ID_LAST    = 0x03,
};

/**
 * @brief PCI Chiplet Id Enum
 */
enum sbePciChipletId
{
    PCI_ID_0     = 0x08,
    PCI_ID_LAST  = 0x09,
};

/**
 * @brief MC Chiplet Id Enum
 */
enum sbeMcChipletId
{
    MC_ID_0     = 0x0C,
    MC_ID_LAST  = 0x0F,
};

/**
 * @brief PAU Chiplet Id Enum
 */
enum sbePauChipletId
{
    PAU_ID_0       = 0x10,
    PAU_ID_LAST    = 0x14,
};

/**
 * @brief IOHS Chiplet Id Enum
 */
enum sbeIohsChipletId
{
    IOHS_ID_0       = 0x18,
    IOHS_ID_LAST    = 0x20,
};

/**
 * @brief Cache Chiplet Id Enum
 */
enum sbeCacheChipletId
{
    EQ_ID_0          = 0x20,
    EQ_ID_LAST       = 0x28,
    EQ_ALL_CHIPLETS  = 0xFF,
};

/*
 * @brief maximum length for putring RS4 payload for chipop operation
*/
static const uint32_t SBE_PUT_RING_RS4_MAX_PAYLOAD_BYTES = 10240;
/*
 * @brief enums for access modes used in ring access chip op
 */
typedef enum
{
    SBE_RING_MODE_NO_HEADER_CHECK           = 0x0001, ///< Don't check header
    SBE_RING_MODE_SET_PULSE_NO_OPCG_COND    = 0x0002, ///< Set pulse with
                                                     ///  no OPCG conditioning
    SBE_RING_MODE_SET_PULSE_NSL             = 0x0004, ///< Set pulse with NSL
                                                    ///  pulse
    SBE_RING_MODE_SET_PULSE_SL              = 0x0008, ///< Set pulse with SL
                                                    ///  pulse
    SBE_RING_MODE_SET_PULSE_ALL             = 0x0010, ///< Set pulse with pulse
                                                    ///  to all hold types
    SBE_RING_MODE_FASTARRAY                 = 0x0020, //Fast array mode

    SBE_RING_MODE_APPLY_OVERRIDE            = 0x0040, //override mode
} sbeRingAccessModes_t;

// Trace array chip-op operation bitmaps
static const uint16_t SBE_TA_RESET              = 0x0001;
static const uint16_t SBE_TA_RESTART            = 0x0002;
static const uint16_t SBE_TA_STOP               = 0x0004;
static const uint16_t SBE_TA_COLLECT_DUMP       = 0x0008;
static const uint16_t SBE_TA_IGNORE_MUX_SETTING = 0x0010;

static const uint32_t SBE_FASTARRAY_MAX_CONTROL_DATA_BYTES = SBE_PUT_RING_RS4_MAX_PAYLOAD_BYTES;
static const uint32_t SBE_FASTARRAY_CUSTOM_CONTROL_SET = 0;  ///< indicates to use custom control data

/*
 * @brief enums for SBE client ID
 */
typedef enum
{
    CLIENT_DEFAULT      = 0x0,
    CLIENT_SP           = 0x1,
    CLIENT_HOST         = 0x2,
    CLIENT_LAB          = 0x3,
    CLIENT_OCC          = 0x4,
} sbeClientId_t;

//Supported MPIPL Metadata version
static const uint16_t SBE_MPIPL_METADATA_VERSION = 0x1;

#ifdef __cplusplus
}
#endif

#endif /* __SBEFW_SBE_SP_INTF_H */
