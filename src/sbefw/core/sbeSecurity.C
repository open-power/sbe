/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeSecurity.C $                                */
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
#include "sbeSecurity.H"
#include "sbetrace.H"
#include "sbeglobals.H"
#include "fapi2.H"

#include "sbeSecurityGen.H"

namespace SBE_SECURITY
{
//----------------------------------------------------------------------------
bool isAllowed(const uint32_t i_addr, uint64_t i_mask,  accessType i_type)
{
    bool ret = true;
    if(SBE_GLOBAL->sbeFWSecurityEnabled)
    {
        if(i_type == WRITE)
        {
            ret =  WHITELIST::isPresent(i_addr);
            if( (ret == false ) && (i_mask != 0xffffffffffffffffull ))
            {
                ret = GREYLIST::isPresent(i_addr, i_mask);
            }
        }
        else if(i_type == READ)
            ret =  !BLACKLIST::isPresent(i_addr);
        SBE_INFO("SBE_SECURITY access[%d] allowed[%d] addr[0x%08x]",
                                        i_type, ret, i_addr);
    }
    return ret;
}
//----------------------------------------------------------------------------
uint32_t updateAndSendSecTOCHdr( sbeMemAccessInterface *i_pMemInterface )
{
    #define SBE_FUNC "updateAndSendSecTOCHdr"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = fapi2::FAPI2_RC_SUCCESS;

    sec_header_dump_t l_secListDumpHdr;
    /// Send the list of _T1, _T2 and _T3 header count
    l_secListDumpHdr.wl_t1_count = WHITELIST::t1.size;
    l_secListDumpHdr.wl_t2_count = WHITELIST::t2.size;
    l_secListDumpHdr.wl_t3_count = WHITELIST::t3.size;

    l_secListDumpHdr.bl_t1_count = BLACKLIST::t1.size;
    l_secListDumpHdr.bl_t2_count = BLACKLIST::t2.size;
    l_secListDumpHdr.bl_t3_count = BLACKLIST::t3.size;

    l_secListDumpHdr.gl_t1_count = GREYLIST::t1.size;

    SBE_INFO("SBE_SECURITY whitelist t1[%d] t2[%d] t3[0x%d] ",
              WHITELIST::t1.size, WHITELIST::t2.size, WHITELIST::t3.size);
    SBE_INFO("SBE_SECURITY blacklist t1[%d] t2[%d] t3[0x%d] ",
              BLACKLIST::t1.size, BLACKLIST::t2.size, BLACKLIST::t3.size);
    SBE_INFO("SBE_SECURITY greylist t1[%d] ", GREYLIST::t1.size);

    fapiRc = i_pMemInterface->accessWithBuffer(&l_secListDumpHdr,
                                               sizeof(l_secListDumpHdr),
                                               false);
    if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Failed to write accessWithBuffer to hostboot");
    }

    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeSecurityWhiteBlackListDump( sbeMemAccessInterface *i_pMemInterface,
                                   const secListType &i_listType  )
{
    #define SBE_FUNC "sbeSecurityWhiteBlackListDump"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = fapi2::FAPI2_RC_SUCCESS;
    _t1_t * t1 = NULL;
    uint32_t t1_size = 0;
    _t2_t * t2 = NULL;
    uint32_t t2_size = 0;
    _t3_t * t3 = NULL;
    uint32_t t3_size = 0;
    if( i_listType == SEC_WHITE_LIST )
    {
        t1 = WHITELIST::_t1;
        t1_size = WHITELIST::t1.size;

        t2 = WHITELIST::_t2;
        t2_size = WHITELIST::t2.size;

        t3 = WHITELIST::_t3;
        t3_size = WHITELIST::t3.size;
    }
    else if( i_listType == SEC_BLACK_LIST )
    {
        t1 = BLACKLIST::_t1;
        t1_size = BLACKLIST::t1.size;

        t2 = BLACKLIST::_t2;
        t2_size = BLACKLIST::t2.size;

        t3 = BLACKLIST::_t3;
        t3_size = BLACKLIST::t3.size;
    }
    do
    {
        // Update and Send the whitelist T1
        for( uint32_t i = 0; i < t1_size; i++)
        {
            fapiRc = i_pMemInterface->accessWithBuffer(&t1[i].key_start,
                                          sizeof(uint8_t), false);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
            fapiRc = i_pMemInterface->accessWithBuffer(&t1[i].key_end,
                                          sizeof(uint8_t), false);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
            fapiRc = i_pMemInterface->accessWithBuffer(&t1[i].value,
                                          sizeof(uint8_t), false);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
        }

        // Update and Send the whitelist T2
        for( uint32_t i = 0; i < t2_size; i++)
        {
            fapiRc = i_pMemInterface->accessWithBuffer(&t2[i].key,
                                          sizeof(uint8_t), false);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
            fapiRc = i_pMemInterface->accessWithBuffer(&t2[i].value,
                                          sizeof(uint16_t), false);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
        }

        // Update and Send the whitelist T3
        for( uint32_t i = 0; i < t3_size; i++)
        {
            fapiRc = i_pMemInterface->accessWithBuffer(&t3[i].value,
                                                sizeof(uint16_t), false);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
        }
    }
    while(0);
    if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Failed to send Black/WhiteListDump to hostboot");
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeSecurityGreyListDump( sbeMemAccessInterface *i_pMemInterface )
{
    #define SBE_FUNC "sbeSecurityGreyListDump"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = fapi2::FAPI2_RC_SUCCESS;

    // Update and Send the blacklist T1
    for(uint32_t i = 0; i < GREYLIST::t1.size; i++)
    {
        fapiRc = i_pMemInterface->accessWithBuffer(&GREYLIST::_t1[i].key,
                                          sizeof(uint32_t), false);
        if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
        fapiRc = i_pMemInterface->accessWithBuffer(&GREYLIST::_t1[i].value,
                                          sizeof(uint64_t), false);
        if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
    }

    if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Failed to send GreyListDump to hostboot");
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------
uint32_t  sendSecurityListDumpToHB(sbeMemAccessInterface *i_pMemInterface)
{
    #define SBE_FUNC "sendSecurityListDumpToHB"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        // Update and Send the list of TOC of _T1, _T2 and _T3 header count
        fapiRc = updateAndSendSecTOCHdr(i_pMemInterface);
        if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;

        // Update and Send the whitelist T1,T2 and T3 data
        fapiRc = sbeSecurityWhiteBlackListDump(i_pMemInterface, SEC_WHITE_LIST);
        if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;

        // Update and Send the blacklist T1,T2 and T3 data
        fapiRc = sbeSecurityWhiteBlackListDump(i_pMemInterface,SEC_BLACK_LIST);
        if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;

        // Update and Send the greylist T1 data
        fapiRc = sbeSecurityGreyListDump(i_pMemInterface);
        if( fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
        uint8_t l_endOfdump = 0;
        fapiRc = i_pMemInterface->accessWithBuffer(&l_endOfdump,
                                               sizeof(l_endOfdump),
                                               true);
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS) break;
    }
    while(0);
    if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Failed to write SecData Dump to hostboot");
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

} // namespace SBE_SECURITY
