/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbesecureboot.C $                      */
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
 * @file: ppe/sbe/sbefw/sbesecureboot.C
 *
 * @brief This file contains the SBE Secure Boot Verification Code
 *
 */

#include "sbesecuritycommon.H"
#include "sbesecureboot.H"
#include "sbevtrace.H"
#include "ecverify.H"
#include "ppe42_string.h"
#include "fapi2.H"
#include "sbeXipUtils.H"
#include "sbeutil.H"
#include "sbevutil.H"
#include "p10_scom_pibms.H"

#define VERIFY_FAILED(_c) { params->log=ERROR_EVENT|CONTEXT|(_c); \
                            return ROM_FAILED; }

#define HBBL_SECURE_HDR_COMPONENT_ID 0x4842424C00000000Ull      //Component ID:HBBL
#define FW_SECURE_HDR_COMPONENT_ID   0x5342455f46570000Ull      //Component ID:SBE_FW
#define VERIFY_SW_SIG_P              0                          //Verify SW Signature P incase of both HBBL and SBE_FW secure Hdr.

using namespace fapi2;

inline uint64_t get64(void* src)
{
    uint64_t dest;
    memcpy_byte(&dest, src,sizeof(uint64_t));
    return dest;
}

inline uint32_t get32(void* src)
{
    uint32_t dest;
    memcpy_byte(&dest, src,sizeof(uint32_t));
    return dest;
}

inline uint16_t get16(void* src)
{
    uint16_t dest;
    memcpy_byte(&dest, src,sizeof(uint16_t));
    return dest;
}

inline uint8_t get8(void* src)
{
    uint8_t dest;
    memcpy_byte(&dest, src,sizeof(uint8_t));
    return dest;
}

static int valid_ver_alg(ROM_version_raw* ver_alg, uint8_t sig_alg)
{
    #define SBEV_FUNC " valid_ver_alg "
    SBEV_ENTER(SBEV_FUNC);

    //Validate header version
    SBEV_INFO("Hdr: Version : %d", get16(&ver_alg->version));
    if(get16(&ver_alg->version) != HEADER_VERSION)
    {
        SBEV_ERROR(SBEV_FUNC "FAILED: bad header version");
        return 0;
    }

    //Validate header hash algo version
    SBEV_INFO("Hdr: hash algo : %d", get8(&ver_alg->hash_alg));
    if(get8(&ver_alg->hash_alg) != HASH_ALG_SHA512)
    {
        SBEV_ERROR(SBEV_FUNC "FAILED: bad algorithm version");
        return 0;
    }

    if (!sig_alg)
    {
        return 1;
    }

    //Validate header sign algo version
    SBEV_INFO("Hdr: Sign Algo : %d", get8(&ver_alg->sig_alg));
    if(get8(&ver_alg->sig_alg) != sig_alg)
    {
        SBEV_ERROR(SBEV_FUNC "FAILED: bad algorithm version");
        return 0;
    }

    SBEV_EXIT(SBEV_FUNC);
    return 1;
    #undef SBEV_FUNC
}

static int valid_ecid(int ecid_count, uint8_t* ecids, uint8_t* hw_ecid)
{
    #define SBEV_FUNC " valid_ecid "
    SBEV_ENTER(SBEV_FUNC);

    if (ecid_count == 0)
    {
        return 1;
    }

    SBEV_EXIT(SBEV_FUNC);
    return 0;
    #undef SBEV_FUNC
}

