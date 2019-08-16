/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeirqregistersave.H $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2019                        */
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
 * $file: ppe/sbe/sbefw/sbeirqregistersave.H
 *
 * @brief This file contains structs of registers to be saved during interrupt
 * contxt.
 */

#ifndef _SBE_IRQ_REGISTER_SAVE_H
#define _SBE_IRQ_REGISTER_SAVE_H

/**
 * @brief Register Save Structure
 * Total of 4 * 5 = 36Bytes of Data
 */
typedef struct registersave
{
    uint32_t  version:16;   // Structure Versioning, won't change unless,
                            // there is change in this structure
    uint32_t  magicbyte:8;  // Magic byte for address validation (0xA5) in SPRG0

    // Valid Byte, if the Register below could be saved off
    uint32_t  validbyte:8;  // One byte for all the registers below

    // Registers to be saved off
    uint32_t register_SRR0;
    uint32_t register_SRR1;
    uint32_t register_ISR;
    uint32_t register_LR;

    // Default Constructor to initialize
    registersave()
    {
        version = 0;
        magicbyte = 0;
        validbyte = 0;
    }
}registersave_t;

extern registersave_t __g_register_save_addr;
#endif  //_SBE_IRQ_REGISTER_SAVE_H
