/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeSpMsg.C $                                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
#include "sbeutil.H"
#include "sbetrace.H"
#include "sbeSpMsg.H"
#include "sbeglobals.H"

void sbeRespGenHdr_t::init(void)
{
    magicCode = 0xC0DE;
    cmdClass  = SBE_GLOBAL->sbeFifoCmdHdr.cmdClass;
    command = SBE_GLOBAL->sbeFifoCmdHdr.command;
    primaryStatus = SBE_PRI_OPERATION_SUCCESSFUL;
    secondaryStatus = SBE_SEC_OPERATION_SUCCESSFUL;
}
