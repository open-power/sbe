/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.C $                       */
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

#include "sbeCollectDump.H"
#include "fapi2.H"
#include "sbeglobals.H"
#include <hwp_data_stream.H>
#include <plat_hwp_data_stream.H>
#include <sbecmdsram.H>
#include <sbecmdmemaccess.H>
#include <sbecmdringaccess.H>
#include <sbecmdmpipl.H>
#include <sbecmdfastarray.H>
#include <sbecmdtracearray.H>
#include <p10_query_host_meminfo.H>
#include <p10_tracearray_defs.H>
#include "sbecmdringaccess.H"

#define FAST_ARRAY_CTRL_SET1_SIZE 0x48E18
#define FAST_ARRAY_CTRL_SET2_SIZE 0x2524

using namespace fapi2;

p10_query_host_meminfo_FP_t p10_query_host_meminfo_hwp = &p10_query_host_meminfo;

void getControlTraceArrayTargetType( const uint8_t i_chipUnitType,
                                    const uint8_t i_chipUnitNum,
                                    uint16_t & o_targetType,
                                    uint8_t  & o_chipletId )
{
    o_targetType = 0x00;
    o_chipletId  = i_chipUnitNum;
    switch(i_chipUnitType)
    {
        case CHIP_UNIT_TYPE_CHIP:
        case CHIP_UNIT_TYPE_UNKNOWN:
                {
                    o_targetType = TARGET_PROC_CHIP;
                    break;
                }
        case CHIP_UNIT_TYPE_C:
                {
                    o_targetType = TARGET_CORE;
                    break;
                }
        case CHIP_UNIT_TYPE_EQ:
                {
                    o_targetType = TARGET_EQ;
                    o_chipletId  = EQ_CHIPLET_OFFSET + i_chipUnitNum;
                    break;
                }
        case CHIP_UNIT_TYPE_PERV:
                {
                    o_targetType = TARGET_PERV;
                    break;
                }
        case CHIP_UNIT_TYPE_IOHS:
                {
                    o_targetType = TARGET_PERV;
                    o_chipletId  = IOHS_CHIPLET_OFFSET + i_chipUnitNum;
                    break;
                }
        case CHIP_UNIT_TYPE_PEC:
                {
                    o_targetType = TARGET_PERV;
                    o_chipletId  = PEC_CHIPLET_OFFSET + i_chipUnitNum;
                    break;
                }
        case CHIP_UNIT_TYPE_PAUC:
                {
                    o_targetType = TARGET_PERV;
                    o_chipletId  = PAUC_CHIPLET_OFFSET + i_chipUnitNum;
                    break;
                }
        case CHIP_UNIT_TYPE_MC:
                {
                    o_targetType = TARGET_PERV;
                    o_chipletId  = MC_CHIPLET_OFFSET + i_chipUnitNum;
                    break;
                }
        default:
                {
                    SBE_ERROR("chipUnitType [0x%02X] and Id[0x%02X] are not "
                          "supported", (uint8_t) i_chipUnitType, i_chipUnitNum);
                    break;
                }
    }
    return;
}

