/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdmemconfig.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
#ifndef __SBE_CMDMEMCONFIG_H
#define __SBE_CMDMEMCONFIG_H

#include <stdint.h>
#include <stddef.h>

//Memory configure request parameters.Hostboot sends the below
//information for SBE to configure memory targets.
typedef struct
{
    uint64_t reserved; 
    uint64_t data_address;      //Cache address to fetch XSCOM address-data pair.
    uint64_t reserved1;
} sbePsuMemConfig_t;

/* @brief Configure DIMM related Targets (0xD801)
 *
 * @param[in] i_pArg Buffer to be passed to the function
 *
 * @return    RC from the FIFO access utility
 */
uint32_t sbePsuMemConfig(uint8_t *i_pArg);

#endif //__SBE_CMDMEMCONFIG_H
