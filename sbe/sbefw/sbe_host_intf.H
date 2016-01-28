/*
 * @file: ppe/sbe/sbefw/sbe_host_intf.H
 *
 * @brief This file contains the Host-SBE interface protocol common details
 */

#ifndef __SBEFW_SBE_HOST_INTF_H
#define __SBEFW_SBE_HOST_INTF_H

#include <stdint.h>

/**
 * $Version: Conforms to HOST-SBE Interface Spec v0.5a
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
    SBE_PSU_CMD_CLASS_UNKNOWN      = 0,
    SBE_PSU_CMD_CLASS_CORE_STATE   = 0xD1,
 };

/**
  * @brief enums for SBE-Host interface core state control commands
  */
enum sbePsuCoreStateControlCommands
{
    SBE_PSU_CMD_CONTROL_DEADMAN    = 0x01,
    SBE_PSU_CMD_UNKNOWN            = 0xFF,
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

#endif // __SBEFW_SBE_HOST_INTF_H
