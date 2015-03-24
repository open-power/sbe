/*
 * @file: ppe/sbe/sbefw/sbecmdscomaccess.H
 *
 * @brief This file contains the Interfaces for the SCOM Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDSCOMACCESS_H
#define __SBEFW_SBECMDSCOMACCESS_H

#include <stdint.h>

/**
  * @brief sbeDownFifoGetStatus : Write data into Downstream FIFO
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetScom (uint8_t *i_pArg);


/**
  * @brief sbeDownFifoGetStatus : Write data into Downstream FIFO
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutScom (uint8_t *i_pArg);




#endif /* __SBEFW_SBECMDSCOMACCESS_H */
