/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.C $                       */
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

#include "sbeCollectDump.H"
#include "fapi2.H"
#include <hwp_data_stream.H>
#include <plat_hwp_data_stream.H>

using namespace fapi2;

inline bool sbeCollectDump::dumpTypeCheck()
{
    return (iv_hdctRow->genericHdr.dumpContent & iv_hdctDumpTypeMap);
}

uint32_t sbeCollectDump::writeDumpPacketRowToFifo()
{
    #define SBE_FUNC "writeDumpPacketRowToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::collectAllHDCTEntries()
{
    #define SBE_FUNC " collectAllHDCTEntries "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        // Write the dump header to FIFO
        uint32_t len = sizeof(dumpHeader_t)/ sizeof(uint32_t);
        rc = iv_oStream.put(len, (uint32_t*)&iv_dumpHeader);
        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        while(sbeCollectDump::parserSingleHDCTEntry())
        {
            SBE_INFO("Collecting Data");
            rc = writeDumpPacketRowToFifo();
            if(rc)
            {
                SBE_ERROR(SBE_FUNC" Dump collection failed");
                rc = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                break;
            }
        }
    }
    while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::parserSingleHDCTEntry()
{
    #define SBE_FUNC " parserSingleHDCTEntry "
    SBE_ENTER(SBE_FUNC);

    do
    {
        if(iv_hdctXipSecDetails.currAddr < iv_hdctXipSecDetails.endAddr)
        {
            //Parse single HDCT row and populate genericHdctRow_t struct
            iv_hdctRow = (genericHdctRow_t*)iv_hdctXipSecDetails.currAddr;
            if(genericHdctRowSize_table[(uint8_t)(iv_hdctRow->genericHdr.command)] == CMD_TYPE_NOT_USED)
            {
                SBE_ERROR("Unknown command type: %X",(uint8_t) iv_hdctRow->genericHdr.command);
                SBE_ERROR("Error in parsing HDCT.bin");
                return false;
            }
            //Increament the current address to point to the next HDCT row
            iv_hdctXipSecDetails.currAddr = iv_hdctXipSecDetails.currAddr + genericHdctRowSize_table[(uint8_t)(iv_hdctRow->genericHdr.command)];
        }
        else
        {
            SBE_INFO("All HDCT entries parsed");
            return false;
        }
    }while(!(sbeCollectDump::dumpTypeCheck()));

    SBE_EXIT(SBE_FUNC);
    return true;
    #undef SBE_FUNC
}

//populateHDCTRowTOC()
uint32_t sbeCollectDump::populateHDCTRowTOC()
{
    #define SBE_FUNC " populateHDCTRowTOC "


    return 0;
    #undef SBE_FUNC
}

//collectSingleEntry()
uint32_t sbeCollectDump::collectSingleEntry(uint8_t * i_data, uint32_t i_len)
{
    return 0;
}

//writeDatatoFIFO()
uint32_t sbeCollectDump::writeDataToFIFO(uint8_t * i_data, uint32_t i_len)
{
    return 0;
}

//checkHWStateofChipAndChiplets()
bool sbeCollectDump::checkHWStateofChipAndChiplets()
{
    return 0;
}
