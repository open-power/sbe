/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_hwp_data_stream.H $                             */
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
#ifndef PLATHWPDATASTREAM_H_
#define PLATHWPDATASTREAM_H_

/**
 *  @file plat_hw_access.H
 *  @brief Define platform specific calls for hardware accesses.
 *
 *  FAPI have the abstract class for hwp_data_istream and hwp_data_ostream.
 *  These classes have declarations for get/put methods.
 *
 *  SBE will derive sub classes out of abstract classes.
 *  It will have the plat implementation for get/put methods.
 *
 */

#include "fapi2.H"
#include "hwp_data_stream.H"
#include "sbeFifoMsgUtils.H"

namespace fapi2
{
    /**
     * @brief Stream data from FIFO into a HWP
     *
     * This class can be used to stream four bytes of data to the caller.
     */

    class sbefifo_hwp_data_istream : public hwp_data_istream
    {
    public:
        /**
         * @brief Construct a FIFO data stream
         *
         * @param[in] i_expected_words Expected number of words (hwp_data_units)
         *                             that will be available in the FIFO.
         * @param[in] i_type Which FIFO to use
         */
        sbefifo_hwp_data_istream(uint32_t i_expected_words,
                                 sbeFifoType i_type = SBE_FIFO) :
            iv_words_left(i_expected_words), iv_type(i_type) {};

        /**
         *	@brief Reads 4 bytes of data from the FIFO to the caller.
         *
         * @param[out] o_data The data unit is returned here
         *
         * @return
         *    FAPI2_RC_SUCCESS if data is available
         *    FAPI2_RC_FALSE   if the end of the stream has been reached
         *    any other RC     if an error occurred
         */
        ReturnCodes get(hwp_data_unit& o_data);

    private:
        uint32_t iv_words_left;
        const sbeFifoType iv_type;
    };

    /**
     * @brief Stream data out from HWP to FIFO.
     *
     * This class can be used to stream out four byte of data into FIFO.
     */

    class sbefifo_hwp_data_ostream : public hwp_data_ostream
    {
    public:
        /**
         * @brief Construct a FIFO data stream
         *
         * @param[in] i_type Which FIFO to use
         */
        sbefifo_hwp_data_ostream(sbeFifoType i_type = SBE_FIFO) :
            iv_words_written(0), iv_type(i_type) {};

        /**
         *	@brief Writes 4 bytes of data from the caller to the FIFO.
         *
         * @param[in] i_data The data to write to FIFO.
         *
         *@return
         *    FAPI2_RC_SUCCESS if data is available
         *    any other RC     if an error occurred and the data could not be written.
         */
        ReturnCodes put(hwp_data_unit i_data);

        /**
         * @brief Return the amount of words written to the stream so far.
         *
         * @return Amount of data written in words (hwp_data_unit)
         */
        uint32_t words_written()
        {
            return iv_words_written;
        }

    private:
        uint32_t iv_words_written;
        const sbeFifoType iv_type;
    };

    /**
     * @brief Stream data from a location in SEEPROM into a HWP
     *
     * This class reads data from the SEEPROM in increments of 8 bytes
     * and hands it back to the caller in increments of 4 bytes - the goal
     * is to minimize the amount of SEEPROM accesses.
     */
    class seeprom_hwp_data_istream : public hwp_data_istream
    {
    public:
        /**
         * @brief Construct a SEEPROM data stream
         *
         * @param[in] i_data Pointer to the beginning of the source
         *                   data in SEEPROM. Must be 4-byte aligned.
         * @param[in] i_size Size of the data block in bytes, will be
         *                   rounded up to a multiple of 4.
         */
        seeprom_hwp_data_istream(const uint32_t *i_data, size_t i_size);

        /**
         *	@brief Reads 4 bytes of data from the SEEPROM to the caller.
         *
         * @param[out] o_data The data unit is returned here
         *
         * @return
         *    FAPI2_RC_SUCCESS if data is available
         *    FAPI2_RC_FALSE   if the end of the stream has been reached
         *    any other RC     if an error occurred
         */
        ReturnCodes get(hwp_data_unit& o_data);

    private:
        const uint64_t *iv_data;
        bool iv_odd;
        uint32_t iv_words_left;
        uint32_t iv_2ndword;
    };
}
#endif // PLATHWPDATASTREAM_H_
