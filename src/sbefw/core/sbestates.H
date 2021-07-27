/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbestates.H $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
/*
 * @file: ppe/sbe/sbefw/sbestates.H
 *
 * @brief This file contains interfaces pertaining to SBE state/role management
 *
 */

#ifndef __SBEFW_SBESTATES_H
#define __SBEFW_SBESTATES_H

/**
 * @brief An enumeration of all SBE states
 *
 */
enum sbeState
{
    SBE_STATE_UNKNOWN = 0x0, // Unkown, initial state
    SBE_STATE_IPLING  = 0x1, // IPL'ing - autonomous mode (transient)
    SBE_STATE_ISTEP   = 0x2, // ISTEP - Running IPL by steps (transient)
    SBE_STATE_MPIPL   = 0x3, // MPIPL
    SBE_STATE_RUNTIME = 0x4, // SBE Runtime
    SBE_STATE_DMT     = 0x5, // Dead Man Timer State (transient)
    SBE_STATE_DUMP    = 0x6, // Dumping
    SBE_STATE_FAILURE = 0x7, // Internal SBE failure
    SBE_STATE_QUIESCE = 0x8, // This state quiesces any seeprom access
    SBE_STATE_HALT    = 0x9, // SBE halted via chip-op driven from Hostboot.Needs SBE reset to get out

    // Max States, Always keep it at the last of the enum and sequential
    SBE_MAX_STATE     = 0xA,
    // Don't count this in the state, just to intialize the state variables
    SBE_INVALID_STATE = 0xF,
};

enum sbeRole
{
    SBE_ROLE_MASTER = 0,
    SBE_ROLE_SLAVE = 1,
    SBE_ROLE_ALT_MASTER = 2,
};

#endif //__SBEFW_SBESTATES_H

