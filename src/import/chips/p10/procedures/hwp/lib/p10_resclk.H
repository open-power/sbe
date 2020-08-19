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
    {0,     3},     //Freq, Index
    {1500,  3},     //Freq, Index
    {2000, 21},     //Freq, Index
    {3400, 19}      //Freq, Index
};

uint64_t RESCLK_TABLE[RESCLK_STEPS / 8] =
{
    0x2030100001030206ULL,  //Entries 0-7
    0x0705040C0D0F0E0AULL,  //Entries 8-15
    0x0B09088898B8A8E8ULL,  //Entries 16-23
    0xF8F8F8F8F8F8F8F8ULL   //Entries 24-31
};

#ifdef __cplusplus
} // end extern C
#endif
#endif    /* __ASSEMBLER__ */
#endif    /* __P10_RESCLK_H__*/
