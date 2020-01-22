/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/p10_sbe_spi_cmd_test.C $                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
 * @file: p10_sbe_spi_cmd.C
 *
 * @brief This file contains and handle for SPI operations
 *
 */

#include "p10_sbe_spi_cmd.H"
#include "sbemtrace.H"
#include "endian.h"
#include "sbeutil.H"

using namespace fapi2;

fapi2::ReturnCode
verify_read_data(uint32_t offset, const uint32_t len, const char * data)
{
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    //Verify the content using sideband
    char *base = (char*)(0xFF800000 + offset);
    for(uint32_t i = 0; i < len; i++)
    {
        if (data[i]  != *base)
        {
            SBEM_ERROR(SBEM_FUNC" Read data verify failed");
            return FAPI2_RC_FALSE;
        }
        base++;
        offset++;
    }
    /*
    for (uint32_t i = 0; i < len; i += 8)
    {
        uint64_t temp1 = (uint64_t*)data[i / 8];
        SBEM_INFO("spi read -- [0x%08X %08X]", SBE::higher32BWord(temp1),SBE::lower32BWord(temp1));
        uint64_t temp2 = (uint64_t*)base[i / 8];
        SBEM_INFO("spi side band read -- [0x%08X %08X]", SBE::higher32BWord(temp2),SBE::lower32BWord(temp2));
    }*/
    return rc;
}

static int
verify_write_data(uint32_t offset, const uint32_t len, const char * data)
{
#define SBEM_FUNC "verify_write_data"
    SBEM_ENTER(SBEM_FUNC);
    //Verify the content using sideband
    char *base = (char*)(0xFF800000);
    uint32_t i;
    uint32_t boffset;

    /*Adjust the offset for ecc*/
    boffset = 8*(int)(offset/9) + offset%9;
    base  = base + boffset;

    for(i = 0; i < len; i++)
    {
        if ((offset != 0)  && ((offset % 9) == 8)) {
            //skipp ecc..we can't verify that through sideband
            i++;
            offset++;
            if (i == len)
                break;
        }

        if (data[i]  != *base)
            {
              SBEM_INFO(SBEM_FUNC" data[i]: %d base: %d", data[i], *base);
              return -1;
            }
        SBEM_DEBUG(SBEM_FUNC" data[i]: 0x%02X :: base: 0x%02X", data[i], *base);
        base++;
        offset++;
    }
    SBEM_EXIT(SBEM_FUNC);
    return 0;
#undef SBEM_FUNC
}


fapi2::ReturnCode
spi_read_verify(SpiControlHandle &handle, uint32_t address, uint32_t len)
{
#define SBEM_FUNC "spi_read_verify"
    uint64_t buf[40]; //Hope this doesn't overshoot the stack. Can be used for 320 bytes of read
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        if (len > sizeof(buf))
        {
            SBEM_ERROR(SBEM_FUNC" Read size is too long to verify");
            rc = fapi2::FAPI2_RC_INVALID_PARAMETER;
            break;
        }
        SBEM_INFO(SBEM_FUNC" Read Start from offset: %d len: %d", address, len);
        rc = spi_read(handle, address, len, STANDARD_ECC_ACCESS, (uint8_t *)&buf);
        if(rc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBEM_ERROR(SBEM_FUNC" ::spi_read failed for offset: %d len: %d", address, len);
            break;
        }
        rc = verify_read_data(address, len, (char*)&buf);
        if(rc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBEM_ERROR(SBEM_FUNC" ::verify_read_data failed for offset: %d len: %d", address, len);
            break;
        }
        SBEM_INFO(SBEM_FUNC" PASSED for offset: %d len: %d", address, len);
    } while (false);
#undef SBEM_FUNC
    return rc;
}

fapi2::ReturnCode
spi_write_verify(SpiControlHandle &handle, uint32_t address, uint32_t len)
{
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    return rc;
    uint64_t buf[40];
    uint64_t rbuf[40];
  
    uint64_t pattern, step;
    if (len > sizeof(buf))
    {
        SBEM_ERROR(SBEM_FUNC" Write size is too long to verify");
        return fapi2::FAPI2_RC_INVALID_PARAMETER;
    }
    rc = spi_read(handle, address, len, STANDARD_ECC_ACCESS, (uint8_t *)&rbuf);
    if ((rc!= fapi2::FAPI2_RC_SUCCESS) || verify_read_data(address, len, (char*)&rbuf))
    {
        SBEM_ERROR(SBEM_FUNC" original data spi_read was failed for offset: %d len: %d", address, len);
        return rc;
    }
    pattern = 0x1122334455667788ULL;
    step = 0x0102030405060708ULL;

    for(uint32_t i=0; i < sizeof(buf)/sizeof(uint64_t); i++)
    {
        buf[i]  = pattern;
        pattern += step;
    }
    rc = spi_write(handle,address, len, (uint8_t *)&buf);
    if (rc || verify_write_data(address, len, (char*)&buf)) {
        SBEM_ERROR("spi_write: failed for offset: %d len: %d", address, len);
        return rc;
    }
    SBEM_INFO("passed for offset: %d len: %d", address, len);
    rc = spi_write(handle,address, len, (uint8_t *)&rbuf);
    if (rc || verify_write_data(address, len, (char*)&rbuf))
    {
        SBEM_ERROR("spi_write: rewrite failed for offset: %d len: %d", address, len);
        return rc;
    }
    return rc;
}

void
spi_test()
{
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    uint8_t spi_engine  = 0;
    Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
    SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine );
    rc = spi_read_verify(handle, 0, 256);
    if( rc != fapi2::FAPI2_RC_SUCCESS )
    {
        SBEM_ERROR("spi_test FAILED while spi_read_verify return error");
        pk_halt();
        return;
    }
    // #define PASS_SPI_TEST 0XF5
    uint64_t loadValue = (uint64_t)(0xF5)<<32;
    PPE_STVD(0x50009, loadValue);
    SBEM_INFO("PASSED spi_test with HW ");
 /*
    spi_read_verify(handle, 0, 8);
    spi_read_verify(handle, 8, 8);
    spi_read_verify(handle, 8, 16);
    spi_read_verify(handle, 0, 80);
    spi_read_verify(handle, 8, 80);
    spi_read_verify(handle, 248, 8);
    spi_read_verify(handle, 248, 16);
    spi_read_verify(handle, 248, 80);
    spi_read_verify(handle, 256, 8);
    spi_read_verify(handle, 256, 16);
    spi_read_verify(handle, 256, 80);
    spi_read_verify(handle, 0, 64);
    spi_read_verify(handle, 256, 128);
    spi_read_verify(handle, 256, 256);
  */  
    spi_write_verify(handle, 0, 8);
    /* TODO:: spi_engine 1 and 3 are not at configured.
    spi_write_verify(handle, 0, 16);
    spi_write_verify(handle, 256, 16);
    spi_write_verify(handle, 240, 280);
    spi_write_verify(handle, 240, 16);
    spi_write_verify(handle, 256, 256);
    spi_write_verify(handle, 240, 256);
*/
}

