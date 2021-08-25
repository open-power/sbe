/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbe_spi_cmd_secure_mode.C $           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
 * sbe_spi_cmd_secure_mode.C
 *
 *  Created on: 21 Sep 2020
 *      Author: HolgerNeuhaus
 */

/*
 * @file: ppe/sbe/sbefw/sbecmdgeneric.C
 *
 * @brief This file contains the SBE generic Commands
 *
 */

#include <sbe_spi_cmd.h>
#include <sbe_spi_cmd_secure_mode.h>
#include "fapi2.H"
#include "endian.h"
using namespace std;

using namespace fapi2;

////waits for write complete flage of the spi-slave
uint64_t SpiWaitForWriteCompleteSECURE(SpiControlHandle& handle){
	  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
	  fapi2::buffer<uint64_t> data64;
	  FAPI_INF("SPI wait for write complete: Entering ...");

	  for (uint32_t p = 0; p < 4; p++) {
	    FAPI_INF("Port set to: %d", p);
	    if (handle.port != p) continue;
	    uint64_t SEQ = 0x1031411000000000ULL | ((uint64_t)p << 56);
	    uint64_t CNT = 0x0;
	    FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
	    FAPI_TRY(putScomSelect(handle, base_addr +  SpimCounterReg, CNT));
	    while(1) {
	      FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, 0x0500000000000000ULL)); //Read status Register
	      SpiWaitForRdrFull(handle);
	      FAPI_TRY(getScomSelect(handle, base_addr +  SpimReceiveDataReg, data64));
	      if(!(data64.getBit<63>())) //Wait until RX Buffer is full
		break;
	      SpiWaitForIdle(handle);
	    }
	  }
	  FAPI_INF("SPI wait for write complete: Exiting ...");
	  return fapi2::FAPI2_RC_SUCCESS;
	  	  fapi_try_exit:
	  return fapi2::current_err;
	}

//enables spi-slave write
uint64_t SpiSetWriteEnableSECURE(SpiControlHandle& handle){
	  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
	  fapi2::buffer<uint64_t> data64;
	  FAPI_INF("SPI set write enable: Entering ...");
	  FAPI_INF("Port set to: %d", handle.port);
	  uint64_t SEQ = 0x1031100000000000ULL | ((uint64_t)handle.port << 56);
	  uint64_t TDR = 0x0600000000000000ULL;
	  uint64_t CNT = 0x0;
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimCounterReg, CNT));
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, TDR));
	  SpiWaitForTdrEmpty(handle);
	  SpiWaitForIdle(handle);
	  //read status for checking
	  SEQ = 0x1031411000000000ULL | ((uint64_t)handle.port << 56);

	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
	  while(1) {
	        FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, 0x0500000000000000ULL)); //Read status Register
	        SpiWaitForRdrFull(handle);
	        FAPI_TRY(getScomSelect(handle, base_addr +  SpimReceiveDataReg, data64));
	        if(data64.getBit<62>()) //Wait until RX Buffer is full
	        	break;
	        SpiWaitForIdle(handle);
	    }
	  FAPI_INF("SPI set write enable: Exiting ...");
	  return fapi2::FAPI2_RC_SUCCESS;
	   fapi_try_exit:
	  return fapi2::current_err;
	}

