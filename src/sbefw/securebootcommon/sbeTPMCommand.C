/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbeTPMCommand.C $                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
#include "sbeTPMCommand.H"
#include "sbeTPMaccess.H"
#include "sbemtrace.H"
#include "target.H"
#include "plat_utils.H"
#include "p10_sbe_spi_cmd.H"
#include "p10_scom_pibms.H"

using namespace fapi2;

fapi2::ReturnCode tpmPoisonPCR()
{
    #define SBEM_FUNC " tpmPoisonPCR "
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
        if(tpmRc || buflen < 10)
        {
            SBEM_ERROR(SBEM_FUNC "TPM2_GetRandom response code is non zero.");
            // TODO Handle TPM failure FFDC
            rc = fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;
            break;
        }
        for(uint32_t i = 0; i < 8; i++)
        {
            uint8_t *hashKeys = (tpmGetRN + 12);
            uint8_t size = 32;
            rc = tpmExtendPCR(i, hashKeys, size);
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmExtendPCR failed with rc 08%08X", rc);
                break;
            }
        }

    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmExtendPCR(uint32_t pcrNum, uint8_t *hashKey, uint32_t size)
{
    #define SBEM_FUNC " tpmExtendPCR "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        // PM2_PCR_Extend
        uint8_t tpmExtendPCR[72] = {0x80, 0x02, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x01, 0x82, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x40, 0x00, 0x00, 0x09, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0B};
        // Add 32 byte input hashKey to the tpmExtendPCR after offset 33. 33-64 ---> hashKey
        uint32_t offset = 33;
        uint32_t pcrOffset = 13;
        for(uint32_t i = 0; i < size; i ++)
        {
            SBEM_INFO(SBEM_FUNC "tpmExtendPCR at %d is 0x%02X", i , *(hashKey + i));
            tpmExtendPCR[offset + i] = *(hashKey + i);
        }
        uint32_t buflen = 72;
        uint32_t cmdLen = 65;
        SBEM_INFO(SBEM_FUNC "Extend PCR %d", pcrNum);
        tpmExtendPCR[pcrOffset] = pcrNum;
        uint8_t *seqBytes = tpmExtendPCR;
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
        uint32_t tpmRc = (tpmExtendPCR[6] << 24) | (tpmExtendPCR[7] << 16) |  (tpmExtendPCR[8] << 8) | tpmExtendPCR[9];
        SBEM_INFO(SBEM_FUNC "TPM2_Extend PCR response code is 0x%08X", tpmRc);
        if(tpmRc || buflen < 10)
        {
            SBEM_ERROR(SBEM_FUNC "TPM2_Extend PCR response code is non zero for PCR %d.", pcrNum);
            // TODO Handle TPM failure FFDC
            rc = fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;
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
            // If we are unsuccessful in setting the deconfig bit we are in an
            // untrusted unsecure state, we must halt
            pk_halt();
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}
