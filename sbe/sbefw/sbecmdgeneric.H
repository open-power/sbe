/*
 * @file: ppe/sbe/sbefw/sbecmdgeneric.H
 *
 * @brief This file contains the SBE Generic command details
 *
 */

#ifndef __SBEFW_SBECMDGENERIC_H
#define __SBEFW_SBECMDGENERIC_H

#include <stdint.h>


/**
  * @brief SBE Generic capabilities  (0xA802)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetCapabilities(uint8_t *i_pArg);

#endif // __SBEFW_SBECMDGENERIC_H
