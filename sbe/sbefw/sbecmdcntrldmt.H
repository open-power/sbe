/*
 * @file: ppe/sbe/sbefw/sbecmdcntrldmt.H
 *
 * @brief This file contains the Core State Control Messages API header
 *
 */

#ifndef __SBEFW_SBECMDCNTRLDMT_H
#define __SBEFW_SBECMDCNTRLDMT_H

#include <stdint.h>


/**
  * @brief Control Deadman Timer command (0xD101)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    RC from the PSU access utility
  */
uint32_t sbeControlDeadmanTimer(uint8_t *i_pArg);

#endif // __SBEFW_SBECMDCNTRLDMT_H