uint32_t collectMpiplHwDump( uint64_t i_hbMemAddr,
                             uint32_t &o_length )
{
    #define SBE_FUNC "collectMpiplHwDump"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint64_t hbMemAddr = i_hbMemAddr;
    do
    {
        o_length = 0;
        if( hbMemAddr == 0x00 )
        {
            SBE_ERROR(SBE_FUNC " Unsupported/Invalid HbMem Address[0x%08X %08X]",
                                 SBE::higher32BWord(hbMemAddr),
                                 SBE::lower32BWord(hbMemAddr));
            rc = SBE_SEC_MPIPL_DUMP_INVALID_PARAMS;
            break;
        }
        SBE_INFO(SBE_FUNC "HbMem Address[0x%08X %08X]",
                           SBE::higher32BWord(hbMemAddr),
                           SBE::lower32BWord(hbMemAddr));
        // Create the sbeCollectDump object for ClockOn
        sbeCollectDump dumpClockOnObj( SBE_DUMP_TYPE_MPIPL, SBE_DUMP_CLOCK_ON,
                                SBE_FIFO, hbMemAddr);
        //Call collectAllEntries to write dump into HbMem
        rc = dumpClockOnObj.collectAllHDCTEntries();
        if(rc)
        {
            // TODO: Verify and modify all error rc to handle all
            // primary/secondary error in DUMP chipOp
            SBE_ERROR(SBE_FUNC" Dump collection failed for Clock State:[0x%02],"
                     "HbMem Address[0x%08X %08X]", SBE_DUMP_CLOCK_ON,
                      SBE::higher32BWord(hbMemAddr),SBE::lower32BWord(hbMemAddr));
            rc = SBE_SEC_GET_DUMP_FAILED;
            break;
        }
        // Get length of dump data to rerun back to caller
        uint32_t clockOnLength = dumpClockOnObj.collectLenInBytesOfWriteData();
        SBE_INFO(SBE_FUNC "ClockOn Total Bytes HW dump written [0x%08X]", clockOnLength);

        o_length = clockOnLength;
//TODO: Enable code bellow once clock off entries from HDCT enabled
/*
        // Aline 128 bytes lenfth and update hbMemAddr
        hbMemAddr = hbMemAddr + ( ((clockOnLength / 0x80) * (0x80) ) +
                                  ((clockOnLength % 0x80 )?(0x80):(0)) );

        // Create the sbeCollectDump object for ClockOff
        sbeCollectDump dumpClockOffObj( SBE_DUMP_TYPE_MPIPL, SBE_DUMP_CLOCK_OFF,
                                SBE_FIFO, hbMemAddr);
        //Call collectAllEntries to write dump into HbMem
        rc = dumpClockOffObj.collectAllHDCTEntries();
        if(rc)
        {
            // TODO: Verify and modify all error rc to handle all
            // primary/secondary error in DUMP chipOp
            SBE_ERROR(SBE_FUNC" Dump collection failed for Clock State:[0x%02],"
                     "HbMem Address[0x%08X %08X]", SBE_DUMP_CLOCK_ON,
                      SBE::higher32BWord(hbMemAddr),SBE::lower32BWord(hbMemAddr));
            rc = SBE_SEC_GET_DUMP_FAILED;
            break;
        }
        // Get length of dump data to rerun back to caller
        uint32_t clockOffLength = dumpClockOffObj.collectLenInBytesOfWriteData();
        SBE_INFO(SBE_FUNC "ClockOff Total Bytes HW dump written [0x%08X]", clockOffLength);
        o_length = o_length + clockOffLength;
*/
    }
    while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

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

uint32_t sbeCollectDump::writeGetFastArrayPacketToFifo()
{
    #define SBE_FUNC "writeGetFastArrayPacketToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        // Set FFDC failed command information and
        // Sequence Id is 0 by default for Fifo interface
        iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_ARRAY_ACCESS, SBE_CMD_CONTROL_FAST_ARRAY);
        // Add HWP specific ffdc data length
        iv_chipOpffdc.lenInWords = 0;
        iv_oStream.setFifoRc(FAPI2_RC_SUCCESS);
        iv_oStream.setPriSecRc(rc);

        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = iv_hdctRow->cmdFastArray.strEqvHash32;
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        if(!iv_tocRow.tgtHndl.getFunctional())
        {
            // Update non functional state DUMP header
            iv_tocRow.tocHeader.preReq = false;
            iv_tocRow.tocHeader.dataLength = 0x00;
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
            SBE_INFO("DUMP GETFASTARRAY: NonFunctional Target UnitNum[0x%08X]",
                      (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
            break;
        }
        // Create the req struct for the sbeFastArray Chip-op
        sbeControlFastArrayCMD_t dumpFastArrayReq = {0};
        //TODO: As there is no way as of now to get the fast array length we
        //will hard code and stream out the data length. If the actual length value
        //deviates from the hardcoded value dump parser will fail.
        if(iv_hdctRow->cmdFastArray.controlSet == 0x01)
            iv_tocRow.tocHeader.dataLength = (FAST_ARRAY_CTRL_SET1_SIZE * 8);
        else
            iv_tocRow.tocHeader.dataLength = (FAST_ARRAY_CTRL_SET2_SIZE * 8);

        uint32_t dummyDataLengthInBits =
            64 * (((uint32_t)(iv_tocRow.tocHeader.dataLength / 64)) + ((uint32_t)(iv_tocRow.tocHeader.dataLength % 64) ? 1:0 ));

        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

        len = sizeof(dumpFastArrayReq)/sizeof(uint32_t);
        //FIXME:We have to fetch the target type from HDCT.bin.
        dumpFastArrayReq.hdr.targetType  = TARGET_CORE;

        dumpFastArrayReq.hdr.chipletId   = iv_tocRow.tocHeader.chipUnitNum;
        dumpFastArrayReq.hdr.control_set = iv_hdctRow->cmdFastArray.controlSet;
        dumpFastArrayReq.hdr.custom_data_length = 0x00;

        uint32_t startCount = iv_oStream.words_written();
        uint32_t totalCountInBytes = dummyDataLengthInBits / 8;
        uint32_t totalCount = totalCountInBytes / (sizeof(uint32_t));
        uint32_t dummyData = 0x00;

        sbefifo_hwp_data_istream istream( iv_fifoType, len,
                (uint32_t*)&dumpFastArrayReq, false );
        rc = sbeControlFastArrayWrap( istream, iv_oStream );
        uint32_t endCount = iv_oStream.words_written();

        //If endCount = startCount means chip-op failed. We will write dummy data
        //All data streamed out need's to be 8 byte aligned.
        //TODO:RC check needs to be done properly.
        if(endCount == startCount || ((endCount - startCount) != totalCount))
        {
            totalCount = totalCount - (endCount - startCount);
            while(totalCount !=0)
            {
                iv_oStream.put(dummyData);
                totalCount = totalCount - 1;
            }
        }

        SBE_INFO("Dump FastArray: control_set[0x%08X], chipUnitNum [0x%08X]",
                iv_hdctRow->cmdFastArray.controlSet, iv_tocRow.tocHeader.chipUnitNum);
    }
    while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::stopClocksOff()
{
    #define SBE_FUNC " stopClocksOff "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        // Set FFDC failed command information and
        // Sequence Id is 0 by default for Fifo interface
        iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_MPIPL_COMMANDS, SBE_CMD_MPIPL_STOPCLOCKS);
        // Add HWP specific ffdc data length
        iv_chipOpffdc.lenInWords = 0;
        iv_oStream.setFifoRc(FAPI2_RC_SUCCESS);
        iv_oStream.setPriSecRc(rc);

        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = iv_hdctRow->cmdStopClocks.strEqvHash32;
        iv_tocRow.tocHeader.dataLength = 0x00;
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

        // Create the req struct for the sbeStopClocks Chip-op
        sbeStopClocksReqMsgHdr_t dumpStopClockReq = {0};
        len = sizeof(dumpStopClockReq)/sizeof(uint32_t);
        dumpStopClockReq.reserved = 0x00;
        dumpStopClockReq.targetType = iv_hdctRow->cmdStopClocks.tgtType;
        if(iv_hdctRow->cmdStopClocks.chipletStart == 0xff && iv_hdctRow->cmdStopClocks.chipletEnd == 0xff)
        {
            //All chiplets
            dumpStopClockReq.chipletId = 0xff;
            sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                          (uint32_t*)&dumpStopClockReq, false );
            rc = sbeStopClocks_Wrap( istream, iv_oStream );
        }
        else
        {
            //TODO:Need to provide support for stop clock on all cache chiplets
            for(uint32_t chiplet=iv_hdctRow->cmdStopClocks.chipletStart; chiplet <= iv_hdctRow->cmdStopClocks.chipletEnd; chiplet++)
            {
                dumpStopClockReq.chipletId = chiplet;
                sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                          (uint32_t*)&dumpStopClockReq, false );
                rc = sbeStopClocks_Wrap( istream, iv_oStream );
            }
        }

        SBE_INFO("dumpStopClocks: dumpStopClocks clockTypeTgt[0x%04X],chipUnitNum[0x%08X],chipletStart[0x%02x], chipletEnd{0x%02x}",dumpStopClockReq.targetType, iv_tocRow.tocHeader.chipUnitNum,iv_hdctRow->cmdStopClocks.chipletStart,iv_hdctRow->cmdStopClocks.chipletEnd);
    }
    while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::writeGetTracearrayPacketToFifo()
{
    #define SBE_FUNC " writeGetTracearrayPacketToFifo "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // Set FFDC failed command information and
    // Sequence Id is 0 by default for Fifo interface
    iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_MEMORY_ACCESS, SBE_CMD_GETMEM);
    //Add HWP specific ffdc data length
    iv_chipOpffdc.lenInWords = 0;
    iv_oStream.setFifoRc(FAPI2_RC_SUCCESS);
    iv_oStream.setPriSecRc(rc);

    // Update address, length and stream header data vai FIFO
    iv_tocRow.tocHeader.address = iv_hdctRow->cmdTraceArray.strEqvHash32;
    uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
    if(!iv_tocRow.tgtHndl.getFunctional())
    {
        // Update non functional state DUMP header
        iv_tocRow.tocHeader.preReq = false;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_INFO("DUMP GETTRACEARRAY: NonFunctional Target UnitNum[0x%08X]",
                  (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }
    // The size of data streamed from SBE is irespective of trace ID
    // and it is 128*16*8 bits. [ PROC_TRACEARRAY_MAX_SIZE ] 
    iv_tocRow.tocHeader.dataLength = PROC_TRACEARRAY_MAX_SIZE * 8;
    iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

    sbeControlTraceArrayCMD_t reqMsg = {};
    len  = sizeof(sbeControlTraceArrayCMD_t)/sizeof(uint32_t);

    uint16_t o_targetType = 0;
    uint8_t  o_chipletId  = 0;
    getControlTraceArrayTargetType((uint8_t)iv_tocRow.tocHeader.chipUnitType,
                                  (uint8_t)iv_tocRow.tocHeader.chipUnitNum,
                                   o_targetType, o_chipletId );
    reqMsg.targetType   = o_targetType;
    reqMsg.chipletId    = o_chipletId;
    reqMsg.traceArrayId = iv_hdctRow->cmdTraceArray.traceArrayID;
    reqMsg.operation    = SBE_TA_STOP;         // 0x0004: Stop Trace array

    SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                     " traceArrayId [0x%04X] operation [0x%04X]",
                       reqMsg.targetType, reqMsg.chipletId,
                       reqMsg.traceArrayId, reqMsg.operation);

    sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                      (uint32_t*)&reqMsg, false );
    rc = sbeControlTraceArrayWrap( istream, iv_oStream );
    if(rc)
    {
        return rc;
    }

    reqMsg.operation    = SBE_TA_COLLECT_DUMP; // 0x0008: Collect Trace Dump

    SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                     " traceArrayId [0x%04X] operation [0x%04X]",
                       reqMsg.targetType, reqMsg.chipletId,
                       reqMsg.traceArrayId, reqMsg.operation);

    sbefifo_hwp_data_istream istreamDump( iv_fifoType, len,
                                      (uint32_t*)&reqMsg, false );
    uint32_t startCount = iv_oStream.words_written();
    rc = sbeControlTraceArrayWrap( istreamDump, iv_oStream );

    uint32_t endCount = iv_oStream.words_written();
    uint32_t totalCountInBytes = iv_tocRow.tocHeader.dataLength/8;
    uint32_t totalCount = totalCountInBytes / (sizeof(uint32_t));
    uint32_t dummyData = 0x00;

    //If endCount = startCount means chip-op failed. We will write dummy data.
    //TODO:Rc is not handled properly. Will hardcode rc=fail based on endCount
    //and startCount.
    if(endCount == startCount || ((endCount - startCount) != totalCount))
    {
        totalCount = totalCount - (endCount - startCount);
        while(totalCount !=0)
        {
            iv_oStream.put(dummyData);
            totalCount = totalCount - 1;
        }
    }
    reqMsg.operation    = SBE_TA_RESTART; // 0x0002: Restart Trace array

    SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                     " traceArrayId [0x%04X] operation [0x%04X]",
                       reqMsg.targetType, reqMsg.chipletId,
                       reqMsg.traceArrayId, reqMsg.operation);

    sbefifo_hwp_data_istream istreamStop( iv_fifoType, len,
                                      (uint32_t*)&reqMsg, false );

    rc = sbeControlTraceArrayWrap( istreamStop, iv_oStream );
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::writeGetRingPacketToFifo()
{
    #define SBE_FUNC " writeGetRingPacketToFifo "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    // Set FFDC failed command information and
    // Sequence Id is 0 by default for Fifo interface
    iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_RING_ACCESS, SBE_CMD_GETRING);
    // Add HWP specific ffdc data length
    iv_chipOpffdc.lenInWords = 0;
    iv_oStream.setFifoRc(FAPI2_RC_SUCCESS);
    iv_oStream.setPriSecRc(rc);

    // Update address, length and stream header data vai FIFO
    iv_tocRow.tocHeader.address = iv_hdctRow->cmdGetRing.strEqvHash32;
    uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
    if(!iv_tocRow.tgtHndl.getFunctional())
    {
        // Update non functional state in DUMP header
        iv_tocRow.tocHeader.preReq = false;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_INFO("DUMP GETRING: NonFunctional Target UnitNum[0x%08X]",
                  (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }
    uint32_t bitlength = iv_hdctRow->cmdGetRing.ringLen;
    //Stream out the actual ring length.
    iv_tocRow.tocHeader.dataLength = bitlength;
    //Dummy data length to be streamed out in case of FFDC.Keep it 8byte
    //alligned based on ring length as ring chip-op streams out data 8byte
    //aligned.
    uint32_t dummyDataLengthInBits =
                64 * (((uint32_t)(bitlength / 64)) + ((uint32_t)(bitlength % 64) ? 1:0 ));

    iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

    sbeGetRingAccessMsgHdr_t l_reqMsg;
    len  = sizeof(sbeGetRingAccessMsgHdr_t)/sizeof(uint32_t);

    l_reqMsg.ringAddr = iv_hdctRow->cmdGetRing.ringAddr;
    l_reqMsg.ringMode = 0x0001;
    l_reqMsg.ringLenInBits = bitlength;

    SBE_INFO(SBE_FUNC "Ring Address 0x%08X User Ring Mode 0x%04X "
             "Length in Bits 0x%08X Length in Bits(8 Byte aligned) 0x%08X", l_reqMsg.ringAddr,
              l_reqMsg.ringMode, l_reqMsg.ringLenInBits, iv_tocRow.tocHeader.dataLength);

    // Verify ring data length in FIFO as per length size
    uint32_t startCount = iv_oStream.words_written();
    uint32_t totalCountInBytes = dummyDataLengthInBits / 8;
    uint32_t totalCount = totalCountInBytes / (sizeof(uint32_t));
    uint32_t dummyData = 0x00;

    sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                      (uint32_t*)&l_reqMsg, false );
    rc = sbeGetRingWrap( istream, iv_oStream );
    uint32_t endCount = iv_oStream.words_written();

    //If endCount = startCount means chip-op failed. We will write dummy data.
    //TODO:Rc is not handled properly. Will hardcode rc=fail based on endCount
    //and startCount.
    if(endCount == startCount || ((endCount - startCount) != totalCount))
    {
        totalCount = totalCount - (endCount - startCount);
        rc = SBE_SEC_PCB_PIB_ERR;
        while(totalCount !=0)
        {
            iv_oStream.put(dummyData);
            totalCount = totalCount - 1;
        }
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::writeGetMemPBAPacketToFifo()
{
    #define SBE_FUNC "writeGetMemPBAPacketToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    do
    {
        // Set FFDC failed command information and
        // Sequence Id is 0 by default for Fifo interface
        iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_MEMORY_ACCESS, SBE_CMD_GETMEM);
        // Add HWP specific ffdc data length
        iv_chipOpffdc.lenInWords = 0;
        iv_oStream.setFifoRc(fapiRc);
        iv_oStream.setPriSecRc(rc);

        fapi2::Target<TARGET_TYPE_ALL> dumpRowTgt(iv_tocRow.tgtHndl);
        uint8_t l_coreId = 0;
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,plat_getChipTarget(),l_coreId);
        fapi2::Target<fapi2::TARGET_TYPE_CORE> core =
              plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(l_coreId);

        // size of host memory in MB
        uint32_t sizeHostMem = 0x0;
        uint64_t hrmor = 0x00;
        uint32_t mode  = 0x00;
        fapi2::Target < fapi2::TARGET_TYPE_PROC_CHIP > procTgt;
        SBE_EXEC_HWP(fapiRc, p10_query_host_meminfo_hwp, core,
                     procTgt, sizeHostMem, hrmor, mode);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            iv_oStream.setFifoRc(fapiRc);
            uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
            // Handle HRMOR is 0
            if( hrmor == 0x00 )
            {
                SBE_ERROR(SBE_FUNC " Core Scratch 1 says HRMOR is 0 ");
                iv_tocRow.tocHeader.address = hrmor;
                iv_tocRow.tocHeader.dataLength = 0;
                iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
                rc = SBE_SEC_INVALID_ADDRESS_PASSED;
                break;
            }
            // Handle NO_MEM case: No HB memory has been initialized so we can
            // only dump our static code load up to 1024 Bits.
            SBE_ERROR(SBE_FUNC " Core Scratch 1 says HRMOR/HB SIZE is 0 ");
            // Handle default FFDC as per DUMP pibmem lenght
            iv_tocRow.tocHeader.address = hrmor;
            // Length of data in bits (128 byte aligned.)
            iv_tocRow.tocHeader.dataLength = 1024;
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

            // Update Getmempba(FFDC) with static code load up to 1024 Bits.
            sbeMemAccessReqMsgHdr_t dumpPbaReq = {0};
            dumpPbaReq.coreId = 0;
            dumpPbaReq.eccByte = 0;
            dumpPbaReq.flags = 0x22;
            dumpPbaReq.addrHi = SBE::higher32BWord(hrmor);
            dumpPbaReq.addrLo = SBE::lower32BWord(hrmor);
            dumpPbaReq.len = 128; // 1024 bits == 128 Bytes

            len  = sizeof(sbeMemAccessReqMsgHdr_t)/sizeof(uint32_t);
            sbefifo_hwp_data_istream istream(iv_fifoType, len,
                                            (uint32_t*)&dumpPbaReq, false);
            rc = sbeMemAccess_Wrap( istream, iv_oStream, true );
            break;
        }
        SBE_INFO("sizeHostMem:[0x%08X], hrmor:[0x%08X%08X]", sizeHostMem,
                  SBE::higher32BWord(hrmor), SBE::lower32BWord(hrmor));

        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = hrmor;
        iv_tocRow.tocHeader.dataLength = P10_QUERY_HOST_MEMINFO_MB * sizeHostMem * 8;
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

        // sizeHostMem = Number of MBs of memory to dump (hex format)
        // Dump ChipOp need length of data in bits
        for (uint32_t i = 0; i < sizeHostMem; i++)
        {
            uint64_t hrmorAddr = hrmor + (i * P10_QUERY_HOST_MEMINFO_MB);
            // Default Master Core Target
            sbeMemAccessReqMsgHdr_t dumpPbaReq = {0};
            dumpPbaReq.coreId = 0;
            dumpPbaReq.eccByte = 0;
            dumpPbaReq.flags = 0x22;
            dumpPbaReq.addrHi = SBE::higher32BWord(hrmorAddr);
            dumpPbaReq.addrLo = SBE::lower32BWord(hrmorAddr);
            dumpPbaReq.len = P10_QUERY_HOST_MEMINFO_MB;

            SBE_INFO("GetMemPBA:hostboot memory[0x%08X%08X] and Length[0x%08X], MBsize[0x%08X]",
                      dumpPbaReq.addrHi, dumpPbaReq.addrLo, dumpPbaReq.len, i);

            len  = sizeof(sbeMemAccessReqMsgHdr_t)/sizeof(uint32_t);
            sbefifo_hwp_data_istream istream(iv_fifoType, len,
                                            (uint32_t*)&dumpPbaReq, false);
            uint32_t startCount = iv_oStream.words_written();
            rc = sbeMemAccess_Wrap( istream, iv_oStream, true );
            uint32_t endCount = iv_oStream.words_written();
            uint32_t totalCount = P10_QUERY_HOST_MEMINFO_MB / (sizeof(uint32_t));
            uint32_t dummyData = 0x00;
            if(endCount == startCount || ((endCount - startCount) != totalCount))
            {
                totalCount = totalCount - (endCount - startCount);
                while(totalCount !=0)
                {
                    iv_oStream.put(dummyData);
                    totalCount = totalCount - 1;
                }
            }
            if( (rc != SBE_SEC_OPERATION_SUCCESSFUL) ||
                (iv_oStream.getFifoRc() != FAPI2_RC_SUCCESS) ||
                (iv_oStream.getPriSecRc() != SBE_SEC_OPERATION_SUCCESSFUL) )
            {
                uint32_t numDataWords = P10_QUERY_HOST_MEMINFO_MB / (sizeof(uint32_t));
                // write dummy data for full length as per sizeHostMem
                for (uint32_t errData = i+1; errData < sizeHostMem; errData++)
                {
                    for(uint32_t errDataWord = 0; errDataWord < numDataWords; errDataWord++)
                    {
                        iv_oStream.put(dummyData);
                    }
                }
                // TODO: Verify and modify all error rc to handle all
                // primary/secondary error in DUMP
                rc = SBE_SEC_INVALID_ADDRESS_PASSED;
                break;
            }
        }
    }
    while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
