#include "sbeXipUtils.H"

// Load section to destination address
int32_t loadSection( P9XipSection * i_section, uint64_t *i_destAddr )
{
    uint32_t idx = 0;
    uint64_t *seepromAddr = (uint64_t *)( g_headerAddr + i_section->iv_offset);
    uint32_t sectionSize  = i_section->iv_size;
    for( idx = 0; idx < sectionSize; idx += 8 )
    {
        *i_destAddr = *seepromAddr;
        i_destAddr++; seepromAddr++;

    }
    return 0;
}

// Function to load base image into PIBMEM
int32_t base_loader()
{
    int32_t rc  = 0;
    P9XipHeader *hdr = getXipHdr();

    uint64_t *pibMemAddr  = (uint64_t *)g_pibMemAddr;
    loadSection(&(hdr->iv_section[P9_XIP_SECTION_BASE]), pibMemAddr);
    // Set the IVPR register. This is required so that interrupt vector table
    // points to pk interfaces.
    uint64_t data = (uint64_t)(SBE_BASE_ORIGIN) << 32;
    PPE_STVD(g_ivprLoc, data);
    // Jump to pk boot function
    uint32_t addr = hdr->iv_kernelAddr;
    JUMP_TO_ADDR(addr);

    return rc;
}
