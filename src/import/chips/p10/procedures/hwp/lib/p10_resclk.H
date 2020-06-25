/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/lib/p10_resclk.H $        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#ifndef __P10_RESCLK_H__
#define __P10_RESCLK_H__

#include <pstates_pgpe.H>

#ifndef __ASSEMBLER__
#ifdef __cplusplus
extern "C" {
#endif

ResClkFreqIdx_t RESCLK_INDEX[RESCLK_FREQ_REGIONS] =
{
    {0, 3},         //Freq, Index
    {1500, 3},      //Freq, Index
    {2000, 21},     //Freq, Index
    {3400, 23}      //Freq, Index
};

//\todo Review if defining this table as array of 8 bytes is ok or not. Spec
//defines it as array of bytes. However, defining it as array 8 bytes makes it
//easy to use in the initialization code below
uint64_t RESCLK_TABLE[RESCLK_STEPS / 8] =
{
    0x0001020304050607ULL,  //Entries 0-7
    0x08090a0b0c0d0e0fULL,  //Entries 8-15
    0x1011121314151617ULL,  //Entries 16-23
    0x18191a1b1c1d1e1fULL   //Entries 24-31
};

#ifdef __cplusplus
} // end extern C
#endif
#endif    /* __ASSEMBLER__ */
#endif    /* __P10_RESCLK_H__*/
