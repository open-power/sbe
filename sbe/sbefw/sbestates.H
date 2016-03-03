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
    SBE_STATE_UNKNOWN = 0, // Unkown, initial state
    SBE_STATE_FFDC = 1, // Waiting for FFDC collection after a reset
    SBE_STATE_IPLING = 2, // IPL'ing - autonomous mode (transient)
    SBE_STATE_ISTEP = 3, // ISTEP - Running IPL by steps (transient)
    SBE_STATE_RUNTIME = 4, // SBE Runtime
    SBE_STATE_DUMP = 5, // Dumping (transient?)
    SBE_STATE_MPIPL = 6, // MPIPL (transient)
    SBE_STATE_FAILURE = 7, // Internal SBE failure
    SBE_STATE_ABORT = 8, // SBE was asked to to abort - need reset to get out
    SBE_STATE_QUIESCE = 0xF, // Final state - needs SBE reset to get out
};

enum sbeRole
{
    SBE_ROLE_MASTER = 0,
    SBE_ROLE_SLAVE = 1
};

extern sbeRole g_sbeRole;

#endif //__SBEFW_SBESTATES_H

