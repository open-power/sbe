/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemutil.H $                            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
#ifndef SBEM_UTIL_H
#define SBEM_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk.h"
#include "pk_api.h"

extern void jump2verificationImage(uint32_t i_destAddr);
extern void jump2bootImage();
#ifdef __cplusplus
}
#endif

/**
 * @brief Load verification image from source address into
 *        destination address after decompressing .
 *
 *  @param[in] *i_srcAddr - Source address
 *
 *  @param[in] *i_destAddr - Destination address
 */
int32_t loadSectionForVerification( uint64_t *i_srcAddr, uint64_t *i_destAddr);

#endif //SBEM_UTIL_H
