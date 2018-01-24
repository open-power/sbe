/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeHostMsg.C $                                 */
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
#include "sbetrace.H"
#include "sbeHostMsg.H"
#include "sbeglobals.H"

void sbeSbe2PsuRespHdr_t::init(void)
{
    _primStatus   = SBE_PRI_OPERATION_SUCCESSFUL;
    _secStatus    = SBE_SEC_OPERATION_SUCCESSFUL;
    _seqID        = SBE_GLOBAL->sbePsu2SbeCmdReqHdr.seqID;
    _cmdClass     = SBE_GLOBAL->sbePsu2SbeCmdReqHdr.cmdClass;
    _command      = SBE_GLOBAL->sbePsu2SbeCmdReqHdr.command;
}
void sbeSbe2PsuRespHdr_t::setStatus(const uint16_t i_prim, const uint16_t i_sec)
{
    _primStatus = i_prim;
    _secStatus  = i_sec;

    if(i_prim != SBE_PRI_OPERATION_SUCCESSFUL)
    {
        SBE_GLOBAL->failedPrimStatus = _primStatus;
        SBE_GLOBAL->failedSecStatus  = _secStatus;
        SBE_GLOBAL->failedSeqId      = _seqID;
        SBE_GLOBAL->failedCmdClass   = _cmdClass;
        SBE_GLOBAL->failedCmd        = _command;
    }
}
