/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/hwpf/fapi2/include/variable_buffer_utils.H $       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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

#ifndef __FAPI2_VARIABLE_BUFFER_UTILS__
#define __FAPI2_VARIABLE_BUFFER_UTILS__

#include <inttypes.h>

#include <ecmdDataBufferBase.H>
#include <ecmdUtils.H>

#include <variable_buffer.H>
#include <return_code_defs.H>
#include <plat_trace.H>

namespace fapi2
{
/** @name Variable buffer utility functions for platforms that support eCMD **/
//@{

/**
 * @brief Copies contents of fapi2::variable_buffer to a ecmdDataBufferBase
 * @param[in] i_src fapi::variable_buffer to copy from
 * @param[out] o_dest ecmdDataBufferBase to copy to
 * @return uint32_t return code zero if success
 */
inline uint32_t bufferCopy(ecmdDataBufferBase& o_dest, const fapi2::variable_buffer& i_src)
{
    uint32_t rc = 0;
    uint32_t l_ecmdRc = 0;
    uint32_t l_bitLength = i_src.getBitLength();
    const uint32_t l_bitsPerWord = sizeof(uint32_t) * 8;
    uint32_t l_wordLength = l_bitLength / l_bitsPerWord;
    uint32_t l_currentWord = 0;
    uint32_t l_extractData = 0;
    fapi2::ReturnCodes l_copy_rc = fapi2::FAPI2_RC_SUCCESS;

    l_ecmdRc = o_dest.setBitLength(l_bitLength);

    if (l_ecmdRc)
    {
        FAPI_ERR("Error calling o_dest.setBitLength(%d) rc = 0x%08X\n",
                 l_bitLength, l_ecmdRc);
        rc = l_ecmdRc;
    }

    while ((l_currentWord < l_wordLength) && (rc == 0))
    {
        l_copy_rc = i_src.extract(l_extractData, l_currentWord * l_bitsPerWord, l_bitsPerWord);

        if (l_copy_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("Error calling i_src.extract(l_extractData, %d, %d) "
                     "rc = 0x%08X \n", l_currentWord * l_bitsPerWord, l_bitsPerWord,
                     static_cast<uint32_t>(l_copy_rc));
            rc = static_cast<uint32_t>(l_copy_rc);
            break;
        }

        l_ecmdRc = o_dest.insert(l_extractData, l_currentWord * l_bitsPerWord, l_bitsPerWord, 0);

        if (l_ecmdRc)
        {
            FAPI_ERR("Error calling o_dest.insert(0x%08X, %d, %d, 0) rc = 0x%08X\n",
                     l_extractData, l_currentWord * l_bitsPerWord, l_bitsPerWord, l_ecmdRc);
            rc = l_ecmdRc;
            break;
        }

        l_currentWord++;
    }

    if ((l_bitLength > (l_wordLength * l_bitsPerWord)) && (rc == 0))
    {
        uint32_t l_bitsRemaining = l_bitLength - (l_wordLength * l_bitsPerWord);
        l_copy_rc = i_src.extract(l_extractData, l_wordLength * l_bitsPerWord, l_bitsRemaining);

        if (l_copy_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("error calling i_src.extract(l_extractData, %d, %d) rc = 0x%08X \n",
                     l_wordLength * l_bitsPerWord, l_bitsRemaining,
                     static_cast<uint32_t>(l_copy_rc));
            rc = static_cast<uint32_t>(l_copy_rc);
        }
        else
        {
            l_ecmdRc = o_dest.insert(l_extractData, l_wordLength * l_bitsPerWord, l_bitsRemaining, 0);

            if (l_ecmdRc)
            {
                FAPI_ERR("Error calling o_dest.insert(%08X, %d, %d, 0) rc = 0x%08X\n",
                         l_extractData, l_wordLength * l_bitsPerWord, l_bitsRemaining, l_ecmdRc);
                rc = l_ecmdRc;
            }
        }
    }

    return rc;
}

/**
 * @brief Copies contents of ecmdDataBufferBase to a fapi2::variable_buffer
 * @param[in] i_src ecmdDataBufferBase to copy from
 * @param[out] o_dest fapi::variable_buffer to copy to
 * @return uint32_t return code zero if success
 */
inline uint32_t bufferCopy(fapi2::variable_buffer& o_dest, ecmdDataBufferBase& i_src)
{
    uint32_t rc = 0;
    uint32_t* l_data = ecmdDataBufferBaseImplementationHelper::getDataPtr(&i_src);

    if (l_data != NULL)
    {
        o_dest = fapi2::variable_buffer(l_data, i_src.getCapacity(), i_src.getBitLength());
    }
    else
    {
        FAPI_ERR("Error getting data pointer to ecmdDataBufferBase\n");
        rc = 1;
    }

    return rc;
}

//@}

} // namespace fapi2

#endif /* __FAPI2_VARIABLE_BUFFER_UTILS__ */
