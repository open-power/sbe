/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeMemAccessInterface.C $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
#include "sbeMemAccessInterface.H"

#ifdef SEEPROM_IMAGE
// Using Function pointer to force long call
p9_adu_access_FP_t p9_adu_access_hwp = &p9_adu_access;
p9_adu_setup_FP_t p9_adu_setup_hwp = &p9_adu_setup;
#endif

using namespace fapi2;

ReturnCode sbeMemAccessInterface::setup()
{
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;

    // Reset the current granule count
    iv_currGranule = 0;
    SBE_INFO("iv_addr [0x%08x%08x]", SBE::higher32BWord(iv_addr), SBE::lower32BWord(iv_addr));
    if(iv_interface == SBE_MEM_ACCESS_PBA)
    {
        // Call the PBA setup HWP
        SBE_EXEC_HWP(l_fapiRc,
                     p9_pba_setup,
                     plat_getChipTarget(),
                     iv_ex,
                     iv_addr,
                     (iv_mode == SBE_MEM_ACCESS_READ),
                     ((p9_PBA_oper_flag*)iv_flags)->setFlag(),
                     iv_maxGranule);
    }
    if(iv_interface == SBE_MEM_ACCESS_ADU)
    {
        // Call the ADU setup HWP
        SBE_EXEC_HWP(l_fapiRc,
                     p9_adu_setup_hwp,
                     plat_getChipTarget(),
                     iv_addr,
                     (iv_mode == SBE_MEM_ACCESS_READ),
                     ((p9_ADU_oper_flag*)iv_flags)->setFlag(),
                     iv_maxGranule)
    }
    // if setup returns error
    if(l_fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC" setup Failed");
    }
    else
    {
        // Assumption is Hwp won't return zero for Num Granules
        assert(0 != iv_maxGranule);

        SBE_INFO(SBE_FUNC "Hwp returned iv_maxGranule=[0x%08X]",
                                             iv_maxGranule);
    }
    return l_fapiRc;
}

ReturnCode sbeMemAccessInterface::accessGranule()
{
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;

    // Check if we need to do a setup before access
    if(iv_maxGranule == 0)
    {
        l_fapiRc = setup();
    }
    if(iv_interface == SBE_MEM_ACCESS_PBA)
    {
        // Call PBA access for read/write
        SBE_EXEC_HWP(l_fapiRc,
                     p9_pba_access,
                     plat_getChipTarget(),
                     iv_addr,
                     (iv_mode == SBE_MEM_ACCESS_READ),
                     ((p9_PBA_oper_flag*)iv_flags)->setFlag(),
                     (iv_currGranule == 0),
                     (iv_lastGranule || (iv_maxGranule == 1)),
                     (uint8_t *)&iv_buffer);
    }
    if(iv_interface == SBE_MEM_ACCESS_ADU)
    {
        // Call ADU access HWP for ADU write/read request
        SBE_EXEC_HWP(l_fapiRc,
                     p9_adu_access_hwp,
                     plat_getChipTarget(),
                     iv_addr,
                     (iv_mode == SBE_MEM_ACCESS_READ),
                     ((p9_ADU_oper_flag*)iv_flags)->setFlag(),
                     (iv_currGranule == 0),
                     (iv_lastGranule || (iv_maxGranule == 1)),
                     (uint8_t *)&iv_buffer)
    }

    // if access returns error
    if( l_fapiRc != FAPI2_RC_SUCCESS )
    {
        SBE_ERROR(SBE_FUNC" access Failed");
    }
    else
    {
        iv_maxGranule--;
        iv_currGranule++;
        // Advance the address
        iv_addr += iv_granuleSize;
        iv_iterator = (iv_mode == SBE_MEM_ACCESS_READ)?
                        iv_granuleSize : 0;
    }
    return l_fapiRc;
}

ReturnCode sbeMemAccessInterface::accessWithBuffer(const void *io_buffer,
                                                   const uint32_t i_len,
                                                   const bool i_isLastAccess)
{
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    uint32_t l_iterator = 0;

    do
    {
        if(iv_mode == SBE_MEM_ACCESS_WRITE)
        {
            // Fill buffer
            while((iv_iterator < iv_granuleSize) && (l_iterator < i_len))
            {
                iv_buffer[iv_iterator++] = ((char*)io_buffer)[l_iterator++];
            }
            // If Adu, put the ecc and itag applicable bytes
            if(iv_interface == SBE_MEM_ACCESS_ADU)
            {
                if(((p9_ADU_oper_flag*)iv_flags)->getEccMode())
                {
                    iv_buffer[iv_iterator++] = ((char*)io_buffer)[l_iterator++];
                }
                if(((p9_ADU_oper_flag*)iv_flags)->getItagMode())
                {
                    iv_buffer[iv_iterator++] = ((char*)io_buffer)[l_iterator++];
                }
            }
        }

        if(((iv_mode == SBE_MEM_ACCESS_WRITE) &&
            (iv_iterator >= iv_granuleSize))
           ||
           ((iv_mode == SBE_MEM_ACCESS_READ) &&
            (iv_iterator == 0)))
        {
            if(i_isLastAccess)
            {
                if((iv_mode == SBE_MEM_ACCESS_WRITE) &&
                            (l_iterator >= i_len))
                {
                    iv_lastGranule = true;
                    alignAccessWithBuffer();
                }
                else if((iv_mode == SBE_MEM_ACCESS_READ) &&
                            ((i_len - l_iterator) <= iv_granuleSize))
                {
                    iv_lastGranule = true;
                }
            }
            l_fapiRc = accessGranule();
        }

        if(iv_mode == SBE_MEM_ACCESS_READ)
        {
            // Fill the buffer
            while((iv_iterator > 0) && (l_iterator < i_len))
            {
                ((char*)io_buffer)[l_iterator++] =
                        iv_buffer[iv_granuleSize - iv_iterator];
                iv_iterator--;
            }
            // If Adu, get the ecc and itag applicable bytes
            if(iv_interface == SBE_MEM_ACCESS_ADU)
            {
                uint32_t l_index = iv_granuleSize;
                if(((p9_ADU_oper_flag*)iv_flags)->getEccMode())
                {
                    ((char*)io_buffer)[l_iterator++] =
                                            iv_buffer[l_index++];
                }
                if(((p9_ADU_oper_flag*)iv_flags)->getItagMode())
                {
                    ((char*)io_buffer)[l_iterator++] =
                                            iv_buffer[l_index];
                }
            }
        }

        // Break out on error or if the data is completely sent
        if((l_fapiRc != FAPI2_RC_SUCCESS) || (l_iterator >= i_len))
        {
            break;
        }
    } while(true);

    return l_fapiRc;
}

void sbeMemAccessInterface::alignAccessWithBuffer()
{
    // Required to fill zeroes only if the iv_buffer is partially occupied
    if(iv_iterator != 0)
    {
        // zero filling
        while(iv_iterator < iv_granuleSize)
        {
            iv_buffer[iv_iterator++] = 0;
        }
    }
}
