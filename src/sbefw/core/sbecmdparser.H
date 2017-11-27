/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbecmdparser.H $                               */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
 * @file: ppe/sbe/sbefw/sbecmdparser.H
 *
 * @brief This file contains the SBE command details
 *
 */

#ifndef __SBEFW_SBECMDPARSER_H
#define __SBEFW_SBECMDPARSER_H

#include <stdint.h>
#include "sbe_sp_intf.H"

// Chip-op response codes
typedef struct sbeChipOpRc
{
    sbePrimResponse primStatus;
    sbeSecondaryResponse secStatus;

    sbeChipOpRc():primStatus(SBE_PRI_OPERATION_SUCCESSFUL),
                  secStatus(SBE_SEC_OPERATION_SUCCESSFUL)
    {}

    bool success()
    {
        return (primStatus == SBE_PRI_OPERATION_SUCCESSFUL) &&
               (secStatus == SBE_SEC_OPERATION_SUCCESSFUL);
    }
} sbeChipOpRc_t;

/**
 *  @brief SBE Command structure associating an opcode of a command
 *         to the processing function as well as the allowed states
 *
 */
typedef uint32_t (*sbeChipOpFunc_t) (uint8_t *i_pArg);

typedef struct sbeCmdStruct {
    sbeChipOpFunc_t  cmd_func;        /* Command function pointer */
    uint8_t          cmd_opcode;      /* Command opcode */
    uint16_t         cmd_state_fence; /* Command fencing based on SBE state */
} sbeCmdStruct_t;

/**
 *  @brief SBE Command Fence attributes
 *
 */
enum sbe_command_fence_attrs
{
    SBE_NO_FENCE                = 0x0000, ///< Allow cmd in all states
    SBE_FENCE_AT_DUMPING        = 0x0001, ///< Fence off at DUMPING State
    SBE_FENCE_AT_MPIPL          = 0x0002, ///< Fence off at MPIPL state
    SBE_FENCE_AT_CONTINUOUS_IPL = 0x0004, ///< Fence off at cont IPL
    SBE_FENCE_AT_ISTEP          = 0x0008, ///< Fence off at istep state
    SBE_FENCE_AT_RUNTIME        = 0x0010, ///< Fence off at Runtime state
    SBE_FENCE_AT_QUIESCE        = 0x0020, ///< Fense off at Quiesce state
    SBE_FENCE_AT_DMT            = 0x0040, ///< Fense off at DMT state
    SBE_FENCE_AT_SECURE_MODE    = 0x0080, ///< Fense off in secure mode
};

/**
  * @brief sbeValidateCmdClass  Validates the command class and opcode
  *
  * @param[in]     i_cmdClass    Command class code
  * @param[in]     i_cmdOpcode   Command opcode
  *
  * @return uint8_t  return code
  *               SBE_SEC_OPERATION_SUCCESSFUL - Command found
  *               SBE_SEC_COMMAND_CLASS_NOT_SUPPORTED
  *               SBE_SEC_COMMAND_NOT_SUPPORTED
  */
uint8_t sbeValidateCmdClass (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode);

/**
  * @brief sbeIsCmdAllowed  Validates if the command is allowed
  *        at the current SBE state
  *
  * @param[in]    i_cmdClass    Command class code
  * @param[in]    i_cmdOpcode   Command opcode
  *
  * @return       sbeChipOpRc_t Indicating primary and secondary status of
  *                             chip-op
  */
sbeChipOpRc_t sbeIsCmdAllowed (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode);


/**
  * @brief sbeCmdFunc_t Typical signature for any SBE ChipOp back-end function
  *
  * @param[in]  uint8_t *i_pArg  Pointer to the argument to be passed to
  *                              the chipOp function
  *
  * @return     uint32_t Return code from the chipOp function
  */
typedef uint32_t ( *sbeCmdFunc_t ) (uint8_t *i_pArg);


/**
  * @brief sbeFindCmdFunc  Finds the function corresponding to the command
  *
  * @param[in]     i_cmdClass    Command class code
  * @param[in]     i_cmdOpcode   Command opcode
  *
  * @return  sbeCmdFunc_t A pointer to the corresponding ChipOps function
  */
sbeCmdFunc_t sbeFindCmdFunc (const uint8_t i_cmdClass,
                             const uint8_t i_cmdOpcode);


#endif // __SBEFW_SBECMDPARSER_H
