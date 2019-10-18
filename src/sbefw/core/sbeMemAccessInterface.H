/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeMemAccessInterface.H $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
#ifndef __SBE_MEM_ACCESS_INTERFACE_H
#define __SBE_MEM_ACCESS_INTERFACE_H

#include "fapi2.H"

#include "p10_pba_coherent_utils.H"
#include "p10_pba_setup.H"
#include "p10_pba_access.H"

#include "p10_adu_setup.H"
#include "p10_adu_access.H"
#include "p10_adu_utils.H"

using namespace fapi2;

/* @brief Enum for read or write interface
 * */
enum sbeMemAccessMode
{
    SBE_MEM_ACCESS_READ = 1,
    SBE_MEM_ACCESS_WRITE = 2,
};

/* @brief Enum for PBA or ADU interface
 * */
enum sbeMemAccessInterfaceType
{
    SBE_MEM_ACCESS_PBA,
    SBE_MEM_ACCESS_ADU,
};

// Helper functions
/*
 * @brief function to check if the length to send can be
 * accomodated in the available memory, else make it a last
 * access on the interface
 */
void MEM_AVAILABLE_CHECK(uint32_t &io_available_len,
                         uint32_t &io_len_to_send,
                         bool &io_is_last_access);

class sbeMemAccessInterface
{
    private:
        // Bigger of PBA/ADU granules
        static constexpr uint32_t MEM_INTERFACE_SIZE_BYTES = 128;

        // Address to start read/write from
        uint64_t iv_addr;
        // Interface type - PBA/ADU
        sbeMemAccessInterfaceType iv_interface;
        // Pointer to procedure flags - p10_PBA_oper_flag/adu_operationFlag
        void *iv_flags;
        // Read or Write mode
        sbeMemAccessMode iv_mode;
        // Core target used in PBA interface, not applicable for ADU
        fapi2::Target<fapi2::TARGET_TYPE_CORE> iv_core;
        // Number of granule that can be sent before calling setup again
        uint32_t iv_maxGranule;
        // Iterator to track the current depth of iv_buffer
        uint32_t iv_iterator;
        // Number of granules sent after the setup
        uint32_t iv_currGranule;
        // Granule size based on interface - PBA/ADU
        uint32_t iv_granuleSize;
        // Buffer size is bigger of the two granules
        char iv_buffer[MEM_INTERFACE_SIZE_BYTES]  __attribute__ ((aligned (8)));
        // Interface cleaned up - indicates if the underlying
        // HWP cleanup is done for the interface.
        // A new object assumes interface is in cleaned up
        // state.
        // If the interface is in cleaned up state,
        // next access will happen only after the setup.
        bool iv_intfCleanedUp;

        /* @brief Wrapper function to call setup procedures
         *        for the interface. This private API is used
         *        internally as per the state of the object,
         *        i.e., whenever the iv_maxGranule
         *        becomes 0, while using interface access APIs
         *
         * @return fapi rc
         */
        fapi2::ReturnCode setup();

        /* @brief Align and finalize the accessWithBuffer calls - useful in case
         *        the data to be read/written is not aligned to the
         *        granule sizes of the interfaces
         *
         * @return fapi rc
         */
         void alignAccessWithBuffer();

    public:
        // PBA / ADU Granule size as per the HWP Requirement
        static constexpr uint32_t PBA_GRAN_SIZE_BYTES  = 128;
        static constexpr uint32_t ADU_GRAN_SIZE_BYTES  = 8;

        /* @brief Constructor
         *
         * @param[in] i_interface   Type of the interface
         * @param[in] i_addr        Address to read/write from
         * @param[in] i_flags       Pointer to PBA/ADU flags
         * @param[in] i_mode        Read/Write mode
         * @param[in] i_granuleSize Granule size of the interface type
         * @param[in] i_core        Core target[Optional in case of ADU]
         */
        sbeMemAccessInterface(
                        sbeMemAccessInterfaceType i_interface,
                        uint64_t i_addr,
                        void *i_flags,
                        sbeMemAccessMode i_mode,
                        uint32_t i_granuleSize,
                        Target<TARGET_TYPE_CORE> i_core
                        = plat_getTargetHandleByInstance<TARGET_TYPE_CORE>(0)):
                        iv_addr(i_addr),
                        iv_interface(i_interface),
                        iv_flags(i_flags),
                        iv_mode(i_mode),
                        iv_core(i_core),
                        iv_maxGranule(0),
                        iv_iterator(0),
                        iv_currGranule(0),
                        iv_granuleSize(i_granuleSize),
                        iv_intfCleanedUp(true)
        {
        }

        void * getBuffer()
        {
            iv_iterator = iv_granuleSize;
            return &iv_buffer;
        }

        /* @brief  Read/Write a single granule on PBA/ADU interface
         *
         * @param[in] i_isLastAccess    Should the interface be cleaned up
         *                              after this access
         *
         * @return fapi rc
         */
        fapi2::ReturnCode accessGranule(const bool i_isLastAccess = false);

        /* @brief Read/Write on PBA/ADU interface with the given buffer.
         *
         * @param[in] io_buffer         Pointer to the data to read/write
         * @param[in] i_len             Length of the data to read/write in bytes
         * @param[in] i_isLastAccess    Should the interface be cleaned up
         *                              after this access
         *
         * @return fapi rc
         */
        fapi2::ReturnCode accessWithBuffer(const void *io_buffer,
                                           const uint32_t i_len,
                                           const bool i_isLastAccess = false);

};
#endif //__SBE_MEM_ACCESS_INTERFACE_H
