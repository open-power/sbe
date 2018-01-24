/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeSpMsg.C $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
    _magicCode = 0xC0DE;
    _cmdClass  = SBE_GLOBAL->sbeFifoCmdHdr.cmdClass;
    _command = SBE_GLOBAL->sbeFifoCmdHdr.command;
    _primaryStatus = SBE_PRI_OPERATION_SUCCESSFUL;
    _secondaryStatus = SBE_SEC_OPERATION_SUCCESSFUL;
}

void sbeCmdRespHdr_t::setStatus(const uint16_t i_prim,
                                const uint16_t i_sec)
{
    prim_status = i_prim;
    sec_status  = i_sec;
    if(i_prim != SBE_PRI_OPERATION_SUCCESSFUL)
    {
        SBE_GLOBAL->failedPrimStatus = i_prim;
        SBE_GLOBAL->failedSecStatus  = i_sec;
        SBE_GLOBAL->failedSeqId      = 0;
        SBE_GLOBAL->failedCmdClass   = SBE_GLOBAL->sbeFifoCmdHdr.cmdClass;
        SBE_GLOBAL->failedCmd        = SBE_GLOBAL->sbeFifoCmdHdr.command;
    }
}

void sbeRespGenHdr_t::setStatus( const uint16_t i_prim, const uint16_t i_sec)
{
    _primaryStatus = i_prim;
    _secondaryStatus = i_sec;

    if(i_prim != SBE_PRI_OPERATION_SUCCESSFUL)
    {
        SBE_GLOBAL->failedPrimStatus = _primaryStatus;
        SBE_GLOBAL->failedSecStatus  = _secondaryStatus;
        SBE_GLOBAL->failedSeqId      = 0;
        SBE_GLOBAL->failedCmdClass   = _cmdClass;
        SBE_GLOBAL->failedCmd        = _command;
    }
}
