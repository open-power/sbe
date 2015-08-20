#include "sbe_xip_image.h"
#include "sbetrace.H"

int32_t base_loader();


int32_t base_loader() {

    int32_t rc = 0;

    SbeXipHeader *hdr = 0;//getXipHdr();
    uint32_t idx;


    for(idx = 0; idx < SBE_XIP_SECTIONS; idx++) {
        hdr++;
    }

    return rc;
}

