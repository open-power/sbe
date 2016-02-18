/*
 * @file: ppe/sbe/sbefw/sbecmdcntrldmt.H
 *
 * @brief This file contains the Core State Control Messages API header
 *
 */

#ifndef __SBEFW_SBECMDCNTRLDMT_H
#define __SBEFW_SBECMDCNTRLDMT_H

#include <stdint.h>

// Define for the Sleep interval between continuous HWP calls
// for DMT functionality in Millisecond
static const uint8_t  SBE_DMT_SLEEP_INTERVAL = 1;

// Bit-32 used to checkstop the system
static const uint64_t  N3_FIR_CORE_CHECKSTOP_BIT = 32;

/**
  * @brief Callback for Timer Expiry for DMT
  *
  * @return void
  */
void sbeDmtPkExpiryCallback(void *arg);

/**
  * @brief Control Deadman Timer command (0xD101)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    RC from the PSU access utility
  */
uint32_t sbeControlDeadmanTimer(uint8_t *i_pArg);

#endif // __SBEFW_SBECMDCNTRLDMT_H
