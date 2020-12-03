/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemTPMSequences.C $                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
#include "sbe_link.H"
#include "sbeXipUtils.H"
#include "sbestates.H"
#include "sbemTPMaccess.H"

using namespace fapi2;

fapi2::ReturnCode initializeTPM()
{
    #define SBEM_FUNC " initializeTPM "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        //Init SPI.
        //TODO:Use SBE method to initilize TPM.
        rc = p10_spi_clock_init(handle);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "p10_spi_clock_init failed with rc 08%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmReadandWriteSequence()
{
    #define SBEM_FUNC " tpmReadandWriteSequence "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = 0x0F00;  // Relative address to read from TPM.
        uint8_t  rbuf[8];
        uint32_t readBytes = 2;

        // Read TPM_DID_VID_0 (Vendor and device ID offset xF00) 
        SBEM_INFO(SBEM_FUNC "Read TPM through SPI.");
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&rbuf);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < readBytes; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM read data is 0x%02X", rbuf[i]);
        }

        // Write 002h to TPM_ACCESS_0 (0000h) to request access to locality 0
        SBEM_INFO(SBEM_FUNC "Write TPM through SPI");
        offsetAddr = 0x0;
        readBytes = 1;
        uint8_t tpmData = 0x02;
        uint8_t *buf = &tpmData;
        rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, readBytes, buf);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed with rc 08%08X", rc);
            break;
        }

        // TPM2_Startup Command(TPM_SU_CLEAR)
        SBEM_INFO(SBEM_FUNC "Send the startup command to TPM.");
        uint8_t tpmStartCommand[64] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x01, 0x44, 0x00, 0x00};
        uint32_t buflen = 64;
        uint32_t cmdLen = 12;
        rc = tpmTransmit(handle, (void *)tpmStartCommand, buflen, cmdLen);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmTransmit failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < buflen; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM transmit data is 0x%02X", tpmStartCommand[i]);
        }
        uint32_t tpmRc = (tpmStartCommand[6] << 24) | (tpmStartCommand[7] << 16) |  (tpmStartCommand[8] << 8) | tpmStartCommand[9];
        SBEM_INFO(SBEM_FUNC "TPM rc is 0x%08X", tpmRc);
        if(tpmRc)
        {
            SBEM_ERROR(SBEM_FUNC "TPM startup command failed.");
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmPosionPCR()
{
    #define SBEM_FUNC " tpmPosionPCR "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        SBEM_INFO(SBEM_FUNC "Call PM2_GetRandom ");
        uint8_t tpmGetRN[64] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x0C,
                                0x00, 0x00, 0x01, 0x7B, 0x00, 0x20};
        uint8_t * seqBytes = tpmGetRN;
        uint32_t buflen = 64;
        uint32_t cmdLen = 12;
        rc = tpmTransmit(handle, (void *)seqBytes, buflen, cmdLen);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmTransmit failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < buflen; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM transmit data is 0x%02X", *(seqBytes + i));
        }
        // Get the command response code.
        uint32_t tpmRc = (tpmGetRN[6] << 24) | (tpmGetRN[7] << 16) |  (tpmGetRN[8] << 8) | tpmGetRN[9];
        SBEM_INFO(SBEM_FUNC "TPM2_GetRandom response code is 0x%08X", tpmRc);
        if(tpmRc)
        {
            SBEM_ERROR(SBEM_FUNC "TPM2_GetRandom response code is non zero.");
            break;
        }
        for(uint32_t i = 0; i < 8; i++)
        {
            uint32_t rnOffset = 12;
            // PM2_PCR_Extend
            uint8_t tpmExtendPCR[72] = {0x80, 0x02, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x01, 0x82, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x40, 0x00, 0x00, 0x09, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0B};
            // Add 32 byte random number to the tpmExtendPCR after offset 33. 33-64 ---> Random number
            uint32_t offset = 33;
            uint32_t pcrOffset = 13;
            for(uint32_t i = 0; i < 32; i ++)
            {
                tpmExtendPCR[offset + i] = tpmGetRN[rnOffset + i];
            }
            buflen = 72;
            cmdLen = 65;
            SBEM_INFO(SBEM_FUNC "Extend PCR %d", i);
            tpmExtendPCR[pcrOffset] = i;
            seqBytes = tpmExtendPCR;
            rc = tpmTransmit(handle, (void *)seqBytes, buflen, cmdLen);
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmTransmit failed with rc 08%08X", rc);
                break;
            }
            for(uint32_t i = 0; i < buflen; i++)
            {
                SBEM_INFO(SBEM_FUNC "TPM transmit data is 0x%02X", *(seqBytes + i));
            }
            // Get the command response code.
            tpmRc = (tpmExtendPCR[6] << 24) | (tpmExtendPCR[7] << 16) |  (tpmExtendPCR[8] << 8) | tpmExtendPCR[9];
            SBEM_INFO(SBEM_FUNC "TPM2_Extend PCR response code is 0x%08X", tpmRc);
            if(tpmRc)
            {
                SBEM_ERROR(SBEM_FUNC "TPM2_Extend PCR response code is non zero for PCR %d.", i);
                break;
            }
        }

    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

