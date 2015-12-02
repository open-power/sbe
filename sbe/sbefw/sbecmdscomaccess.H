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


/**
  * @brief sbeModifyScom : Modify the Scom data
  *     This chipOp needs to do the following
  *        1. Read Register Data (getscom)
  *        2. modify the scomData using the given op mode
  *        3. Write the modified Data into the Register (putscom)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeModifyScom (uint8_t *i_pArg);


/**
  * @brief sbePutScomUnderMask : Write data into Downstream FIFO
  *    The following steps need to be done as part of this command :
  *        1. Read Register Data (getscom)
  *        2. 'AND' the Mask with the data read from register
  *        3. 'OR' the modifying data with the result of step 2
  *        4. Write the result of step 3 into the register (putscom)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutScomUnderMask (uint8_t *i_pArg);


#endif /* __SBEFW_SBECMDSCOMACCESS_H */
