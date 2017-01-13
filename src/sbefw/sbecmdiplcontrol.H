/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdiplcontrol.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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

// Major isteps which are supported
typedef enum
{
    SBE_ISTEP2 = 2,
    SBE_ISTEP_FIRST = SBE_ISTEP2,
    SBE_ISTEP3 = 3,
    SBE_ISTEP_LAST_SLAVE = SBE_ISTEP3,
    SBE_ISTEP4 = 4,
    SBE_ISTEP5 = 5,
    SBE_ISTEP_LAST_MASTER = SBE_ISTEP5,
}sbe_supported_steps_t;

// Major MPIPL isteps which are supported
static const uint32_t SBE_ISTEP_MPIPL_START         = 96;
static const uint32_t SBE_ISTEP_MPIPL_CONTINUE      = 97;
static const uint32_t SBE_ISTEP_STOPCLOCK           = 98;
static const uint32_t MPIPL_START_MAX_SUBSTEPS      = 8;
static const uint32_t MPIPL_CONTINUE_MAX_SUBSTEPS   = 3;
static const uint32_t ISTEP_STOPCLOCK_MAX_SUBSTEPS  = 1;

// constants
static const uint32_t ISTEP2_MAX_SUBSTEPS = 17;
static const uint32_t ISTEP3_MAX_SUBSTEPS = 22;
static const uint32_t ISTEP4_MAX_SUBSTEPS = 34;
static const uint32_t ISTEP5_MAX_SUBSTEPS = 2;
static const uint8_t ISTEP_MINOR_START = 1;
static const uint8_t SLAVE_LAST_MINOR_ISTEP = 20;
static const uint8_t ISTEP2_MINOR_START = 2;

/**
 * @brief Support function to execute specific istep
 *
 * @param[in] i_major Major Istep Number
 * @param[in] i_minor Minor Istep Number
 *
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
fapi2::ReturnCode sbeExecuteIstep (uint8_t i_major, uint8_t i_minor);


/**
  * @brief execute istep chipop (0xA101)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeHandleIstep(uint8_t *i_pArg);

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


#endif // __SBEFW_SBECMDIPLCONTROL_H
