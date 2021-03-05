/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbeTPMCommand.H $                  */
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
#ifndef __SBEM_TPM_COMMAND_H
#define __SBEM_TPM_COMMAND_H

#include "fapi2.H"
#include "stdint.h"

/**
 * @brief This function peforms the poisoning of the PCR.
 *
 * @return errlHndl_t NULL if successful, otherwise a pointer to the
 *      error log.
 */

fapi2::ReturnCode tpmPosionPCR();


/**
 * @brief This function peforms the sequencing to do extend on the TPM.
 * @param[in] pcrNum   PCT to be extended.
 *            hashKey  key to be extended on PCR.
 *            size     Size of the hash key.
 *
 * @return errlHndl_t NULL if successful, otherwise a pointer to the
 *      error log.
 */

fapi2::ReturnCode tpmExtendPCR(uint32_t pcrNum, uint8_t *hashKey, uint32_t size);
#endif
