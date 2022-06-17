/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdupdatecorefuncstates.H $            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
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

#ifndef __SBECMDUPDATECOREFUNCSTATES_H__
#define __SBECMDUPDATECOREFUNCSTATES_H__


typedef struct
{

    //MBox 1 - Core Functional State
    //b1: core non-functional
    //b0: core functional
    uint64_t _coreFunctionalState;
    public:
    uint64_t coreFunctionalState() { return _coreFunctionalState; }
} psu2SbeCoreFunctionalState_t;

/* @brief update target state for scratch register. (0xD103)
 *
 * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 * @return Rc from Psu access utility
 */
uint32_t sbeSyncCoreTargetPSU(uint8_t *i_pArg);

#endif
