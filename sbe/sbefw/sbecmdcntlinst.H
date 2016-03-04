/*
 * @file: ppe/sbe/sbefw/sbecmdcntlinst.H
 *
 * @brief This file contains the Interfaces for Control Instructions chip-ops
 *
 */

#ifndef __SBEFW_SBECMDCNTLINST_H
#define __SBEFW_SBECMDCNTLINST_H

#include <stdint.h>

/**
  * @brief sbeCntlInst : Implements SBE Control instructions ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeCntlInst (uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDCNTLINST_H */
