/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdfastarray.H $                       */
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
#ifndef __SBE_CMDFASTARRAY_H
#define __SBE_CMDFASTARRAY_H

#include <stdint.h>
#include "sbe_sp_intf.H"
#include "sbe_link.H"
#include "p10_ipl_image.H"
#include "plat_hwp_data_stream.H"

static const uint32_t SBE_FASTARRAY_MAX_CONTROL_DATA_WORDS = SBE_FASTARRAY_MAX_CONTROL_DATA_BYTES / 4;
static const uint32_t FA_XIP_SECTION_START = 14;
/*
 * @brief Fast array command structure
 *
 * */
typedef struct
{
    struct {
        uint32_t targetType  :16;
        uint32_t chipletId   :8;
        uint32_t control_set :8;

        uint32_t custom_data_length;
        bool isControlSetValid()
        {
            bool flag = false;
            if( control_set > 0 && control_set <= 2 )
            {
                flag = true;
            }
            return flag;
        }

        //Get the FastArray Xip section offset basis the control_set
        uint32_t getFastArrayXipOffset()
        {
#if defined DFT || defined PIBMEM_ONLY_IMAGE
            uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
            uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif
            P9XipHeader* imgHdr = (P9XipHeader*)(base);
            p9_xip_section_sbe_t sectionName = (control_set - 1) ? P9_XIP_SECTION_SBE_FA_EC_MMA_FAR : P9_XIP_SECTION_SBE_FA_EC_CL2_FAR;
            P9XipSection* pSection = &imgHdr->iv_section[sectionName];
            return ( (uint32_t)base + pSection->iv_offset);
        }

        //Get the FastArray Xip section size basis the control_set
        uint32_t getFastArrayXipSize()
        {
#if defined DFT || defined PIBMEM_ONLY_IMAGE
            uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
            uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif
            P9XipHeader* imgHdr = (P9XipHeader*)(base);
            p9_xip_section_sbe_t sectionName = (control_set - 1) ? P9_XIP_SECTION_SBE_FA_EC_MMA_FAR : P9_XIP_SECTION_SBE_FA_EC_CL2_FAR;
            P9XipSection* pSection = &imgHdr->iv_section[sectionName];
            return (pSection->iv_size);
        }
    } hdr;

    //FIXME:Adding the below pice of code utilises a large amount of stack and
    //ultimately corrupts it.
    //We need to get rid off "req.custom_control_data" array.
    //    uint32_t custom_control_data[SBE_FASTARRAY_MAX_CONTROL_DATA_WORDS];
} sbeControlFastArrayCMD_t;

/**
 * @brief sbeControlFastArrayWrap: write the data Buffer
 *
 * @param[in]  i_getStream  upstream fifo for chip-op /
 *                          memory interface for dump
 * @param[in]  i_putStream  down-stream fifo for chip-op /
 *                          memory interface for dump
 *
 * @return    Rc from the FIFO access utility
 */
uint32_t sbeControlFastArrayWrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                                 fapi2::sbefifo_hwp_data_ostream& i_putStream);

/**
 * @brief Control Fast Array Command (0xA601)
 *
 * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 * @return    RC from the FIFO access utility
 */
uint32_t sbeControlFastArray(uint8_t *i_pArg);

#endif //__SBE_CMDFASTARRAY_H