static int multi_key_verify(uint8_t* digest, int key_count, uint8_t* keys,
        uint8_t* sigs, int sig_to_verify)
{
    #define SBEV_FUNC " multi_key_verify "
    SBEV_ENTER(SBEV_FUNC);

    int no_of_keys = key_count;
    bool found_key = false;
    for (;key_count;key_count--,keys+=sizeof(ecc_key_t),
            sigs+=sizeof(ecc_signature_t))
    {
        if((no_of_keys - key_count) == sig_to_verify)
        {
            found_key = true;
            SBEV_INFO("Verifying signature: %d",(no_of_keys-key_count));
            if(!SBE::isSimicsRunning())
            {
                if (ec_verify (keys, digest, sigs)<1)
                {
                    SBEV_INFO(SBEV_FUNC "Entering EC Verify Failed");
                    return 0;
                }
                SBEV_INFO(SBEV_FUNC "EC Verify Passed");
            }
            else
            {
                SBEV_INFO("Skipping ECDSA - Simics running");
            }
        }
        else
        {
            SBEV_INFO("Skiping verification of signature: %d",(no_of_keys-key_count));
        }
    }

    if (!found_key)
    {
        SBEV_INFO("We didn't find a key to verify, this isn't good: %d",sig_to_verify);
        return 0;
    }
    SBEV_EXIT(SBEV_FUNC);
    return 1; // This is return success
    #undef SBEV_FUNC
}

static void populateHWParams(ROM_hw_params* params)
{
    #define SBEV_FUNC " populateHWParams "
    SBEV_ENTER(SBEV_FUNC);

    SBEV_INFO("SBE Settings(HW Key Hash):Start Offset: [0x%08X] Size: [0x%08X] ", getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS), getXipSize(P9_XIP_SECTION_SBE_SB_SETTINGS));

    //Populate params struct
    //Get HW key hash from .sb_settings
    uint32_t start_address = (uint32_t)params->hw_key_hash;
    uint32_t endAddress = 0; // dummy variable to keep loadSeepromtoPibmem happy
    uint32_t size = SHA512_DIGEST_LENGTH;
    uint32_t fapiRc = loadSeepromtoPibmem(P9_XIP_SECTION_SBE_SB_SETTINGS, start_address, endAddress, SHA512_DIGEST_LENGTH, size, NULL);
    if(fapiRc)
    {
        SBEV_INFO(SBEV_FUNC "loadSeepromtoPibmem failed with rc 0x%08X for start address:"
                            "0x%08X end Address: 0x%08X", start_address, endAddress);
        pk_halt();
    }
    //Get MSV from .sb_settings
    params->log = get8((uint8_t *)(getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS) + SHA512_DIGEST_LENGTH)); // single sideband access

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

/**
 * @brief Verify Secure container.
 *
 * @param ROM_container_raw* Pointer to secure container start address
 * @param ROM_hw_params*     Pointer to HW Keys Hash
 * @param hw_sig_to_verify   The hardware signature that has to be verified(Among the 3 HW signaturs that are present)
 * @param *payload_hash      calculated payload hash to verify with signature (SBE_FW or HBBL Payload hash)
 * @param *payload_size      SBE_FW or HBBL Payload size
 * @param flag Prefix Hdr    flag
 *
 * @return Secure container verification response.
 */
