/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdtpmextendmode.H $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

#ifndef _SBECMDTPMEXTENDMODE_H_
#define _SBECMDTPMEXTENDMODE_H_

#include <stdint.h>

/**
 * @brief Enter and Exit TPM Extend mode chip-op
 *        Sequence of operations done if Enter TPM Extend mode:
 *        1) Stop Host
 *        2) Disable XSCOM's
 *        3) Run part of build SMP HWP (Switch AB)
 *        4) Retrive secondary chip measurements and extend into TPM
 *        5) Enable XSCOM's
 *        6) Start Host Alive timer
 *        7) Start Host
 *
 *        Sequence of operations done if Exit TPM Extend mode:
 *        1) Stop Host Alive timer
 *
 * @param i_pArg args if any
 * @return uint32_t RC
 */
uint32_t sbeTpmExtendMode(uint8_t *i_pArg);


#endif /* _SBECMDTPMEXTENDMODE_H_ */
