/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeSecureMemRegionManager.C $                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
#include "sbeSecureMemRegionManager.H"
#include "sbetrace.H"
#include "sbeutil.H"
#include "sbeglobals.H"

#ifndef __SBEFW_SEEPROM__
SBESecureMemRegionManager mainStoreSecMemRegionManager(
                                    &SBE_GLOBAL->mainMemRegions[0],
                                    MAX_MAIN_STORE_REGIONS);
SBEOccSramSecMemRegionManager occSramSecRegionManager(
                                    &SBE_GLOBAL->occSramRegions[0],
                                    MAX_OCC_SRAM_REGIONS);

secureMemRegion_t SBESecureMemRegionManager::getPartialRegionSize(
                                            const secureMemRegion_t i_region)
{
    secureMemRegion_t ret = {};
    for(size_t i = 0; i < iv_regionsOpenCnt; i++)
    {
        uint64_t minStartAddr = i_region.startAddress < iv_memRegions[i].startAddress ?
                        i_region.startAddress : iv_memRegions[i].startAddress;
        uint64_t iRegionEndAddress = i_region.startAddress + i_region.size - 1;
        uint64_t existingRegionEndAddress = iv_memRegions[i].startAddress + iv_memRegions[i].size - 1;
        uint64_t maxEndAddr = iRegionEndAddress >= existingRegionEndAddress ?
                        iRegionEndAddress : existingRegionEndAddress;

        // detect overlap
        if((maxEndAddr - minStartAddr + 1) < (i_region.size + iv_memRegions[i].size))
        {
            ret = iv_memRegions[i];
            // Give preference to first region
            if(i_region.startAddress >= iv_memRegions[i].startAddress &&
               i_region.startAddress < (iv_memRegions[i].startAddress +
                                        iv_memRegions[i].size))
            {
                // Return the existing window to the extent of input window
                ret.startAddress = i_region.startAddress;
                ret.size = iv_memRegions[i].startAddress + iv_memRegions[i].size
                            - i_region.startAddress;
                if(ret.size > i_region.size)
                    ret.size = i_region.size;
                break;
            }
        }
    }
    return ret;
}

#endif //__SBEFW_SEEPROM__
#ifdef __SBEFW_SEEPROM__

// Public functions
sbeSecondaryResponse SBESecureMemRegionManager::add(const uint64_t i_startAddr,
                                                    const uint32_t i_size,
                                                    const uint8_t i_mode)
{
    #define SBE_FUNC "SBESecureMemRegionManager::add"
    sbeSecondaryResponse rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        if(getPartialRegionSize({i_startAddr, i_size, i_mode}).mode)
        {
            SBE_ERROR(SBE_FUNC" SBE_SEC_MEM_REGION_AMEND_ATTEMPTED");
            rc = SBE_SEC_MEM_REGION_AMEND_ATTEMPTED;
            break;
        }
        if(iv_regionsOpenCnt  >= iv_maxRegions)
        {
            SBE_ERROR(SBE_FUNC" SBE_SEC_MAXIMUM_MEM_REGION_EXCEEDED");
            rc = SBE_SEC_MAXIMUM_MEM_REGION_EXCEEDED;
            break;
        }
        SBE_INFO(SBE_FUNC" Adding region Mem[0x%08X%08X], size[0x%08X]",
                    SBE::higher32BWord(i_startAddr),
                    SBE::lower32BWord(i_startAddr),
                    i_size);
        iv_memRegions[iv_regionsOpenCnt++] = {i_startAddr, i_size, i_mode};
        SBE_INFO(SBE_FUNC" after addition iv_regionsOpenCnt [%d]",
                iv_regionsOpenCnt);
    } while(0);
    return rc;
    #undef SBE_FUNC
}

sbeSecondaryResponse SBESecureMemRegionManager::remove(const uint64_t i_startAddr)
{
    #define SBE_FUNC "SBESecureMemRegionManager::remove"
    size_t i = 0;
    sbeSecondaryResponse rc = SBE_SEC_OPERATION_SUCCESSFUL;
    for(; i < iv_regionsOpenCnt; i++)
    {
        if(i_startAddr == iv_memRegions[i].startAddress)
        {
            break;
        }
    }
    if(i < iv_regionsOpenCnt)
    {
        SBE_INFO(SBE_FUNC" Deleting region i[%d], Mem[0x%08X%08X], size[0x%08X]",
                    i,
                    SBE::higher32BWord(iv_memRegions[i].startAddress),
                    SBE::lower32BWord(iv_memRegions[i].startAddress),
                    iv_memRegions[i].size);
        // Remove the empty slot and maintain contiguous list
        for(size_t j = i; j < iv_regionsOpenCnt-1; j++)
        {
            iv_memRegions[j].startAddress = iv_memRegions[j+1].startAddress;
            iv_memRegions[j].size = iv_memRegions[j+1].size;
            iv_memRegions[j].mode = iv_memRegions[j+1].mode;
        }

        iv_regionsOpenCnt--;
        SBE_INFO(SBE_FUNC" After deletion : iv_regionsOpenCnt[%d]", iv_regionsOpenCnt);
    }
    else
    {
        SBE_ERROR(SBE_FUNC" SBE_SEC_MEM_REGION_NOT_FOUND");
        rc = SBE_SEC_MEM_REGION_NOT_FOUND;
    }
    return rc;
    #undef SBE_FUNC
}

#endif //__SBEFW_SEEPROM__
#ifndef __SBEFW_SEEPROM__

sbeSecondaryResponse SBESecureMemRegionManager::isAccessAllowed(
                                        secureMemRegion_t i_region)
{
    #define SBE_FUNC "SBESecureMemRegionManager::isAccessAllowed"
    sbeSecondaryResponse rc = SBE_SEC_OPERATION_SUCCESSFUL;
    if(SBE_GLOBAL->sbeFWSecurityEnabled)
    {
        while(i_region.size > 0)
        {
            secureMemRegion_t foundregion = getPartialRegionSize(i_region);
            // Check if the found region has allowable access level
            // and that the region overlap is from the beginning itself
            if((i_region.mode & foundregion.mode) &&
                    (i_region.startAddress == foundregion.startAddress))
            {
                SBE_INFO(SBE_FUNC" foundRegion Mem[0x%08X%08X], size[0x%08X]",
                        SBE::higher32BWord(foundregion.startAddress),
                        SBE::lower32BWord(foundregion.startAddress),
                        foundregion.size);
                i_region.size -= foundregion.size;
                i_region.startAddress += foundregion.size;
            }
            else
            {
                SBE_ERROR(SBE_FUNC" Non secure access to memory blocked "
                          "Addr[0x%08X%08X] Size[0x%08X]",
                          SBE::higher32BWord(i_region.startAddress),
                          SBE::lower32BWord(i_region.startAddress),
                          i_region.size);
                rc = SBE_SEC_BLACKLISTED_MEM_ACCESS;
                break;
            }
        }
    }
    return rc;
    #undef SBE_FUNC
}
#endif //__SBEFW_SEEPROM__
