/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemutil.C $                            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
#include "sbemutil.H"
#include "fapi2.H"
#include "sbemtrace.H"
#include "sbeDecompression.h"
#include "sbeutil.H"

extern "C"
{

/*
 * API to jump to verification code.
 */
void jump2verificationImage(uint32_t i_pkBootAddr, uint32_t i_ivprAddr)
{
    if(!SBE::isSimicsRunning())
    {
        // Set the IVPR register. This is required so that interrupt vector table
        // points to pk interfaces.
        uint64_t data = (uint64_t)i_ivprAddr << 32;
        PPE_STVD(0xc0000160, data);
    }
    asm volatile ( "mr %0, %1" : : "i" (6), "r" (i_pkBootAddr) : "memory" );
    asm(
            "mtctr %r6\n"
            "bctr\n"
       );
}
} // end extern "C"

int32_t loadSectionForVerification( uint64_t *i_srcAddr, uint64_t *i_destAddr )
{
    uint32_t rc = 0;
    do {
         uint8_t rc = decompress((uint8_t *)i_srcAddr, (uint8_t *)i_destAddr);
         if (rc != 0 )
           break;
       } while(0);
    return rc;
}
