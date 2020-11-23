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
#include <sbecmdsram.H>

using namespace fapi2;

inline bool sbeCollectDump::dumpTypeCheck()
{
    return (iv_hdctRow->genericHdr.dumpContent & iv_hdctDumpTypeMap);
}

bool sbeCollectDump::isChipUnitNumAllowed(fapi2::plat_target_handle_t i_target)
{
    fapi2::Target<TARGET_TYPE_ALL> dumpRowTgt(i_target);
    uint32_t chipUnitNum = dumpRowTgt.getChipletNumber();
    //  Verify check as per -c0..31 values dump data
    return ( (!iv_hdctRow->genericHdr.chipletStart) ||
             ( (chipUnitNum-1 >= iv_hdctRow->genericHdr.chipletStart) &&
               (chipUnitNum-1 <= iv_hdctRow->genericHdr.chipletEnd)) );
}

uint32_t sbeCollectDump::writeGetSramPacketToFifo()
{
    #define SBE_FUNC "writeGetSramPacketToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = iv_hdctRow->cmdGetSram.addr;
        iv_tocRow.tocHeader.dataLength = 0x2000; // Length in bits (1024 bytes)
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

        uint32_t addr = iv_hdctRow->cmdGetSram.addr;
        uint32_t mode = iv_hdctRow->cmdGetSram.extGenericHdr.mode;

        seeprom_hwp_data_istream stream((uint32_t*)&iv_hdctRow->cmdGetSram.value,
                                         sizeof(uint64_t));
        uint32_t msbValue, lsbValue;
        stream.get(msbValue), stream.get(lsbValue);
        uint64_t value = (((uint64_t)msbValue << 32 ) | ((uint64_t)lsbValue));

        // Create the req struct for the SRAM Chip-op
        sbeSramAccessReqMsgHdr_t dumpSramReq = {0};
        len  = sizeof(sbeSramAccessReqMsgHdr_t)/sizeof(uint32_t);
        dumpSramReq.chipletId = iv_tocRow.tocHeader.chipUnitNum; 
        dumpSramReq.multicastAccess = 0x00;
        dumpSramReq.reserved = 0x00;
        if( iv_hdctRow->cmdGetSram.extGenericHdr.mode )
        {
            dumpSramReq.mode = 0b01000000;
        }
        else
        {
            dumpSramReq.mode = 0b10000000;
        }
        dumpSramReq.addressWord0 = 0x00;
        dumpSramReq.addressWord1 = addr;
        dumpSramReq.length = (uint32_t) value;
        sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                          (uint32_t*)&dumpSramReq, false );

        rc = sbeSramAccess_Wrap( istream, iv_oStream, true );

        SBE_INFO("getSram value:[0x%08X%08X]",
                  SBE::higher32BWord(value), SBE::lower32BWord(value));
        SBE_INFO("getSram address:[0x%08X] mode:[0x%08X]", addr, mode);
    }
    while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::writePutScomPacketToFifo()
{
    #define SBE_FUNC "writePutScomPacketToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint64_t dumpData = 0;
    do
    {
        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = iv_hdctRow->cmdPutScom.addr;
        iv_tocRow.tocHeader.dataLength = 0x00;
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        
        uint32_t addr = iv_tocRow.tocHeader.address;
        uint32_t maskType = iv_hdctRow->cmdPutScom.extGenericHdr.bitModifier;
        fapi2::Target<TARGET_TYPE_ALL> dumpRowTgt(iv_tocRow.tgtHndl);
        seeprom_hwp_data_istream stream((uint32_t*)&iv_hdctRow->cmdPutScom.value,
                                         sizeof(uint64_t));
        uint32_t msbValue, lsbValue;
        stream.get(msbValue), stream.get(lsbValue);
        uint64_t mask = (((uint64_t)msbValue << 32 ) | ((uint64_t)lsbValue));
        dumpData = mask; // maskType is nnone then putScom data is mask value.

        SBE_INFO("putscom address:[0x%08X] mask:[0x%08X%08X]", addr,
                  SBE::higher32BWord(mask), SBE::lower32BWord(mask));

        if( B_NONE != maskType )
        {
            uint64_t readData = 0;
            fapiRc = getscom_abs_wrap(&dumpRowTgt, addr, &readData);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                // TODO: Verify and modify all error rc to handle all
                // primary/secondary error in DUMP
                rc = SBE_SEC_INVALID_ADDRESS_PASSED;
                break;
            }
            SBE_INFO(SBE_FUNC " putscom scom value: 0x%.8X%.8X ",
                     SBE::higher32BWord(readData),SBE::lower32BWord(readData));
            if( B_OR == maskType )
            {
                dumpData = (readData | mask);
            }
            if( B_AND != maskType )
            {
                dumpData = (readData & mask);
            }
        }
        SBE_INFO(SBE_FUNC " maskType[0x%02X], data [0x%08X %08X] ", maskType,
                      SBE::higher32BWord(dumpData),SBE::lower32BWord(dumpData));
        fapiRc = putscom_abs_wrap(&dumpRowTgt, addr, dumpData);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            // TODO: Verify and modify all error rc to handle all
            // primary/secondary error in DUMP
            rc = SBE_SEC_INVALID_ADDRESS_PASSED;
            break;
        }
    }
    while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
