/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbeXipUtils.C $                    */
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
/*
 * @file: ppe/sbe/sbefw/core/sbeXipUtils.C
 *
 * @brief This file contains the SBE Secure Boot Verification Code
 *
 */

#include "sbeXipUtils.H"
#include "p10_sbe_spi_cmd.H"
#include "sbeglobals.H"

#define SBE_LFR_REG_ADDR    0xc0002040

void getMSeepromXipHdr(uint8_t * io_xipHeader)
{
    fapi2::ReturnCode fapiRc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t xipOffset = 0;

    // Load the LFR.
    sbe_local_LFR lfrReg;
    uint32_t lfrAddress = SBE_LFR_REG_ADDR;
    PPE_LVD(lfrAddress, lfrReg);

    // Create Spi handle.
    Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
    size_t engine = lfrReg.sec_meas_seeprom ? SPI_ENGINE_BACKUP_MVPD_SEEPROM :
                                       SPI_ENGINE_PRIMARY_MVPD_SEEPROM;

    SpiControlHandle handle = SpiControlHandle(i_target_chip, engine);

    fapiRc = spi_read(handle, xipOffset, sizeof(P9XipHeader), DISCARD_ECC_ACCESS, io_xipHeader);
    if(fapiRc)
    {
        SBE_INFO("spi_read failed with fapiRC 0x%08X for Offset 0x%08X", fapiRc, xipOffset);
        pk_halt();
    }
}

void getXipHdr(uint8_t * io_xipHeader)
{
    fapi2::ReturnCode fapiRc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t xipOffset = 0;

    // Load the LFR.
    sbe_local_LFR lfrReg;
    uint32_t lfrAddress = SBE_LFR_REG_ADDR;
    PPE_LVD(lfrAddress, lfrReg);

    // Create Spi handle.
    Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
    size_t engine = lfrReg.sec_boot_seeprom ? SPI_ENGINE_BACKUP_BOOT_SEEPROM :
                                       SPI_ENGINE_PRIMARY_BOOT_SEEPROM;

    SpiControlHandle handle = SpiControlHandle(i_target_chip, engine);

    fapiRc = spi_read(handle, xipOffset, sizeof(P9XipHeader), DISCARD_ECC_ACCESS, io_xipHeader);
    if(fapiRc)
    {
        SBE_INFO("spi_read failed with fapiRC 0x%08X for offset 0x%08X", fapiRc, xipOffset);
        pk_halt();
    }
}

uint32_t getXipOffset(p9_xip_section_sbe_t xipSection)
{
    uint8_t buf[sizeof(P9XipHeader)] __attribute__ ((aligned(8))) = {0};
    getXipHdr(buf);
    P9XipHeader *imgHdr = (P9XipHeader *)buf;
    p9_xip_section_sbe_t sectionName = xipSection;
    P9XipSection* pSection = &imgHdr->iv_section[sectionName];
    return pSection->iv_offset;
}

uint32_t getXipOffsetAbs(p9_xip_section_sbe_t xipSection)
{
    uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
    return ( (uint32_t)base + getXipOffset(xipSection));
}

uint32_t getXipSize(p9_xip_section_sbe_t xipSection)
{
    uint8_t buf[sizeof(P9XipHeader)] __attribute__ ((aligned(8))) = {0};
    getXipHdr(buf);
    P9XipHeader *imgHdr = (P9XipHeader *)buf;
    p9_xip_section_sbe_t sectionName = xipSection;
    P9XipSection* pSection = &imgHdr->iv_section[sectionName];
    return (pSection->iv_size);
}

uint32_t getXipSizeMeasurement(p9_xip_section_sbe_t xipSection)
{
    uint8_t buf[sizeof(P9XipHeader)] __attribute__ ((aligned(8))) = {0};
    getMSeepromXipHdr(buf);
    P9XipHeader* imgHdr = (P9XipHeader*)buf;
    P9XipSection* pSection = &imgHdr->iv_section[xipSection];
    return (pSection->iv_size);
}

uint32_t getXipOffsetAbsMeasurement(p9_xip_section_sbe_t xipSection)
{
    uint8_t *base = (uint8_t*)(SBE_MEASUREMENT_BASE_ORIGIN);
    uint8_t buf[sizeof(P9XipHeader)] __attribute__ ((aligned(8))) = {0};
    getMSeepromXipHdr(buf);
    P9XipHeader *imgHdr = (P9XipHeader *)buf;
    p9_xip_section_sbe_t sectionName = xipSection;
    P9XipSection* pSection = &imgHdr->iv_section[sectionName];
    return ( (uint32_t)base + pSection->iv_offset);
}
