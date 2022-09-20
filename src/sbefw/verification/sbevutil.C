/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevutil.C $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021,2022                        */
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

#include "sbevutil.H"
#include "p10_sbe_spi_cmd.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"
#include "sbevtrace.H"
#include "p10_scom_pibms.H"
#include "status_codes.H"

#define SBE_LCL_IVPR                    0xc0000160
#define SBE_SYST_RESET_VECTOR_OFFSET    0x40

extern "C"
{

/*
 *  ** API to jump to the boot seeprom.
 *   */
void jump2boot()
{
    uint64_t data = (uint64_t)SBE_BASE_IMAGE_START << 32;
    PPE_STVD(SBE_LCL_IVPR, data);
    uint32_t bootAddr = SBE_BASE_IMAGE_START + SBE_SYST_RESET_VECTOR_OFFSET;
    asm volatile ( "mr %0, %1" : : "i" (6), "r" (bootAddr) : "memory" );
    asm(
            "mtctr %r6\n"
            "bctr\n"
        );
}

} // end extern "C"

#define SPI_READ_SIZE_BYTES 4096       //4KBytes SPI Buffer size
#define SBE_LFR_REG_ADDR    0xc0002040

enum: uint32_t
{
    RC_INVALID_ARGUMENT = 1,
    RC_PIBMEM_OVERFLOW,

};

void memcpy_byte(void* i_dest, const void* i_src, size_t i_len)
{

    // Loop, copying 1 byte
    uint8_t* cdest = (uint8_t *)i_dest;
    const uint8_t* csrc = (const uint8_t *)i_src;
    size_t i = 0;

    for (; i < i_len; ++i)
    {
        cdest[i] = csrc[i];
    }
}

