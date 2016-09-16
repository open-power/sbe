/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdiplcontrol.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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

namespace fapi2
{
    class ReturnCode;
}

/**
  * @brief execute istep chipop (0xA101)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeHandleIstep(uint8_t *i_pArg);


/**
  * @brief  Handles Sbe continue boot after collect ffdc chip-op (0xA102)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeContinueBoot(uint8_t *i_pArg);


/**
 * @brief Executes IPL steps in continuous mode.
 *
 * @par On the master SBE, this will run
 *      all steps from 2.2 to 5.2. On the slave SBE, it runs all steps from 2.2
 *      to 3.18.
 *      In case an error is encountered, the execution is aborted.
 */
void sbeDoContinuousIpl();

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
  * @brief  Handles Sbe Get FFDC chip-op (0xA801)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetSbeFfdc(uint8_t *i_pArg);

/**
  * @brief  Handles Sbe Get FFDC chip-op (0xA803)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetFreqSupported(uint8_t *i_pArg);

/**
  * @brief  Handles Sbe Abort chip-op (0xA804)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeAbort(uint8_t *i_pArg);


#endif // __SBEFW_SBECMDIPLCONTROL_H