static ROM_response ROM_verify( ROM_container_raw* container,
                         ROM_hw_params* params,
                         int hw_sig_to_verify,
                         SHA512_t* payload_hash,
                         uint64_t payload_size,
                         uint32_t *flag)
{
    #define SBEV_FUNC " ROM_verify "
    SBEV_ENTER(SBEV_FUNC);

    ROM_prefix_header_raw *prefix;
    ROM_prefix_data_raw* hw_data;
    ROM_sw_header_raw* header;
    ROM_sw_sig_raw* sw_sig;
    SHA512_t digest;
    //NOTE: Keep the array size 8 byte aligned to overcome sram allignment issues.
    //396 bytes is MAX hash we calculate and hence buffer size is 400 bytes.
    const uint32_t hashDataBuffSize = sizeof(uint64_t) * 50;
    uint8_t hashDataBuff[hashDataBuffSize]  __attribute__ ((aligned(8))) = {0x00};
    uint64_t size;
    uint8_t swKeyCount;
    uint64_t sectionSize = 0;
    SBEV_INFO(SBEV_FUNC "payload_size=%u", payload_size);

    // params.log is used to pass in a FW Secure Version to
    // compare against the container's sw header's fw_secure_version field
    uint8_t i_fw_secure_version = static_cast<uint8_t>(params->log);

    params->log = CONTEXT|BEGIN;

    //Validate Magic Number
    SBEV_INFO("Magic number: 0x%X", get32(&container->magic_number));
    if(!(get32(&container->magic_number) == ROM_MAGIC_NUMBER))
    {
        SBEV_ERROR (SBEV_FUNC "FAILED : bad container magic number");
        VERIFY_FAILED(MAGIC_NUMBER_TEST);
    }

    //Validate Container Version
    SBEV_INFO("Container Version: 0x%X", get16(&container->version));
    if(!(get16(&container->version) == CONTAINER_VERSION))
    {
        SBEV_ERROR (SBEV_FUNC "FAILED : bad container version");
        VERIFY_FAILED(CONTAINER_VERSION_TEST);
    }

    //Process HW Keys and verify HW keys Hash
    memcpy_byte(hashDataBuff, &container->hw_pkey_a, HW_KEY_COUNT*sizeof(ecc_key_t));
    SHA512_Hash(hashDataBuff, HW_KEY_COUNT*sizeof(ecc_key_t), &digest);
    if(memcmp(params->hw_key_hash, digest, sizeof(SHA512_t)))
    {
        SBEV_ERROR (SBEV_FUNC "FAILED : invalid hw keys");
        VERIFY_FAILED(HW_KEY_HASH_TEST);
    }

    // process prefix header
    prefix = (ROM_prefix_header_raw*)&container->prefix;
    // test for valid header version, hash & signature algorithms (sanity check)
    if(!valid_ver_alg(&prefix->ver_alg, SIG_ALG_ECDSA521))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : bad prefix header version,alg's");
        VERIFY_FAILED(PREFIX_VER_ALG_TEST);
    }

    // test for valid prefix header signatures (all)
    hw_data = (ROM_prefix_data_raw*)(prefix->ecid + get8(&prefix->ecid_count)*ECID_SIZE);

    // Validate the PREFIX_HEADER_SIZE fits in our hashDataBuff
    if (hashDataBuffSize < PREFIX_HEADER_SIZE(prefix))
    {
        VERIFY_FAILED(PREFIX_HEADER_SZ_TEST);
    }

    //Calculate Hash of prefix header
    memcpy_byte(hashDataBuff, prefix, PREFIX_HEADER_SIZE(prefix));
    SHA512_Hash(hashDataBuff, PREFIX_HEADER_SIZE(prefix), &digest);

    //Verify HW signatures a if HBBL secure hdr and HW signature c if SBE-FW secure hdr
    if(!multi_key_verify(digest, HW_KEY_COUNT, container->hw_pkey_a,
                                  hw_data->hw_sig_a,hw_sig_to_verify))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : invalid hw signature");
        VERIFY_FAILED(HW_SIGNATURE_TEST);
    }

    SBEV_INFO("Prefix Hdr: code start offset : %d", get64(&prefix->code_start_offset));
    SBEV_INFO("Prefix Hdr: Reserved : %d", get64(&prefix->reserved));
    SBEV_INFO("Prefix Hdr: flags : %X", get32(&prefix->flags));
    //Return the Prefix Hdr flag
    *flag = get32(&prefix->flags);

    // test for machine specific matching ecid
    SBEV_INFO("Prefix Hdr: ECID Count: %d",get8(&prefix->ecid_count));
    //Need not copy  prefix->ecid to SRAM as it is not de-referenced, since ecid_count will be always 0
    if(!valid_ecid(get8(&prefix->ecid_count), prefix->ecid, params->my_ecid))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : unauthorized prefix ecid");
        VERIFY_FAILED(PREFIX_ECID_TEST);
    }

    // test for valid prefix payload hash
    SBEV_INFO("Prefix Hdr: Payload Size: %d", get64(&prefix->payload_size));
    size = get64(&prefix->payload_size);

    // Validate the prefix payload fits in our hashDataBuff
    if (hashDataBuffSize < size)
    {
        VERIFY_FAILED(PREFIX_PAYLD_SZ_TEST);
    }
    memcpy_byte(hashDataBuff, &hw_data->sw_pkey_p, size);
    SHA512_Hash(hashDataBuff, size, &digest);
    memcpy_byte(hashDataBuff, &prefix->payload_hash, SHA512_DIGEST_LENGTH);
    if(memcmp(&hashDataBuff, digest, sizeof(SHA512_t)))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : invalid prefix payload hash");
        VERIFY_FAILED(PREFIX_HASH_TEST);
    }

    // test for valid sw key count
    SBEV_INFO("Prefix Hdr: SW Key Count: %d", get8(&prefix->sw_key_count));
    if (get8(&prefix->sw_key_count) < SW_KEY_COUNT_MIN ||
            get8(&prefix->sw_key_count) > SW_KEY_COUNT_MAX)
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : sw key count not between 1-3");
        VERIFY_FAILED(SW_KEY_INVALID_COUNT);
    }

    // finish procesing prefix header
    // test for protection of all sw key material (sanity check)
    if(size != (get8(&prefix->sw_key_count) * sizeof(ecc_key_t)))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : incomplete sw key protection in prefix header");
        VERIFY_FAILED(SW_KEY_PROTECTION_TEST);
    }

    // start processing sw header
    header = (ROM_sw_header_raw*)(hw_data->sw_pkey_p + get8(&prefix->sw_key_count)*sizeof(ecc_key_t));

    // test for fw secure version - compare what was passed in via
    // params.log to what the container's sw header has
    SBEV_INFO("SW Hdr: Secure Version: %d", get8(&header->fw_secure_version));
    if(get8(&header->fw_secure_version) < i_fw_secure_version)
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : bad container fw secure version");
        VERIFY_FAILED(SECURE_VERSION_TEST);
    }

    // test for valid header version, hash & signature algorithms (sanity check)
    if(!valid_ver_alg(&header->ver_alg, 0))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : bad sw header version,alg's");
        VERIFY_FAILED(HEADER_VER_ALG_TEST);
    }

    // test for machine specific matching ecid
    SBEV_INFO("SW Hdr: ECID Count: %d",get8(&header->ecid_count));
    //Need not copy  header->ecid to SRAM as it is not de-referenced, since ecid_count will be always 0
    if(!valid_ecid(get8(&header->ecid_count), header->ecid, params->my_ecid))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : unauthorized SW ecid");
        VERIFY_FAILED(HEADER_ECID_TEST);
    }

    sw_sig = (ROM_sw_sig_raw*) (header->ecid + get8(&header->ecid_count)*ECID_SIZE);

    swKeyCount = get8(&prefix->sw_key_count);
    //Calculate Hash of SW/FW header
    memcpy_byte(hashDataBuff, header, SW_HEADER_SIZE(header));
    SHA512_Hash(hashDataBuff, SW_HEADER_SIZE(header), &digest);

    // test for valid sw header signatures (all)
    if(!multi_key_verify(digest, swKeyCount, hw_data->sw_pkey_p,
                                  sw_sig->sw_sig_p,VERIFY_SW_SIG_P))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : invalid sw signature");
        VERIFY_FAILED(SW_SIGNATURE_TEST);
    }

    // test for entry point within protected payload (sanity check)
    params->entry_point = get64(&header->code_start_offset);
    //check if the entry is HRMOR-relative and aligned
    if(params->entry_point & ~(ENTRY_MASK))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : entry is not HRMOR relative or not aligned");
        VERIFY_FAILED(ENTRY_VALID_TEST);
    }

    // test for valid component-id
    SBEV_INFO("SW/FW Hdr: Payload Size: %d", get64(&header->payload_size));
    if(!(((hw_sig_to_verify == VERIFY_HW_SIG_A_HBBL) &&
          (get64(&header->component_id) == HBBL_SECURE_HDR_COMPONENT_ID)) ||
         ((hw_sig_to_verify == VERIFY_HW_SIG_C_SBE_FW) &&
          (get64(&header->component_id) == FW_SECURE_HDR_COMPONENT_ID))))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : invalid component ID (%d) for hw_sig_to_verify (%d)",
                        get64(&header->component_id), hw_sig_to_verify);
        VERIFY_FAILED(COMPONENT_ID_TEST);
    }

    if(get64(&header->payload_size) != payload_size)
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : Invalid payload section size exp:%d, act:%d",
                   get64(&header->payload_size),sectionSize);
        VERIFY_FAILED(SW_PAYLD_SZ_TEST);
    }

    memcpy_byte(hashDataBuff, &header->payload_hash, SHA512_DIGEST_LENGTH);
    if(memcmp(&hashDataBuff, payload_hash, sizeof(SHA512_t)))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : invalid sw payload hash");
        VERIFY_FAILED(HEADER_HASH_TEST);
    }

    SBEV_INFO("Secure HDR Verified");
    params->log=CONTEXT|COMPLETED;

    SBEV_EXIT(SBEV_FUNC);
    return ROM_DONE;
    #undef SBEV_FUNC
}