fapi2::ReturnCode loadSeepromtoPibmem(
                    p9_xip_section_sbe_t i_section,
                    uint32_t& io_startAddr,
                    uint32_t& io_endAddr,
                    uint32_t  i_availSize,
                    uint32_t& io_size,
                    SHA512_CTX* io_context,
                    bool measSection)
{
    #define SBEV_FUNC " loadSeepromtoPibmem "
    SBEV_ENTER(SBEV_FUNC);

    fapi2::ReturnCode fapiRc = fapi2::FAPI2_RC_SUCCESS;
    uint8_t buf[SPI_READ_SIZE_BYTES] __attribute__ ((aligned(8))) = {0};

    do
    {
        SBEV_INFO(SBEV_FUNC "Start Address : 0x%08X End Adddress : 0x%08X Size : 0x%08X"
                            " Available Size is 0x%08X",
                            io_startAddr, io_endAddr, io_size, i_availSize);
        // Populate the Start Address and End Address.
        // There is an assumption that caller will always sent either of the startAddress or endAddress.
        if((io_startAddr == 0) && (io_endAddr == 0))
        {
            SBE_ERROR(SBEV_FUNC " Both io_startAddr and io_endAddr are zero");
            fapiRc = RC_INVALID_ARGUMENT;
            break;
        }
        else if((io_startAddr != 0) && (io_endAddr != 0))
        {
            SBE_ERROR(SBEV_FUNC " Both io_startAddr and io_endAddr are non-zero");
            fapiRc = RC_INVALID_ARGUMENT;
            break;
        }
        else if((io_startAddr & 0x07) || (io_endAddr & 0x07))
        {
            SBE_ERROR(SBEV_FUNC " io_startAddr or io_endAddr is not a multiple of 8");
            fapiRc = RC_INVALID_ARGUMENT;
            break;
        }

        // Source address and Size in Seeprom.
        uint32_t xipSectionOffset = 0;
        if(measSection)
        {
            xipSectionOffset = getXipOffsetAbsMeasurement(i_section) - SBE_MEASUREMENT_BASE_ORIGIN;
        }
        else
        {
            xipSectionOffset = getXipOffset(i_section);
        }
        if(xipSectionOffset == 0)
        {
            SBE_ERROR(SBEV_FUNC " xipSectionOffset is 0. Section passed is %u", i_section);
        }
        if(io_size == 0)
        {
            if(measSection)
            {
                io_size = getXipSizeMeasurement(i_section);
            }
            else
            {
                io_size = getXipSize(i_section);
            }
        }

        if(io_size == 0)
        {
            SBEV_INFO(SBEV_FUNC " Section size is 0. There will be no loading of section %u",
                      i_section);
            io_startAddr = io_startAddr ? io_startAddr : io_endAddr;
            io_endAddr = io_endAddr ? io_endAddr : io_startAddr;
            break;
        }
        uint32_t xipSectionSize = io_size;
        SBEV_INFO(SBEV_FUNC "XIP section size is 0x%08X", xipSectionSize);
        uint64_t padSize = 0;
        if(xipSectionSize % 8)
        {
            padSize = 8 - (xipSectionSize % 8);
            xipSectionSize = xipSectionSize + padSize;
        }

        if(io_endAddr == 0)
        {
            io_endAddr = io_startAddr + xipSectionSize;
        }

        if(io_startAddr == 0)
        {
            io_startAddr = io_endAddr - xipSectionSize;
        }

        if(io_size > i_availSize)
        {
            SBE_ERROR(SBEV_FUNC " There is shortage of space in pibmem. End Address: 0x%08X"
                                " Start Address: 0x%08X Available Size 0x%08X",
                                io_endAddr, io_startAddr, i_availSize);
            fapiRc = RC_PIBMEM_OVERFLOW;
            break;
        }
        SBEV_INFO(SBEV_FUNC "Calculated Start Address : 0x%08X End Address : 0x%08X"
                            " XIP section size is 0x%08X",
                             io_startAddr, io_endAddr, xipSectionSize);

        // Create Spi handle.
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();

        //   isSecondaryBootsSeeprom = 1 ==> Backup Boots Seeprom
        //   isSecondaryBootsSeeprom = 0 ==> Primary Boots seeprom

        // Load the LFR.
        sbe_local_LFR lfrReg;
        uint32_t lfrAddress = SBE_LFR_REG_ADDR;
        PPE_LVD(lfrAddress, lfrReg);
        SBEV_INFO(SBEV_FUNC "isSecondaryBootsSeeprom [0x%02x]", (uint8_t)lfrReg.sec_boot_seeprom);
        SBE_INFO(SBE_FUNC "isSecondaryMeasSeeprom [0x%02x]", (uint8_t)lfrReg.sec_meas_seeprom);

        size_t engine;
        if(measSection)
        {
            engine = lfrReg.sec_meas_seeprom ? SPI_ENGINE_BACKUP_MVPD_SEEPROM :
                                                             SPI_ENGINE_PRIMARY_MVPD_SEEPROM;
        }
        else
        {
            engine = lfrReg.sec_boot_seeprom ? SPI_ENGINE_BACKUP_BOOT_SEEPROM :
                                                             SPI_ENGINE_PRIMARY_BOOT_SEEPROM;
        }

        SpiControlHandle handle = SpiControlHandle(i_target_chip, engine);

        uint32_t *pibmemAddr =(uint32_t *)(io_startAddr);
        while(xipSectionSize > 0)
        {
            SBEV_INFO(SBEV_FUNC " xipSectionOffset is 0x%08X", xipSectionOffset);
            SBEV_INFO(SBEV_FUNC " xipSectionSize is 0x%08X", xipSectionSize);
            uint32_t readSize = 0, actReadSize = 0;
            if(xipSectionSize >= SPI_READ_SIZE_BYTES)
            {
                readSize = SPI_READ_SIZE_BYTES;
            }
            else
            {
                readSize = xipSectionSize;
                actReadSize = readSize - padSize;
            }
            SBEV_INFO(SBEV_FUNC " readSize is 0x%08X", readSize);
            // Perform SPI read to Boot Seeprom.
            fapiRc = spi_read(handle, xipSectionOffset, readSize, DISCARD_ECC_ACCESS, buf);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBEV_ERROR(SBEV_FUNC " Failed to read the Seeprom at address 0x%08X rc=0x%08X",
                                       xipSectionOffset, (uint32_t)fapiRc);
                break;
            }
            if(actReadSize)
            {
                //FIXME: This is a hack, to ensure if length is unaligned pad bytes of
                //the XIP section are zero.
                //Due to issues in IPL image tool .base setion has junk data
                //insted of zero pads. Once tool has been fixed the below piece of code
                //can be removed.
                for(uint32_t i = 0; i < padSize; i++)
                {
                    buf[actReadSize + i] = 0;
                }
            }
            for(uint32_t i = 0 ; i < readSize; i++)
            {
                SBEV_DEBUG(SBEV_FUNC " SPI read is 0x%08X", buf[i]);
            }

            // Calculate SHA512 hash
            if(io_context)
            {
                SHA512_Update(io_context, buf, readSize);
            }

            xipSectionOffset += readSize;
            xipSectionSize -= readSize;

            // Write to Pibmem;
            if(actReadSize)
            {
                readSize = actReadSize;
            }
            memcpy_byte(pibmemAddr, buf, readSize);
            pibmemAddr = pibmemAddr + readSize / 4;
        }
    }while(0);
    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
    return fapiRc;
}

