/*
 * @file: ppe/sbe/sbefw/sbecmdregaccess.H
 *
 * @brief This file contains the Interfaces for the SCOM Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDREGACCESS_H
#define __SBEFW_SBECMDREGACCESS_H

#include <stdint.h>

/**
  * @brief sbeGetRegs : Get the reg data
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetReg(uint8_t *i_pArg);


/**
  * @brief sbePutRegs : Put the reg data
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutReg(uint8_t *i_pArg);


#endif /* __SBEFW_SBECMDREGACCESS_H */
