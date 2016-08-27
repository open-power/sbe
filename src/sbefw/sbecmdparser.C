/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdparser.C $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdparser.C
 *
 * @brief This file contains the SBE FIFO Commands
 *
 */

#include "sbecmdparser.H"
#include "sbecmdscomaccess.H"
#include "sbecmdiplcontrol.H"
#include "sbecmdgeneric.H"
#include "sbecmdmemaccess.H"
#include "sbecmdregaccess.H"
#include "sbecmdcntrldmt.H"
#include "sbecmdringaccess.H"
#include "sbecmdsram.H"
#include "sbecmdcntlinst.H"
#include "sbecmdringaccess.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeHostMsg.H"
#include "sbe_host_intf.H"
#include "sbestates.H"
#include "sberegaccess.H"

// Declaration
static const uint16_t HARDWARE_FENCED_STATE =
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_QUIESCE|
     SBE_FENCE_AT_DMT;

static const uint16_t PUT_HARDWARE_FENCED_STATE =
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_MPIPL;

////////////////////////////////////////////////////////////////
// @brief g_sbeScomCmdArray
////////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeScomCmdArray [] =
{
    {sbeGetScom,
     SBE_CMD_GETSCOM,
     HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
    {sbePutScom,
     SBE_CMD_PUTSCOM,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
    {sbeModifyScom,
     SBE_CMD_MODIFYSCOM,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
    {sbePutScomUnderMask,
     SBE_CMD_PUTSCOM_MASK,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
    {sbeMultiScom,
     SBE_CMD_MULTISCOM,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
};

////////////////////////////////////////////////////////////////
// @brief g_sbeIplControlCmdArray
//
////////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeIplControlCmdArray [] =
{
    {sbeHandleIstep,
     SBE_CMD_EXECUTE_ISTEP,
     PUT_HARDWARE_FENCED_STATE|SBE_FENCE_AT_RUNTIME|
     SBE_FENCE_AT_DUMPING,
     // This is allowed in FFDC Collect state
     // TODO - Issue 157287 - Allow MPIIPL in Isteps state
    },

    {sbeContinueBoot,
     SBE_CMD_CONTINUE_BOOT,
     PUT_HARDWARE_FENCED_STATE|SBE_FENCE_AT_RUNTIME|
     SBE_FENCE_AT_DUMPING|SBE_FENCE_AT_ISTEP,
     // This is allowed only in FFDC Collect State in PLCK mode
    },
};

////////////////////////////////////////////////////////////////
// @brief g_sbeGenericCmdArray
//
////////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeGenericCmdArray [] =
{
    {sbeGetCapabilities,
     SBE_CMD_GET_SBE_CAPABILITIES,
     SBE_STATE_FFDC_COLLECT,
     // Fence in FFDC Collect State, since it might over-write traces
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbeMemoryAccessCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeMemoryAccessCmdArray [] =
{
    {sbeGetMem,
     SBE_CMD_GETMEM,
     HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },

    {sbePutMem,
     SBE_CMD_PUTMEM,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },

    {sbeGetOccSram,
     SBE_CMD_GETSRAM_OCC,
     HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },

    {sbePutOccSram,
     SBE_CMD_PUTSRAM_OCC,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbeInstructionCntlCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeInstructionCntlCmdArray[] =
{
    {sbeCntlInst,
     SBE_CMD_CONTROL_INSTRUCTIONS,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbeRegAccessCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeRegAccessCmdArray [] =
{
    {sbeGetReg,
     SBE_CMD_GETREG,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },

    {sbePutReg,
     SBE_CMD_PUTREG,
     PUT_HARDWARE_FENCED_STATE|SBE_STATE_FFDC_COLLECT,
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbeMpiplCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeMpiplCmdArray[] =
{
    {sbeEnterMpipl,
     SBE_CMD_MPIPL_ENTER,
     PUT_HARDWARE_FENCED_STATE|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_DUMPING|SBE_FENCE_AT_ABORT,
     // Allow Fspless system to enter MPIPL
     // Honour MPIPL at FFDC Collect state
     // Issue 157287
    },

    {sbeContinueMpipl,
     SBE_CMD_MPIPL_CONTINUE,
     HARDWARE_FENCED_STATE|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_RUNTIME|SBE_FENCE_AT_DUMPING|
     SBE_FENCE_AT_ABORT|SBE_FENCE_AT_FFDC_COLLECT,
     // Only allowed State is MPIPL
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbeRingAccessCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeRingAccessCmdArray [] =
{
    {sbeGetRing,
     SBE_CMD_GETRING,
     SBE_FENCE_AT_CONTINUOUS_IPL,
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbeCoreStateControlCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbeCoreStateControlCmdArray [] =
{
    {sbeControlDeadmanTimer,
     SBE_PSU_CMD_CONTROL_DEADMAN,
     SBE_FENCE_AT_CONTINUOUS_IPL|SBE_FENCE_AT_QUIESCE|
     SBE_FENCE_AT_MPIPL|SBE_FENCE_AT_ISTEP|
     SBE_FENCE_AT_DUMPING|SBE_FENCE_AT_ABORT|
     SBE_FENCE_AT_FFDC_COLLECT,
    },
};

//////////////////////////////////////////////////////////////
// @brief g_sbePutRingFromImageCmdArray
//
//////////////////////////////////////////////////////////////
static sbeCmdStruct_t g_sbePutRingFromImageCmdArray [] =
{
    {sbePutRingFromImagePSU,
     SBE_PSU_MSG_PUT_RING_FROM_IMAGE,
     SBE_FENCE_AT_CONTINUOUS_IPL,
    },
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
uint8_t sbeGetCmdStructAttr (const uint8_t  i_cmdClass,
                                   sbeCmdStruct_t **o_ppCmd)
{
    #define SBE_FUNC " sbeGetCmdStructAttr "
    uint8_t l_numCmds  = 0;
    *o_ppCmd = NULL;

    switch(i_cmdClass)
    {
        // FIFO Commands
        case SBE_CMD_CLASS_IPL_CONTROL:
            // @TODO via RTC : 128655
            //       Use C++ style typecase
            l_numCmds = sizeof(g_sbeIplControlCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd    = (sbeCmdStruct_t*)g_sbeIplControlCmdArray;
            break;
        case SBE_CMD_CLASS_SCOM_ACCESS:
            l_numCmds = sizeof(g_sbeScomCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd    = (sbeCmdStruct_t*)g_sbeScomCmdArray;
            break;
        case SBE_CMD_CLASS_GENERIC_MESSAGE:
            l_numCmds = sizeof(g_sbeGenericCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd    = (sbeCmdStruct_t*)g_sbeGenericCmdArray;
            break;

        case SBE_CMD_CLASS_MEMORY_ACCESS:
            l_numCmds = sizeof(g_sbeMemoryAccessCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbeMemoryAccessCmdArray;
            break;

        case SBE_CMD_CLASS_INSTRUCTION_CONTROL:
            l_numCmds = sizeof(g_sbeInstructionCntlCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbeInstructionCntlCmdArray;
            break;

        case SBE_CMD_CLASS_REGISTER_ACCESS:
            l_numCmds = sizeof(g_sbeRegAccessCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbeRegAccessCmdArray;
            break;

        case SBE_CMD_CLASS_RING_ACCESS:
            l_numCmds = sizeof(g_sbeRingAccessCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbeRingAccessCmdArray;
            break;

        case SBE_CMD_CLASS_MPIPL_COMMANDS:
            l_numCmds = sizeof(g_sbeMpiplCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbeMpiplCmdArray;
            break;

        // PSU Commands
        case SBE_PSU_CMD_CLASS_CORE_STATE:
            l_numCmds = sizeof(g_sbeCoreStateControlCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbeCoreStateControlCmdArray;
            break;

        case  SBE_PSU_CMD_CLASS_RING_ACCESS:
            l_numCmds = sizeof(g_sbePutRingFromImageCmdArray) /
                        sizeof(sbeCmdStruct_t);
            *o_ppCmd  = (sbeCmdStruct_t*)g_sbePutRingFromImageCmdArray;
            break;

        // This will grow with each class of chipOp in future
        default:
                break;
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
        sbeCmdStruct_t *l_pCmd = NULL;

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

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
bool sbeIsCmdAllowedAtState (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode)
{
    #define SBE_FUNC " sbeIsCmdAllowedAtState "
    uint8_t l_numCmds = 0;
    sbeCmdStruct_t *l_pCmd = NULL;
    bool l_ret = false;
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
                case SBE_STATE_QUIESCE:
                case SBE_STATE_UNKNOWN:
                case SBE_STATE_FAILURE:
                    // All operations are fenced here, return false
                    // Reset is the only Option available
                    break;

                case SBE_STATE_FFDC_COLLECT:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_FFDC_COLLECT)? false:true);
                    break;
                }

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

                case SBE_STATE_ABORT:
                {
                    l_ret = ((l_pCmd->cmd_state_fence &
                             SBE_FENCE_AT_ABORT)? false:true);
                    break;
                }

                default: break;
            }
        }
    }
    // For any other state, which is not handled above, return from here
    return l_ret;
    #undef SBE_FUNC
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
sbeCmdFunc_t sbeFindCmdFunc (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode)

{
    #define SBE_FUNC " sbeFindCmdFunc "
    uint8_t l_numCmds      = 0;
    sbeCmdStruct_t *l_pCmd = NULL;

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
