/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbeTPMaccess.H $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#ifndef __SBEM_TPM_ACCESS_H
#define __SBEM_TPM_ACCESS_H

#include "stdint.h"
#include "fapi2.H"
#include "p10_sbe_spi_cmd.H"

/// TPM Timeouts listed in ms
/// Timeout names and durations are as described in the TCG specification
enum tpm_timeouts
{
    TPM_TIMEOUT_A = 750000, ///< 750ms in usecs
    TPM_TIMEOUT_B = 2000000, ///< 2000ms in usecs
    TPM_TIMEOUT_C = 750000, ///< 750ms in usecs
    TPM_TIMEOUT_D = 750000, ///< 750ms in usecs
};

/// TPM Registers
enum tpm_reg
{
    TPM_INT_STATUS_0         = 0x18,
    TPM_INT_BURSTCOUNT_0     = 0x19,
    TPM_DATA_FIFO_0          = 0x24,
};

/// TPM Driver values
enum
{
    MAX_STSVALID_POLLS = 50000, ///< Max poll of 50ms (5*10ms) in usecs
};

/**
 * @brief TPM STS register definition (PTP 2.0 Spec)
 */
union tpm_sts_reg_t
{
    uint8_t value;
    struct __attribute__((packed))
    {
        uint8_t stsValid       : 1; // 00 // RO
        uint8_t isCommandReady : 1; // 01 // RW
        uint8_t tpmGo          : 1; // 02 // WO
        uint8_t dataAvail      : 1; // 03 // RO
        uint8_t expect         : 1; // 04 // RO
        uint8_t selfTestDone   : 1; // 05 // RO
        uint8_t responseRetry  : 1; // 06 // WO
        uint8_t rsvd2          : 1; // 07 // RO
    } fields;
};

/**
 * @brief This function peforms the sequencing to do a transmit of the
 *      TPM that is identified.
 *
 * @param[in] i_tpmInfo Structure of SPI parameters needed to execute
 *      the command to the SPI device driver.
 *
 * @param[in/out] io_buffer Data buffer
 *       INPUT: Command to be written to the TPM
 *       OUTPUT: Response from the TPM
 *
 * @param[in/out] io_buflen Buffer Length
 *       INPUT: Full length of io_buffer allocated.
 *       OUTPUT: Length of the response read from TPM.
 *
 * @param[in] i_commandlen Length of command to send in bytes
 *
 * @return errlHndl_t NULL if successful, otherwise a pointer to the
 *      error log.
 */

fapi2::ReturnCode tpmTransmit( SpiControlHandle & handle,
                         void * io_buffer,
                         uint32_t & io_buflen,
                         uint32_t i_commandlen);
#endif
