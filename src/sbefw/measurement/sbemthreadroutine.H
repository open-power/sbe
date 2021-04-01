/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemthreadroutine.H $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
#ifndef __SBEM_THREAD_ROUTINE_H
#define __SBEM_THREAD_ROUTINE_H

#include "sbesecuritycommon.H"

/**
 * @brief sbemSecureBoot_routine
 *    One shot thread.
 *    The major responsibilities of this thread are :
 *        - Perform Secure boot flow.
 *        - Jump to verification image.
 *
 *  @param[in]  i_pArg - Any buffer needed to be passed to the thread routine
 */
void sbemthreadroutine(void *i_pArg);

#endif
