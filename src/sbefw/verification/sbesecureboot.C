/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbesecureboot.C $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
#include "plat_hwp_data_stream.H"
#include "fapi2.H"
#include "sbeXipUtils.H"
#include "sbeutil.H"

#define VERIFY_FAILED(_c) { params->log=ERROR_EVENT|CONTEXT|(_c); \
                            return ROM_FAILED; }

#define HBBL_SECURE_HDR_COMPONENT_ID 0x4842424C00000000Ull      //Component ID:HBBL
#define FW_SECURE_HDR_COMPONENT_ID   0x5342455f46570000Ull      //Component ID:SBE_FW
#define VERIFY_SW_SIG_P              0                          //Verify SW Signature P incase of both HBBL and SBE_FW secure Hdr.

using namespace fapi2;

static void memcpy_byte(void* vdest, const void* vsrc, size_t len)
{

    // Loop, copying 1 byte
    uint8_t* cdest = (uint8_t *)vdest;
    const uint8_t* csrc = (const uint8_t *)vsrc;
    size_t i = 0;

    for (; i < len; ++i)
    {
        cdest[i] = csrc[i];
    }
}

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
    for (;key_count;key_count--,keys+=sizeof(ecc_key_t),
            sigs+=sizeof(ecc_signature_t))
    {
        if((no_of_keys - key_count) == sig_to_verify)
        {
            SBEV_INFO("Verifying signature: %d",(no_of_keys-key_count));
            //TODO:Enable below piec of code for HW.
            //Curently Seeing stack corruption issues in SHA512 if
            //ECDSA Enabled
            return 1;
            if(!SBE::isSimicsRunning())
            {
                if (ec_verify (keys, digest, sigs)<1)
                {
                    return 0;
                }
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

    SBEV_EXIT(SBEV_FUNC);
    return 1;
    #undef SBEV_FUNC
}

static void SHA512_FW_Payload_Hash(SHA512_t *result)
{
    #define SBEV_FUNC " SHA512_FW_Payload_Hash "
    SBEV_ENTER(SBEV_FUNC);

    SHA512_CTX context;

    SHA512_Init(&context);

    SHA512UpdateXipSection(P9_XIP_SECTION_SBE_LOADERTEXT, &context);
    SHA512UpdateXipSection(P9_XIP_SECTION_SBE_TEXT, &context);
    SHA512UpdateXipSection(P9_XIP_SECTION_SBE_BASELOADER, &context);
    SHA512UpdateXipSection(P9_XIP_SECTION_SBE_BASE, &context);

    SHA512_Final(&context, result);

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

static void SHA512_HBBL_Payload_Hash(SHA512_t *result)
{
    #define SBEV_FUNC " SHA512_HBBL_Payload_Hash "
    SBEV_ENTER(SBEV_FUNC);

    SHA512_CTX context;

    SHA512_Init(&context);
    SHA512UpdateXipSection(P9_XIP_SECTION_SBE_HBBL, &context);
    SHA512_Final(&context, result);

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

static void populateHWParams(ROM_hw_params* params)
{
    #define SBEV_FUNC " populateHWParams "
    SBEV_ENTER(SBEV_FUNC);

    SBEV_INFO("SBE Settings(HW Key Hash):Start Offset: [0x%08X] Size: [0x%08X] ", getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS), getXipSize(P9_XIP_SECTION_SBE_SB_SETTINGS));

    //Populate params struct
    //Get HW key hash from .sb_settings
    memcpy_byte(params->hw_key_hash,(uint8_t *)getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS), SHA512_DIGEST_LENGTH);
    //Get MSV from .sb_settings
    params->log = get8((uint8_t *)(getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS) + SHA512_DIGEST_LENGTH));

    SBEV_EXIT(SBEV_FUNC);
    #undef SBEV_FUNC
}

ROM_response ROM_verify( ROM_container_raw* container,
                         ROM_hw_params* params,
                         int hw_sig_to_verify,
                         SHA512_t *SHA512Result,
                         uint32_t *flag)
{
    #define SBEV_FUNC " ROM_verify "
    SBEV_ENTER(SBEV_FUNC);

    ROM_prefix_header_raw* prefix;
    ROM_prefix_data_raw* hw_data;
    ROM_sw_header_raw* header;
    ROM_sw_sig_raw* sw_sig;
    SHA512_t digest;
    //NOTE: Keep the array size 8 byte aligned to overcome sram allignment issues.
    //396 bytes is MAX hash we calculate and hence buffer size is 400 bytes.
    uint8_t hashDataBuff[sizeof(uint64_t) * 50]  __attribute__ ((aligned(8))) = {0x00};
    uint64_t size;
    uint8_t swKeyCount;

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

    SBEV_INFO("Prefix Hdr: code start offset : %d", get64(&prefix->code_start_offset));
    SBEV_INFO("Prefix Hdr: Reserved : %d", get64(&prefix->reserved));
    SBEV_INFO("Prefix Hdr: flags : %X", get32(&prefix->flags));
    //Return the Prefix Hdr flag
    *flag = get32(&prefix->flags);

    // test for valid prefix header signatures (all)
    hw_data = (ROM_prefix_data_raw*)(prefix->ecid + prefix->ecid_count*ECID_SIZE);

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

    // test for machine specific matching ecid
    SBEV_INFO("Prefix Hdr: ECID Count: %d",get8(&prefix->ecid_count));
    //Need not copy  prefix->ecid to SRAM as it is not de-referenced.
    if(!valid_ecid(get8(&prefix->ecid_count), prefix->ecid, params->my_ecid))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : unauthorized prefix ecid");
        VERIFY_FAILED(PREFIX_ECID_TEST);
    }

    // test for valid prefix payload hash
    SBEV_INFO("Prefix Hdr: Payload Size: %d", get64(&prefix->payload_size));
    size = get64(&prefix->payload_size);
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

    sw_sig = (ROM_sw_sig_raw*) (header->ecid + header->ecid_count*ECID_SIZE);

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

    // test for machine specific matching ecid
    SBEV_INFO("SW Hdr: ECID Count: %d",get8(&header->ecid_count));
    //Need not copy  header->ecid to SRAM as it is not de-referenced.
    if(!valid_ecid(get8(&header->ecid_count), header->ecid, params->my_ecid))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : unauthorized SW ecid");
        VERIFY_FAILED(HEADER_ECID_TEST);
    }

    // test for entry point within protected payload (sanity check)
    params->entry_point = get64(&header->code_start_offset);
    //check if the entry is HRMOR-relative and aligned
    if(params->entry_point & ~(ENTRY_MASK))
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : entry is not HRMOR relative or not aligned");
        VERIFY_FAILED(ENTRY_VALID_TEST);
    }

    // test for valid sw payload hash
    SBEV_INFO("SW/FW Hdr: Payload Size: %d", get64(&header->payload_size));
    if(get64(&header->component_id) == HBBL_SECURE_HDR_COMPONENT_ID)
    {
        SHA512_HBBL_Payload_Hash(&digest);
    }
    else if(get64(&header->component_id) == FW_SECURE_HDR_COMPONENT_ID)
    {
        SHA512_FW_Payload_Hash(&digest);
    }
    else
    {
        SBEV_ERROR(SBEV_FUNC "FAILED : invalid component ID . SBE Check");
        return ROM_FAILED;
    }
    for (uint8_t i=0; i<sizeof(digest); i=i+4)
    {
        SBEV_INFO("SHA512 of payload is %x %x %x %x",
                digest[i],digest[i+1],digest[i+2],digest[i+3]);
    }
    memcpy_byte(hashDataBuff, &header->payload_hash, SHA512_DIGEST_LENGTH);
    //Return the calculated SHA512.
    memcpy(SHA512Result, digest, sizeof(SHA512_t));
    if(memcmp(&hashDataBuff, digest, sizeof(SHA512_t)))
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

