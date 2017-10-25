/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeSecureMemRegionManager.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "sbe_sp_intf.H"

enum class memRegionMode:uint8_t
{
    READ = 0x01,
    WRITE = 0x02,
};

// Structure to define a mem region
typedef struct
{
    uint64_t startAddress;
    uint32_t size;
    uint32_t mode;
} secureMemRegion_t;

// OCC SRAM Command buffer
constexpr uint64_t OCC_CMD_ADDR = 0xFFFBE000ull;
constexpr uint32_t OCC_CMD_SIZE = 0xFFFBEFFF - 0xFFFBE000;
// OCC SRAM Response buffer
constexpr uint64_t OCC_RESP_ADDR = 0xFFFBF000ull;
constexpr uint32_t OCC_RESP_SIZE = 0xFFFBFFFF- 0xFFFBF000;

class SBESecureMemRegionManager
{
    protected:
    secureMemRegion_t *iv_memRegions;
    const size_t iv_maxRegions;
    size_t iv_regionsOpenCnt;

    /*
     * @brief getPartialRegionSize - get the overlapping region
     *                               if it exists.
     *                               Also if the overlap is from the beginning,
     *                               first region is returned
     *
     *
     * @param[in] i_region           region to check for overlap
     *
     * @return overlapping region
     */
    secureMemRegion_t getPartialRegionSize(const secureMemRegion_t i_region);

    public:
    SBESecureMemRegionManager(secureMemRegion_t *i_regions,
                              size_t i_maxRegions):
                              iv_memRegions(i_regions),
                              iv_maxRegions(i_maxRegions)
    {
    }

    // Disable copy constructors
    SBESecureMemRegionManager(const SBESecureMemRegionManager&) = delete;
    SBESecureMemRegionManager& operator=(const SBESecureMemRegionManager&) = delete;

    /*
     * @brief add - Open a new memory region
     *
     * @param[in] i_startAddr  Start address of the memory region
     * @param[in] i_size       Size of the memory region
     * @param[in] i_mode       Bit mapped access mode of the memory region
     *
     * @return SBE secondary RC
     */
    sbeSecondaryResponse add(const uint64_t i_startAddr,
                             const uint32_t i_size,
                             const uint8_t i_mode);
    /*
     * @brief remove - Close a memory region
     *
     * @param[in] i_startAddr  Start address of the memory region
     *
     * @return SBE secondary RC
     */
    sbeSecondaryResponse remove(const uint64_t i_startAddr);
    /*
     * @brief isAccessAllowed - Check if the access is allowed
     *
     * @param[in] i_region     region to check the access for
     *
     * @return SBE secondary RC
     */
    sbeSecondaryResponse isAccessAllowed(secureMemRegion_t i_region);

    /*
     * @brief closeAllRegions - close all non-secure regions
     */
    void closeAllRegions()
    {
        iv_regionsOpenCnt = 0;
    }
};

class SBEOccSramSecMemRegionManager : public SBESecureMemRegionManager
{
    public:
        SBEOccSramSecMemRegionManager(secureMemRegion_t *i_regions,
                                  size_t i_maxRegions):
        SBESecureMemRegionManager(i_regions,
                                  i_maxRegions)
        {
            add(OCC_CMD_ADDR,
                OCC_CMD_SIZE,
                static_cast<uint8_t>( memRegionMode::WRITE) |
                static_cast<uint8_t>(memRegionMode::READ));
            add(OCC_RESP_ADDR,
                OCC_RESP_SIZE,
                static_cast<uint8_t>(memRegionMode::READ));
        }

        // Disable copy constructors
        SBEOccSramSecMemRegionManager(const SBEOccSramSecMemRegionManager&) = delete;
        SBEOccSramSecMemRegionManager& operator=(const SBEOccSramSecMemRegionManager&) = delete;
        // Disable delete functions
        sbeSecondaryResponse remove(const uint64_t i_startAddr) = delete;
        void closeAllRegions() = delete;

};

extern SBESecureMemRegionManager mainStoreSecMemRegionManager;
extern SBEOccSramSecMemRegionManager occSramSecRegionManager;