uint32_t checkSbeRole()
{
#define SBEM_FUNC " checkSbeRole "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t sbeRole = SBE_ROLE_SLAVE;
    do
    {
        Target<TARGET_TYPE_PROC_CHIP> target =  plat_getChipTarget();
        fapi2::buffer<uint64_t> cbsreg;
        fapi2::buffer<uint64_t> scratchreg8;
        fapi2::buffer<uint64_t> scratchreg6;
        l_rc = getscom_abs_wrap (&target, 0x50004, &cbsreg());
        SBE_INFO(SBE_FUNC "FSXCOMP_FSXLOG_CBS_ENVSTAT_RO value is 0x%08X %08X",
            ((cbsreg >> 32) & 0xFFFFFFFF), (cbsreg & 0xFFFFFFFF));

        //Read Scratch register 8.
        l_rc = getscom_abs_wrap (&target, 0x5003F, &scratchreg8());
        SBE_INFO(SBE_FUNC "SCRTATCH_REG 8 value is 0x%08X %08X",
            ((scratchreg8 >> 32) & 0xFFFFFFFF), (scratchreg8 & 0xFFFFFFFF));

        if(scratchreg8.getBit<5>())
        {
            //Read Scratch Reg 6.
            l_rc = getscom_abs_wrap (&target, 0x5003D, &scratchreg6());
            SBE_INFO(SBE_FUNC "SCRATCH_REG 6 value is 0x%08X %08X",
                      ((scratchreg6 >> 32) & 0xFFFFFFFF), (scratchreg6 & 0xFFFFFFFF));
            if ( scratchreg6.getBit<24>() )
            {
                sbeRole = SBE_ROLE_MASTER;
                if( !((cbsreg >> 32) & 0x8000000) )
                {
                    sbeRole = SBE_ROLE_SLAVE;
                }
            }
            else
            {
               //ALT master
               if( ((cbsreg >> 32) & 0x8000000) )
                {   
                    sbeRole = SBE_ROLE_ALT_MASTER;
                }
            }
        }
        else
        {
            if( (cbsreg >> 32) & 0x8000000 )
            {
                sbeRole = SBE_ROLE_MASTER;
            }
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return sbeRole;
    #undef SBEM_FUNC
}

fapi2::ReturnCode performTPMSequences()
{
    #define SBEM_FUNC " performTPMSequences "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        SBEM_INFO(SBEM_FUNC "Perform Sequence of Read, Write and Startup TPM");

        uint32_t sbeRole = checkSbeRole();
        if(sbeRole == SBE_ROLE_SLAVE)
        {
            SBEM_INFO(SBE_FUNC "Current Proc is slave. Do not access TPM");
            break;
        }

        // Perform TPM operatiions.
        l_rc = tpmReadandWriteSequence();
        if (l_rc)
        {
            SBEM_ERROR(SBEM_FUNC "tpmReadandWriteSequence failed with rc 0x%08X", l_rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "Completed tpmReadandWriteSequence");

        // Poison TPM incase of ALT master.
        if(sbeRole == SBE_ROLE_ALT_MASTER)
        {
            l_rc = tpmPosionPCR();
            if (l_rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmPosionPCR failed with rc 0x%08X", l_rc);
                break;
            }
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return l_rc;
    #undef SBEM_FUNC
}