ROM_response verifySecureHdr(
        p9_xip_section_sbe_t secureHdrXipSection,
        int hw_sig_to_verify,
        SHA512_t* payload_hash,
        uint64_t payload_size,
        secureHdrResponse_t *secureHdrResponse)
{
    #define SBEV_FUNC " verifySecureHdr "
    SBEV_ENTER(SBEV_FUNC);
    ROM_response status;
    uint32_t fapirc = 0;

    // Declare local input struct
    ROM_hw_params l_hw_parms __attribute__ ((aligned(8)));
    // Clear/zero-out the struct since we want 0 ('zero') values for
    // struct elements my_ecid, entry_point and log
    memset(&l_hw_parms,0x00,sizeof(ROM_hw_params));
    populateHWParams(&l_hw_parms);

    SBEV_INFO(SBEV_FUNC "Secure Header:Start Offset: [0x%08X] Size: [0x%08X] ", getXipOffsetAbs(secureHdrXipSection), getXipSize(secureHdrXipSection));
    uint8_t container[SECURE_HDR_SIZE];
    uint32_t start_address = (uint32_t)container;
    uint32_t endAddress = 0; // dummy variable to keep loadSeepromtoPibmem happy
    uint32_t size = sizeof(container);
    fapirc = loadSeepromtoPibmem(secureHdrXipSection, start_address, endAddress, sizeof(container), size, NULL);
    if(fapirc)
    {
        SBEV_ERROR(SBEV_FUNC " Loading data to pibmem is failed with rc [0x%08X], start [0x%08X] end [0x%08X]",
                fapirc, start_address, endAddress);
        secureBootStatus_t secureBootStatus;
        if ( secureHdrXipSection == P9_XIP_SECTION_SBE_SBH_FIRMWARE )
        {
            UPDATE_ERROR_REG_VERIFICATION_STATUS_AND_HALT(SBH_FIRMWARE_LOAD_FAILED);
        }
        else
        {
            UPDATE_ERROR_REG_VERIFICATION_STATUS_AND_HALT(SBH_HBBL_LOAD_FAILED);
        }
    }

    status = ROM_verify((ROM_container_raw*)container, &l_hw_parms, hw_sig_to_verify, payload_hash, payload_size, &secureHdrResponse->flag);
    secureHdrResponse->statusCode = (uint8_t)l_hw_parms.log;
    SBEV_INFO(SBEV_FUNC "Status code is [0x%08X%08X]", SBE::higher32BWord(l_hw_parms.log), SBE::lower32BWord(l_hw_parms.log));

    if(status == ROM_DONE && secureHdrResponse->statusCode == COMPLETED)
    {
        // We only want to return a non-zero status on failure
        secureHdrResponse->statusCode = 0;
        SBEV_INFO(SBEV_FUNC "Container verification Passed");
    }

    memcpy(&secureHdrResponse->sha512Truncated, payload_hash, sizeof(SHA512truncated_t));

    SBEV_EXIT(SBEV_FUNC);
    return status;
    #undef SBEV_FUNC
}
