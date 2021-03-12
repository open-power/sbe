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
#include "sbeTPMaccess.H"
#include "sbeTPMCommand.H"
#include "sbeRoleIdentifier.H"
#include "p10_scom_pibms.H"

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

fapi2::ReturnCode tpmSequenceToReadDIDAndVendor()
{
    #define SBEM_FUNC " tpmSequenceToReadDIDAndVendor "
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

        SBEM_INFO(SBE_FUNC " Read TPM_DID_VID_0 (Vendor and device ID offset xF00)");
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
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmSequenceToAccessLocality0()
{
    #define SBEM_FUNC " tpmSequenceToAccessLocality0 "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = 0x0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;
        uint8_t tpmData = 0x02;
        uint8_t *buf = &tpmData;

        SBEM_INFO(SBE_FUNC "Write 002h to TPM_ACCESS_0 (0000h) to request access to locality 0");
        rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, readBytes, buf);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed with rc 08%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmSequenceToStartup()
{
    #define SBEM_FUNC " tpmSequenceToStartup "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);
        uint8_t tpmStartCommand[64] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x01, 0x44, 0x00, 0x00};
        uint32_t buflen = 64;
        uint32_t cmdLen = 12;
        SBEM_INFO(SBEM_FUNC "Send the startup command to TPM.");
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

fapi2::ReturnCode tpmSequenceToDetectPCRs(bool &pcrAllocation)
{
    #define SBEM_FUNC " tpmSequenceToDetectPCRs "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        uint8_t tpmCapPCR[64] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x01, 0x7A, 0x00,
                                 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40};
        uint8_t tpmPCRAllocated[64] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x03,
                                       0x00, 0x00, 0x00, 0x00, 0x0b, 0x03, 0xff, 0xff, 0xff, 0x00, 0x0c,
                                       0x03, 0x00, 0x00, 0x00};
        uint32_t buflen = 64;
        uint32_t cmdLen = 22;
        SBEM_INFO(SBEM_FUNC "Call TPM_CAP_PCRS ");
        rc = tpmTransmit(handle, (void *)tpmCapPCR, buflen, cmdLen);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmTransmit failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < buflen; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM transmit data is 0x%02X", tpmCapPCR[i]);
            SBEM_INFO(SBEM_FUNC "TPM expected data is 0x%02X", tpmPCRAllocated[i]);
            if(tpmCapPCR[i] != tpmPCRAllocated[i])
            {
                pcrAllocation = false;
                break;
            }
        }
        uint32_t tpmRc = (tpmCapPCR[6] << 24) | (tpmCapPCR[7] << 16) |  (tpmCapPCR[8] << 8) | tpmCapPCR[9];
        SBEM_INFO(SBEM_FUNC "TPM rc is 0x%08X", tpmRc);
        if(tpmRc)
        {
            SBEM_ERROR(SBEM_FUNC " tpmSequenceToDetectPCRs command failed.");
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmSequenceToAllocatePCRs()
{
    #define SBEM_FUNC " tpmSequenceToAllocatePCRs "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        uint8_t tpmAllocatePCR[64] = {0x80, 0x02, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x01, 0x2b, 0x40, 0x00,
                                      0x00, 0x0c, 0x00, 0x00, 0x00, 0x09, 0x40, 0x00, 0x00, 0x09, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x03, 0x00, 0x00,
                                      0x00, 0x00, 0x0b, 0x03, 0xff, 0xff, 0xff, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x00};
        uint32_t buflen = 64;
        uint32_t cmdLen = 49;
        SBEM_INFO(SBEM_FUNC "Call TPM_CAP_PCRS ");
        rc = tpmTransmit(handle, (void *)tpmAllocatePCR, buflen, cmdLen);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmTransmit failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < buflen; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM transmit data is 0x%02X", tpmAllocatePCR[i]);
        }
        uint32_t tpmRc = (tpmAllocatePCR[6] << 24) | (tpmAllocatePCR[7] << 16) |  (tpmAllocatePCR[8] << 8) | tpmAllocatePCR[9];
        SBEM_INFO(SBEM_FUNC "TPM rc is 0x%08X", tpmRc);
        if(tpmRc)
        {
            SBEM_ERROR(SBEM_FUNC " tpmSequenceToAllocatePCRs command failed.");
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode setTPMDeconfigBit()
{
    #define SBEM_FUNC " setTPMDeconfigBit "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        // putscom 0x10005 0x00080000_00000000
        Target<TARGET_TYPE_PROC_CHIP> target =  plat_getChipTarget();
        constexpr uint64_t tpmDeconfigMask = 0x0008000000000000ULL;
        rc = putscom_abs_wrap (&target, OTP_SECURITY_SWITCH, tpmDeconfigMask);
        if(rc)
        {
            SBEM_ERROR(SBEM_FUNC " putscom failed on OTP_SECURITY_SWITCH with rc 0x%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

// TODO: Set the proper response code incase of TPM failure.
// For time being set the scratch register with 0xFF.

fapi2::ReturnCode setTPMFailureRespCode(uint64_t failCode)
{
    #define SBEM_FUNC " setTPMFailureRespCode "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        Target<TARGET_TYPE_PROC_CHIP> target =  plat_getChipTarget();
        rc = putscom_abs_wrap (&target, MAILBOX_SCRATCH_REG_11, failCode);
        if(rc)
        {
            SBEM_ERROR(SBEM_FUNC " putscom failed on MAILBOX_SCRATCH_REG_11 with rc 0x%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode performTPMSequences()
{
    #define SBEM_FUNC " performTPMSequences "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t sbeRole = checkSbeRole();
    do
    {
        if(sbeRole == SBE_ROLE_SLAVE)
        {
            SBEM_INFO(SBE_FUNC "Current Proc is slave. Do not access TPM");
            rc = setTPMDeconfigBit();
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "Failed to set the deconfig bit with rc 0x%08X", rc);
            }
            break;
        }

        SBEM_INFO(SBEM_FUNC "Perfrom TPM sequence to Read Vendor and DeviceID");
        rc = tpmSequenceToReadDIDAndVendor();
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmSequenceToReadDIDAndVendor Failed with rc 0x%08X", rc);
            break;
        }

        SBEM_INFO(SBEM_FUNC "Perfrom TPM sequence to access the TPM locality");
        rc = tpmSequenceToAccessLocality0();
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmSequenceToAccessLocality0 Failed with rc 0x%08X", rc);
            break;
        }

        SBEM_INFO(SBEM_FUNC "Perfrom TPM sequence to Start TPM");
        rc = tpmSequenceToStartup();
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmSequenceToStartup Failed with rc 0x%08X", rc);
            break;
        }

        SBEM_INFO(SBEM_FUNC "Perfrom TPM sequence to detect allocated PCRs");
        bool pcrAllocation = true;
        rc = tpmSequenceToDetectPCRs(pcrAllocation);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmSequenceToDetectPCRs Failed with rc 0x%08X", rc);
            break;
        }

        SBEM_INFO(SBEM_FUNC "PCR Allocation is set to %d", pcrAllocation);
        if(!pcrAllocation)
        {
            SBEM_INFO(SBEM_FUNC "Perfrom TPM sequence to allocate PCRs");
            rc = tpmSequenceToAllocatePCRs();
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmSequenceToAllocatePCRs Failed with rc 0x%08X", rc);
                break;
            }
        }

        if(sbeRole == SBE_ROLE_ALT_MASTER)
        {
            SBEM_INFO(SBEM_FUNC "Poison TPM incase of ALT master");
            rc = tpmPosionPCR();
            if (rc)
            {
                SBEM_ERROR(SBEM_FUNC "tpmPosionPCR failed with rc 0x%08X", rc);
            }
            break;
        }
    }while(0);
    if((sbeRole != SBE_ROLE_SLAVE) && (rc != fapi2::FAPI2_RC_SUCCESS))
    {
        rc = setTPMDeconfigBit();
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "Failed to set the deconfig bit with rc 0x%08X", rc);
        }
        rc = setTPMFailureRespCode(0xFF);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "Failed to set the scratch reg with the response code, rc 0x%08X", rc);
        }

    }
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}
