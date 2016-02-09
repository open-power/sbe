/*
 * @file: ppe/sbe/sbefw/sbecmdmemaccess.H
 *
 * @brief This file contains the Interfaces for the Memory Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDMEMACCESS_H
#define __SBEFW_SBECMDMEMACCESS_H

#include <stdint.h>

/**
  * @brief sbeGetMem : Implements SBE Get Memory ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetMem (uint8_t *i_pArg);

/**
  * @brief sbePutMem : Implements SBE Put Memory ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutMem  (uint8_t *i_pArg);


#endif /* __SBEFW_SBECMDMEMACCESS_H */
