/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdcntlinst.H $                    */
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
 * @file: ppe/sbe/sbefw/sbecmdcntlinst.H
 *
 * @brief This file contains the Interfaces for Control Instructions chip-ops
 *
 */

#ifndef __SBEFW_SBECMDCNTLINST_H
#define __SBEFW_SBECMDCNTLINST_H

#include <stdint.h>
#include "fapi2.H"

// This is used to find out the array index in g_control_reg_map in
// p10_thread_control.C
static const uint8_t SINGLE_THREAD_BIT_MASK = 0x08;

/**
  * @brief stopAllCoreInstructions  Stop all core instructions function
  *
  * @return    Rc from the FAPI access utility
  */
fapi2::ReturnCode stopAllCoreInstructions( );

/**
  * @brief sbeCntlInst : Implements SBE Control instructions ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeCntlInst (uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDCNTLINST_H */
