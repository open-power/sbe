/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevPcrStates.C $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021,2025                        */
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
#include "sbevPcrStates.H"
#include "sbevtrace.H"
#include "sbesecuritycommon.H"
#include "sbeTPMCommand.H"

void securitySwitchReg_PCR1::update(uint64_t securitySwitchReg)
{
    #define SBEV_FUNC "securityState_PCR1::update"
    SBEV_ENTER(SBEV_FUNC);

    //Fetch Security Switch Register
    securitySwitch = SBE::higher32BWord(securitySwitchReg);
    SBEV_INFO("Security Switch Reg [0x%08x 0x%08x]", SBE::higher32BWord(securitySwitchReg), SBE::lower32BWord(securitySwitchReg));

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

