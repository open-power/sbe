/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbe_host_intf.H $                              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
 * @file: ppe/sbe/sbefw/sbe_host_intf.H
 *
 * @brief This file contains the Host-SBE interface protocol common details
 */

#ifndef __SBEFW_SBE_HOST_INTF_H
#define __SBEFW_SBE_HOST_INTF_H

#include <stdint.h>

/**
 * $Version: Conforms to HOST-SBE Interface Spec v0.9b
 */

/*
 * Constants for SBE FW major and minor number
 */
static const uint16_t SBE_SBEFW_HOST_MAJOR_VERSION = 1;
static const uint16_t SBE_SBEFW_HOST_MINOR_VERSION = 0;

/**
  * @brief enums for SBE-Host interface command class
  */
enum sbePsuCommandClass
 {
    SBE_PSU_CMD_CLASS_UNKNOWN               = 0,
    SBE_PSU_CMD_CLASS_CORE_STATE            = 0xD1,
    SBE_PSU_CMD_CLASS_RING_ACCESS           = 0xD3,
    SBE_PSU_CMD_CLASS_CNTRL_TIMER           = 0xD4,
    SBE_PSU_CMD_CLASS_REGISTER_ACCESS       = 0xD5,
    SBE_PSU_CMD_CLASS_SECURITY_CONTROL_MSG  = 0xD6,
    SBE_PSU_CMD_CLASS_GENERIC               = 0xD7,
 };

/**
  * @brief enums for SBE-Host interface core state control commands
  */
enum sbePsuCoreStateControlCommands
{
    SBE_PSU_CMD_CONTROL_DEADMAN       = 0x01,
    SBE_PSU_EXIT_CACHE_CONTAINED_MODE = 0x02,
    SBE_PSU_CMD_UNKNOWN               = 0xFF,
};

/**
 * @brief enums for GPR/SPR/FPR Access Messages
 *
*/
enum sbePsuRegisterAccessCommands
{
    SBE_PSU_CMD_GETHWREG                 = 0x03, /* Get HW Register (Scoms, OCMB Scoms) */
    SBE_PSU_CMD_REG_ACCESS_UNKNOWN       = 0xFF,
};

/**
  * @brief enums for SBE-Host interface control timer commands
  */
enum sbePsuControlTimerCommands
{
    SBE_PSU_CMD_CONTROL_TIMER                 = 0x01,
    SBE_PSU_CMD_CONTROL_TIMER_UNKNOWN         = 0xFF,
};

/**
  * @brief enums for SBE-Host interface ring access messages
  */
enum sbePsuRingAccessMessages
{
    SBE_PSU_MSG_PUT_RING_FROM_IMAGE = 0x01,
    SBE_PSU_MSG_UNKNOWN             = 0xFF,
};

/**
 * @brief enums for SBE-Host interface for unsecure memory regions commands
 */
enum sbePsuUpdateMemoryRegionMessages
{
    SBE_PSU_MSG_UPDATE_MEM_REGION      = 0x01,
};

/**
 * @brief enums for SBE-Host interface for generic commands
 */
enum sbePsuGenericMessages
{
    SBE_PSU_GENERIC_MSG_GET_CAPABILITIES   = 0x02,
    SBE_PSU_GENERIC_MSG_READ_SBE_MEM       = 0x03,
    SBE_PSU_GENERIC_MSG_SET_FFDC_ADDR      = 0x04,
    SBE_PSU_GENERIC_MSG_QUIESCE            = 0x05,
    SBE_PSU_GENERIC_MSG_SYSTEM_FABRIC_MAP  = 0x06,
    SBE_PSU_GENERIC_MSG_STASH_MPIPL_CONFIG = 0x07,
    SBE_PSU_GENERIC_MSG_SECURITY_LIST_BIN_DUMP  = 0x08,
    SBE_PSU_GENERIC_MSG_UPDATE_OCMB_TARGET      = 0x09,
    SBE_PSU_GENERIC_MSG_SYNC_FAB_TOPO_ID_TAB    = 0x0A,
    SBE_PSU_GENERIC_CMD_MEM_CONFIG              = 0x0B,
    SBE_PSU_PMIC_HEALTH_CHECK                   = 0x0C,
    SBE_PSU_GENERIC_MSG_UNKNOWN                 = 0xFF,
};


/**
  * @brief enums denoting control flags
  *
  */
enum sbePsuControlFlags
{
    SBE_PSU_FLAGS_RESP_REQUIRED   = 0x0100,
    SBE_PSU_FLAGS_ACK_REQUIRED    = 0x0200,
};

