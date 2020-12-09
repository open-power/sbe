/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/chipop_handler.H $                             */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2021                        */
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
#pragma once

#include <stdint.h>
#include <stddef.h>
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
 * @brief SBE chip-op parameter definition
 *
 */
typedef struct chipOpParam
{
    uint8_t  fifoType;          // Refer to enum sbeFifoType
    uint16_t reserved;          // Reserved
    uint8_t* pkThreadParam;     // Private data to be passed as the argument to
                                // the thread routine when it begins execution
} chipOpParam_t;

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

////////////////////////////CHIP-OP table helper////////////////////////////
typedef struct {
    sbeChipOpFunc_t  cmd_func;         /* Command function pointer */
    uint8_t          cmd_opcode;       /* Command opcode */
    uint16_t         cmd_state_fence;  /* Command fencing based on SBE state */
} cmdStruct_t;

typedef struct {
    cmdStruct_t* cmdArr;
    size_t len;
} cmdStructTable_t;

typedef struct
{
    uint8_t cmdClass;
    cmdStructTable_t* cmdTable;
} cmdClass_t;

typedef struct
{
    cmdClass_t* cmdClassArr;
    size_t len;
} cmdClassTable_t;

extern cmdClassTable_t cmdClassTable;
extern cmdStructTable_t cmdTable;

///////////////Auto population of chip-op table in project files///////////////
#define CMD_CLASS(cmdClass) \
        { 0x##cmdClass, cmdStructP_t<0x##cmdClass>::value }

template <int T>
struct cmdStructP_t
{
    static constexpr cmdStructTable_t* value = NULL;
};

#define CMD_ARR(cmdclass, ...) \
    cmdStruct_t cmdClassArr_##cmdclass[] = \
    { \
        __VA_ARGS__ \
    }; \
    \
    cmdStructTable_t cmdClassArr_table_##cmdclass = { \
        cmdClassArr_##cmdclass, \
        sizeof(cmdClassArr_##cmdclass)/sizeof(cmdClassArr_##cmdclass[0]) \
    }; \
    template <> \
    struct cmdStructP_t<0x##cmdclass> \
    { \
        static constexpr cmdStructTable_t* value = &cmdClassArr_table_##cmdclass; \
    };

// Keep this hash updated with the cmdClassArr
#define HASH_KEY(key) (key < 0xD1 ? (key-0xA1) : (key-0xD1+0x0A))

#ifdef DFT
#define CMD_CLASS_DEFAULT_INTIALISATION \
constexpr cmdClass_t cmdClassArr[] = { \
    CMD_CLASS(A1), \
    CMD_CLASS(A2), \
    CMD_CLASS(A3), \
    CMD_CLASS(A4), \
    CMD_CLASS(A5), \
    CMD_CLASS(A6), \
    CMD_CLASS(A7), \
    CMD_CLASS(A8), \
    CMD_CLASS(A9), \
    CMD_CLASS(AA), \
    CMD_CLASS(D1), \
    CMD_CLASS(D2), \
    CMD_CLASS(D3), \
    CMD_CLASS(D4), \
    CMD_CLASS(D5), \
    CMD_CLASS(D6), \
    CMD_CLASS(D7), \
    CMD_CLASS(D8), \
}; \
\
cmdClassTable_t cmdClassTable = { \
    const_cast<cmdClass_t*>(cmdClassArr), \
    sizeof(cmdClassArr)/sizeof(cmdClassArr[0]) \
}; \
\
constexpr bool validateCmdClassHash(uint8_t index) { \
    return (index >= sizeof(cmdClassArr)/sizeof(cmdClassArr[0])) || \
            ((HASH_KEY(cmdClassArr[index].cmdClass)) == index && \
             validateCmdClassHash(index + 1)); \
} \
\
static_assert(validateCmdClassHash(0), "Bad chipop_table, check cmdclass hashes");

#else //DFT

#define CMD_CLASS_DEFAULT_INTIALISATION \
constexpr cmdClass_t cmdClassArr[] = { \
    CMD_CLASS(A1), \
    CMD_CLASS(A2), \
    CMD_CLASS(A3), \
    CMD_CLASS(A4), \
    CMD_CLASS(A5), \
    CMD_CLASS(A6), \
    CMD_CLASS(A7), \
    CMD_CLASS(A8), \
    CMD_CLASS(A9), \
    CMD_CLASS(AA), \
    CMD_CLASS(D1), \
    CMD_CLASS(D2), \
    CMD_CLASS(D3), \
    CMD_CLASS(D4), \
    CMD_CLASS(D5), \
    CMD_CLASS(D6), \
    CMD_CLASS(D7), \
}; \
\
cmdClassTable_t cmdClassTable = { \
    const_cast<cmdClass_t*>(cmdClassArr), \
    sizeof(cmdClassArr)/sizeof(cmdClassArr[0]) \
}; \
\
constexpr bool validateCmdClassHash(uint8_t index) { \
    return (index >= sizeof(cmdClassArr)/sizeof(cmdClassArr[0])) || \
            ((HASH_KEY(cmdClassArr[index].cmdClass)) == index && \
             validateCmdClassHash(index + 1)); \
} \
\
static_assert(validateCmdClassHash(0), "Bad chipop_table, check cmdclass hashes");

#endif //DFT
