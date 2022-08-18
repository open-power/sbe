/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdfastarray.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2022                        */
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
