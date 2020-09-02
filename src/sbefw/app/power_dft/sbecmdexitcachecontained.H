/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdexitcachecontained.H $              */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
#ifndef __SBE_CMDEXITCACHECONTAINED_H
#define __SBE_CMDEXITCACHECONTAINED_H

#include <stdint.h>
#include <stddef.h>
//Exit Cache contained mode request parameters.Hostboot sends the below
//information for SBE to perfrom the XSCOM operations.
typedef struct
{
    uint64_t xscomEntries:32; //Number of address-data pair stored at 
                              //the cache address.
    uint64_t stepInfo:32;     //Step information which defines the Exit
                              //cache contained mode procedure execution
    uint64_t address;         //Cache address to fetch XSCOM address-data pair.
} sbePsuExitCacheContainedMode_t;

/* @brief Exit Cache Contained Command (0xD801)
 *
 * @param[in] i_pArg Buffer to be passed to the function
 *
 * @return    RC from the FIFO access utility
 */
uint32_t sbePsuExitCacheContainedMode(uint8_t *i_pArg);

/* @brief Fetch the list of XSCOM address-data pair from the address shared by
 * the Hostboot via the chipop and execute the procedure to apply the
 * XSCOM INITS.
 *
 * @param[in]i_xscomPairSize  Number of registers to be initialized.
 * @param[in] i_xscomInitAddress Address which hostboot has saved the array of
 *                               register inits to be performed via XSCOM.
 *                               Each entryis expected to contain a
 *                               fully formed XSCOM address and its associated
 *                               data
 * @return  fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
 */
uint32_t sbeFetchAndApplyXSCOMInit(const size_t i_xscomPairSize, const uint64_t i_xscomInitAddress);

#endif //__SBE_CMDFASTARRAY_H
