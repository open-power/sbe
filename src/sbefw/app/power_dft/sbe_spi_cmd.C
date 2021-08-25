/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbe_spi_cmd.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
 * @file: ppe/sbe/sbefw/sbecmdgeneric.C
 *
 * @brief This file contains the SBE generic Commands
 *
 */

#include <sbe_spi_cmd.h>
#include "fapi2.H"
#include "endian.h"
using namespace std;

using namespace fapi2;

//selects between pib and fcam bus access
fapi2::ReturnCode putScomSelect(SpiControlHandle& handle, const uint64_t address, const uint64_t data){
	if(handle.spibus == PIB){
		FAPI_TRY(fapi2::putScom(handle.i_target_chip, address, data));
	}
	else if(handle.spibus == CFAM){
		//0x1000 FSI2PIB Engine Offset
		//0x1000 Addr for SPI-Master
		//0x1001 Data0
		//0x1002 Data1
		//0x1001 Put CFAM Data0
		//0x1002 Put CFAM Data1
		//0x1000 Command Reg (Pib addr, Bit0=1 for write)
		//0x1007 Poll status (Bit_7=busy, Bit_19:21=return code)
	}

	return fapi2::FAPI2_RC_SUCCESS;
		fapi_try_exit:
	return fapi2::current_err;
}

//selects between pib and fcam bus access
fapi2::ReturnCode getScomSelect(SpiControlHandle& handle, const uint64_t address, fapi2::buffer<uint64_t>& data){
	if(handle.spibus == PIB){
		FAPI_TRY(fapi2::getScom(handle.i_target_chip, address, data));
	}
	else if(handle.spibus == CFAM){
		//0x1000 Command Reg (Pib addr, Bit0=0 for read)
		//0x1007 Poll status (Bit_7=busy, Bit_19:21=return code)
		//0x1001 Get CFAM Data0
		//0x1002 Get CFAM Data1
	}

	return fapi2::FAPI2_RC_SUCCESS;
		fapi_try_exit:
	return fapi2::current_err;
}

/*
 * reset of spi-master component
 * 0x5 -> clock_config_reg
 * 0xA -> clock_config_reg
 */
uint64_t SpiEngineReset(SpiControlHandle& handle){
   uint32_t base_addr = (SpimBaseAddress<<16) + (handle.engine * SpimEngineOffset);
   fapi2::buffer<uint64_t> data64a,data64b;
   uint32_t reset_pattern_1 = 0x5;
   uint32_t reset_pattern_2 = 0xA;
   FAPI_INF("SPI engine reset: Entering ...");
   FAPI_INF("Writing Reset register: %016x\n", base_addr + SpimClockConfigReg);
   data64a.insertFromRight(reset_pattern_1, 24, 4);
   FAPI_TRY(putScomSelect(handle, base_addr + SpimClockConfigReg, data64a));
   data64b.insertFromRight(reset_pattern_2, 24, 4);
   FAPI_TRY(putScomSelect(handle, base_addr + SpimClockConfigReg, data64b));
   FAPI_INF("SPI engine reset: Exiting ...");
   //Reseting status register !!will break all possible error massages!!
   data64a= 0x0000000200000000ULL;
   FAPI_TRY(putScomSelect(handle, base_addr + SpimClockConfigReg, data64a));
   return fapi2::FAPI2_RC_SUCCESS;
    fapi_try_exit:
   return fapi2::current_err;
}

//sets the bit-rate-divider, the receive-delay and the ecc-mode
uint64_t SpiSetBrdAndDly(SpiControlHandle& handle){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  FAPI_INF("SPI engine set bit rate divider and receiver delay: Entering ...");
  FAPI_INF("Bit rate divider %#06lX",handle.brd);
  data64.insertFromRight(handle.brd,      0, 12);
  data64.insertFromRight(handle.rcv_dly, 12,  4);
  data64.insertFromRight(handle.ecc_select,28,4);
  data64.insertFromRight(handle.addr_corr,28,1);
  data64.insertFromRight(handle.mmSPIsm_init,31,1);
  FAPI_TRY(putScomSelect(handle, base_addr + SpimClockConfigReg, data64));
  FAPI_INF("SPI engine set bit rate divider and receiver delay: Exiting ...");
  return fapi2::FAPI2_RC_SUCCESS;
   fapi_try_exit:
  return fapi2::current_err;
}

