/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/loader_l2.c $                                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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
#include "sbeXipUtils.H"
#include "sbeDecompression.h"

// Load section to destination address
int32_t loadSection( P9XipSection * i_section, uint64_t *i_destAddr )
{
    uint32_t rc = 0;
    do {
         uint8_t *seepromAddr = (uint8_t *)( g_headerAddr + i_section->iv_offset);
         uint8_t rc = decompress(seepromAddr, (uint8_t *)i_destAddr);

         if (rc != 0 )
           break;
       } while(0);

    return rc;
}

// Function to load base image into PIBMEM
int32_t l2_loader()
{
    int32_t rc  = 0;
    uint64_t loadValue = (uint64_t)(SBE_CODE_BOOT_PIBMEM_L2_LOADER_MSG)<<32;
    PPE_STVD(0x50009, loadValue);
    P9XipHeader *hdr = getXipHdr();

    uint64_t *pibMemAddr  = (uint64_t *)g_pibMemAddr;
    uint64_t *tempPibMemAddr  = (uint64_t *)g_pibMemAddr;
    uint64_t *pibEndAddr  = (uint64_t *)(SBE_LOADER_BASE_ORIGIN);

    // Initialise complete PIBMEM with 0
    // Though this is inefficient, this is quick fix to solve the
    // HW write of data which is not 8 byte alligned. All write which
    // are not 8 byte alligned, translates to read-modify-write. So
    // if code does a write for 4 byte, first HW will read 8 bytes. If
    // there is no data written there earlier and random data is present
    // it can cause ecc errors.
    // TODO via RTC 158797
    // Though we may be able to optimise it, but than we have to put the
    // logic in loadSection function which is not desired. So currently
    // initialising all PIBMEM. If performance become concern during initial
    // SBE load, we can optimise this.
    // We have three options here in optimisation
    //   1.  Put this logic in SEEPROM loader ( l1loader ). This was we can
    //       reclaim space taken by l2 loader as stack/bss can grow in base
    //       loader section after image load.
    //   2.  Keep this code in PIBMEM. But do not initialise the memory taken
    //       by base image.
    //   3.  Do what is done in option 1 but only initialise memoty after base
    //       section to end of PIBMEM. This will involve some more hardcoding
    //       in seeprom loader.
    // My preference will be option 1 but depends upon performance goals.
    for( ; tempPibMemAddr < pibEndAddr; tempPibMemAddr++ )
    {
        *tempPibMemAddr = 0;
    }

    loadSection(&(hdr->iv_section[P9_XIP_SECTION_SBE_BASE]), pibMemAddr);
    // Set the IVPR register. This is required so that interrupt vector table
    // points to pk interfaces.
    uint64_t data = (uint64_t)(SBE_BASE_ORIGIN) << 32;
    PPE_STVD(g_ivprLoc, data);
    // Jump to pk boot function
    uint32_t addr = hdr->iv_kernelAddr;
    JUMP_TO_ADDR(addr);

    return rc;
}
