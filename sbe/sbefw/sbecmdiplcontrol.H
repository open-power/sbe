/*
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.H
 *
 * @brief This file contains the SBE command details
 *
 * Change Log *****************************************************************
 * Flag     Defect/Feature  User        Date         Description
 * ----     --------------  ----        ----         -----------
 *                          basengup  2015/03/17        Created
 *
 *  @endverbatim
 */

#ifndef __SBEFW_SBECMDIPLCONTROL_H
#define __SBEFW_SBECMDIPLCONTROL_H

#include <stdint.h>


uint32_t sbeExecuteIstep (uint8_t *i_pArg);


uint32_t sbeWaitForSbeIplDone (uint8_t *i_pArg);


#endif // __SBEFW_SBECMDIPLCONTROL_H