//initilaizes the spi-master by reseting and conifguring
uint64_t SpiInit(SpiControlHandle& handle){
  SpiEngineReset(handle);
  SpiSetBrdAndDly(handle);
  return fapi2::FAPI2_RC_SUCCESS;
   fapi_try_exit:
  return fapi2::current_err;
}

//waits for transmit-data-register empty
uint64_t SpiWaitForTdrEmpty(SpiControlHandle& handle){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  FAPI_INF("SPI wait for TDR empty: Entering ...");
  while(1){
     fapi2::buffer<uint64_t> status_reg;
     FAPI_TRY(getScomSelect(handle, base_addr + SpimStatusReg, data64));
     if((data64.getBit<50>())){//checking for multiplexing error
    	 FAPI_INF("Port Multiplexer Setting Error");
    	 return fapi2::current_err;
     }
     if(!(data64.getBit<4>())){ //Wait until TX Buffer is empty
       break;
     }
   }
  FAPI_INF("SPI wait for TDR empty: Exiting ...");
  return fapi2::FAPI2_RC_SUCCESS;
   fapi_try_exit:
  return fapi2::current_err;
}

//waits for receive-data-register full
uint64_t SpiWaitForRdrFull(SpiControlHandle& handle){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  FAPI_INF("SPI wait for RDR full: Entering ...");
  while(1){
     fapi2::buffer<uint64_t> status_reg;
     FAPI_TRY(getScomSelect(handle, base_addr +  SpimStatusReg, data64));
     if((data64.getBit<50>())){//checking for multiplexing error
    	 FAPI_INF("Port Multiplexer Setting Error");
    	 return fapi2::current_err;
     }
     if(data64.getBit<0>()){ //Wait until RX Buffer is full
    	 break;
     }
   }
  FAPI_INF("SPI wait for RDR full: Exiting ...");
  return fapi2::FAPI2_RC_SUCCESS;
  	  fapi_try_exit:
  return fapi2::current_err;
}

uint64_t WaitForSeqIndexPass(SpiControlHandle& handle, uint32_t index){
	uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
	fapi2::buffer<uint64_t> data64;
	FAPI_INF("SPI wait for RDR full: Entering ...");
	while(1){
		fapi2::buffer<uint64_t> status_reg;
		FAPI_TRY(getScomSelect(handle, base_addr +  SpimStatusReg, data64));
		if((data64.getBit<50>())){//checking for multiplexing error
			FAPI_INF("Port Multiplexer Setting Error");
			return fapi2::current_err;
		}
		data64 = (data64>>32)&0x000000000000000FULL;
		if(data64>index){ //Wait until RX Buffer is full
			break;
		}
	}
	FAPI_INF("SPI wait for RDR full: Exiting ...");
	return fapi2::FAPI2_RC_SUCCESS;
		fapi_try_exit:
	return fapi2::current_err;
}

//waits for the fsm of the spi-master to be idle
uint64_t SpiWaitForIdle(SpiControlHandle& handle){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  while(1) {
	 FAPI_TRY(getScomSelect(handle, base_addr +  SpimStatusReg, data64));
	 if((data64.getBit<50>())){//checking for multiplexing error
		 FAPI_INF("Port Multiplexer Setting Error");
		 return fapi2::current_err;
	 }
     if(data64.getBit<27>()){ //Idle
    	 break;
     }
  }
  FAPI_INF("SPI wait for idle: Exiting ...");
  return fapi2::FAPI2_RC_SUCCESS;
  	  fapi_try_exit:
  return fapi2::current_err;
}

