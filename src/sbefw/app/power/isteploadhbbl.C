/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/isteploadhbbl.C $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2025                             */
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

#include "isteploadhbbl.H"
#include "p10_sbe_spi_cmd.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"
#include "sbetrace.H"
#include "p10_scom_pibms.H"
#include "status_codes.H"
#include "p10_sbe_load_bootloader.H"

#define SPI_READ_SIZE_BYTES 4096       //4KBytes SPI Buffer size
fapi2::ReturnCode loadHbbl(SB_SETTING_SB_MODES i_sbMode,
                           SHA_DIGEST_t* o_payloadHash
                                     )
{
    #define SBE_FUNC " loadHbbl "
    SBE_ENTER(SBE_FUNC);

    fapi2::ReturnCode fapiRc = fapi2::FAPI2_RC_SUCCESS;
    uint8_t buf[SPI_READ_SIZE_BYTES] __attribute__ ((aligned(8))) = {0};

    /* Global SHA context's */
    /* Union struct used to optimize the stack */
    typedef union
    {
        SHA512_CTX sha512Ctx;
        sha3_ctx_t sha3Ctx;
    }sha_t;

    sha_t sha __attribute__ ((aligned(8)));
    if (o_payloadHash)
    {
        if (i_sbMode == SB_MODE_V1)
        {
            SHA512_Init(&sha.sha512Ctx);
        }
        else if(i_sbMode == SB_MODE_V3)
        {
            sha3_init(&sha.sha3Ctx);
        }
    }

    // Source address and Size in Seeprom.
    uint32_t xipSectionOffset = getXipOffset(P9_XIP_SECTION_SBE_HBBL);
    uint32_t xipSectionSize = getXipSize(P9_XIP_SECTION_SBE_HBBL);;
    SBE_INFO(SBE_FUNC " .hbbl xipSectionOffset is 0x%08X, xipSectionSize is 0x%08X", xipSectionOffset, xipSectionSize);

    uint8_t padSize = 0;
    if(xipSectionSize % 8)
    {
        padSize = 8 - (xipSectionSize % 8);
        xipSectionSize = xipSectionSize + padSize;
    }

    // Create Spi handle.
    Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();

    //   isSecondaryBootsSeeprom = 1 ==> Backup Boots Seeprom
    //   isSecondaryBootsSeeprom = 0 ==> Primary Boots seeprom

    // Load the LFR.
    sbe_local_LFR lfrReg;
    uint32_t lfrAddress = SBE_LFR_REG_ADDR;
    PPE_LVD(lfrAddress, lfrReg);
    SBE_INFO(SBE_FUNC "isSecondaryBootsSeeprom [0x%02x]", (uint8_t)lfrReg.sec_boot_seeprom);

    size_t engine;
    engine = lfrReg.sec_boot_seeprom ? SPI_ENGINE_BACKUP_BOOT_SEEPROM :
                                            SPI_ENGINE_PRIMARY_BOOT_SEEPROM;

    SpiControlHandle handle = SpiControlHandle(i_target_chip, engine);

    // Get master Core
    uint8_t coreId = 0;
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,proc,coreId);
    fapi2::Target<fapi2::TARGET_TYPE_CORE >
    coreTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(coreId));

    while(xipSectionSize > 0)
    {
        SBE_INFO(SBE_FUNC " xipSectionOffset is 0x%08X, xipSectionSize is 0x%08X", xipSectionOffset, xipSectionSize);
        uint32_t readSize , actReadSize ;
        readSize = 0;
        actReadSize = 0;
        if(xipSectionSize >= SPI_READ_SIZE_BYTES)
        {
            readSize = SPI_READ_SIZE_BYTES;
        }
        else
        {
            readSize = xipSectionSize;
            actReadSize = readSize - padSize;
        }
        SBE_INFO(SBE_FUNC " readSize is 0x%08X", readSize);

        // Perform SPI read to Boot Seeprom.
        fapiRc = spi_read(handle, xipSectionOffset, readSize, DISCARD_ECC_ACCESS, buf);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC " Failed to read the Seeprom at address 0x%08X rc=0x%08X",
                                    xipSectionOffset, (uint32_t)fapiRc);
            break;
        }

        if(actReadSize)
        {
            //FIXME: This is a hack, to ensure if length is unaligned pad bytes of
            //the XIP section are zero.
            //Due to issues in IPL image tool .base section has junk data
            //insted of zero pads. Once tool has been fixed the below piece of code
            //can be removed.
            memset(&buf[actReadSize],0x00,padSize);
        }

        if (o_payloadHash)
        {
            if (i_sbMode == SB_MODE_V1)
            {
                SHA512_Update(&sha.sha512Ctx, buf, readSize);
            }
            else if (i_sbMode == SB_MODE_V3)
            {
                sha3_update(&sha.sha3Ctx, buf, readSize);
            }
        }

        // Write HBBL in chunks
        SBE_EXEC_HWP(fapiRc, p10_sbe_load_bootloader, proc, coreTgt, readSize, buf)
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(" p10_sbe_load_bootloader failed while loading offset 0x%08x size 0x%08x", xipSectionOffset, xipSectionSize);
            break;
        }

        xipSectionOffset += readSize;
        xipSectionSize -= readSize;
    }

    // Calculate SHA hash final
    if (o_payloadHash)
    {
        if (i_sbMode == SB_MODE_V1)
        {
            SHA512_Final(&sha.sha512Ctx, (SHA512_t *)o_payloadHash);
        }
        else if (i_sbMode == SB_MODE_V3)
        {
            sha3_final((sha3_t *)o_payloadHash, &sha.sha3Ctx);
        }
        SBE_INFO(SBE_FUNC " Sha final done");
    }

    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
    return fapiRc;
}
