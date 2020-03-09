/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdtracearray.H $                  */
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
 * @file: ppe/sbe/sbefw/sbecmdtracearray.H
 *
 * @brief This file contains the Interfaces for the Tracearray Control chip-op
 *
 */

#ifndef __SBEFW_SBECMDTRACEARRAY_H
#define __SBEFW_SBECMDTRACEARRAY_H

#include <stdint.h>

typedef struct
{
    uint32_t targetType:16;
    uint32_t reserved:8;
    uint32_t chipletId:8;
    uint32_t traceArrayId:16;
    uint32_t operation:16;
} sbeControlTraceArrayCMD_t;

/**
 * @brief Control Trace Array Command (0xA602)
 *
 * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 * @return    RC from the FIFO access utility
 */
uint32_t sbeControlTraceArray(uint8_t *i_pArg);

#endif //__SBEFW_SBECMDTRACEARRAY_H
