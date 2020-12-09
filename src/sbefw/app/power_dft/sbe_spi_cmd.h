/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbe_spi_cmd.h $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
 * sbe_spi_cmd.h
 *
 *  Created on: 1 Apr 2019
 *      Author: HolgerNeuhaus
 */

#include "fapi2.H"


#ifndef SRC_SBECMDCODEUPDATESPI_H_
#define SRC_SBECMDCODEUPDATESPI_H_

const uint32_t SpimBaseAddress      	= 0xc;//P10 //= 0x00060000;z
const uint32_t PibMemBaseAddress		= 0x8;//P10
const uint32_t SbeBaseAddress			= 0xe;//P10
const uint32_t Axi2PibBaseAddress		= 0x2;//P10
const uint32_t SpimEngineOffset     	= 0x20;
const uint32_t SpimCounterReg       	= 1;
const uint32_t SpimConfigReg1       	= 2;
const uint32_t SpimClockConfigReg	  	= 3;
const uint32_t SpimmmSPIsmReg  			= 4;
const uint32_t SpimTransmitDataReg 		= 5;
const uint32_t SpimReceiveDataReg  		= 6;
const uint32_t SpimSequencerOpReg  		= 7;
const uint32_t SpimStatusReg         	= 8;
const uint32_t SpimPortControlReg	    = 9;
const uint32_t SbeRestReg  				= 0;
const uint32_t SbeIarReg	  			= 5;
enum SPIBus {CFAM=0, PIB=1};
enum ECCSelect {ecc_on=0, transparent=2, discard=4};
enum ECCAdressCorrection {ecc_address_correction=0, no_ecc_address_correction=1};
enum mmSPIsm {mmSPIsm_enable = 1, mmSPIsm_disable = 0};



struct SpiControlHandle{
public:
	SpiControlHandle (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip, SPIBus spibus, uint16_t brd, uint16_t rcv_dly, uint8_t engine, uint16_t port, ECCSelect ecc_select, ECCAdressCorrection addr_corr, mmSPIsm mmSPIsm_init) {
		this->i_target_chip 	= i_target_chip;
		this->spibus			= spibus;
		this->brd				= brd;
		this->rcv_dly			= rcv_dly;
		this->engine			= engine;
		this->port				= port;
		this->ecc_select		= ecc_select;
		this->addr_corr			= addr_corr;
		this->mmSPIsm_init		= mmSPIsm_init;
	}

	fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> i_target_chip;
	SPIBus spibus;
	uint16_t brd;
	uint16_t rcv_dly;
	uint8_t engine;
	uint16_t port;
	ECCSelect ecc_select;
	ECCAdressCorrection addr_corr;
	mmSPIsm mmSPIsm_init;
};

using namespace fapi2;

fapi2::ReturnCode putScomSelect(SpiControlHandle& handle, const uint64_t address, const uint64_t data);

fapi2::ReturnCode getScomSelect(SpiControlHandle& handle, const uint64_t address, fapi2::buffer<uint64_t>& data);

uint64_t SpiEngineReset(SpiControlHandle& handle);

uint64_t SpiSetBrdAndDly(SpiControlHandle& handle);

uint64_t SpiInit(SpiControlHandle& handle);

uint64_t SpiMasterLock(SpiControlHandle& handle, uint64_t PIB_master_ID);

uint64_t SpiMasterRelinquish(SpiControlHandle& handle, uint64_t PIB_master_ID);

uint64_t SpiWaitForTdrEmpty(SpiControlHandle& handle);

uint64_t SpiWaitForRdrFull(SpiControlHandle& handle);

uint64_t WaitForSeqIndexPass(SpiControlHandle& handle, uint32_t index);

uint64_t SpiWaitForIdle(SpiControlHandle& handle);

uint64_t SpiWaitForWriteComplete(SpiControlHandle& handle);

void SpiSlaveReset(SpiControlHandle& handle, uint8_t set);

uint64_t SpiSetWriteEnable(SpiControlHandle& handle);

uint64_t SpiPageWrite(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer);

uint64_t SpiRead(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

uint64_t SpiReadModifyWrite(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

uint8_t SpiEccGen(uint8_t *bytes);

fapi2::ReturnCode SpiWriteChunk(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

fapi2::ReturnCode SpiReadChunk(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc);

fapi2::ReturnCode SpiSectorErase(SpiControlHandle& handle, uint32_t address);

fapi2::ReturnCode SpiBlockErase(SpiControlHandle& handle, uint32_t address);

#endif /* SRC_SBECMDCODEUPDATESPI_H_ */


