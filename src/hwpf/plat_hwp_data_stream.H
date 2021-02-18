/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_hwp_data_stream.H $                             */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#include "sbeMemAccessInterface.H"

namespace fapi2
{

    static const uint64_t ADDR_NOT_INITAILSED = 0x00;
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
         * @param[in] expected_words Expected number of words (hwp_data_units)
         *                           that will be available in the FIFO.
         * @param[in] data           ponter to hold istream data [Only DUMP usage]
         * @param[in] fifoType       Which FIFO to use
         * @param[in] doFifoAccess   Do FIFO data stream to get data
         */
        sbefifo_hwp_data_istream( sbeFifoType fifoType = SBE_FIFO,
                                  uint32_t words_left = 0,
                                  uint32_t *data = NULL,
                                  bool doFifoAccess = true ):
                                  iv_fifoType(fifoType),
                                  iv_words_left(words_left), iv_data(data),
                                  iv_doFifoAccess(doFifoAccess) {};

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

        /**
         * @brief Reads bytes of buffer data from the caller to the FIFO.
         *
         * @param[in] length     The amount of data buffer units the buffer
         * @param[in] buffer     The buffer data to read from FIFO/dump data.
         * @param[in] isEot      fifo eot expected then true or else false 
         * @param[in] flush      fifo flush then true or else false
         *
         * @return
         *    FAPI2_RC_SUCCESS if data is available
         *    any other RC     if an error occurred and the data could not be
         *                     read.
         */
        uint32_t get( uint32_t length, uint32_t* buffer,
                      bool isEot = true, bool flush = false );

        /**
         * @brief Return FIFO type as per the stream init
         *
         * @return return Fifo Type
         */
        sbeFifoType getFifoType()
        {
            return iv_fifoType;
        }

    private:
        const sbeFifoType iv_fifoType;
        uint32_t iv_words_left;
        uint32_t* iv_data;
        bool iv_doFifoAccess;
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
         * @param[in] fifoType       Which FIFO to use
         * @param[in] doRespHeader   Do FIFO header response
         *
         */
        sbefifo_hwp_data_ostream( sbeFifoType type = SBE_FIFO,
                                  bool doRespHeader = true,
                                  uint64_t hbMemAddr = ADDR_NOT_INITAILSED ):
                                  iv_words_written(0),
                                  iv_isResponseHeader(doRespHeader),
                                  iv_hbMemAddr(hbMemAddr),
                                  iv_isPBALastAccess(false),
                                  iv_fifoType(type),
                                  iv_primarySecondaryRc(SBE_PRI_OPERATION_SUCCESSFUL),
                                  iv_ffdcRc(FAPI2_RC_SUCCESS),
                                  pbaFlag(),
                                  iv_memInterface(SBE_MEM_ACCESS_PBA,hbMemAddr,&pbaFlag,SBE_MEM_ACCESS_WRITE,sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES)
        {

            SBE_INFO("sbefifo_hwp_data_ostream() Contructor");
            if(hbMemAddr)
            {

                pbaFlag.setOperationType(p10_PBA_oper_flag::INJ);
                SBE_DEBUG(" sbefifo_hwp_data_ostream() will write to address iv_hbMemAddr=0x%.8x%.8x",
                            SBE::higher32BWord(iv_hbMemAddr),SBE::lower32BWord(iv_hbMemAddr));
            }
            else
            {
                SBE_DEBUG("sbefifo_hwp_data_ostream() will write to FIFO");
            }
        };

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
         * @brief Writes bytes of buffer data from the caller to the FIFO.
         *
         * @param[in] i_length The amount of data buffer units the buffer can hold
         * @param[in] i_buffer The buffer data to write to FIFO.
         *
         * @return
         *    FAPI2_RC_SUCCESS if data is available
         *    any other RC     if an error occurred and the data could not be
         *                     written.
         */
        uint32_t put(uint32_t i_length, uint32_t* i_buffer);

        /**
         * @brief Return the amount of words written to the stream so far.
         *
         * @return Amount of data written in words (hwp_data_unit)
         */
        uint32_t words_written()
        {
            return iv_words_written;
        }

        /**
         * @brief Return FIFO Response Header is required to the stream or not
         *
         * @return return Response Header
         */
        bool isStreamRespHeader( uint32_t hdrRc  = SBE_PRI_OPERATION_SUCCESSFUL,
                                 uint32_t ffdcRc = FAPI2_RC_SUCCESS )
        {
            if(!iv_isResponseHeader)
            {
                iv_primarySecondaryRc = hdrRc;
                iv_ffdcRc = ffdcRc;
            }
            return iv_isResponseHeader;
        }

        /**
          * @brief set primary secondary RC as per the stream init
          *
          * @return return PriSec Rc
          */
        void setPriSecRc(uint32_t priSecRc = SBE_PRI_OPERATION_SUCCESSFUL)
        {
            iv_primarySecondaryRc = priSecRc;
            return;
        }

       /**
         * @brief Return primary secondary RC as per the stream init
         *
         * @return return PriSec Rc
         */
        uint32_t getPriSecRc()
        {
            return iv_primarySecondaryRc;
        }

        /**
         * @brief set FIFO RC as per the stream init
         *
         * @return return Fifo Rc
         */
        void setFifoRc(uint32_t ffdcRc = FAPI2_RC_SUCCESS)
        {
            iv_ffdcRc = ffdcRc;
            return;
        }
        /**
         * @brief Return FIFO RC as per the stream init
         *
         * @return return Fifo Rc
         */
        uint32_t getFifoRc()
        {
            return iv_ffdcRc;
        }

        /**
         * @brief Return FIFO type as per the stream init
         *
         * @return return Fifo Type
         */
        sbeFifoType getFifoType()
        {
            return iv_fifoType;
        }
        /**
         * @brief set PBA mode set inteface stop as per the ostream
         *
         * @return return void
         */
        void setPBALastAccess()
        {
            iv_isPBALastAccess = true;
        }

        /**
         * @brief return updated HB MEM address after adding DUMP data
         *
         * @return return updated value back to caller
         */ 
        uint64_t getUpdatedHbMemAddr()
        {
            return iv_hbMemAddr + (iv_words_written * 4);
        }

    private:
        uint32_t                 iv_words_written;
        bool                     iv_isResponseHeader;
        uint64_t                 iv_hbMemAddr;
        // Flag to push less than 128 bytes data into FIFO using PBA interface.
        bool                     iv_isPBALastAccess;
        const sbeFifoType        iv_fifoType;
        uint32_t                 iv_primarySecondaryRc;
        uint32_t                 iv_ffdcRc;
        p10_PBA_oper_flag        pbaFlag;
        sbeMemAccessInterface    iv_memInterface;
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
