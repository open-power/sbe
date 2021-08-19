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

/*************************DO NOT MODIFY THE BELOW ARRAY WITHOUT CONSENT OF akhis023@in.ibm.com***********************
 *List of OCMB register read access allowed.
 * TODO: This list needs to be moved into a new tag read_allowlist in .csv
 * security file. Parsing support needs to be added for the same.
 * NOTE:The below array is bubble sorted in assending order for binary search to work on
 * it.
 */
uint32_t ocmbReadAllowList[] = { 0x002000b0 ,0x00200860 ,0x002010b0 ,0x00201860 ,0x002020b0 ,0x00202860 ,
                                 0x002030b0 ,0x00203860 ,0x002040b0 ,0x00204860 ,0x002050b0 ,0x00205860 ,
                                 0x002060b0 ,0x00206860 ,0x002070b0 ,0x00207860 ,0x0300009c ,0x030000a0 ,
                                 0x030000a4 ,0x08010001 ,0x08010002 ,0x08010824 ,0x08010850 ,0x08010851 ,
                                 0x08010852 ,0x08010853 ,0x08010854 ,0x08010855 ,0x08010856 ,0x08010857 ,
                                 0x08010858 ,0x08010870 ,0x08010873 ,0x08010876 ,0x08010877 ,0x0801087c ,
                                 0x0801087e ,0x08010880 ,0x08010882 ,0x080108d2 ,0x080108e8 ,0x080108ec ,
                                 0x080108ed ,0x08011400 ,0x08011403 ,0x08011406 ,0x08011407 ,0x08011408 ,
                                 0x0801140a ,0x0801140b ,0x0801140c ,0x0801140d ,0x0801140e ,0x0801140f ,
                                 0x08011410 ,0x08011411 ,0x08011415 ,0x08011416 ,0x08011417 ,0x08011418 ,
                                 0x08011419 ,0x0801141a ,0x0801141b ,0x0801141c ,0x0801141d ,0x08011420 ,
                                 0x0801142a ,0x0801142b ,0x0801142c ,0x0801142d ,0x0801142e ,0x08011431 ,
                                 0x08011434 ,0x08011435 ,0x08011436 ,0x08011437 ,0x08011438 ,0x08011800 ,
                                 0x08011803 ,0x08011806 ,0x08011807 ,0x08011808 ,0x0801180a ,0x0801180b ,
                                 0x08011855 ,0x08011856 ,0x08011857 ,0x08011858 ,0x08011859 ,0x0801185a ,
                                 0x0801185b ,0x0801185c ,0x0801185d ,0x0801185e ,0x0801185f ,0x08011860 ,
                                 0x08011869 ,0x0801186a ,0x0801186b ,0x0801186c ,0x0801186d ,0x0801186e ,
                                 0x0801186f ,0x08011870 ,0x08011871 ,0x08011873 ,0x08011874 ,0x0801187e ,
                                 0x080118a6 ,0x080118d6 ,0x080118d7 ,0x080118db ,0x080118dc ,0x080118e0 ,
                                 0x080118e7 ,0x080118ec ,0x08011c00 ,0x08011c03 ,0x08011c06 ,0x08011c07 ,
                                 0x08011c08 ,0x08011c0a ,0x08011c0b ,0x08011c0c ,0x08011c0d ,0x08011c0e ,
                                 0x08011c0f ,0x08011c10 ,0x08011c11 ,0x08011c12 ,0x08011c13 ,0x08011c14 ,
                                 0x08011c15 ,0x08011c16 ,0x08011c17 ,0x08011c18 ,0x08011c19 ,0x08011c1a ,
                                 0x08011c1b ,0x08011c1c ,0x08011c1d ,0x08011c1e ,0x08011c1f ,0x08011c20 ,
                                 0x08011c28 ,0x08011c2d ,0x08011c2e ,0x08011c30 ,0x08011c31 ,0x08011c32 ,
                                 0x08012002 ,0x08012006 ,0x08012007 ,0x08012008 ,0x08012009 ,0x08012400 ,
                                 0x08012403 ,0x08012406 ,0x08012407 ,0x08012408 ,0x0801240a ,0x0801240b ,
                                 0x0801240c ,0x0801240e ,0x08012410 ,0x08012411 ,0x08012412 ,0x08012413 ,
                                 0x08012414 ,0x08012415 ,0x08012416 ,0x0801241c ,0x0801241d ,0x0801241e ,
                                 0x08012800 ,0x08012803 ,0x08012806 ,0x08012807 ,0x08012808 ,0x0801280a ,
                                 0x0801280b ,0x0801280e ,0x0801280f ,0x08012810 ,0x08012811 ,0x08012812 ,
                                 0x08012813 ,0x08012814 ,0x08012815 ,0x08012816 ,0x08012817 ,0x08012818 ,
                                 0x08012819 ,0x0801281d ,0x0801281e ,0x0801281f ,0x08040000 ,0x08040001 ,
                                 0x08040002 ,0x08040004 ,0x08040007 ,0x08040008 ,0x0804000a ,0x0804000d ,
                                 0x08040010 ,0x08040011 ,0x08040017 ,0x08040018 ,0x08040019 ,0x080f0000 ,
                                 0x080f0001 ,0x080f0004 ,0x080f0005
};

/* @brief binarySearch    - A iterative binary search function using pointers.
 *
 * @param[in] arr         - Input array for binary search
 * @param[in] l           - Array start offset
 * @param[in] r           - Array end offset
 * @param[in] x           - Element to be searched
 *
 * @return                - boolean to denote if the access on the address
 *                          is allowed or not.True if element is present in
 *                          array else false.
 */
static bool binarySearch(uint32_t *arr, int l, int r, uint32_t x)
{
    uint32_t *temp = arr;

    while (l <= r)
    {
        int m = l + (r - l) / 2;
        arr = temp;
        arr = arr+m;

        // Check if x is present at mid
        if (*arr == x)
            return true;

        // If x greater, ignore left half
        if (*arr < x)
            l = m + 1;

        // If x is smaller, ignore right half
        else
            r = m - 1;
    }

    // if we reach here, then element was not present
    return false;
}

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

bool isOcmbReadAllowed(const uint32_t i_addr)
{
    bool ret = true;
    if(SBE_GLOBAL->sbeFWSecurityEnabled)
    {
        int n = sizeof(ocmbReadAllowList) / sizeof(ocmbReadAllowList[0]);
        ret = binarySearch(ocmbReadAllowList, 0, n - 1, i_addr);

        SBE_INFO("SBE_OCMB_READ_SECURITY allowed[%d] addr[0x%08x]",
                                         ret, i_addr);
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
