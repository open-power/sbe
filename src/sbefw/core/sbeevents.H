/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeevents.H $                                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2022                        */
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
 * @file: ppe/sbe/sbefw/sbeevents.H
 *
 * @brief This file contains interfaces pertaining to the events for state
 * transition trigger.
 *
 */

#ifndef __SBEFW_SBEEVENTS_H
#define __SBEFW_SBEEVENTS_H

/**
 * @brief An enumeration of all SBE state transition events
 *
 */
enum sbeEvent
{
    SBE_RUNTIME_EVENT          = 0x0, // From Unknown/ISTEP/IPLING State
    SBE_ISTEP_EVENT            = 0x1, // From Unknown State
    SBE_PLCK_EVENT             = 0x2, // From Unknown state
    SBE_DUMP_FAILURE_EVENT     = 0x3, // From IPLING/RUNTIME/MPIPL/DMT/Unknown State
    SBE_ENTER_MPIPL_EVENT      = 0x4, // From Runtime State
    SBE_CONTINUE_MPIPL_EVENT   = 0x5, // From MPIPL Wait State
    SBE_DMT_ENTER_EVENT        = 0x6, // From Runtime State
    SBE_DMT_COMP_EVENT         = 0x7, // From DMT State
    SBE_FAILURE_EVENT          = 0x8, // From Any State
    SBE_QUIESCE_EVENT          = 0x9, // From Any State
    SBE_HALT_VIA_HOST_EVENT    = 0xA, // From Runtime State
    SBE_TPM_EXTEND_MODE_HALT_EVENT   = 0xB, // From Runtime State
    SBE_WAIT_FOR_S1_EVENT      = 0xC, // From Any State
};

// Maximum number of Events per State
enum maxEventPerState
{
    SBE_STATE_UNKNOWN_MAX_EVENT = 4,
    SBE_STATE_IPLING_MAX_EVENT  = 4,
    SBE_STATE_ISTEP_MAX_EVENT   = 3,
    SBE_STATE_MPIPL_MAX_EVENT   = 3,
    SBE_STATE_RUNTIME_MAX_EVENT = 7,
    SBE_STATE_DMT_MAX_EVENT     = 3,
    SBE_STATE_DUMP_MAX_EVENT    = 0,
    SBE_STATE_FAILURE_MAX_EVENT = 0,
    SBE_STATE_QUIESCE_MAX_EVENT = 0,
    SBE_STATE_HALT_MAX_EVENT    = 0,
    SBE_STATE_TPM_EXTEND_MODE_HALT_MAX_EVENT = 0,
    SBE_STATE_WAIT_FOR_S1_MAX_EVENT = 0, // Although this event can be triggered from any state, we
                                         // will not have any state handling for it in C.
                                         // All state handling is in assembly in __wait_for_s1 .
                                         // The only way to come out of this state is a HRESET of PPE via S1 Interrupt

    // Total number of State Transition Events, Addition of all the above
    SBE_MAX_TRANSITIONS = 24,
};


// Entry Point to stateTransitionStr_t Map Structure, This adds up all the state
// transition of the previous state, If any more transition is added, this will
// add up to all subsequent entries. This is closely mapped with the sbestates.H
// as well.
enum entryToStateMap
{
    SBE_STATE_UNKNOWN_ENTRY_TO_MAP = 0,
    SBE_STATE_IPLING_ENTRY_TO_MAP  = SBE_STATE_UNKNOWN_ENTRY_TO_MAP + SBE_STATE_UNKNOWN_MAX_EVENT, //4
    SBE_STATE_ISTEP_ENTRY_TO_MAP   = SBE_STATE_IPLING_ENTRY_TO_MAP + SBE_STATE_IPLING_MAX_EVENT, //8
    SBE_STATE_MPIPL_ENTRY_TO_MAP   = SBE_STATE_ISTEP_ENTRY_TO_MAP + SBE_STATE_ISTEP_MAX_EVENT, //11
    SBE_STATE_RUNTIME_ENTRY_TO_MAP = SBE_STATE_MPIPL_ENTRY_TO_MAP + SBE_STATE_MPIPL_MAX_EVENT, //14
    SBE_STATE_DMT_ENTRY_TO_MAP     = SBE_STATE_RUNTIME_ENTRY_TO_MAP + SBE_STATE_RUNTIME_MAX_EVENT, //20
    SBE_STATE_DUMP_ENTRY_TO_MAP    = SBE_STATE_DMT_ENTRY_TO_MAP + SBE_STATE_DMT_MAX_EVENT, //23
    SBE_STATE_FAILURE_ENTRY_TO_MAP = SBE_STATE_DUMP_ENTRY_TO_MAP + SBE_STATE_DUMP_MAX_EVENT, //23
    SBE_STATE_QUIESCE_ENTRY_TO_MAP = SBE_STATE_FAILURE_ENTRY_TO_MAP + SBE_STATE_FAILURE_MAX_EVENT, //23
    SBE_STATE_HALT_ENTRY_TO_MAP    = SBE_STATE_QUIESCE_ENTRY_TO_MAP + SBE_STATE_QUIESCE_MAX_EVENT, //23
    SBE_STATE_TPM_EXTEND_MODE_HALT_ENTRY_TO_MAP = SBE_STATE_HALT_ENTRY_TO_MAP +
                                                        SBE_STATE_TPM_EXTEND_MODE_HALT_MAX_EVENT, //23
};

#endif //__SBEFW_SBEEVENTS_H

