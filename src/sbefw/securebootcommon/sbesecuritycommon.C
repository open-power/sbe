/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbesecuritycommon.C $              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2025                        */
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
 * @file: ppe/sbe/sbefw/sbesecureboot.C
 *
 * @brief This file contains the SBE Secure Boot Verification Code
 *
 */

#include "sbesecuritycommon.H"
#include "sbetrace.H"
#include "sha512.H"
#include "sbeXipUtils.H"
#include "ppe42_string.h"
#include "plat_hwp_data_stream.H"
#include "p10_sbe_spi_cmd.H"
#include "sbeglobals.H"

#define SPI_READ_SIZE_BYTES 4096       //4KBytes SPI Buffer size for SHA512

void SHA512UpdateXipSection(p9_xip_section_sbe_t xipSection, SHA512_CTX* context, uint64_t* sectionSize)
{
    uint8_t buf[SPI_READ_SIZE_BYTES] __attribute__ ((aligned(8))) = {0x00};
    uint32_t xipSectionSize = getXipSize(xipSection);
    uint32_t xipSectionOffset = getXipOffset(xipSection);
    sbe_local_LFR lfrReg;

    SBE_INFO("Xip section details:Start Offset: [0x%08X] Size: [0x%08X] ", xipSectionOffset, xipSectionSize);

    // Load the LFR, to fetch if secondary boots seeprom is in use.
    PPE_LVD(0xc0002040, lfrReg);

    SBE_INFO("isSecondaryBootsSeeprom [0x%02x]", (uint8_t)lfrReg.sec_boot_seeprom);

    Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
    //If isSecondaryBootsSeeprom = 1 ==> Backup Boots Seeprom
    //   isSecondaryBootsSeeprom = 0 ==> Primary Boots seeprom
    SpiControlHandle handle = SpiControlHandle(i_target_chip,
                          (lfrReg.sec_boot_seeprom ? SPI_ENGINE_BACKUP_BOOT_SEEPROM : SPI_ENGINE_PRIMARY_BOOT_SEEPROM));

    //Calculate SAH512 hash by reading chunks as per buf size
    while(xipSectionSize >= SPI_READ_SIZE_BYTES)
    {
        spi_read(handle, xipSectionOffset, SPI_READ_SIZE_BYTES, DISCARD_ECC_ACCESS, (uint8_t *)&buf);
        SHA512_Update(context, (uint8_t *)&buf, SPI_READ_SIZE_BYTES);
        xipSectionOffset += SPI_READ_SIZE_BYTES;
        xipSectionSize -= SPI_READ_SIZE_BYTES;
        *sectionSize += SPI_READ_SIZE_BYTES;
    }

    //Calculate SHA512 hash of left over bytes
    if(xipSectionSize != 0)
    {
        uint32_t xipSectionSizeActual = xipSectionSize;

        //SPI driver reads 8bytes chunk. If the length passed is not 8 Byte
        //alligned, it will not read that data.
        if(xipSectionSize % 8 != 0 )
            xipSectionSize += (8 - (xipSectionSize % 8));

        spi_read(handle, xipSectionOffset, xipSectionSize, DISCARD_ECC_ACCESS, (uint8_t *)&buf);

        //FIXME: This is a hack, to ensure if length is unaligned pad bytes of
        //the XIP section are zero.
        //Due to issues in IPL image tool .base setion has junk data
        //insted of zero pads. Once tool has been fixed the below piece of code
        //can be removed.
        memset(&buf[xipSectionSizeActual], 0x00, (xipSectionSize - xipSectionSizeActual));

        //Calculate SHA512 for complete data read from SPI as we are aligning data to 8byte by
	    //padding zero's if unaligned during sigining process
        SHA512_Update(context, (uint8_t *)&buf, xipSectionSize);
        *sectionSize += xipSectionSize;
    }
}

void SHA512_XIP_section(p9_xip_section_sbe_t xipSection, SHA512_t *result)
{
    SHA512_CTX context;
    uint64_t sectionSize = 0;

    SHA512_Init(&context);
    SHA512UpdateXipSection(xipSection , &context, &sectionSize);
    SHA512_Final(&context, result);
}

ROM_response verifyPayloadSize( const p9_xip_section_sbe_t i_sections,
                                       uint32_t i_calPayloadSize,
                                       uint32_t i_shPayloadSize)
{
    ROM_response resp = ROM_DONE;
    if (i_calPayloadSize != i_shPayloadSize)
    {
        SBE_ERROR(SBE_FUNC "FAILED : SH verified payload size mismatch"
                            " with loaded pibmem payload size");

        secureBootStatus_t secureBootStatus;
        if (i_sections == P9_XIP_SECTION_SBE_BASE)
        {
            UPDATE_ERROR_REG_SBEFW(PAYLOAD_SIZE_MISMATCH);
        }
        else
        {
            UPDATE_ERROR_REG_HBBL(PAYLOAD_SIZE_MISMATCH);
        }

        resp = ROM_FAILED;
    }
    return resp;
}

ROM_response verifyPayloadHash( const p9_xip_section_sbe_t i_sections,
                                       SHA_DIGEST_t& i_calPayloadHash,
                                       SHA_DIGEST_t& i_shPayloadHash)
{
    ROM_response resp = ROM_DONE;
    if(memcmp(&i_calPayloadHash, &i_shPayloadHash, sizeof(SHA_DIGEST_t)))
    {
        SBE_ERROR(SBE_FUNC "FAILED : invalid payload hash");

        uint32_t * calHash = (uint32_t*) &i_calPayloadHash;
        uint32_t * shHash = (uint32_t*) &i_shPayloadHash;

        SBE_ERROR("Calculated payload hash | Secure header payload hash");
        for (uint8_t i=0; i < (sizeof(SHA_DIGEST_t)/sizeof(uint32_t)); i++)
        {
            SBE_ERROR("        %08x        |        %08x", calHash[i], shHash[i]);
        }

        secureBootStatus_t secureBootStatus;
        if (i_sections == P9_XIP_SECTION_SBE_BASE)
        {
            UPDATE_ERROR_REG_SBEFW(HEADER_HASH_TEST);
        }
        else
        {
            UPDATE_ERROR_REG_HBBL(HEADER_HASH_TEST);
        }

        resp = ROM_FAILED;
    }
    return resp;
}

bool writeandverifytruncatedsha512(uint32_t regs[], int regCount, SHA512truncated_t i_sha512Truncated)
{
    bool resp = true;
    uint64_t hashData = 0x00;
    uint64_t scom_value = 0;
    for(int i=0; i<regCount; i++)
    {
        hashData = 0x00;
        memcpy(&hashData, &i_sha512Truncated[sizeof(uint64_t) * i], sizeof(uint64_t));
        putscom_abs(regs[i], hashData);
        scom_value = 0;
        getscom_abs(regs[i], &scom_value);
        if(scom_value != hashData)
        {
            SBE_ERROR(" Verification of register %08X has failed.", regs[i]);
            SBE_ERROR(" Write data: [0x%08X %08X], read data: [0x%08X %08X]",
                            SBE::higher32BWord(hashData), SBE::lower32BWord(hashData), SBE::higher32BWord(scom_value) , SBE::lower32BWord(scom_value));
            resp = false;
            break;
        }
    }
    return resp;
}