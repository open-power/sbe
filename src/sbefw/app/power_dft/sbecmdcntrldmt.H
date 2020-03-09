/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdcntrldmt.H $                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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

// Bit-32 used to checkstop the system, since this is directly getting
// inserted, we need to use bit (63-32) = 31st bit.
static const uint64_t  N3_FIR_CORE_CHECKSTOP_BIT = 31; // 63-32 = 31

/**
 * @brief Information received from Host as part of PSU chip-op in
 *        the mailbox registers.
 * Mbox Reg1 - timeValue in millisec to wait for stop state
 * Mbox Reg2 - Number of XSCOM register address-data pair to be initialized.
 * Mbox Reg3 - Hostboot allocated cache address containing
 *             XSCOM address-data pair information.
 */
typedef struct
{
    uint64_t timeValueMsec;
    uint64_t numXscoms;
    uint64_t xscomStartAddr;
} psu2SbeDeadmanTimerReg_t;

/**
  * @brief Callback for Timer Expiry for DMT
  *
  * @return void
  */
void sbeDmtPkExpiryCallback(void *arg);

/**
  * @brief  Called to collect FFDC for the Deadman loop timeout
  *         into the SBE global HWP FFDC region
  * @return SBE Secondary RC
  */
uint32_t sbeCollectDeadmanFfdc (void);

/**
  * @brief Control Deadman Timer command (0xD101)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    RC from the PSU access utility
  */
uint32_t sbeControlDeadmanTimer(uint8_t *i_pArg);

#endif // __SBEFW_SBECMDCNTRLDMT_H
