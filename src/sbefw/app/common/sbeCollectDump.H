/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.H $                       */
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

#ifndef __SBEFW_SBECOLLECTDUMP_H
#define __SBEFW_SBECOLLECTDUMP_H

#include <stdint.h>
#include "sbefifo.H"
#include "sbecmddump.H"
#include "p10_ddco.H"
#include "sbeDumpConstants.H"
#include "sbeFifoMsgUtils.H"
#include "plat_hw_access.H"
#include "plat_hwp_data_stream.H"

//Dump chip-op Footer - DONE
static const uint32_t DUMP_CHIP_OP_FOOTER = 0x444F4E45;
static const uint32_t FIFO_DOUBLEWORD_LEN = 2;

extern size_t genericHdctRowSize_table[NO_OF_CMD_TYPES];

//Struct with complete HDCT.bin header section
//(HDCT XIP section Header + EKB Commit ID)
typedef struct __attribute__((packed))
{
     DdContHeader_t hdctXipSectionHdr;
     //TODO:Remove p9_dd_block once DdContHeader_t struct issue is fixed in ekb
     struct p9_dd_block p9_dd_blk;
     uint64_t ekbCommitId;
}hdctSectionHdr_t;

//Struct containing details of HDCT XIP section
typedef struct hdctXipSecDetails
{
    uint32_t startAddr;
    uint32_t size;
    uint32_t endAddr;
    uint32_t currAddr;

    /**
     * @brief Get the HDCT Xip offset(Start location) from SBE Image.
     *
     * @return  HDCT Xip offset(Start location)
     */
    uint32_t getHdctXipOffset()
    {
#if defined DFT || defined PIBMEM_ONLY_IMAGE
        uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
        uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif
        P9XipHeader* imgHdr = (P9XipHeader*)(base);
        p9_xip_section_sbe_t sectionName = P9_XIP_SECTION_SBE_HDCT;
        P9XipSection* pSection = &imgHdr->iv_section[sectionName];
        return ( (uint32_t)base + pSection->iv_offset);
    }

    /**
     * @brief Get the HDCT Xip Size.
     *
     * @return  HDCT Xip Size
     */
    uint32_t getHdctXipSize()
    {
#if defined DFT || defined PIBMEM_ONLY_IMAGE
        uint8_t *base = (uint8_t*)(SBE_BASE_ORIGIN);
#else
        uint8_t *base = (uint8_t*)(SBE_SEEPROM_BASE_ORIGIN);
#endif
        P9XipHeader* imgHdr = (P9XipHeader*)(base);
        p9_xip_section_sbe_t sectionName = P9_XIP_SECTION_SBE_HDCT;
        P9XipSection* pSection = &imgHdr->iv_section[sectionName];
        return (pSection->iv_size);
    }

    /**
     * @brief Init all members(Constructor)
     */
    hdctXipSecDetails()
    {
        startAddr = getHdctXipOffset();
        size = getHdctXipSize();
        endAddr = startAddr + size;
        //Set currAddr to start of HDCT row
        currAddr = startAddr + sizeof(hdctSectionHdr_t);

        SBE_INFO("Start Offset: [0x%08X] Size: [0x%08X] End offset: [0x%08X] ",
                startAddr,size,endAddr );
    }

}hdctXipSecDetails_t;

//Struct for getDump chip-op header
typedef struct __attribute__((packed)) dumpHeader
{
    uint32_t dataLayoutVersion:8;
    uint32_t reserved:24;
    //EKB Commit ID
    uint64_t hdctVer;

    /**
     * @brief Constructor
     */
    dumpHeader()
    {
        //TODO: Needs to be updated to right version
        dataLayoutVersion = 0x01;
        reserved = 0x00;
    }

}dumpHeader_t;

//Generic struct for each HDCT row TOC.
//(Data to be streamed out of FIFO)
typedef struct
{
    struct __attribute__((packed))
    {
        uint32_t preReq:1;
        uint32_t reserved:7;
        uint32_t cmdType:8;
        uint32_t chipUnitType:8;
        uint32_t chipUnitNum:8;
        uint64_t address;
        uint32_t dataLength;
    }tocHeader;

    uint32_t ffdcLen;
    uint64_t cpuCycles;
    fapi2::plat_target_handle_t tgtHndl;

    void tocHeaderInit(genericHdctRow_t* genericHdctRow)
    {
        //TODO:Need to be filled with right value
        tocHeader.preReq = true;
        tocHeader.reserved = 0x0;
        tocHeader.cmdType = (uint8_t) genericHdctRow->genericHdr.command;
        tocHeader.chipUnitType = (uint8_t) genericHdctRow->genericHdr.chipUnitType;
    }
}genericRowTOC_t;