uint32_t sbeCollectDump::writeGetSramPacketToFifo()
{
    #define SBE_FUNC "writeGetSramPacketToFifo"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        // Set FFDC failed command information and
        // Sequence Id is 0 by default for Fifo interface
        iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_MEMORY_ACCESS, SBE_CMD_GETSRAM);
        // Add HWP specific ffdc data length
        iv_chipOpffdc.lenInWords = 0;
        iv_oStream.setFifoRc(FAPI2_RC_SUCCESS);
        iv_oStream.setPriSecRc(rc);

        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = iv_hdctRow->cmdGetSram.addr;
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        if(!iv_tocRow.tgtHndl.getFunctional())
        {
            // Update non functional state in DUMP header
            iv_tocRow.tocHeader.preReq = false;
            iv_tocRow.tocHeader.dataLength = 0x00;
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
            SBE_INFO("DUMP GETSRAM: NonFunctional Target UnitNum[0x%08X]",
                      (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
            break;
        }
        iv_tocRow.tocHeader.dataLength = 0x2000; // Length in bits (1024 bytes)
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
        // Set FFDC failed command information and
        // Sequence Id is 0 by default for Fifo interface
        iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_SCOM_ACCESS, SBE_CMD_PUTSCOM);
        // Add HWP specific ffdc data length
        iv_chipOpffdc.lenInWords = 0;
        iv_oStream.setFifoRc(fapiRc);
        iv_oStream.setPriSecRc(rc);

        // Update address, length and stream header data vai FIFO
        iv_tocRow.tocHeader.address = iv_hdctRow->cmdPutScom.addr;
        iv_tocRow.tocHeader.dataLength = 0x00;
        uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        if(!iv_tocRow.tgtHndl.getFunctional())
        {
            iv_tocRow.tocHeader.preReq = false; 
            SBE_INFO("DUMP PUTSCOM: NonFunctional Target UnitNum[0x%08X]",
                     (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
            break;
        }

        uint32_t addr = iv_tocRow.tocHeader.address;
        uint32_t maskType = iv_hdctRow->cmdPutScom.extGenericHdr.bitModifier;
        fapi2::Target<TARGET_TYPE_ALL> dumpRowTgt(iv_tocRow.tgtHndl);
        seeprom_hwp_data_istream stream((uint32_t*)&iv_hdctRow->cmdPutScom.value,
                                         sizeof(uint64_t));
        uint32_t msbValue, lsbValue;
        stream.get(msbValue), stream.get(lsbValue);
        uint64_t mask = (((uint64_t)msbValue << 32 ) | ((uint64_t)lsbValue));
        dumpData = mask; // maskType is nnone then putScom data is mask value.

        SBE_INFO("putscom address:[0x%08X], maskType:[0x%08X], mask:[0x%08X%08X]",
                  addr, maskType, SBE::higher32BWord(mask), SBE::lower32BWord(mask));

        if( B_NONE != maskType )
        {
            uint64_t readData = 0;
            fapiRc = getscom_abs_wrap(&dumpRowTgt, addr, &readData);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                iv_oStream.setFifoRc(fapiRc);
                rc = SBE_SEC_INVALID_ADDRESS_PASSED;
                break;
            }
            SBE_INFO(SBE_FUNC " putscom scom value: 0x%.8X%.8X ",
                     SBE::higher32BWord(readData),SBE::lower32BWord(readData));
            if( B_OR == maskType )
            {
                dumpData = (readData | mask);
            }
            if( B_AND == maskType )
            {
                dumpData = (readData & mask);
            }
        }
        SBE_INFO(SBE_FUNC " maskType[0x%02X], data [0x%08X %08X] ", maskType,
                      SBE::higher32BWord(dumpData),SBE::lower32BWord(dumpData));
        fapiRc = putscom_abs_wrap(&dumpRowTgt, addr, dumpData);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            iv_oStream.setFifoRc(fapiRc);
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

    // Set FFDC failed command information and
    // Sequence Id is 0 by default for Fifo interface
    iv_chipOpffdc.setCmdInfo(0, SBE_CMD_CLASS_SCOM_ACCESS, SBE_CMD_GETSCOM );
    // Add HWP specific ffdc data length
    iv_chipOpffdc.lenInWords = 0;
    iv_chipOpffdc.setRc(FAPI2_RC_SUCCESS);
    iv_oStream.setPriSecRc(SBE_PRI_OPERATION_SUCCESSFUL);

    // Update address, length and stream header data vai FIFO
    iv_tocRow.tocHeader.address = iv_hdctRow->cmdGetScom.addr;
    uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
    if(!iv_tocRow.tgtHndl.getFunctional())
    {
        // Update non functional state DUMP header
        iv_tocRow.tocHeader.preReq = false;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_INFO("DUMP GETSCOM: NonFunctional Target UnitNum[0x%08X]",
                  (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }
    iv_tocRow.tocHeader.dataLength = 0x40; // 64 bits -or- 2 words
    iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
    uint64_t dumpData;
    fapi2::Target<TARGET_TYPE_ALL> dumpRowTgt(iv_tocRow.tgtHndl);
    fapiRc = getscom_abs_wrap(&dumpRowTgt, iv_tocRow.tocHeader.address, &dumpData);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        //TODO: Verify and modify all error rc to handle all primary/secondary
        //error in DUMP chipOp
        iv_oStream.setFifoRc(fapiRc);
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
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " PERV: [0x%08X]", target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_EQ:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " EQ: [0x%08X] ",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_C:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " CORE:[0x%08X]", target.get().getTargetInstance());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PHB:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PHB>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " PHB: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_MI:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_MI>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " MI: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_MC:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_MC>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " MC: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PAUC:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PAUC>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " PAUC: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_IOHS:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_IOHS>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " IOHS: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_NMMU:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_NMMU>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " NMMU: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PEC:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PEC>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " PEC: [0x%08X]",target.get());
                }
            }
            break;
        case CHIP_UNIT_TYPE_PAU:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PAU>(fapi2::TARGET_STATE_PRESENT))
            {
                if(isChipUnitNumAllowed(target))
                {
                    o_targetList.push_back(target);
                    SBE_DEBUG(SBE_FUNC " PAU: [0x%08X]",target.get());
                }
            }
            break;
        //TODO: omic and mcc target types are not yet implemented.
        /*
        //TODO: Bellow targets are required or not should be checked.
        case CHIP_UNIT_TYPE_MCS:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCS>();
        case CHIP_UNIT_TYPE_MCBIST:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCBIST>();
        case CHIP_UNIT_TYPE_CAPP:
            return procTgt.getChildren<fapi2::TARGET_TYPE_CAPP>();
        case CHIP_UNIT_TYPE_XBUS:
            return procTgt.getChildren<fapi2::TARGET_TYPE_XBUS>();
        case CHIP_UNIT_TYPE_MCA:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCA>();
        case CHIP_UNIT_TYPE_MBA:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MBA>();
        case CHIP_UNIT_TYPE_DMI:
            return procTgt.getChildren<fapi2::TARGET_TYPE_DMI>();
        case CHIP_UNIT_TYPE_MCC:
            return procTgt.getChildren<fapi2::TARGET_TYPE_MCC>();
        case CHIP_UNIT_TYPE_OMI:
            return procTgt.getChildren<fapi2::TARGET_TYPE_OMI>();
        */
        default:
        {
            SBE_ERROR(SBE_FUNC "command Id [0x%02X] Type[0x%02X] not supported",
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
    if( !( (iv_tocRow.tocHeader.cmdType == CMD_GETSCOM)    ||
           (iv_tocRow.tocHeader.cmdType == CMD_PUTSCOM)    ||
           (iv_tocRow.tocHeader.cmdType == CMD_GETMEMPBA)  ||
           (iv_tocRow.tocHeader.cmdType == CMD_GETSRAM)    ||
           (iv_tocRow.tocHeader.cmdType == CMD_STOPCLOCKS) ||
           (iv_tocRow.tocHeader.cmdType == CMD_GETFASTARRAY ) ||
           (iv_tocRow.tocHeader.cmdType == CMD_GETTRACEARRAY) ||
           (iv_tocRow.tocHeader.cmdType == CMD_GETRING) ) )
        return rc;

    // Map Dump target id with plat target list
    std::vector<plat_target_handle_t> targetList;
    getTargetList(targetList);

    for( auto &target : targetList )
    {
        //CPU cycles to complete the chip-op.
        iv_tocRow.cpuCycles = pk_timebase_get();

        // write dump row header contents using FIFO
        fapi2::Target<TARGET_TYPE_ALL> dumpRowTgtHnd(target);
        iv_tocRow.tgtHndl = target;
        iv_tocRow.tocHeader.preReq = true;
        iv_tocRow.tocHeader.chipUnitNum = dumpRowTgtHnd.get().getTargetInstance();
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
            case CMD_GETFASTARRAY:
            {
                rc = writeGetFastArrayPacketToFifo();
                break;
            }
            case CMD_GETRING:
            {
                rc = writeGetRingPacketToFifo();
                break;
            }
            case CMD_GETSRAM:
            {
                rc = writeGetSramPacketToFifo();
                break;
            }
            case CMD_STOPCLOCKS:
            {
                rc = stopClocksOff();
                break;
            }
            case CMD_GETMEMPBA:
            {
                // HbDump only for Master Proc based on the
                // attribute ATTR_PROC_SBE_MASTER_CHIP value
                Target<TARGET_TYPE_PROC_CHIP> procTarget = plat_getChipTarget();
                fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type isMaster = false;
                FAPI_ATTR_GET(ATTR_PROC_SBE_MASTER_CHIP,procTarget,isMaster);
                if( isMaster )
                {
                    rc = writeGetMemPBAPacketToFifo();
                    break;
                }
                continue;
            }
            case CMD_GETTRACEARRAY:
            {
                rc = writeGetTracearrayPacketToFifo();
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
            iv_chipOpffdc.setRc(iv_oStream.getFifoRc());
            // Update FFDC lenth + PrimarySecondary(32 bits) RC lenth
            iv_tocRow.ffdcLen = sizeof(sbeResponseFfdc_t) + sizeof(uint32_t);
            // write FFDC data on failed case using FIFO
            iv_oStream.put(iv_tocRow.ffdcLen);
            iv_oStream.put(iv_oStream.getPriSecRc()); // Set Primary Secondary RC
            iv_oStream.put(sizeof(sbeResponseFfdc_t)/sizeof(uint32_t), (uint32_t*)&iv_chipOpffdc);
            iv_oStream.setPriSecRc(SBE_PRI_OPERATION_SUCCESSFUL);
            iv_chipOpffdc.setRc(FAPI2_RC_SUCCESS);
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
        // setPBALastAccess will set a flag to clear all bytes to
        // write data into FIFO via PBA interface before last PBA
        // update to DONE fifo data.
        iv_oStream.setPBALastAccess();
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
