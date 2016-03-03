/*
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.H
 *
 * @brief This file contains the SBE command details
 *
 */

#ifndef __SBEFW_SBECMDIPLCONTROL_H
#define __SBEFW_SBECMDIPLCONTROL_H

#include <stdint.h>

namespace fapi2
{
    class ReturnCode;
}

/**
  * @brief execute istep chipop (0xA101)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeHandleIstep(uint8_t *i_pArg);


/**
  * @brief  Handles wait for IPL done chipop (0xA102)
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeWaitForSbeIplDone (uint8_t *i_pArg);


/**
 * @brief Executes IPL steps in continuous mode.
 *
 * @par On the master SBE, this will run
 *      all steps from 2.2 to 5.2. On the slave SBE, it runs all steps from 2.2
 *      to 3.18.
 *      In case an error is encountered, the execution is aborted.
 */
void sbeDoContinuousIpl();

#endif // __SBEFW_SBECMDIPLCONTROL_H