enum sbePsuDmtCmdFlags
{
    SBE_PSU_FLAGS_START_DMT       = 0x0001,
    SBE_PSU_FLAGS_STOP_DMT        = 0x0002,
};

enum sbePsuCntrlTimerFlags
{
    SBE_PSU_FLAGS_START_TIMER       = 0x0001,
    SBE_PSU_FLAGS_STOP_TIMER        = 0x0002,
};

enum sbeUnsecureMemRegionControlFlags
{
    SBE_MEM_REGION_OPEN_RO = 0x0011,
    SBE_MEM_REGION_OPEN_RW = 0x0012,
    SBE_MEM_REGION_CLOSE   = 0x0020,
};

/**
  * @brief capabilities index values.
  *        Get Capability response will return 18 capabilities. This
  *        enum tells the index for each capability. Currently each generic
  *        functionality( scom, IPL ) etc span across two capabilities.
  */
enum PSU_CAPABILITIES_INDEX
{
    PSU_GENERIC_CAPABILTITY_START_IDX           = 0,
    PSU_CORE_CONTROL_CAPABILITY_START_IDX       = 2,
    PSU_SCOM_CAPABILITY_START_IDX               = 4,
    PSU_RING_CAPABILITY_START_IDX               = 6,
    PSU_TIMER_CAPABILITY_START_IDX              = 8,
    PSU_MPIPL_CAPABILITY_START_IDX              = 10,
    PSU_SECURITY_CONTROL_CAPABILITY_START_IDX   = 12,
    PSU_GENERIC_CHIPOP_CAPABILITY_START_IDX     = 14,
    // Keep in sync with the spec
    CAPABILITIES_LAST_INDEX_PSU                 = 15,
};
/**
  * @brief capabilities enum values.
  *
  */
enum PSU_CAPABILITIES
{
    // Capabilities 2, 3
    PSU_CONTROL_DEADMAN_LOOP                     = 0xD1000001,
    PSU_EXIT_CACHE_CONTAINED_MODE                = 0xD1000002,
    PSU_RESERVED_1                               = 0xD1800000,

    // Capabilities 4, 5
    PSU_DEFAULT_CAPABILITY_D2                    = 0xD2000000,
    PSU_EXECUTE_MULTI_SCOM_SUPPORTED             = 0xD2000001,
    PSU_RESERVED_2                               = 0xD2800000,

    // Capabilites 6, 7
    PSU_PUT_RING_FROM_IMAGE_SUPPORTED            = 0xD3000001,
    PSU_RESERVED_3                               = 0xD3800000,

    // Capabilities 8, 9
    PSU_CONTROL_TIMER_SUPPORTED                  = 0xD4000001,
    PSU_RESERVED_4                               = 0xD4800000,

    // Capabilities 10, 11
    PSU_DEFAULT_CAPABILITY_D5                    = 0xD5000000,
    PSU_CMD_GETHWREG                             = 0xD5000004,
    PSU_RESERVED_5                               = 0xD5800000,

    // Capabilities 12, 13
    PSU_UNSECURE_MEM_REGION_SUPPORTED            = 0xD6000001,
    PSU_RESERVED_6                               = 0xD6800000,

    // Capabilities 14, 15
    PSU_GET_SBE_FFDC_SUPPPORTED                  = 0xD7000001,
    PSU_GET_SBE_CAPABILITIES_SUPPPORTED          = 0xD7000002,
    PSU_READ_SBE_SEEPROM_SUPPORTED               = 0xD7000004,
    PSU_SET_FFDC_ADDRESS_SUPPORTED               = 0xD7000008,
    PSU_QUISCE_SUPPORTED                         = 0xD7000010,
    PSU_SET_SYSTEM_FABRIC_ID_MAP_SUPPORTED       = 0xD7000020,
    PSU_STASH_MPIPL_CONFIG_SUPPORTED             = 0xD7000040,
    PSU_SECURITY_LIST_BIN_DUMP_SUPPORTED         = 0xD7000080,
    PSU_GENERIC_MSG_UPDATE_OCMB_TARGET_SUPPORTED = 0xD7000100,
    PSU_SYNC_FAB_TOPOLOGY_ID_SUPPORTED           = 0xD7000200,
    PSU_GENERIC_CMD_MEMORY_CONFIG                = 0xD7000400,
    PSU_PMIC_HEALTH_CHECK_SUPPORT                = 0xD7000800,
    PSU_RESERVED_7                               = 0xD7800000,
};

#endif // __SBEFW_SBE_HOST_INTF_H
