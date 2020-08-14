/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.H $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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

#include<stdint.h>
#include "sbefifo.H"

class sbeCollectDump
{
    private:
        // Generic Structre for each HDCT row.
        //TODO: Match this with the actual hdctEntry
        typedef struct
        {
            uint32_t dumpContent:10;
            uint32_t command:4;
            uint32_t chipType:6;
            uint32_t chipletStart:6;
            uint32_t chipletEnd:6;
        }genericHDCTHdt_t;

        // Generic Structre for each HDCT row with command type args.
        // TODO: Create a union of this struct.
        // No need to create different strucuture definition for differnt command.
        typedef struct
        {
            genericHDCTHdt_t hdr;
            uint64_t address;
            uint64_t value;
        }genericHDCTRow_t;

        // Generic struct for each HDCT row TOC.
        typedef struct
        {
            uint8_t preReq;
            uint8_t commandId;
            uint16_t chipletId;
            uint64_t address;
            uint32_t dataLength;
        }genericRowTOC_t;
        //TODO: Create the enum of the dumpType.
        uint32_t dumpType;    //Type of the dump.

        genericHDCTRow_t hdctRow;
        genericRowTOC_t tocRow;

        uint32_t *buffer;   //Data or FFDC for each row.
        uint32_t dataLen;   //Lenght of data or FFDC.
        uint64_t cpuCycles; //CPU cycles to complete the chip-op.

        sbeFifoType type;  //Type of fifo to stream the data.

    public:
    
        /**
         * @brief Constructor for the class sbeCollectDump
         *
         * @param [in] dumpID  The id of the dumpType.
         * @param [in] type    sbe fifo type.
         * @param [in] errHndl If the instance is created successfully or not..
         */
        sbeCollectDump(uint32_t dumpID, sbeFifoType type, uint32_t & errHndl);

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
        * @brief Validate the dumpType with the sbe supported dump type.
        *        eg Checkstop, HB, MPIPL.
        * @return bool true  if the dumpType is sbe supported dump type.
        *              false if the dumpType is not sbe supported.
        */
        bool validateDumpType();

       /**
        * @brief Parse the single HDCT entry and populate the genericHDCTRow_t object.
        *
        * @return SUCCESS,  if HDCT row is parsed and structure is populated.
        *         ERROR,    Incase of failure.
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
        uint32_t collectSingleEntry(uint8_t * data, uint32_t len);

       /**
        * @brief Write data to the FIFO.
        *
        * @param [in] data The data/FFDC returned from chip-op.
        * @prama [in] len  The length of the data returned.
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        uint32_t writeDatatoFIFO(uint8_t * data, uint32_t len);

       /**
        * @brief Check the HW state of the chip and its chipltes before chip-op.
        *
        * @return SUCCESS,  If chip-op is allowed.
        *         ERROR,    If chip-op is not allowed.
        */
        bool checkHWStateofChipAndChiplets();
};
