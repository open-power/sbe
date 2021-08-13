/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdmpipl.H $                           */
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
 * @file: ppe/src/sbefw/sbecmdmpipl.H
 *
 * @brief This file contains the Interfaces for MPIPL chip-ops
 *
 */

#ifndef __SBEFW_SBECMDMPIPL_H
#define __SBEFW_SBECMDMPIPL_H

#include <stdint.h>
#include "sbestates.H"
#include "plat_hwp_data_stream.H"

#define TI_DATA_LEN 1024
#define PBA_GRAN_SIZE 128 // PBA has to read in multiple of 128 Bytes.
#define CORE_SCRATCH_REG0 0x20028486

//Variables used for LQA sync and quiesce procedures
static const uint32_t SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP = 20000;
static const uint64_t SBE_LQA_DELAY_HW_US = 1000000ULL; // 1ms 
static const uint64_t SBE_LQA_DELAY_SIM_CYCLES = 0x1ULL;

/* @brief Bitmapped enumeration to identify the stop clock HWP call
 */
enum stopClockHWPType
{
    SC_NONE     = 0x00,
    SC_PROC     = 0x01, // Call p10_stopclocks
    SC_CACHE    = 0x02, // Call p10_hcd_cache_stopclocks
    SC_CORE     = 0x04, // Call p10_hcd_core_stopclocks
    SC_EQ       = 0x08, // Call p10_hcd_eq_stopclocks
};

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
  * @brief sbeStopClocks_Wrap Sbe Stop Clocks function
  * @param[in]  i_getStream      up-stream fifo for chip-op /
  *                              memory interface for dump
  * @param[in]  i_putStream      down-stream fifo for chip-op /
  *                              memory interface for dump
  * @return  RC from the underlying FIFO utility
  */
uint32_t sbeStopClocks_Wrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                            fapi2::sbefifo_hwp_data_ostream& i_putStream );

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

/**
  * @brief sbeGetTIInfo : Implements SBE Get TI Info ChipOp (0xA904)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetTIInfo  (uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDMPIPL_H */
