/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbesecureboot.H $                      */
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
 * @file: ppe/sbe/sbefw/sbesecureboot.H
 *
 * @brief This file contains the Interfaces for SBE Secure Boot Verification
 *
 */

#ifndef __SBEFW_SBESECUREBOOT_H
#define __SBEFW_SBESECUREBOOT_H

#include <stdint.h>
#include "p10_ipl_image.H"
#include "ROM.H"
#include "sbesecuritycommon.H"

#define VERIFY_HW_SIG_A_HBBL         0                          //Verify HW Signature A incase of HBBL secure Hdr
#define VERIFY_HW_SIG_C_SBE_FW       2                          //Verify HW Signature C incase of SBE_FW secure Hdr

typedef struct secureHdrResponse
{
    SHA512truncated_t sha512Truncated;
    uint32_t          flag;
    uint8_t           statusCode;
}secureHdrResponse_t;

/**
 * @brief Verify Secure Header.
 *
 * @param[in] secureHdrXipSection  index of the xip section, for which secure header verification needed
 * @param[in] hw_sig_to_verify     either VERIFY_HW_SIG_A_HBBL or VERIFY_HW_SIG_C_SBE_FW
 * @param[in] *payload_hash        calculated payload hash to verify with signature (SBE_FW or HBBL Payload hash)
 * @param[in] payload_size         SBE_FW or HBBL Payload size
 * @param[out] *secureHdrResponse  After completing function, this structure will contain
 *                                    verification response
 *
 * @return Secure container verification response.
 */
ROM_response verifySecureHdr(
        p9_xip_section_sbe_t secureHdrXipSection,
        int hw_sig_to_verify,
        SHA512_t* payload_hash,
        uint64_t payload_size,
        secureHdrResponse_t *secureHdrResponse);

#endif /* __SBEFW_SBESECUREBOOT_H */