uint32_t sbeCollectDump::writeGetScomPacketToFifo()
{
    #define SBE_FUNC "writeGetScomPacketToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;

    // Update address, length and stream header data vai FIFO
    iv_tocRow.tocHeader.address = iv_hdctRow->cmdGetScom.addr;
    iv_tocRow.tocHeader.dataLength = 0x40; // 64 bits -or- 2 words
    uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
    iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
    uint64_t dumpData; 
    fapi2::Target<TARGET_TYPE_ALL> dumpRowTgt(iv_tocRow.tgtHndl);
    fapiRc = getscom_abs_wrap(&dumpRowTgt, iv_tocRow.tocHeader.address, &dumpData);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        //TODO: Verify and modify all error rc to handle all primary/secondary
        //error in DUMP chipOp
        rc = SBE_SEC_INVALID_ADDRESS_PASSED;
    }
    else
    {
        SBE_INFO("getScom: address: 0x%08X, data HI: 0x%08X, data LO: 0x%08X ",
                  iv_tocRow.tocHeader.address, (dumpData >> 32),
                  static_cast<uint32_t>(dumpData & 0xFFFFFFFF));
    }
    iv_oStream.put(FIFO_DOUBLEWORD_LEN, (uint32_t*)&dumpData);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

void sbeCollectDump::getTargetList(std::vector<plat_target_handle_t> &o_targetList)
{
    #define SBE_FUNC "getTargetList"
    SBE_ENTER(SBE_FUNC);
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> procTgt =  plat_getChipTarget();
    switch(iv_tocRow.tocHeader.chipUnitType)
    {
        case CHIP_UNIT_TYPE_CHIP:
        case CHIP_UNIT_TYPE_UNKNOWN:
            {
                o_targetList.push_back(procTgt.get());
                SBE_DEBUG(SBE_FUNC "PROC: [0x%08X]", procTgt.get());
                break;
            }
        case CHIP_UNIT_TYPE_PERV:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "PERV: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_EQ:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "EQ: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_C:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "CORE id: [0x%08X]",target.get().getTargetInstance());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PHB:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PHB>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "PHB: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_MI:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_MI>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "MI: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_MC:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_MC>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "MC: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PAUC:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PAUC>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "PAUC: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_IOHS:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_IOHS>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "IOHS: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_NMMU:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_NMMU>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "NMMU: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PEC:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PEC>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "PEC: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PAU:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PAU>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC "PAU: [0x%08X]",target.get());
                }
            }
            break;
        //TODO: omic and mcc target types are not yet implemented.
        /*
        //TODO: Bellow targets are required or not should be checked.
        case CHIP_UNIT_TYPE_MCS:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCS>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_MCBIST:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCBIST>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_CAPP:
            return procTgt.getChildren<fapi2::TARGET_TYPE_CAPP>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_XBUS:
            return procTgt.getChildren<fapi2::TARGET_TYPE_XBUS>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_MCA:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCA>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_MBA:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MBA>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_DMI:
            return procTgt.getChildren<fapi2::TARGET_TYPE_DMI>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_MCC:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCC>(fapi2::TARGET_STATE_FUNCTIONAL);
        case CHIP_UNIT_TYPE_OMI:
            return procTgt.getChildren<fapi2::TARGET_TYPE_OMI>(fapi2::TARGET_STATE_FUNCTIONAL);
        */
        default:
        {
            SBE_INFO(SBE_FUNC "command Id [0x%02X] Type[0x%02X] not supported.",
                               (uint8_t) iv_tocRow.tocHeader.chipUnitType,
                               (uint8_t) iv_tocRow.tocHeader.cmdType);
            break;
        }
    }
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::writeDumpPacketRowToFifo()
{
    #define SBE_FUNC "writeDumpPacketRowToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // Get update row values by using HDCT bin data
    iv_tocRow.tocHeaderInit(iv_hdctRow);

    // TODO: Clean-Up once other chip-op ennabled.
    if( !( (iv_tocRow.tocHeader.cmdType == CMD_GETSCOM) ||
           (iv_tocRow.tocHeader.cmdType == CMD_PUTSCOM) ||
           (iv_tocRow.tocHeader.cmdType == CMD_GETSRAM) ) )
        return rc;

    // Map Dump target id with plat target list
    std::vector<plat_target_handle_t> targetList;
    getTargetList(targetList);

    //CPU cycles to complete the chip-op.
    iv_tocRow.cpuCycles = pk_timebase_get();

    for( auto &target : targetList )
    {
        // write dump row header contents using FIFO
        fapi2::Target<TARGET_TYPE_ALL> dumpRowTgtHnd(target);
        iv_tocRow.tgtHndl = target;
        iv_tocRow.tocHeader.chipUnitNum = dumpRowTgtHnd.getChipletNumber();

        switch(iv_tocRow.tocHeader.cmdType)
        {
            case CMD_GETSCOM:
            {
                rc = writeGetScomPacketToFifo();
                break;
            }
            case CMD_PUTSCOM:
            {
                rc = writePutScomPacketToFifo();
                break;
            }
            case CMD_GETSRAM:
            {
                rc = writeGetSramPacketToFifo();
                break;
            }
            default:
            {
                SBE_INFO(SBE_FUNC " command Id [0x%08X] is not supported.",
                                    (uint8_t)iv_tocRow.tocHeader.cmdType);
                rc = SBE_SEC_INVALID_CHIPLET_ID_PASSED;
                break;
            }
        } // End switch

        if(rc == SBE_SEC_OPERATION_SUCCESSFUL)
        {
            uint32_t ffdcDataLength = 0x00;
            // write FFDC data as a zero for success using FIFO
            iv_oStream.put(ffdcDataLength);
        }
        else
        {
            //TODO: Need to update FFDC and fail scenario handling here
            iv_tocRow.ffdcLen = 0x08;
            uint64_t ffdcData = 0x0000FFDC0000FFDC;
            // write FFDC data on failed case using FIFO
            iv_oStream.put(iv_tocRow.ffdcLen);
            iv_oStream.put(FIFO_DOUBLEWORD_LEN, (uint32_t*)&ffdcData);
            rc = SBE_SEC_OPERATION_SUCCESSFUL;
        }
        // FIFO the cpuCycles value
        iv_tocRow.cpuCycles = pk_timebase_get() - iv_tocRow.cpuCycles; // Delay time
        iv_oStream.put(FIFO_DOUBLEWORD_LEN, (uint32_t*)&iv_tocRow.cpuCycles);

    } // End For loop

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
            // TODO: Verify and modify all error rc to handle all
            // primary/secondary error in DUMP chipOp
            rc = writeDumpPacketRowToFifo();
            if(rc)
            {
                SBE_ERROR(SBE_FUNC" Dump collection failed");
                rc = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                break;
            }
        }
        //Dump chip-op Footer - DONE
        iv_oStream.put(DUMP_CHIP_OP_FOOTER);
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

    //Return status
    bool status = true;

    do
    {
        //Check if all HDCT entries are parsed
        if(!(iv_hdctXipSecDetails.currAddr < iv_hdctXipSecDetails.endAddr))
        {
            SBE_INFO("All HDCT entries parsed for Clock State: %d", iv_clockState);
            status = false;
            break;
        }

        //Parse single HDCT row and populate genericHdctRow_t struct
        iv_hdctRow = (genericHdctRow_t*)iv_hdctXipSecDetails.currAddr;

        //Increament the current address to point to the next HDCT row
        iv_hdctXipSecDetails.currAddr = iv_hdctXipSecDetails.currAddr + genericHdctRowSize_table[(uint8_t)(iv_hdctRow->genericHdr.command)];

        //Error Check
        if(genericHdctRowSize_table[(uint8_t)(iv_hdctRow->genericHdr.command)] == CMD_TYPE_NOT_USED)
        {
            SBE_ERROR("Unknown command type: %X",(uint8_t) iv_hdctRow->genericHdr.command);
            SBE_ERROR("Error in parsing HDCT.bin");
            status = false;
            break;
        }

        //Clock off state filter
        if (iv_clockState == SBE_DUMP_CLOCK_OFF && iv_clockOffEntryFlag == true && iv_hdctRow->genericHdr.command == CMD_STOPCLOCKS)
        {
            iv_clockOffEntryFlag = false;
        }

        //Clock On state filter
        if (iv_clockState == SBE_DUMP_CLOCK_ON && iv_hdctRow->genericHdr.command == CMD_STOPCLOCKS)
        {
            SBE_INFO("All HDCT entries parsed for Clock State: %d", iv_clockState);
            status = false;
            break;
        }

    }while(iv_clockOffEntryFlag ? true : (!(sbeCollectDump::dumpTypeCheck())));

    SBE_EXIT(SBE_FUNC);
    return status;
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
