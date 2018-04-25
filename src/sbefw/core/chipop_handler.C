/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/chipop_handler.C $                             */
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
#include "sbe_sp_intf.H"
#include "sbestates.H"
#include "sberegaccess.H"
#include "sbeglobals.H"

#include "chipop_handler.H"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
uint8_t sbeGetCmdStructAttr (const uint8_t  i_cmdClass,
                                   cmdStruct_t **o_ppCmd)
{
    #define SBE_FUNC " sbeGetCmdStructAttr "
    uint8_t l_numCmds  = 0;
    *o_ppCmd = NULL;

    size_t cmdClassIndex = HASH_KEY(i_cmdClass);
    if(cmdClassIndex < cmdClassTable.len &&
       cmdClassTable.cmdClassArr[cmdClassIndex].cmdTable)
    {
        l_numCmds = cmdClassTable.cmdClassArr[cmdClassIndex].cmdTable->len;
        *o_ppCmd = cmdClassTable.cmdClassArr[cmdClassIndex].cmdTable->cmdArr;
    }

    return l_numCmds;
    #undef SBE_FUNC
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
uint8_t sbeValidateCmdClass (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode)
{
    #define SBE_FUNC " sbeValidateCmdClass "
    uint8_t l_rc           = SBE_SEC_COMMAND_NOT_SUPPORTED;

    SBE_INFO(SBE_FUNC"i_cmdClass[0x%02X], "
              "i_cmdOpcode[0x%02X]", i_cmdClass, i_cmdOpcode);

    do
    {
        uint8_t l_numCmds      = 0;
        cmdStruct_t *l_pCmd = NULL;

        l_numCmds = sbeGetCmdStructAttr (i_cmdClass, &l_pCmd);
        if (!l_numCmds)
        {
            SBE_ERROR(SBE_FUNC"SBE_SEC_COMMAND_CLASS_NOT_SUPPORTED "
                "i_cmdClass[0x%02X], i_cmdOpcode[0x%02X]",
                i_cmdClass, i_cmdOpcode);
            // Command class not supported
            l_rc = SBE_SEC_COMMAND_CLASS_NOT_SUPPORTED;
            break;
        }

        // @TODO via RTC : 128654
        //       Analyze on merging the validation functions into one
        //       and also on using loop vs switch case performance
        for (uint8_t l_cnt = 0; l_cnt < l_numCmds; ++l_cnt, ++l_pCmd)
        {
            if (i_cmdOpcode == l_pCmd->cmd_opcode)
            {
                // Command found
                l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
                break;
            }
        }
    } while (false);

    return l_rc;
    #undef SBE_FUNC
}

sbeCmdFunc_t sbeFindCmdFunc (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode)

{
    #define SBE_FUNC " sbeFindCmdFunc "
    uint8_t l_numCmds      = 0;
    cmdStruct_t *l_pCmd = NULL;

    l_numCmds = sbeGetCmdStructAttr (i_cmdClass, &l_pCmd);

    for (uint8_t l_cnt = 0; l_cnt < l_numCmds; ++l_cnt, ++l_pCmd)
    {
        if (i_cmdOpcode == l_pCmd->cmd_opcode)
        {
            break;
        }
    }

    return l_pCmd ? (l_pCmd->cmd_func) : NULL;
    #undef SBE_FUNC
}

sbeChipOpRc_t sbeIsCmdAllowed (const uint8_t i_cmdClass,
                               const uint8_t i_cmdOpcode)
{
    #define SBE_FUNC " sbeIsCmdAllowedAtState "
    bool l_ret = true;
    sbeChipOpRc_t retRc;
    uint8_t l_numCmds = 0;
    cmdStruct_t *l_pCmd = NULL;
    l_numCmds = sbeGetCmdStructAttr (i_cmdClass, &l_pCmd);

    for (uint8_t l_cnt = 0; l_cnt < l_numCmds; ++l_cnt, ++l_pCmd)
    {
        if (i_cmdOpcode == l_pCmd->cmd_opcode)
        {
            // Get the Present State
            uint64_t l_state =
                SbeRegAccess::theSbeRegAccess().getSbeState();
            SBE_INFO(SBE_FUNC "SBE State [0x%08X] Fence State[0x%04X]",
                (uint32_t)(l_state & 0xFFFFFFFF),l_pCmd->cmd_state_fence);

            switch(l_state)
            {
                case SBE_STATE_UNKNOWN:
                case SBE_STATE_FAILURE:
                    // All operations are fenced here, return false
                    // Reset is the only Option available
                    break;

                case SBE_STATE_IPLING:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_CONTINUOUS_IPL)? false:true);
                    break;
                }

                case SBE_STATE_ISTEP:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_ISTEP)? false:true);
                    break;
                }

                case SBE_STATE_RUNTIME:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_RUNTIME)? false:true);
                    break;
                }

                case SBE_STATE_DUMP:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_DUMPING)? false:true);
                    break;
                }

                case SBE_STATE_MPIPL:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_MPIPL)? false:true);
                    break;
                }

                case SBE_STATE_DMT:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_DMT)? false:true);
                    break;
                }

                case SBE_STATE_QUIESCE:
                {
                    // fence off all the chip-ops in quiesce state
                    l_ret = false;
                    break;
                }

                default:
                    l_ret = false;
                    break;
            }
            if(false == l_ret)
            {
                retRc.primStatus = SBE_PRI_INVALID_COMMAND;
                retRc.secStatus  = SBE_SEC_COMMAND_NOT_ALLOWED_IN_THIS_STATE;
                break;
            }
            // Check if the command is allowed in current security mode
            if((SBE_GLOBAL->sbeFWSecurityEnabled)
                && (SBE_FENCE_AT_SECURE_MODE & l_pCmd->cmd_state_fence)
                && (!SBE::isSimicsRunning()))
            {
                retRc.primStatus = SBE_PRI_UNSECURE_ACCESS_DENIED;
                retRc.secStatus  = SBE_SEC_BLACKLISTED_CHIPOP_ACCESS;
                break;
            }
            break;
        }
    }
    // For any other state, which is not handled above, return from here
    return retRc;
    #undef SBE_FUNC
}
