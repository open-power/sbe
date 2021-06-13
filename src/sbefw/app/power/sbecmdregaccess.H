/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdregaccess.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
 * @file: ppe/sbe/sbefw/sbecmdregaccess.H
 *
 * @brief This file contains the Interfaces for the SCOM Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDREGACCESS_H
#define __SBEFW_SBECMDREGACCESS_H

#include <stdint.h>
#include "plat_hwp_data_stream.H"


/**
  * @brief sbeGetHWReg_Wrap get the reg data access wrapper function
  *
  * @param[in]  i_getStream      up-stream fifo for chip-op /
  *                              memory interface for dump
  * @param[in]  i_putStream      down-stream fifo for chip-op /
  *                              memory interface for dump
  *
  * @return     Rc from the FIFO access utility
  */
uint32_t sbeGetHWReg_Wrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                          fapi2::sbefifo_hwp_data_ostream& i_putStream );

/**
  * @brief sbeGetRegs : Get the reg data
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetReg(uint8_t *i_pArg);


/**
  * @brief sbePutRegs : Put the reg data
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutReg(uint8_t *i_pArg);

/**
  * @brief sbeGetHWReg : Get the scom data for proc, proc chiplet and ocmb scoms.
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetHWReg(uint8_t *i_pArg);


/**
  * @brief sbePutReg : Put the scom data for proc, proc chiplet and ocmb scoms.
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutHWReg(uint8_t *i_pArg);

/**
  * @brief sbePsuGetHWReg : Get the scom data from proc, proc chiplet and ocmb scoms.
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the PSU access utility
  */
uint32_t sbePsuGetHWReg(uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDREGACCESS_H */