ROM_response verifySecureHdr(p9_xip_section_sbe_t secureHdrXipSection, int hw_sig_to_verify, secureHdrResponse_t *secureHdrResponse)
{
    #define SBEV_FUNC " verifySecureHdr "
    SBEV_ENTER(SBEV_FUNC);

    ROM_response status;
    SHA512_t digest;

    // Declare local input struct
    ROM_hw_params l_hw_parms;
    // Clear/zero-out the struct since we want 0 ('zero') values for
    // struct elements my_ecid, entry_point and log
    memset(&l_hw_parms,0x00,sizeof(ROM_hw_params));
    populateHWParams(&l_hw_parms);

    SBEV_INFO("Secure Header:Start Offset: [0x%08X] Size: [0x%08X] ", getXipOffsetAbs(secureHdrXipSection), getXipSize(secureHdrXipSection));
    ROM_container_raw* container = (ROM_container_raw *)getXipOffsetAbs(secureHdrXipSection);

    status = ROM_verify(container, &l_hw_parms, hw_sig_to_verify, &digest, &secureHdrResponse->flag);
    secureHdrResponse->statusCode = (uint8_t)l_hw_parms.log;
    SBEV_DEBUG("Status code is [0x%08X%08X]", SBE::higher32BWord(l_hw_parms.log), SBE::lower32BWord(l_hw_parms.log));

    if(status == ROM_FAILED && secureHdrResponse->statusCode == HEADER_HASH_TEST)
    {
        if(secureHdrXipSection == P9_XIP_SECTION_SBE_SBH_FIRMWARE)
        {
            SBEV_INFO("Container verification failed. Calculating SBE_FW Payload SHA512 Hash");
            SHA512_FW_Payload_Hash(&digest);
        }

        if(secureHdrXipSection == P9_XIP_SECTION_SBE_SBH_HBBL)
        {
            SBEV_INFO("Container verification failed. Calculating HBBL Payload SHA512 Hash");
            SHA512_HBBL_Payload_Hash(&digest);
        }

        for (uint8_t i=0; i<sizeof(digest); i=i+4)
        {
            SBEV_INFO("SHA512 of payload is %x %x %x %x",
                    digest[i],digest[i+1],digest[i+2],digest[i+3]);
        }

    }

    memcpy(&secureHdrResponse->sha512Truncated, digest, sizeof(SHA512truncated_t));

    SBEV_EXIT(SBEV_FUNC);
    return status;
    #undef SBEV_FUNC
}
