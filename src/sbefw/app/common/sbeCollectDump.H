/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.H $                       */
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

#ifndef __SBEFW_SBECOLLECTDUMP_H
#define __SBEFW_SBECOLLECTDUMP_H

#include<stdint.h>
#include "sbefifo.H"
#include "sbecmddump.H"

class sbeCollectDump
{
    private:

        // Generic struct for each HDCT row TOC.
        typedef struct
        {
            uint8_t preReq;
            uint8_t commandId;
            uint16_t chipletId;
            uint64_t address;
            uint32_t dataLength;
        }genericRowTOC_t;

        uint8_t iv_dumpType;    //Type of the dump requested

        genericRowTOC_t iv_tocRow;

        uint32_t *iv_buffer;   //Data or FFDC for each row.
        uint32_t iv_dataLen;   //Lenght of data or FFDC.
        uint64_t iv_cpuCycles; //CPU cycles to complete the chip-op.

        sbeFifoType iv_fifoType;  //Type of fifo to stream the data.

    public:

        /**
         * @brief Constructor for the class sbeCollectDump
         *
         * @param [in] dumpID  The id of the dumpType.
         * @param [in] type    sbe fifo type.
         */
        sbeCollectDump(uint32_t i_dumpID, sbeFifoType i_type);

        /**
         * @brief Collect the data for all the entries present in HDCT.
         *        It will be called from sbeGetDump() func.
         *
         * @return SUCCESS, Incase of no error.
         *         error,   Incase of failure.
         */
        uint32_t collectAllHDCTEntries();

    private:

        /**
         * @brief Parse the single HDCT entry and populate the genericHDCTRow_t object.
         *
         * @return SUCCESS,  if HDCT row is parsed and structure is populated and the requested dump
         *                   type present in the current HDCT generic header.
         *
         *         ERROR,    When there are no more HDCT entries/rows to be
         *                   parsed.
         */
        uint32_t parserSingleHDCTEntry();

        /**
         * @brief Parse the single HDCT entry and populate the genericHDCTRow_t object.
         *
         * @return SUCCESS,  if HDCT row is parsed and structure is populated.
         *         ERROR,    Incase of failure.
         */
        uint32_t populateHDCTRowTOC();

        /**
         * @brief Call the chip-opbased on HDCT row to gather the chip-op data.
         *
         * @param [in] data The data/FFDC returned from chip-op.
         * @prama [in] len  The length of the data returned.
         *
         * @return SUCCESS,  If chip-op is successful.
         *         ERROR,    Incase of failure.
         */
        uint32_t collectSingleEntry(uint8_t * i_data, uint32_t i_len);

        /**
         * @brief Write data to the FIFO.
         *
         * @param [in] data The data/FFDC returned from chip-op.
         * @prama [in] len  The length of the data returned.
         *
         * @return SUCCESS,  If chip-op is successful.
         *         ERROR,    Incase of failure.
         */
        uint32_t writeDataToFIFO(uint8_t * i_data, uint32_t i_len);

        /**
         * @brief Check the HW state of the chip and its chipltes before chip-op.
         *
         * @return SUCCESS,  If chip-op is allowed.
         *         ERROR,    If chip-op is not allowed.
         */
        bool checkHWStateofChipAndChiplets();
};

#endif /* __SBEFW_SBECOLLECTDUMP_H */

