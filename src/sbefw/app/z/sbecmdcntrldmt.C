/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/z/sbecmdcntrldmt.C $                            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2018                        */
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
/*
 * @file: sbe/sbefw/sbecmdcntrldmt.C
 *
 * @brief This file contains the Core State Control Messages
 *
 */

#include "sbecmdcntrldmt.H"
#include "sbetrace.H"
#include "fapi2.H"
#include "sbeFFDC.H"

using namespace fapi2;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint32_t sbeCollectDeadmanFfdc (void)
{
    #define SBE_FUNC "sbeCollectDeadmanFfdc"
    uint32_t rc = SBE_SEC_COMMAND_NOT_SUPPORTED;

    // trace the saved aync ffdc reason and SBE

    return rc;
    #undef SBE_FUNC
}
