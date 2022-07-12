/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevutil.H $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021,2022                        */
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
#ifndef SBEV_UTIL_H
#define SBEV_UTIL_H

#include "fapi2.H"
#include "sha512.H"

#ifdef __cplusplus
extern "C" {
#endif

#include "pk.h"
#include "pk_api.h"

extern void jump2boot();

#ifdef __cplusplus
}
#endif

#define ALIGN_8_BYTE_FLOOR(x)   ((x) & 0xFFFFFFF8)
#define ALIGN_8_BYTE_CIELING(x) ((x + 7) & 0xFFFFFFF8)

///
/// @brief Copy a memory block from source to destination.
///
/// @param[in]          i_dest    Pointer to destination block.
/// @param[in]          i_src     Pointer to source block.
/// @param[in]          i_len     Size of bytes to be copied.
///
/// @return void
///
void memcpy_byte(void* i_dest, const void* i_src, size_t i_len);

///
/// @brief  Load the SEEPROM content to PIBMEM.
///
/// @param[in]          i_section     Xip section of Seeprom to be loaded into PIBMEM.
/// @param[in, out]     io_startAddr  Start address in PIBMEM.
/// @param[in, out]     io_endAddr    End address in PIBMEM.
/// @param[in, out]     i_availSize   Available Size in PIBMEM.
/// @param[in, out]     io_size       Size of section in PIBMEM.
/// @param[in, out]     io_context    SHA512 context struct pointer used during SHA512 Init
/// @param[in]          i_measSection measurement Section to be loaded. By Default false.
///
/// @return FAPI_RC_SUCCESS if the spi master read completes successfully,
///         else error code.
///
fapi2::ReturnCode loadSeepromtoPibmem(
                    p9_xip_section_sbe_t i_section,
                    uint32_t& io_startAddr,
                    uint32_t& io_endAddr,
                    uint32_t  i_availSize,
                    uint32_t& io_size,
                    SHA512_CTX* io_context,
                    bool i_measSection = false);

#endif //SBEV_UTIL_H
