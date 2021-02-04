/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeSpMsg.C $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2020                        */
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

extern sbeRole g_sbeRole;

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


void sbeCntlInstMsgHdr_t::processInputDataToIterate(uint8_t & o_core,
        uint8_t & o_coreCntMax,
        uint8_t & o_threadMask)
{
    //Default Init to call Core and all threads
    o_threadMask = SMT4_THREAD_ALL;
    o_core = SMT4_CORE0_ID;
    o_coreCntMax = SMT4_CORE_ID_LAST+1;
    fapi2::ATTR_FUSED_CORE_MODE_Type fusedMode = false;

    do{
        //For a specific thread
        if( SMT4_THREAD_ALL != threadId )
        {
            o_threadMask = (SINGLE_THREAD_BIT_MASK >> threadId);
        }

        //If particular Core
        if( SMT4_ALL_CORES != coreId )
        {
            o_core = coreId;
            o_coreCntMax = coreId;
            break;
        }
        //If the Cmd type is not StopInstruction and Core Id is 0xFF
        if(threadOps != THREAD_STOP_INS)
        {
            //Consider all cores in the system. Functional filter will be applied
            //by the caller.These are already initialized by default.
            break;
        }

        //If Cmd Type is Stop Instruction and Core Id is 0xFF , then based of the SBE
        //state and fused core mode select the cores required to be stopped.
        uint64_t prevState = SbeRegAccess::theSbeRegAccess().getSbePrevState();
        uint64_t curState = SbeRegAccess::theSbeRegAccess().getSbeState();
        //If the SBE has not completed the SBE_STATE_DMT state
        if( (g_sbeRole == SBE_ROLE_MASTER) && (! SBE_GLOBAL->sbeDmtStateComplete) )
        {
            //SBE is in the State where we should only consider stopping the instructions
            //on the master core pair
            Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
            fapi2::ATTR_MASTER_CORE_Type masterCore;
            FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,procTgt,masterCore);
            FAPI_ATTR_GET(ATTR_FUSED_CORE_MODE,Target<TARGET_TYPE_SYSTEM>(),fusedMode);
            o_core = masterCore;
            o_coreCntMax = fusedMode ? (masterCore+1) : masterCore;
        }
        //Otherwise Consider all cores in the system. Functional filter will be applied
        //by the caller.These are already initialized by default
        SBE_INFO("Previous State:0x%.8x%.8x Current State:0%.8x%.8x",
                ((prevState & 0xFFFFFFFF00000000ull) >> 32),(prevState & 0xFFFFFFFF),
                ((curState & 0xFFFFFFFF00000000ull) >> 32),(curState & 0xFFFFFFFF));

    }while(0);
    SBE_INFO("processInputDataToIterate: o_core=0x%.8x o_coreCntMax=0x%.8x o_threadMask=0x%.8x",
            o_core,o_coreCntMax,o_threadMask);

}
