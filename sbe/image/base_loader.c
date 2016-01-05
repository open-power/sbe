#include "p9_xip_image.h"
#include "sbetrace.H"

int32_t base_loader();


int32_t base_loader() {

    int32_t rc = 0;

    P9XipHeader *hdr = 0;//getXipHdr();
    uint32_t idx;


    for(idx = 0; idx < P9_XIP_SECTIONS; idx++) {
        hdr++;
    }

    return rc;
}

