/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeConsole.C $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018-2020                        */
/* [+] International Business Machines Corp.                              */
/* [+] Raptor Engineering, LLC                                            */
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
#include "sbetrace.H"
#include "fapi2.H"

#include "sbeEarlyLPC.H"
#include "sbeIPLStatusLPC.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include "p9_misc_scom_addresses.H"
#include "p9_misc_scom_addresses_fld.H"

#include "sberegaccess.H"
#include "sbeglobals.H"
#include "p9_lpc_utils.H"

using namespace fapi2;

void postPutIStep(char major, char minor)
{
    #define SBE_FUNC "postPutIStep"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do {
        rc = writeLPCReg(0x81, major);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC " failure to write IPL status 1");
            break;
        }

        rc = writeLPCReg(0x82, minor);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC " failure to write IPL status 2");
            break;
        }

    } while(0);

    #undef SBE_FUNC
}