class sbeCollectDump
{
    private:
        fapi2::sbefifo_hwp_data_ostream iv_oStream; // ostream to push FIFO data
        uint8_t iv_clockState;    //Dump collection clock state(ON or OFF)
        sbeFifoType iv_fifoType;  //Type of fifo to stream the data.
        uint16_t iv_hdctDumpTypeMap; //Equivalent HDCT dump type wrt requested dump type.
        //Unset the below flag once we find the 1st clock HDCT entry for Clock OFF state. Flag needs to be unset if clock ON state(Done in constructor).
        bool iv_clockOffEntryFlag = true;

        genericHdctRow_t *iv_hdctRow;
        genericRowTOC_t iv_tocRow;
        dumpHeader_t iv_dumpHeader;
        hdctXipSecDetails_t iv_hdctXipSecDetails;
        hdctSectionHdr_t *iv_hdctSectionHdr;

    public:

        /**
         * @brief Constructor for the class sbeCollectDump
         *
         * @param [in] dumpID      The id of the dumpType.
         * @param [in] clockState  Dump collection clock state.(ON or OFF)
         * @param [in] type        sbe fifo type.
         */
         sbeCollectDump( uint8_t i_dumpID, uint8_t i_clockState,
                         sbeFifoType i_type ):iv_oStream(i_type, false),
                         iv_clockState(i_clockState), iv_fifoType(i_type)
         {
             iv_clockState = i_clockState;
             if (iv_clockState == SBE_DUMP_CLOCK_ON)
                 iv_clockOffEntryFlag = false;

             //Parse the HDCT Xip section header
             iv_hdctSectionHdr = (hdctSectionHdr_t*)iv_hdctXipSecDetails.startAddr;

             //Get the equivalent HDCT dump type based on the requested Dump type
             iv_hdctDumpTypeMap = getEquivDumpType(i_dumpID);
             SBE_DEBUG("Dump Type Map is %x ", iv_hdctDumpTypeMap);

             //Populate dump header struct
             iv_dumpHeader.hdctVer = iv_hdctSectionHdr->ekbCommitId;
             SBE_INFO("EKB Commit ID: 0x%.8x%.8x ",
                       SBE::higher32BWord(iv_dumpHeader.hdctVer),
                       SBE::lower32BWord(iv_dumpHeader.hdctVer));
         }

        /**
         * @brief Collect the data for all the entries present in HDCT.
         *        It will be called from sbeGetDump() func.
         *
         * @return SUCCESS, Incase of no error.
         *         error,   Incase of failure.
         */
         uint32_t collectAllHDCTEntries();

    private:

        /*
         * @brief Check if requested dump type is present in the current HDCT row generic header.
         *        eg Checkstop, HB, MPIPL.
         * @return bool true  if the dumpType is present in the current HDCT row generic header.
         *              false if the dumpType is not present in the current HDCT row generic header.
         */
        inline bool dumpTypeCheck();

        /*
         * @brief Check if requested chip unit is present in the current HDCT row generic header.
         *        allowed unit chip numbers like -c2..39 / -c2..39 /  -c1..31
         *
         * @param [in] i_target plat target handle for chip type number.
         *
         * @return bool true  if the chip unit is present in the current HDCT row.
         *              false if the chip unit is not present in the current HDCT row.
         */
        bool isChipUnitNumAllowed(fapi2::plat_target_handle_t i_target);

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

       /** @brief stop clocks off as per HDCT row data.
        *
        * @return SUCCESS,  If chip-op is successful.
        *          ERROR,    Incase of failure.
        */
        uint32_t stopClocksOff();

       /**
        * @brief As per p10_query_host_meminfo HWP to determine address and size
        *        of HB memory and then write data to the FIFO
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        uint32_t writeGetMemPBAPacketToFifo();

       /** @brief Write getRing data to the FIFO.
        *
        * @return SUCCESS,  If chip-op is successful.
        *          ERROR,    Incase of failure.
        */
        uint32_t writeGetRingPacketToFifo();

       /**
        * @brief Write get sram data to the FIFO
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        uint32_t writeGetSramPacketToFifo();

       /**
        * @brief Write put scom updated data to the FIFO
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        uint32_t writePutScomPacketToFifo();

       /**
        * @brief Write get scom data to the FIFO.
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        uint32_t writeGetScomPacketToFifo();

       /**
        * @brief Write DUMP data to the FIFO
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        uint32_t writeDumpPacketRowToFifo();

       /**
        * @brief Get target list as per DUMP target id's.
        *
        * @param [in] o_targetList list of plat target handles
        *
        * @return SUCCESS,  If chip-op is successful.
        *         ERROR,    Incase of failure.
        */
        void getTargetList(std::vector<fapi2::plat_target_handle_t> &o_targetList);

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