//writes data within the same page //TODO
uint64_t SpiPageWriteSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer){
	handle.ecc_select = transparent;
	SpiInit(handle);
	  
	  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
	  fapi2::buffer<uint64_t> data64;
	  int number_tdr = length/8;

	  if(length > 40){
		  FAPI_INF("Write operation is to long for secure mode! only up to 5*8=40byte are allowed to be written at once.");
		  return fapi2::current_err;
	  }

	  FAPI_INF("SPI page program: Entering ...");
	  uint64_t SEQ = 0x1034000000000000 | ((uint64_t)handle.port << 56); //= 0x103438E210000000ULL | ((uint64_t)handle.port << 56);
	  uint64_t CNT = 0x0; //= (uint64_t)((length / 8) - 1) << 32;
	  uint64_t TDR = 0x0200000000000000ULL | (((uint64_t)address << 32)&0x00ffffffffffffffULL);

	  for(int i=0; i<number_tdr; i++){
		  SEQ |= (uint64_t)(0x38) << (40-(i*8));
	  }
	  if(length%8!=0){
		  SEQ |= (uint64_t)(0x30 + (length%8)) << (40-(number_tdr*8));
		  SEQ |= (uint64_t)(0x10) << (40-((number_tdr+1)*8));
	  }
	  else{
		  SEQ |= (uint64_t)(0x10) << (40-((number_tdr)*8));
	  }

	  SpiSetWriteEnableSECURE(handle);
	  SpiWaitForIdle(handle);
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimCounterReg, CNT));

	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, TDR));
	  for (uint32_t i = 0; i < length; i += 8) {
	    data64.insertFromRight((((uint64_t *)buffer)[i / 8]), 0, 64); 
	    SpiWaitForTdrEmpty(handle);
	    FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, data64));
	  }
	  SpiWaitForIdle(handle);
	  SpiWaitForWriteCompleteSECURE(handle);
	  FAPI_INF("SPI page program: Exiting ...");
	  return fapi2::FAPI2_RC_SUCCESS;
	   fapi_try_exit:
	  return fapi2::current_err;
	}

//reads data
uint64_t SpiReadSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc){
	  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
	  fapi2::buffer<uint64_t> data64;
	  uint64_t temp;
	  uint64_t SEQ;
	  uint64_t CNT;
	  int number_rdr = length/8;

	  if(ecc){
		  if(length <= 8){
			  SEQ = 0x1034491000000000ULL | ((uint64_t)handle.port << 56); //slave0, TX4, RX9, loop
			  CNT = 0x0;
		  }
		  else if(length <= 40){
			  if(length%8!=0){
				  FAPI_INF("Read length not 8-byte aligned. This is not allowed in ecc Mode.");
				  return fapi2::current_err;
			  }
			  SEQ = 0x1034000000000000ULL | ((uint64_t)handle.port << 56);
			  for(int i=0; i<number_rdr; i++){
				  SEQ |= (uint64_t)(0x40) << (40-(i*8));
			  }
			  SEQ |= (uint64_t)(0x10) << (40-((number_rdr)*8));

			  CNT = ((uint64_t)(0x48)<<48)|((uint64_t)(0xb)<<8); // #shifted bits (9x8=64=0x48) | reload+tx+rx
		  }
		  else{
			  SEQ = 0x103440E210000000ULL | ((uint64_t)handle.port << 56); //slave0, TX4, RX9, loop
			  CNT = ((uint64_t)(0x48)<<48)|((uint64_t)(((length + 7) / 8) - 1) << 32)|((uint64_t)(0xb)<<8); // #shifted bits (9x8=64=0x48) | length of loop | reload+tx+rx
		  }
	  }
	  else{
		  if(length <= 8){
			  SEQ = 0x1034481000000000ULL | ((uint64_t)handle.port << 56); //slave0, TX4, RX8, loop
			  CNT = 0x0;
		  }
		  else if(length <= 40){
			  SEQ = 0x1034000000000000ULL | ((uint64_t)handle.port << 56);
			  for(int i=0; i<number_rdr; i++){
				  SEQ |= (uint64_t)(0x40) << (40-(i*8));
			  }
			  if(length%8!=0){
				  SEQ |= (uint64_t)(0x40) << (40-(number_rdr*8));
				  SEQ |= (uint64_t)(0x10) << (40-((number_rdr+1)*8));
			  }
			  else{
				  SEQ |= (uint64_t)(0x10) << (40-((number_rdr)*8));
			  }

			  CNT = ((uint64_t)(0x40)<<48)|((uint64_t)(0xb)<<8); // #shifted bits (8x8=64=0x40) | reload+tx+rx
		  }
		  else{
			  SEQ = 0x103440E210000000ULL | ((uint64_t)handle.port << 56); //slave0, TX4, RX8, loop
			  CNT = ((uint64_t)(0x40)<<48)|((uint64_t)(((length + 7) / 8) - 1) << 32)|((uint64_t)(0xb)<<8); // #shifted bits (8x8=64=0x40) | length of loop | reload+tx+rx
		  }
	  }

	  FAPI_INF("seq: %016llx for length: %d with addr: %d", SEQ, length, address);
	  uint64_t TDR = 0x0300000000000000ULL | ((uint64_t)address << 32);
		FAPI_INF("SpimSequencerOpReg: %d, SpimCounterReg : %d, SpimTransmitDataReg: %d",SpimSequencerOpReg,SpimCounterReg,SpimTransmitDataReg);
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimCounterReg, CNT));
	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, TDR));

	  if(length>8){
		  //one time zeros to trigger read
		  SpiWaitForTdrEmpty(handle);
	  	  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, 0x0ULL));
	  }

	  for (uint32_t i = 0; i < length; i += 8) {
	    SpiWaitForRdrFull(handle);
	    FAPI_TRY(getScomSelect(handle, base_addr +  SpimReceiveDataReg, data64));
	    data64.extract<0, 64>(temp);
	    ((uint64_t *)buffer)[i / 8] = temp; 
	  }
	  SpiWaitForIdle(handle);
	  FAPI_INF("SPI read: Exiting ...");
	  return fapi2::FAPI2_RC_SUCCESS;
	   fapi_try_exit:
	  return fapi2::current_err;
	}

