/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevPcrStates.C $                      */
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
#include "sbevPcrStates.H"
#include "sbevtrace.H"
#include "sbesecuritycommon.H"
#include "sbeTPMCommand.H"

//SHA256 separator
SHA512truncated_t SHA256separator = { 0xAD, 0x95, 0x13, 0x1B, 0xC0, 0xB7, 0x99, 0xC0,
                                      0xB1, 0xAF, 0x47, 0x7F, 0xB1, 0x4F, 0xCF, 0x26,
                                      0xA6, 0xA9, 0xF7, 0x60, 0x79, 0xE4, 0x8B, 0xF0,
                                      0x90, 0xAC, 0xB7, 0xE8, 0x36, 0x7B, 0xFD, 0x0E };

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

