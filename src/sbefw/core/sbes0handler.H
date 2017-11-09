/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbes0handler.H $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
 * @file: ppe/src/sbefw/sbes0handler.H
 *
 * @brief This file contains the Interfaces for S0 Handlers
 *
 */

#ifndef __SBEFW_SBES0HANDLER_H
#define __SBEFW_SBES0HANDLER_H

#include <stdint.h>
#include <sbeexeintf.H>

/**
  * @brief  Handles Sbe S0 Interrupt
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the function
  */
uint32_t sbeHandleS0(uint8_t *i_pArg);

/**
  * @brief  Clear S0 interrupt source bit in PERV_SB_CS_SCOM
  *
  * @param[in] i_pArg Interrupt bit to identify S0
  *
  * @return    Rc from the function
  */
uint32_t clearS0interrupt();

#endif /* __SBEFW_SBES0HANDLER_H */
