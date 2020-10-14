/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdringaccess.H $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
 * @file: ppe/sbe/sbefw/sbecmdringaccess.H
 *
 * @brief This file contains the Interfaces for the RING Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDRINGACCESS_H
#define __SBEFW_SBECMDRINGACCESS_H

#include <stdint.h>
#include "plat_hwp_data_stream.H"

typedef struct
{
    uint64_t TargetType:16;
    uint64_t ChipletID:16;
    uint64_t RingID:16;
    uint64_t RingMode:16;
} sbePSUPutRingCMD_t;

/**
 * @brief Put Ring From Image Command (0xD301)
 *
 * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 * @return    RC from the PSU access utility
 */
uint32_t sbePutRingFromImagePSU(uint8_t *i_pArg);

/**
  * @brief sbeGetRing : write the ring data Buffer
  *
  * @param[in]  i_getStream      fifo down stream interface
  * @param[in]  i_putStream      fifo up stream interface
  *
  * @return    Rc from the FIFO access utility
  */

uint32_t sbeGetRingWrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                        fapi2::sbefifo_hwp_data_ostream& i_putStream);

/**
  * @brief sbeGetRing : Get the ring data
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetRing(uint8_t *i_pArg);

/**
 * @brief Put Ring Command
 *
 * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 * @return    RC from the FIFO access utility
 */
uint32_t sbePutRing(uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDRINGACCESS_H */
