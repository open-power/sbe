/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeConsole.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018-2020                        */
/* [+] International Business Machines Corp.                              */
/* [+] Raptor Engineering, LLC                                            */
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
#pragma once

#include "sbe_sp_intf.H"
#include "sbe_build_info.H"

#define SBE_LPC_INIT {         \
    SBE_GLOBAL->sbeLPCActive = true; \
}

#define SBE_LPC_DISABLE {       \
    SBE_GLOBAL->sbeLPCActive = false; \
}

#define LPC_IO_SPACE        0xD0010000
#define LPC_MAX_IO_SPACE    (64*1024)

uint32_t readLPCReg(uint16_t i_addr, uint8_t &i_data);
uint32_t writeLPCReg(uint16_t i_addr, uint8_t i_data);
uint32_t readLPCRegIndexed(uint8_t i_addr,
                  uint8_t i_index, uint8_t &o_data);
uint32_t writeLPCRegIndexed(uint8_t i_addr,
                  uint8_t i_index, uint8_t i_data);
