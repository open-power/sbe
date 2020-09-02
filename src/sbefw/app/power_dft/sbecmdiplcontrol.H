/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdiplcontrol.H $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.H
 *
 * @brief This file contains the SBE command details
 *
 */

#ifndef __SBEFW_SBECMDIPLCONTROL_H
#define __SBEFW_SBECMDIPLCONTROL_H

#include <stdint.h>
#include "sbecmdgeneric.H"
#include "fapi2.H"

/**
  * @brief execute suspend IO chip-op (0xA103)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeHandleSuspendIO(uint8_t *i_pArg);

// Utility function to do TPM reset
fapi2::ReturnCode performTpmReset();

//Utility function to update PHB functional State
fapi2::ReturnCode updatePhbFunctionalState( void );

//Utility function to clear crest error latch
fapi2::ReturnCode resetCrespErrLatch( void );

fapi2::ReturnCode performAttrSetup( void );

#endif // __SBEFW_SBECMDIPLCONTROL_H
