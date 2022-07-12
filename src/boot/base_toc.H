/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/base_toc.H $                                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2022                             */
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

#ifndef BASE_TOC_H
#define BASE_TOC_H

typedef struct
{
    // HBBL Offset
    uint32_t hbbl_start;
    uint32_t hbbl_size;

    // HDCT Offset
    uint32_t hdct_start;
    uint32_t hdct_size;

    // Rings Offset
    uint32_t rings_start;
    uint32_t rings_size;

    // Ring Overrides Offset
    uint32_t overrides_start;
    uint32_t overrides_size;

    // FA CL2 Offset
    uint32_t fa_cl2_start;
    uint32_t fa_cl2_size;

    // FA MMA Offset
    uint32_t fa_mma_start;
    uint32_t fa_mma_size;

    // FA Overrides Offset
    uint32_t fa_overrides_start;
    uint32_t fa_overrides_size;

    // BOOT SB SETTINGS Offset
    uint32_t bSbSettings_start;
    uint32_t bSbSettings_size;

    // MEAS SB SETTINGS Offset
    uint32_t mSbSettings_start;
    uint32_t mSbSettings_size;

    // Attribute Offset
    uint32_t attribute_start;
    uint32_t attribute_size;

}base_toc_t;

#endif
