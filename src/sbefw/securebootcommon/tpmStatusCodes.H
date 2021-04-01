/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/tpmStatusCodes.H $                 */
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

#ifndef TPM_STATUS_CODES_H
#define TPM_STATUS_CODES_H

enum sbemTPMRespCode
{
    SBEM_TPM_OPERATION_SUCCESSFUL                            = 0x00,
    SBEM_TPM_READ_DID_AND_VID_FAILURE                        = 0x01,
    SBEM_TPM_ACCESS_LOCALITY0_FAILURE                        = 0x02,
    SBEM_TPM_START_TPM_SEQUENCE_FAILURE                      = 0x03,
    SBEM_TPM_DETECT_PCR_ALLOCATION_FAILURE                   = 0x04,
    SBEM_TPM_PCR_ALLOCATION_FAILURE                          = 0x05,
    SBEM_TPM_POISON_FAILURE                                  = 0x06,
    SBEM_TPM_EXTEND_HW_KEY_HASH_PCR6_FAILURE                 = 0x07,
    SBEM_TPM_EXTEND_HW_KEY_HASH_PCR1_FAILURE                 = 0x08,
    SBEM_TPM_EXTEND_SECURITY_STATE_PCR6_FAILURE              = 0x09,
    SBEM_TPM_EXTEND_SECURITY_STATE_PCR1_FAILURE              = 0x0A,
    SBEM_TPM_EXTEND_VERIFICATION_IMAGE_HASH_PCR0_FAILURE     = 0x0B,
    SBEM_TPM_EXTEND_VERIFICATION_IMAGE_HASH_PCR6_FAILURE     = 0x0C,
    SBEV_TPM_EXTEND_SBE_FW_PAYLOAD_IMAGE_HASH_PCR0_FAILURE   = 0x0D,
    SBEV_TPM_EXTEND_HBBL_PAYLOAD_IMAGE_HASH_PCR0_FAILURE     = 0x0E,
    SBEV_TPM_EXTEND_SECURITY_SWITCH_REG_PCR1_FAILURE         = 0x0F,
    SBEV_TPM_EXTEND_SEPERATOR_PCR0_FAILURE                   = 0x10,
    SBEV_TPM_EXTEND_SEPERATOR_PCR1_FAILURE                   = 0x11,
    SBEV_TPM_EXTEND_SEPERATOR_PCR2_FAILURE                   = 0x12,
    SBEV_TPM_EXTEND_SEPERATOR_PCR3_FAILURE                   = 0x13,
    SBEV_TPM_EXTEND_SEPERATOR_PCR4_FAILURE                   = 0x14,
    SBEV_TPM_EXTEND_SEPERATOR_PCR5_FAILURE                   = 0x15,
    SBEV_TPM_EXTEND_SEPERATOR_PCR6_FAILURE                   = 0x16,
    SBEV_TPM_EXTEND_SEPERATOR_PCR7_FAILURE                   = 0x17,

};

#endif
