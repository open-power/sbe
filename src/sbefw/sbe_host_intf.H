/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbe_host_intf.H $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
    SBE_PSU_CMD_CLASS_SECURITY_CONTROL_MSG  = 0xD6,
    SBE_PSU_CMD_CLASS_GENERIC               = 0xD7,
 };

/**
  * @brief enums for SBE-Host interface core state control commands
  */
enum sbePsuCoreStateControlCommands
{
    SBE_PSU_CMD_CONTROL_DEADMAN     = 0x01,
    SBE_PSU_CMD_UNKNOWN             = 0xFF,
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
    SBE_PSU_GENERIC_MSG_READ_SBE_MEM       = 0x03,
    SBE_PSU_GENERIC_MSG_SET_FFDC_ADDR      = 0x04,
    SBE_PSU_GENERIC_MSG_QUIESCE            = 0x05,
    SBE_PSU_GENERIC_MSG_SYSTEM_FABRIC_MAP  = 0x06,
    SBE_PSU_GENERIC_MSG_STASH_MPIPL_CONFIG = 0x07,
    SBE_PSU_GENERIC_MSG_UNKNOWN            = 0xFF,
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

#endif // __SBEFW_SBE_HOST_INTF_H
