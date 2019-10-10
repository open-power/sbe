/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/build/utils/base_measurement_header.S $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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

#include "p10_ipl_image.H"
#include "proc_sbe_fixed.H"

    .macro    P9XipHeader, magic, link_address, image_size

    .section .header, "a", @progbits


    //////////////////////////////////////////////////////////////////////
    // Identification - 8-byte aligned; 8 Entries; TOC-Indexed
    //////////////////////////////////////////////////////////////////////

__magic:
        .quad (\magic)
__L1_LoaderAddr:
        .quad 0
__L2_LoaderAddr:
        .quad 0
__kernelAddr:
        .quad 0
__dataAddr:
        .quad 0
__link_address:
        .quad (\link_address)
__header_64_reserved:
        .quad 0, 0

        .xip_toc magic, P9_XIP_UINT64, __magic
        .xip_toc L1_LoaderAddr, P9_XIP_UINT64, __L1_LoaderAddr
        .xip_toc L2_LoaderAddr, P9_XIP_UINT64, __L2_LoaderAddr
        .xip_toc kernelAddr, P9_XIP_UINT64, __kernelAddr
        .xip_toc link_address, P9_XIP_UINT64, __link_address

        //////////////////////////////////////////////////////////////////////
        // Section Table - Not TOC-Indexed
        //////////////////////////////////////////////////////////////////////

        .xip_section header
        .xip_section fixed,                8
        .xip_section fixed_toc,            8
        .xip_section toc,                  4
        .xip_section strings

        .xip_section loader_text,          4
        .xip_section pibrepr_data,         8, empty=1
        .xip_section text,                 4
        .xip_section data,                 8, empty=1
        .xip_section base,                 4
        .xip_section baseloader,           8, empty=1
        .xip_section rings,                8, empty=1
        .xip_section overrides,            8, empty=1
        .xip_section hbbl,                 8, empty=1
        .xip_section fa_ec_cl2_far,        8, empty=1
        .xip_section fa_ec_mma_far,        8, empty=1
        .xip_section ringidfeatlist,       8, empty=1
        .xip_section
        .xip_section
        .xip_section
        .xip_section
        .xip_section
        .xip_section
        .xip_section

        //////////////////////////////////////////////////////////////////////
        // Other Information - 4-byte aligned; 8 entries; TOC-Indexed
        //////////////////////////////////////////////////////////////////////

__image_size:
        .long (\image_size)
__build_date:
        .long 0
__build_time:
        .long 0
__build_tag:
        .asciz "unknown            "                     # 20 Characters

        .xip_toc image_size, P9_XIP_UINT32, __image_size
        .xip_toc build_date, P9_XIP_UINT32, __build_date
        .xip_toc build_time, P9_XIP_UINT32, __build_time
        .xip_toc build_tag,  P9_XIP_STRING, __build_tag


        //////////////////////////////////////////////////////////////////////
        // Other Information - 1-byte aligned; 8 entries; TOC-Indexed
        //////////////////////////////////////////////////////////////////////

__header_version:
        .byte IPL_IMAGE_HEADER_VERSION
__toc_normalized:
        .byte 0
__toc_sorted:
        .byte 0
__header_8_reserved:
        .byte 0, 0, 0, 0, 0

        .xip_toc header_version, P9_XIP_UINT8, __header_version
        .xip_toc toc_normalized, P9_XIP_UINT8, __toc_normalized
        .xip_toc toc_sorted, P9_XIP_UINT8, __toc_sorted

        //////////////////////////////////////////////////////////////////////
        // Strings; 64 characters allocated; TOC-Indexed
        //////////////////////////////////////////////////////////////////////

__build_user:
        .asciz "unknown        "                 # 16 Characters
__build_host:
        .asciz "unknown                        " # 32 characters
__build_head_commit:
        .asciz "unknown "                        #  8 characters
__header_string_reserved:
        .space    8, 0

        .xip_toc build_user,        P9_XIP_STRING, __build_user
        .xip_toc build_host,        P9_XIP_STRING, __build_host
        .xip_toc build_head_commit, P9_XIP_STRING, __build_head_commit

    .endm

    .section .fixed, "a", @progbits
    .section .fixed_toc, "a", @progbits
    .section .pibrepr_data, "a", @progbits
    .section .loader_text, "a", @progbits

    .section .toc, "a", @progbits
    .section .strings, "aS", @progbits
    .section .base, "a", @progbits

    P9XipHeader P9_XIP_MAGIC_SEEPROM, SBE_MEASUREMENT_BASE_ORIGIN, _sbe_image_size

    // Create the .fixed section
    .proc_sbe_fixed_system
