/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeSecurity.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2021                        */
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
#ifndef __SBE_SECURITY_H
#define __SBE_SECURITY_H

#include <stdint.h>
#include <securityAlgo.H>
#include "sbeMemAccessInterface.H"

namespace SBE_SECURITY
{
    typedef enum
    {
        READ = 0,
        WRITE
    } accessType;

    /**
     * @brief An enumeration of all Security list types
     */
    enum secListType
    {
        SEC_WHITE_LIST = 0x0,
        SEC_BLACK_LIST = 0x1,
        SEC_GREY_LIST  = 0x2,
        // Don't count this in the list type, just to intialize the list type
        SEC_INVALID_LIST = 0xF,
    };

    /* @brief isAllowed       - Public function used for address verification
     *                          for a given type of access
     *
     * @param[in] i_addr      - given address to verify
     * @param[in] i_mask      - mask for data
     * @param[in] i_type      - access type - READ/WRITE
     *
     * @return                - boolean to denote if the access on the address
     *                          is allowed or not
     */
    bool isAllowed(const uint32_t i_addr, const uint64_t i_mask,
                   accessType i_type);

    /* @brief isOcmbReadAllowed - Public function used for address verification
     *                          for a given OCMB read scom
     *
     * @param[in] i_addr      - given address to verify
     *
     * @return                - boolean to denote if the access on the address
     *                          is allowed or not
     */
    bool isOcmbReadAllowed(const uint32_t i_addr);

    /* @brief sendSecurityListDumpToHB - Public function used for sending
     *                         security dump to hostboot
     *
     * @param[in] i_pMemInterface - pointer to memory interface object
     *
     * @return                - SBE secondary RC
     */
    uint32_t  sendSecurityListDumpToHB(sbeMemAccessInterface *i_pMemInterface);
}

#endif //__SBE_SECURITY_H