//waits for write complete flage of the spi-slave
uint64_t SpiWaitForWriteComplete(SpiControlHandle& handle){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  FAPI_INF("SPI wait for write complete: Entering ...");
  for (uint32_t p = 0; p < 4; p++) {
    FAPI_INF("Port set to: %d", p);
    if (handle.port != p) continue;
    uint64_t SEQ = 0x1031411000000000ULL | ((uint64_t)p << 56);
    FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
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
uint64_t SpiSetWriteEnable(SpiControlHandle& handle){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  FAPI_INF("SPI set write enable: Entering ...");
  FAPI_INF("Port set to: %d", handle.port);
  uint64_t SEQ = 0x1031100000000000ULL | ((uint64_t)handle.port << 56);
  uint64_t TDR = 0x0600000000000000ULL;
  uint64_t CNT = ((uint64_t)(0x6)<<12);
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

//writes data within the same page
uint64_t SpiPageWrite(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  FAPI_INF("SPI page program: Entering ...");
  uint64_t SEQ = 0x103438E210000000ULL;
  uint64_t CNT = (uint64_t)((length / 8) - 1) << 32;
  uint64_t TDR = 0x0200000000000000ULL | (((uint64_t)address << 32)&0x00ffffffffffffffULL);
  if (length < 8) {
    SEQ = 0x1034301000000000ULL | ((uint64_t)(length % 8) << 40);
    CNT = 0;
  } else if (length % 8 != 0)  {
    SEQ = 0x103438E230100000ULL | ((uint64_t)(length % 8) << 24);
  }
  SEQ |= ((uint64_t)handle.port << 56);
  CNT |= ((uint64_t)(0x6)<<12);
  SpiSetWriteEnable(handle);
  FAPI_INF("SPI page program: Programming ...");
  FAPI_INF("Port set to: %d", handle.port);
  SpiWaitForIdle(handle);
  FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
  FAPI_TRY(putScomSelect(handle, base_addr +  SpimCounterReg, CNT));

  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, TDR));
  for (uint32_t i = 0; i < length; i += 8) {
    data64.insertFromRight(((uint64_t *)buffer)[i / 8], 0, 64); 
    SpiWaitForTdrEmpty(handle);
    FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, data64));
  }
  SpiWaitForIdle(handle);
  SpiWaitForWriteComplete(handle);
  FAPI_INF("SPI page program: Exiting ...");
  return fapi2::FAPI2_RC_SUCCESS;
   fapi_try_exit:
  return fapi2::current_err;
}

//reads data
uint64_t SpiRead(SpiControlHandle& handle, uint32_t address, uint32_t length, uint8_t *buffer, bool ecc){
  uint32_t base_addr = (SpimBaseAddress<<16) + handle.engine * SpimEngineOffset;
  fapi2::buffer<uint64_t> data64;
  uint64_t temp;
  FAPI_INF("SPI read: Entering ...");
  FAPI_INF("Port set to: %d", handle.port);
  uint64_t SEQ;
  if(ecc){
	  SEQ = 0x103449E210000000ULL | ((uint64_t)handle.port << 56); //slave0, TX4, RX8, loop
  }
  else{
	  SEQ = 0x103448E210000000ULL | ((uint64_t)handle.port << 56); //slave0, TX4, RX8, loop
  }
  uint64_t CNT = ((uint64_t)(((length + 7) / 8) - 1) << 32)|((uint64_t)(0xf)<<8);
  uint64_t TDR = 0x0300000000000000ULL | ((uint64_t)address << 32);
  FAPI_TRY(putScomSelect(handle, base_addr +  SpimSequencerOpReg, SEQ));
  FAPI_TRY(putScomSelect(handle, base_addr +  SpimCounterReg, CNT));
  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, TDR));
  //one time zeros to trigger read
  SpiWaitForTdrEmpty(handle);
  FAPI_TRY(putScomSelect(handle, base_addr +  SpimTransmitDataReg, 0x0ULL));
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
