/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbehandleresponse.H $                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
#ifndef __SBEFW_SBEHANDLERESPONSE_H
#define __SBEFW_SBEHANDLERESPONSE_H

#include "sbefifo.H"

/**
 * @brief sbeHandleFifoResponse : Function for handling FIFO response
 * @param[in] i_rc - return code as input
 * @param[in] i_fifoType - FIFO type.
 **/
void sbeHandleFifoResponse ( const uint32_t i_rc, sbeFifoType i_fifoType = SBE_FIFO );

/**
 * @brief sbeHandlePsuResponse : Function for handling PSU response
 * @param[in] i_rc - return code as input
 **/
void sbeHandlePsuResponse ( const uint32_t i_rc );

#endif // __SBEFW_SBEHANDLERESPONSE_H
