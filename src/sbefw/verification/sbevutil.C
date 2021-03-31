/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevutil.C $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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

#include "sbevutil.H"

extern "C"
{

/*
 *  ** API to jump to the boot seeprom.
 *   */
void jump2boot()
{
    asm(
            "lis %r4, 0xFF80\n"
            "lvd %d0, 0(%r4)\n"
            "lis %r2 , 0x5849\n"
            "ori %r2 , %r2 , 0x5020\n"
            "lis %r3 , 0x5345\n"
            "ori %r3 , %r3, 0x504d\n"
            "cmplwbc 0, 2, %r0, %r2, magic_failed\n"
            "cmplwbc 0, 2, %r1, %r3, magic_failed\n"
            "ori %r4, %r4, 8\n"
            "lvd %d0 , 0(%r4)\n"
            "mtctr %r1\n"
            "bctr\n"
"magic_failed:\n"
            "trap\n"
       );
}

} // end extern "C"