fapi2::ReturnCode SpiWriteChunkSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc) {
	ECCSelect ecc_select_temp = handle.ecc_select;
	handle.ecc_select = transparent;
	SpiInit(handle);

	uint8_t pbuffer[256];
  FAPI_INF("SPI write chunk: Entering ...");
  if (address % 8 != 0) {
    FAPI_ERR("Address not 8 byte aligned!");
  }
  if (length % 8 != 0) {
    FAPI_ERR("Length not 8 byte aligned!");
  }
  if (ecc) {
    address = (address * 9) / 8;
  }
  FAPI_INF("Device Address %#08lX", address);

  for (uint32_t i = 0, j = 0, k = address; i < length; i++) {
      pbuffer[j] = buffer[i];
      if ((j+1==40) || (k % 256 == 255) || (i == (length - 1))) {
        SpiPageWriteSECURE(handle, k - j, j + 1, pbuffer);
        k++;
        j = 0;
      } else {
        j++; k++;
      }
    }

  handle.ecc_select = ecc_select_temp;
  SpiInit(handle);

  FAPI_INF("SPI write chunk: Exiting ...");
  return fapi2::FAPI2_RC_SUCCESS;
  	  fapi_try_exit:
  return fapi2::current_err;
}

/*
 * reads a chunck of data from memory
 * handles the page boundary's as well as ecc-process if necessary
 */
fapi2::ReturnCode SpiReadChunkSECURE(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc) {
	FAPI_INF("SPI read chunk: Entering ...");
	bool secure_mode=1;

	if(secure_mode){
		if (address % 8 != 0) {
			FAPI_ERR("Address not 8 byte aligned!");
		}
		if (length % 8 != 0) {
	    	FAPI_ERR("Length not 8 byte aligned!");
		}
		if(ecc){
			address = (address * 9) / 8;
		}

		uint32_t temp_addr = address;
		while(temp_addr<(address+length)){
			if((temp_addr+40)<(address+length)){
				if(ecc){
					SpiReadSECURE(handle, ((temp_addr*9)/8), 40, buffer+(temp_addr-address), ecc);
				}
				else{
					SpiReadSECURE(handle, temp_addr, 40, buffer+(temp_addr-address), ecc);
				}
				temp_addr+=40;
			}
			else{
				if(ecc){
					SpiReadSECURE(handle, ((temp_addr*9)/8), ((address+length)-temp_addr), buffer+(temp_addr-address), ecc);
				}
				else{
					SpiReadSECURE(handle, temp_addr, ((address+length)-temp_addr), buffer+(temp_addr-address), ecc);
				}
				temp_addr=address+length;
			}
		}
	}
	else{
		if (address % 8 != 0) {
			FAPI_ERR("Address not 8 byte aligned!");
		}
		if (length % 8 != 0) {
	    	FAPI_ERR("Length not 8 byte aligned!");
		}
		if(ecc){
		//if(handle.ecc_select==ecc | handle.ecc_select==discard){
			address = (address * 9) / 8;
		}

		SpiReadSECURE(handle, address, length, buffer, ecc);

		FAPI_INF("SPI read chunk: Exiting ...");
	}

	return fapi2::FAPI2_RC_SUCCESS;
		fapi_try_exit:
	return fapi2::current_err;
	}

