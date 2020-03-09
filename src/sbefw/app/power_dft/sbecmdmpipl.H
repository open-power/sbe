/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdmpipl.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
 * @file: ppe/src/sbefw/sbecmdmpipl.H
 *
 * @brief This file contains the Interfaces for MPIPL chip-ops
 *
 */

#ifndef __SBEFW_SBECMDMPIPL_H
#define __SBEFW_SBECMDMPIPL_H

#include <stdint.h>
#include "sbestates.H"

/**
  * @brief  Handles Sbe Enter Mpipl chip-op (0xA901)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeEnterMpipl(uint8_t *i_pArg);

/**
  * @brief  Handles Sbe Continue Mpipl chip-op (0xA902)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeContinueMpipl(uint8_t *i_pArg);

/**
  * @brief  Handles Sbe Stop Clocks chip-op (0xA903)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeStopClocks(uint8_t *i_pArg);

/**
  * @brief  Handles Sbe Start MPIPL iSteps
  *
  * @return    Rc from the FIFO access utility
  */
fapi2::ReturnCode startMpiplIstepsExecute(void);

/**
  * @brief  Handles Sbe Continue MPIPL iSteps based on Master/Slave SBE
  *
  * @param[in] i_sbeRole Master/Slave SBE
  *
  * @return    Rc from the FIFO access utility
  */
fapi2::ReturnCode continueMpiplIstepsExecute(const sbeRole i_sbeRole);

#ifdef _S0_
/**
  * @brief  Handles Sbe Stop Clock (Core/Cache) procedure in S0 context
  *
  * @return    Rc from the FIFO access utility
  */
fapi2::ReturnCode stopClockS0();
#endif

#endif /* __SBEFW_SBECMDMPIPL_H */
