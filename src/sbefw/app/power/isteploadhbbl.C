/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/isteploadhbbl.C $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2025                             */
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

#include "isteploadhbbl.H"
#include "p10_sbe_spi_cmd.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"
#include "sbetrace.H"
#include "p10_scom_pibms.H"
#include "status_codes.H"
#include "p10_sbe_load_bootloader.H"
#include "base_toc.H"
#include "sbesecuritycommon.H"
#include "sbeOtpromMeasurementReg.H"
#include "tpmStatusCodes.H"
#include "sbeTPMCommand.H"
#include "sbeRoleIdentifier.H"

#define SPI_READ_SIZE_BYTES 4096       //4KBytes SPI Buffer size

const uint32_t regListHbbl[] __attribute__((aligned(8))) = {
    OTPROM_MEASUREMENT_REG12,
    OTPROM_MEASUREMENT_REG13,
    OTPROM_MEASUREMENT_REG14,
    OTPROM_MEASUREMENT_REG15
};

static fapi2::ReturnCode loadHbbl(SB_SETTING_SB_MODES i_sbMode,
                           SHA_DIGEST_t* o_payloadHash,
                           uint32_t i_payloadSize)
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

fapi2::ReturnCode loadAndVerifyHbbl(const SB_SETTING_SB_MODES i_sbMode,
                                    const uint32_t i_hbblPayloadSize,
                                    const uint8_t i_isMpipl)
{
   #define SBE_FUNC " loadAndVerifyHbbl "
   SBE_ENTER(SBE_FUNC)

   fapi2::ReturnCode fapiRc = fapi2::FAPI2_RC_SUCCESS;
   fapi2::ReturnCode tpmFapiRc = fapi2::FAPI2_RC_SUCCESS;
   secureBootStatus_t secureBootStatus;
   SHA_DIGEST_t calPayloadHashHbbl = {0};
   uint64_t *calPayloadHashHbblptr = (uint64_t*)&calPayloadHashHbbl;
   uint64_t *expPayloadHashHbblptr = NULL;
   SHA512truncated_t sha512TruncatedHbbl = {0};
   fapi2::buffer<uint64_t> securityReg;
   uint32_t tpmRespCode = SBEM_TPM_OPERATION_SUCCESSFUL;
   bool resp = false;

    fapiRc = loadHbbl(i_sbMode, &calPayloadHashHbbl, i_hbblPayloadSize);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC " loadHbbl failed with FAPI RC 0x%08x", fapiRc);
    }

    hbbl_SB_data_t *hbbl_SB_data = (hbbl_SB_data_t*)(((base_toc_t*)(SBE_BASE_ORIGIN))->hbbl_SB_data_start);
    expPayloadHashHbblptr = (uint64_t*)&hbbl_SB_data->iv_shPayloadHashHbbl;

    // Verify the SH payload size with loaded pibmem size
    if (hbbl_SB_data->iv_sbeHbblSecureHdrRsp == ROM_DONE)
    {
        SBE_INFO(SBE_FUNC ".hbbl SH verified payload size: %d, "
                        ".hbbl loaded pibmem payload size: %d",
                        hbbl_SB_data->iv_hbblPayloadSize, i_hbblPayloadSize);
        hbbl_SB_data->iv_sbeHbblSecureHdrRsp = verifyPayloadSize( P9_XIP_SECTION_SBE_HBBL,
                                                hbbl_SB_data->iv_hbblPayloadSize,
                                                i_hbblPayloadSize);
    }

    // Verify the SH .hbbl payload hash with calculated payload hash
    if (hbbl_SB_data->iv_sbeHbblSecureHdrRsp == ROM_DONE)
    {
        SBE_INFO(SBE_FUNC "Verifying .hbbl calculated payload hash with secure header payload hash");
        hbbl_SB_data->iv_sbeHbblSecureHdrRsp = verifyPayloadHash( P9_XIP_SECTION_SBE_HBBL,
                                                calPayloadHashHbbl,
                                                hbbl_SB_data->iv_shPayloadHashHbbl);
        //Copy the calculated payload hash into truncated hash to write into measurement reg
        memcpy(&sha512TruncatedHbbl, &calPayloadHashHbbl, sizeof(SHA512truncated_t));
    }
    else
    {
        //Copy 0xff into truncated hash to write into measurement reg in case of SHV fail
        memset(&sha512TruncatedHbbl, 0xFF, sizeof(SHA512truncated_t));
    }

    do
    {

        // Write into measurement regs on a cold IPL only .
        // On MPIPL
        //    1. Skip writing into measurement regs which are one time writable and cleared only on a CBS.
        //    2. Skip extending into TPM as PHYP has TPM lock at Runtime.
        if(!(i_isMpipl))
        {
            //Write HBBL truncated payload hash into otprom register 12-15 (x1001C-x1001F)
            SBE_INFO(SBE_FUNC "Writing truncated HBBL payload hash into otprom register 12-15(x1001C-x1001F)");
            resp = writeandverifytruncatedsha512((uint32_t*) regListHbbl, sizeof(regListHbbl)/sizeof(regListHbbl[0]), sha512TruncatedHbbl);
            if((resp == false) && (hbbl_SB_data->iv_sbeHbblSecureHdrRsp == ROM_DONE))
            {
                UPDATE_ERROR_REG_HBBL(OTP_MEASUREMENT_RWC_MISMATCH);
            }
        }
        else //MPIPL path.
        {
            SBE_INFO("MPIPL path");
            // Incase of hash check fail or size check fail on a MPIPL path, gracefully fail continue MPIPL chipop.
            // SP will create a PEL.
            // NOTE: PHYP dump will be  lost in this case.
            PLAT_FAPI_ASSERT(!((fapiRc == FAPI2_RC_SUCCESS) && (hbbl_SB_data->iv_sbeHbblSecureHdrRsp == ROM_FAILED)),
                                SBE_VERIFY_HBBL_FAIL_IN_MPIPL()
                                .set_CAL_HASH_0(*(calPayloadHashHbblptr + 0))
                                .set_CAL_HASH_1(*(calPayloadHashHbblptr + 1))
                                .set_CAL_HASH_2(*(calPayloadHashHbblptr + 2))
                                .set_CAL_HASH_3(*(calPayloadHashHbblptr + 3))
                                .set_CAL_HASH_4(*(calPayloadHashHbblptr + 4))
                                .set_CAL_HASH_5(*(calPayloadHashHbblptr + 5))
                                .set_CAL_HASH_6(*(calPayloadHashHbblptr + 6))
                                .set_CAL_HASH_7(*(calPayloadHashHbblptr + 7))
                                .set_EXP_HASH_0(*(expPayloadHashHbblptr + 0))
                                .set_EXP_HASH_1(*(expPayloadHashHbblptr + 1))
                                .set_EXP_HASH_2(*(expPayloadHashHbblptr + 2))
                                .set_EXP_HASH_3(*(expPayloadHashHbblptr + 3))
                                .set_EXP_HASH_4(*(expPayloadHashHbblptr + 4))
                                .set_EXP_HASH_5(*(expPayloadHashHbblptr + 5))
                                .set_EXP_HASH_6(*(expPayloadHashHbblptr + 6))
                                .set_EXP_HASH_7(*(expPayloadHashHbblptr + 7))
                                .set_ACTUAL_SIZE(i_hbblPayloadSize)
                                .set_EXP_SIZE(hbbl_SB_data->iv_hbblPayloadSize)
                                .set_SCRATCH_11(secureBootStatus.statusReg),
                                "Failed to verify .hbbl in MPIPL. Failing continue MPIPL chipop");
            break;
        }

        //Check if TPM Deconfig bit is set.
        getscom_abs(0x10005, &securityReg());

        //Skip if error/rc/deconfig bit set in TPM sequence.
        if((!(securityReg.getBit<TPM_DECONFIG_BIT>())))
        {
            //Extend calculated truncated hash of HBBL secure Hdr into PCR0
            SBE_INFO(SBE_FUNC "Extending calculated truncated hash of .hbbl into PCR0");
            tpmFapiRc = tpmExtendPCR(TPM_PCR0, sha512TruncatedHbbl, sizeof(SHA512truncated_t));
            if(tpmFapiRc)
            {
                SBE_ERROR(SBE_FUNC "tpmExtendPCR failed while extending truncated hash of .hbbl into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_HBBL_PAYLOAD_IMAGE_HASH_PCR0_FAILURE;
                break;
            }

            /*************************************************TPM_EXTEND_SEPARATOR*******************************************************/

            //Extend separator into PCR0
            SBE_INFO(SBE_FUNC "Extending separator  TPM_PCR0");
            tpmFapiRc = tpmExtendPCR(TPM_PCR0, SHA256separator, sizeof(SHA512truncated_t));
            if(tpmFapiRc)
            {
                SBE_ERROR(SBE_FUNC "tpmExtendPCR failed while extending separator into PCR0");
                tpmRespCode = SBEV_TPM_EXTEND_SEPERATOR_PCR0_FAILURE;
                break;
            }

            /*************************************************TPM_EXTEND_SEPARATOR_END***************************************************/
        }
    }while(false);

    if((!(i_isMpipl)) &&
    (tpmRespCode != SBEM_TPM_OPERATION_SUCCESSFUL) &&
    (!(securityReg.getBit<TPM_DECONFIG_BIT>())))
    {
        SBE_INFO(SBE_FUNC "Setting the TPM deconfig bit");
        tpmFapiRc = setTPMDeconfigBit();
        if( tpmFapiRc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Failed to set the deconfig bit with rc 0x%08X", tpmFapiRc);
        }

        SBE_INFO(SBE_FUNC "Setting the TPM response code into Scratch Reg 11");
        getscom_abs(MAILBOX_SCRATCH_REG_11, &secureBootStatus.statusReg);
        secureBootStatus.status.tpmStatus = (uint8_t)tpmRespCode;
        putscom_abs(MAILBOX_SCRATCH_REG_11, secureBootStatus.statusReg);
        SBE_INFO("Updated SBE-FW & HBBL secure header status & TPM status into Mailbox scratch 11 [0x%08x 0x%08x]",
                SBE::higher32BWord(secureBootStatus.statusReg), SBE::lower32BWord(secureBootStatus.statusReg));
    }

fapi_try_exit:
   SBE_EXIT(SBE_FUNC)
   return fapi2::current_err;
   #undef SBE_FUNC
}