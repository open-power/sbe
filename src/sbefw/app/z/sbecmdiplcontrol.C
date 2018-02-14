/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/z/sbecmdiplcontrol.C $                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2018                        */
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
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.C
 *
 * @brief This file contains the SBE istep chipOps
 *
 */
#include "sbecmdiplcontrol.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbecmdcntrldmt.H"
#include "sbeglobals.H"
// TODO Workaround
#include "plat_target_parms.H"

using namespace fapi2;

static const uint32_t PEC_PHB_IOVALID_BIT_SHIFT = 59;
static const uint64_t PEC_PHB_IOVALID_BIT_MASK = 0x1ULL;

#ifdef __SBEFW_SEEPROM__
/* ----------------------------------- start SEEPROM CODE */
//Utility function to mask special attention
//----------------------------------------------------------------------------
/* end SEEPROM CODE ----------------------------- */
#endif // ifdef __SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__
/* ----------------------------------- start PIBMEM CODE */


//----------------------------------------------------------------------------

ReturnCode performAttrSetup( )
{
    #define SBE_FUNC "performAttrSetup "
    SBE_ENTER("performAttrSetup ");
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {

     }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

/* end PIBMEM CODE ----------------------------- */
#endif //#ifndef __SBEFW_SEEPROM__
