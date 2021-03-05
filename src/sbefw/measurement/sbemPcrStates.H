/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemPcrStates.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
 *  @file: ppe/sbe/sbefw/measurement/sbemPcrStates.H
 *
 *  @brief This file contains basic structures to create PCR definitions
 *
 */

#include <stdint.h>
#include "ppe42_string.h"

typedef struct securityState_PCR6
{
    uint8_t jumperState;
    uint8_t minimumSecureVersion;
    uint8_t reserved[6];

    //Constructor
    securityState_PCR6()
    {
        jumperState = 0;
        minimumSecureVersion = 0;
        memset(reserved, 0x0, 6 * sizeof(uint8_t));
    }

    void update(uint32_t sbe_role);

}securityState_PCR6_t;

typedef struct securityState_PCR1
{
    uint8_t  jumperState;
    uint8_t  isPrimary;
    uint8_t  mSeepromLock;
    uint8_t  minimumSecureVersion;
    uint32_t mSeepromVersion;

    //Constructor
    securityState_PCR1()
    {
        jumperState = 0;
        isPrimary = 0;
        mSeepromLock = 0;
        minimumSecureVersion = 0;
        mSeepromVersion = 0;
    }

    void update(uint32_t sbe_role);

}securityState_PCR1_t;
