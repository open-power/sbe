/*
 * @file: ppe/sbe/sbefw/sbeerrorcodes.H
 *
 * @brief This file contains the SBE internal error status codes
 *
 */

#ifndef __SBEFW_SBEERRORCODES_H
#define __SBEFW_SBEERRORCODES_H

#include <stdint.h>


/**
 * @brief enums SBE internal error codes
 *
*/
enum sbeInternalResponseCodes
{
    SBE_FIFO_RESET_RECEIVED  = 0xFA00,
    SBE_FIFO_RESET_HANDLING_FAILED = 0xFB00,
    SBE_FUNC_NOT_SUPPORTED   = 0xFC00,
};


#endif // __SBEFW_SBEERRORCODES_H