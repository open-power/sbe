/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdcntlinst.H $                        */
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
 * @file: ppe/sbe/sbefw/sbecmdcntlinst.H
 *
 * @brief This file contains the Interfaces for Control Instructions chip-ops
 *
 */

#ifndef __SBEFW_SBECMDCNTLINST_H
#define __SBEFW_SBECMDCNTLINST_H

#include <stdint.h>
#include "fapi2.H"
#include "p10_query_corecachemma_access_state.H"
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

/**
  * @brief Returns SCOMable states for input Core and L2
  *
  * @param[in] i_coreTgt Core Target 
  * @param[in] scomStatus_t entry for particular core target
  *
  * @return  Scom state for particular core and its assocaied cache
  */
bool isCoreOrL2CacheScomEnabled(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_coreTgt ,scomStatus_t& i_scomStateData);

/**
  * @brief Returns SCOMable states for L3 associated with Core 
  *
  * @param[in] i_coreTgt Core Target
  * @param[in] scomStatus_t entry for particular core target
  *
  * @return  Scom state for L3 assocaited with the core target
  */
bool isL3CacheScomEnabled(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_coreTgt ,scomStatus_t& i_scomStateData);


#endif /* __SBEFW_SBECMDCNTLINST_H */
