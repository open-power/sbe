/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_hwp_data_stream.C $                             */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
#include "plat_hwp_data_stream.H"
#include "sbeFifoMsgUtils.H"

using namespace fapi2;

ReturnCodes sbefifo_hwp_data_istream::get(hwp_data_unit& o_data)
{
    #define SBE_FUNC "sbefifo_hwp_data_ostream::get"
    SBE_ENTER(SBE_FUNC);

    if (iv_words_left == 0)
    {
        return FAPI2_RC_FALSE;
    }

    uint32_t l_len = 1;
    uint32_t l_rc = sbeUpFifoDeq_mult(l_len, &o_data, iv_words_left == 1, false, iv_type);
    if (l_rc)
    {
        return FAPI2_RC_PLAT_ERR_SEE_DATA;
    }

    iv_words_left--;
    return FAPI2_RC_SUCCESS;
    #undef SBE_FUNC
}

ReturnCodes sbefifo_hwp_data_ostream::put(hwp_data_unit i_data)
{
    #define SBE_FUNC "sbefifo_hwp_data_ostream::put"
    SBE_ENTER(SBE_FUNC);

    uint32_t l_len = 1;
    uint32_t l_rc = sbeDownFifoEnq_mult(l_len, &i_data, iv_type);
    if (l_rc)
    {
        return FAPI2_RC_PLAT_ERR_SEE_DATA;
    }

    iv_words_written++;
    return FAPI2_RC_SUCCESS;
    #undef SBE_FUNC
}

seeprom_hwp_data_istream::seeprom_hwp_data_istream(const uint32_t *i_data, size_t i_size) :
    // round iv_data up to an 8byte boundary
    iv_data(reinterpret_cast<uint64_t *>(reinterpret_cast<unsigned long>(i_data + 1) & ~7)),
    // determine whether i_data starts on an even or odd 4-byte word
    iv_odd(reinterpret_cast<unsigned long>(i_data) & 4),
    // round size up to a multiple of 4 bytes
    iv_words_left((i_size + 3) >> 2)
{
    if (iv_odd)
    {
        // i_data begins on an odd word boundary; we already
        // set iv_data to point to the next even word, and now
        // have to read the first word we skipped into iv_2ndword
        // for the first get() operation.
        iv_2ndword = *i_data;
    }
}

ReturnCodes seeprom_hwp_data_istream::get(hwp_data_unit& o_data)
{
    #define SBE_FUNC "seeprom_hwp_data_ostream::get"
    SBE_ENTER(SBE_FUNC);

    if (iv_words_left == 0)
    {
        return FAPI2_RC_FALSE;
    }
    else if (iv_odd)
    {
        // Return the previously stored lower half
        o_data = iv_2ndword;
    }
    else
    {
        // Read one 8-byte word from the SEEPROM
        uint64_t l_seeprom_data;
        PPE_LVD(iv_data, l_seeprom_data);
        iv_data++;

        // Return the upper half, store the lower half
        o_data = l_seeprom_data >> 32;
        iv_2ndword = l_seeprom_data & 0xFFFFFFFF;
    }

    iv_words_left--;
    iv_odd = !iv_odd;
    return FAPI2_RC_SUCCESS;
    #undef SBE_FUNC
}
