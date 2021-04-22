/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemutil.H $                            */
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
#ifndef SBEM_UTIL_H
#define SBEM_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk.h"
#include "pk_api.h"

//Below macros are used in measurement_main.C
#define SPI_CLOCK_DELAY_SHIFT                          44
#define SPI_CLOCK_DIVIDER_SHIFT                        52
#define SPI_CLOCK_DIVIDER_DELAY_MASK                   0x00000FFFFFFFFFFF
#define DEFAULT_SPI_CLOCK_DELAY                        0x80
#define BIT15_SHIFT                                    0x30
#define BIT12_SHIFT                                    0x33
#define BIT5_SHIFT                                     0x3A
#define BIT0_MASK                                      1
#define ECC_SPIMM_ADDR_CORRECTION_DIS                  1
#define SPI_ECC_SPIMM_ADDR_CORRECTION_SHIFT            33
#define ECC_CONTROL_TRANSPARENT_READ                   1
#define SPI_ECC_CONTROL_SHIFT                          35
#define DD1                                            1
#define DD2                                            2
#define EC_MAJOR_VERSION_BIT_SHIFT                     36

extern void jump2verificationImage(uint32_t i_pkBootAddr, uint32_t i_ivprAddr);

#ifdef __cplusplus
}
#endif

/**
 * @brief Load verification image from source address into
 *        destination address after decompressing .
 *
 *  @param[in] *i_srcAddr - Source address
 *
 *  @param[in] *i_destAddr - Destination address
 */
int32_t loadSectionForVerification( uint64_t *i_srcAddr, uint64_t *i_destAddr);

#endif //SBEM_UTIL_H
