/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbe_spi_cmd_secure_mode.h $           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
 * sbe_spi_cmd_secrue_mode.h
 *
 *  Created on: 21 Sep 2020
 *      Author: HolgerNeuhaus
 */


#include "fapi2.H"
//#include "axi_pib_lib.h"
#include "sbe_spi_cmd.h"

#ifndef SRC_SBE_SPI_CMD_SECRUE_MODE_H_
#define SRC_SBE_SPI_CMD_SECRUE_MODE_H_

using namespace fapi2;

uint64_t SpiWaitForWriteCompleteSECURE(SpiControlHandle& handle);

uint64_t SpiSetWriteEnableSECURE(SpiControlHandle& handle);

uint64_t SpiPageWriteSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer);

uint64_t SpiReadSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

uint64_t SpiReadModifyWriteSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

fapi2::ReturnCode SpiWriteChunkSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

fapi2::ReturnCode SpiReadChunkSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

fapi2::ReturnCode SpiTpmWriteWithWaitSECURE(SpiControlHandle& handle, const uint32_t locality, const uint64_t address, uint8_t *buffer, uint8_t length);

fapi2::ReturnCode SpiTpmReadWithWaitSECURE(SpiControlHandle& handle, const uint32_t locality, const uint32_t address,  uint8_t *buffer, uint8_t length);

//TODO
void GetLocalityAccessSECURE(SpiControlHandle& handle, const uint32_t locality);

void RelinquishLocalityAccessSECURE(SpiControlHandle& handle, const uint32_t locality);

void SpiTpmResetEstablishedSECURE(SpiControlHandle& handle, const uint32_t locality);

fapi2::ReturnCode SpiTpmWriteFifoSECURE(SpiControlHandle& handle, const uint32_t locality, uint8_t *buffer, uint8_t length);

fapi2::ReturnCode SpiTpmReadFifoSECURE(SpiControlHandle& handle, const uint32_t locality, uint8_t *buffer, uint8_t length);

fapi2::ReturnCode SpiTpmCommandSendSECURE(SpiControlHandle& handle, const uint32_t locality, uint8_t *buffer, uint8_t length);

fapi2::ReturnCode SpiTpmStartupSECURE(SpiControlHandle& handle, const uint32_t locality, uint8_t *buffer, uint8_t length);

fapi2::ReturnCode SpiTpmReadResponseSECURE(SpiControlHandle& handle, const uint32_t locality, uint8_t *buffer, uint8_t length);

#endif /* SRC_SBE_SPI_CMD_SECRUE_MODE_H_ */


