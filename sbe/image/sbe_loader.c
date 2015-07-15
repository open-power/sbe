#include "sbe_xip_image.h"
#include "sbetrace.H"

int32_t sbe_loader() __attribute__ ((section (".loader_text")));


int32_t sbe_loader() {

    int32_t rc = 0;

    SbeXipHeader *hdr = 0;//getXipHdr();
    uint32_t idx;


    for(idx = 0; idx < SBE_XIP_SECTIONS; idx++) {
        
        SBE_TRACE("Section Idx:%u Size:0x%08X", idx, hdr->iv_section[idx].iv_size);
        
    }

    return rc;
}
