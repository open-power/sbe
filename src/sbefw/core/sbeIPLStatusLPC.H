/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeConsole.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018                             */
/* [+] International Business Machines Corp.                              */
/* [+] Raptor Engineering, LLC                                            */
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
#pragma once

#include "sbe_sp_intf.H"
#include "sbe_build_info.H"

#ifndef SBE_IPL_STATUS_LPC_SUPPORT

#define SBE_PUT_ISTEP_LPC(major, minor)

#else

#define SBE_PUT_ISTEP_LPC(major, minor) \
    if(SBE_GLOBAL->sbeLPCActive) \
    { \
        postPutIStep(major, minor); \
    }

void postPutIStep(char major, char minor);

#endif // SBE_IPL_STATUS_LPC_SUPPORT
