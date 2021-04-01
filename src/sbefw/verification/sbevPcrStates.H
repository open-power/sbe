/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevPcrStates.H $                      */
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

#ifndef __SBEV_PCR_STATES_H
#define __SBEV_PCR_STATES_H

#include <stdint.h>
#include "ppe42_string.h"
#include "sbesecuritycommon.H"

typedef struct securitySwitchReg_PCR1
{
    uint32_t securitySwitch;

    //Constructor
    securitySwitchReg_PCR1()
    {
        securitySwitch = 0;
    }

    void update(uint64_t securitySwitchReg);

}securitySwitchReg_PCR1_t;

#endif
