/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/boot/loader_l2.c $                                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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

#if 0
// Copy the section to destination address in pibmem
// This is used to copy the data section which is not compressed
int32_t
copySection(P9XipSection * i_section, uint64_t *i_destAddr )
{
    uint32_t rc = 0, i= 0;
    uint32_t dsize = i_section->iv_size;
    uint64_t *srcAddr;

    //dsize should be 8byte aligned
    //assert(!(dsize & 0x7));

    //Source address in the seeprom
    srcAddr = (uint64_t*)(i_section->iv_offset + g_headerAddr);
    while(i < dsize)
    {
        *i_destAddr = *srcAddr;
        i_destAddr++;
        srcAddr++;
        i += sizeof(uint64_t);
    }
    return rc;
}
#endif

// Function to load base image into PIBMEM
int32_t l2_loader()
{
    int32_t rc  = 0;
    uint64_t loadValue = (uint64_t)(SBE_CODE_BOOT_PIBMEM_L2_LOADER_MSG)<<32;
    PPE_STVD(0x50009, loadValue);
    P9XipHeader *hdr = getXipHdr();
    uint64_t *pibMemAddr  = (uint64_t *)g_pibMemAddr;

    loadSection(&(hdr->iv_section[P9_XIP_SECTION_SBE_BASE]), pibMemAddr);

    // If reg 0xc0002040 bit 14 is set then MPIPL
    // Reset is inprogress. In MPIPL reset Only base section should update
    // on PIBMEM and data section should not update.
    // Check sbe_local_LFR struc for  more info
    uint64_t fetchValue = 0;
    PPE_LVD(0xc0002040, fetchValue);
    if (!(fetchValue & 0x0002000000000000ull))
    {
        loadSection(&(hdr->iv_section[P9_XIP_SECTION_SBE_DATA]),
                        (uint64_t *)((uint32_t)hdr->iv_dataAddr));
    }

    // Set the IVPR register. This is required so that interrupt vector table
    // points to pk interfaces.
    uint64_t data = (uint64_t)(SBE_BASE_ORIGIN) << 32;
    PPE_STVD(g_ivprLoc, data);
    // Jump to pk boot function
    uint32_t addr = hdr->iv_kernelAddr;
    JUMP_TO_ADDR(addr);

    return rc;
}
