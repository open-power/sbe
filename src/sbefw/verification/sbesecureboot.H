/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbesecureboot.H $                      */
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
    SHA512truncated_t SHA512TruncatedResult;
    uint32_t          flags;
    uint8_t           statusCode;
}secureHdrResponse_t;

/**
 * @brief Verify Secure container.
 *
 * @param ROM_container_raw* Pointer to secure container start address
 *
 * @param ROM_hw_params* Pointer to HW Keys Hash
 *
 * @param hw_sig_to_verify The hardware signature that has to be verified(Among the 3 HW signaturs that are present)
 *
 * @return Secure container verification response/result/status_code.
 */
ROM_response ROM_verify(ROM_container_raw* container, ROM_hw_params* params, int hw_sig_to_verify, SHA512_t *SHA512Result);

/**
 * @brief Verify Secure Header.
 *
 * @return Secure container verification response/result/status_code
 */
ROM_response verifySecureHdr(p9_xip_section_sbe_t secureHdrXipSection, int hw_sig_to_verify, secureHdrResponse_t *secureHdrResponse);

#endif /* __SBEFW_SBESECUREBOOT_H */
