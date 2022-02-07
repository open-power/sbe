/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeCollectDump.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2022                        */
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
#include "target_filters.H"
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
#include "sbecmdiplcontrol.H"
#include "p10_query_corecachemma_access_state.H"
#include "sbecmdcntlinst.H"
#include "plat_target_parms.H"
#include "sbecmdregaccess.H"

// FastArray Data Size for DD1
#define FAST_ARRAY_CTRL_SET1_DD1_SIZE 0x48E18
#define FAST_ARRAY_CTRL_SET2_DD1_SIZE 0x2524
// FastArray Data Size for DD2
#define FAST_ARRAY_CTRL_SET1_DD2_SIZE 0x474A8
#define FAST_ARRAY_CTRL_SET2_DD2_SIZE 0x2514

using namespace fapi2;

p10_query_host_meminfo_FP_t p10_query_host_meminfo_hwp = &p10_query_host_meminfo;

void getAbsoluteAddressForRing( const fapi2::plat_target_handle_t tgtHndl,
                                const uint32_t relativeAddr,
                                uint32_t &absoluteAddr)
{
    #define SBE_FUNC " getAbsoluteAddressForRing "
    SBE_ENTER(SBEM_FUNC);
    do
    {

        switch(tgtHndl.getTargetType())
        {
            case PPE_TARGET_TYPE_CORE:
                 {
                     uint32_t chipletNum = tgtHndl.fields.chiplet_num;
                     chipletNum = chipletNum << 24;
                     uint32_t coreSelect = (tgtHndl.fields.type_target_num % 4);
                     coreSelect = 1 << (3 - coreSelect);
                     // Ringsname     Ring Addr

                     // ec_cl2_mode  0x20032001
                     // ec_cl2_regf  0x20032003
                     // ec_cl2_abst  0x20032005
                     // ec_cl2_func  0x20032000
                     // ec_cl2_gptr  0x20032002
                     // ec_cl2_repr  0x20032006
                     // ec_cl2_far   0x20032009
                     // ec_cl2_inex  0x2003200B

                     // ec_mma_mode  0x20830001
                     // ec_mma_abst  0x20830005
                     // ec_mma_func  0x20830000
                     // ec_mma_gptr  0x20830002
                     // ec_mma_far   0x20830009

                     // ec_l3_func   0x20030200
                     // ec_l3_gptr   0x20030202
                     // ec_l3_repr   0x20030206
                     // ec_l3_inex   0x2003020B

                     // L2 rings
                     if(relativeAddr >= 0x20032000 && relativeAddr <= 0x200320FF)
                     {
                         // Bit 18 - 21
                         coreSelect = (coreSelect << 10);
                         absoluteAddr = (relativeAddr & 0x00FFC3FF) | chipletNum | coreSelect;
                     }

                     // MMA rings
                     if(relativeAddr >= 0x20830000 && relativeAddr <= 0x208300FF)
                     {
                         // Bit 8 - 11
                         coreSelect = (coreSelect << 20);
                         absoluteAddr = (relativeAddr & 0x000FFFFF ) | chipletNum | coreSelect;
                     }

                     // L3 rings
                     if(relativeAddr >= 0x20030200 && relativeAddr <= 0x200302FF)
                     {
                         // Bit 22-25
                         coreSelect = (coreSelect << 6);
                         absoluteAddr = (relativeAddr & 0x00FFFC3F) | chipletNum | coreSelect;
                     }
                     break;
                 }

            case PPE_TARGET_TYPE_EQ:
            case PPE_TARGET_TYPE_PEC:
            case PPE_TARGET_TYPE_MC:
                 {
                     uint32_t chipletNum = tgtHndl.fields.chiplet_num;
                     chipletNum = chipletNum << 24;
                     absoluteAddr = (relativeAddr & 0x00FFFFFF) | chipletNum;
                     break;
                 }

            default:
                 {
                     absoluteAddr = relativeAddr;
                     break;
                 }
        }
        SBE_INFO(SBE_FUNC "Target, Relative Addr, Absolute Addr is 0x%08X, 0x%08X 0x%08X",
                           tgtHndl.value, relativeAddr, absoluteAddr);
    }while(0);
    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
}

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

        // In  MPIPL dumps, there is no collection of fastarray data.
        uint8_t collectFastArray = 0;
        // Create the sbeCollectDump object for ClockOn
        sbeCollectDump dumpClockOnObj( SBE_DUMP_TYPE_MPIPL, SBE_DUMP_CLOCK_ON, collectFastArray,
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
        // Aline 128 bytes lenfth and update hbMemAddr
        hbMemAddr = hbMemAddr + ( ((clockOnLength / 0x80) * (0x80) ) +
                                  ((clockOnLength % 0x80 )?(0x80):(0)) );

        // Create the sbeCollectDump object for ClockOff
        sbeCollectDump dumpClockOffObj( SBE_DUMP_TYPE_MPIPL, SBE_DUMP_CLOCK_OFF, collectFastArray,
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
    //  Verify check as per -c1..31 values dump data
    return ( (!iv_hdctRow->genericHdr.chipletStart) ||
             ( (chipUnitNum >= iv_hdctRow->genericHdr.chipletStart) &&
               (chipUnitNum <= iv_hdctRow->genericHdr.chipletEnd)) );
}

void sbeCollectDump::populateAllCoresScomScanState()
{
    #define SBE_FUNC "populateAllCoresScomScanState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    do
    {
        //Fetch all EQ Targets
        Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
        for(auto eqTgt: procTgt.getChildren<fapi2::TARGET_TYPE_EQ>())
        {
            scomStatus_t scomStateData;
            scanStatus_t scanStateData;
            uint32_t eqNum = eqTgt.get().getTargetInstance();
            SBE_EXEC_HWP(fapiRc,p10_query_corecachemma_access_state, eqTgt,
                                          scomStateData, scanStateData,false);
            SBE_DEBUG(SBE_FUNC "scomStateData and scanstateData is 0x%08X 0x%08X", scomStateData.scomState, scanStateData.scanState);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC " p10_query_corecachemma_access_state "
                                   " failed. RC=[0x%08X]", fapiRc);
                continue;
            }
            iv_EQScanState[eqNum] = isEqScanEnabled(scanStateData);

            //For all Core targets associated with the EQ target,
            //populate the scom and scan state.
            for (auto coreTgt : eqTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
            {
                uint32_t coreNum = coreTgt.get().getTargetInstance();
                // Populate L2ScomState for all the cores.
                iv_L2ScomState[coreNum] = isCoreOrL2CacheScomEnabled(coreTgt,scomStateData);

                // Populate L3ScomState for all the cores.
                iv_L3ScomState[coreNum] = isL3CacheScomEnabled(coreTgt,scomStateData);

                // Populate L2ScanState for all the cores.
                iv_L2ScanState[coreNum] = isCoreOrL2CacheScanEnabled(coreTgt,scanStateData);

                // Populate L3ScanState for all the cores.
                iv_L3ScanState[coreNum] = isL3CacheScanEnabled(coreTgt,scanStateData);

                // Populate L3MMAState for all the cores.
                iv_MMAScanState[coreNum] = isCoreOrMmaScanEnabled(coreTgt,scanStateData);

            }
        }
    } while(0);

    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
}

bool sbeCollectDump::checkScomAndScanStateForCore()
{
    #define SBE_FUNC "checkScomAndScanStateForCore"
    SBE_ENTER(SBE_FUNC);
    // By default scomAndScanState will set to true for cases like scom address in QME region.
    bool scomAndScanState = true;
    do
    {
        if(iv_tocRow.tocHeader.chipUnitType == CHIP_UNIT_TYPE_EQ)
        {
            if( iv_tocRow.tocHeader.cmdType == CMD_GETRING )
            {
                scomAndScanState = iv_EQScanState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }
        }

        // StopState verification only for Core
        if(iv_tocRow.tocHeader.chipUnitType != CHIP_UNIT_TYPE_C)
        {
            break;
        }

        // Verify only for getScom, traceArray, getRing and FastArray
        // There is no putscom entry for core in HDCT.
                if( !( (iv_tocRow.tocHeader.cmdType == CMD_GETSCOM) ||
               (iv_tocRow.tocHeader.cmdType == CMD_GETTRACEARRAY) ||
               (iv_tocRow.tocHeader.cmdType == CMD_GETRING) ||
               (iv_tocRow.tocHeader.cmdType == CMD_GETFASTARRAY) ) )
        {
            break;
        }

        if( iv_tocRow.tocHeader.cmdType == CMD_GETSCOM )
        {
            uint32_t addr = iv_hdctRow->cmdGetScom.addr;
            // Check if the scom address is of L3 region.
            if( 0x20010000 == ( addr &(0xFFFF0000) ))
            {
                scomAndScanState = iv_L3ScomState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }

            // Check if the scom address is of L2 region.
            if( 0x20020000 == ( addr &(0xFFFF0000) ))
            {
                scomAndScanState = iv_L2ScomState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }
            // For other case scom and scan state will be 1
        }
        if( iv_tocRow.tocHeader.cmdType == CMD_GETTRACEARRAY)
        {
            uint32_t traceArrayId = iv_hdctRow->cmdTraceArray.traceArrayID;
            // Check if tracearray ID is of L2 region.
            // _PROC_TB_LAST_IOHS_TARGET  = 0xC0,
            // Core regions - TARGET_TYPE_COREdd
            // PROC_TB_L20,
            // PROC_TB_L21,
            if( (traceArrayId == 0xC1) || (traceArrayId == 0xC2))
            {
                scomAndScanState = iv_L2ScomState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }
            // For other case scom and scan state will be 1
        }
        if( iv_tocRow.tocHeader.cmdType == CMD_GETRING )
        {
            uint32_t addr = iv_hdctRow->cmdGetRing.ringAddr;
            // Check if the scom address is of L2 region.
            if( 0x20032000 == ( addr &(0xFFFFFF00) ))
            {
                scomAndScanState = iv_L2ScanState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }

            // Check if the scom address is of L3 region.
            if( 0x20030200 == ( addr &(0xFFFFFF00) ))
            {
                scomAndScanState = iv_L3ScanState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }
            // Check if the scom address is of MMA region.
            if( 0x20830000 == ( addr &(0xFFFFFF00) ))
            {
                scomAndScanState = iv_MMAScanState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }
            // For other case scom and scan state will be 1
        }
        if( iv_tocRow.tocHeader.cmdType == CMD_GETFASTARRAY )
        {
            // Check for the control set.
            // 0x01: ec_cl2_far
            // 0x02: ec_mma_far
            if( iv_hdctRow->cmdFastArray.controlSet == 0x01 )
            {
                scomAndScanState = iv_L2ScanState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 
            }
            if( iv_hdctRow->cmdFastArray.controlSet == 0x02 )
            {
                scomAndScanState = iv_MMAScanState[(uint32_t)iv_tocRow.tocHeader.chipUnitNum]; 

            }
            // For other case scom and scan state will be 1
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return scomAndScanState;
    #undef SBE_FUNC
}

uint32_t sbeCollectDump::writeGetFastArrayPacketToFifo()
{
    #define SBE_FUNC "writeGetFastArrayPacketToFifo "
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
            iv_tocRow.tocHeader.preReq = PRE_REQ_NON_FUNCTIONAL;
            iv_tocRow.tocHeader.dataLength = 0x00;
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
            SBE_DEBUG(SBE_FUNC "DUMP GETFASTARRAY: NonFunctional Target UnitNum[0x%08X]",
                      (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
            break;
        }
        if( !checkScomAndScanStateForCore() )
        {
            // Update Core Stop Status DUMP header
            iv_tocRow.tocHeader.preReq = PRE_REQ_CORE_NOT_SCOM_SCAN_STATE;
            iv_tocRow.tocHeader.dataLength = 0x00;
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
            SBE_INFO("DUMP GETTRAEARRAY: StopStatus Enabled Target UnitNum[0x%08X]",
                      (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
            break;
        }
        // Get the fast array length as per DD level
        // 4 Bits ecMajor should be [0001 == DD1], [ 0002 == DD2 ]
        if( iv_dumpHeader.ecMajor == 0x01 ) // Update FastArray data size for DD1
        {
            // To get the fast array length we will hard code and stream out the
            // data length as per the actual length value as per DD levels
            if(iv_hdctRow->cmdFastArray.controlSet == 0x01)
                iv_tocRow.tocHeader.dataLength = (FAST_ARRAY_CTRL_SET1_DD1_SIZE * 8);
            else
                iv_tocRow.tocHeader.dataLength = (FAST_ARRAY_CTRL_SET2_DD1_SIZE * 8);
        }
        else  // Update FastArray data size for DD2
        {
            if(iv_hdctRow->cmdFastArray.controlSet == 0x01)
                iv_tocRow.tocHeader.dataLength = (FAST_ARRAY_CTRL_SET1_DD2_SIZE * 8);
            else
                iv_tocRow.tocHeader.dataLength = (FAST_ARRAY_CTRL_SET2_DD2_SIZE * 8);
        }
        // Update TOC Header
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);

        // Create the req struct for the sbeFastArray Chip-op
        sbeControlFastArrayCMD_t dumpFastArrayReq = {0};
        uint32_t dummyDataLengthInBits =
            64 * (((uint32_t)(iv_tocRow.tocHeader.dataLength / 64)) + ((uint32_t)(iv_tocRow.tocHeader.dataLength % 64) ? 1:0 ));
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
        SBE_INFO(SBE_FUNC "Dump FastArray: control_set[0x%08X], chipUnitNum [0x%08X]",
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
        // TODO: Remove bellow if condition
        if(iv_hdctRow->cmdStopClocks.tgtType == 0x01)
        {
            dumpStopClockReq.targetType = 0x05;
        }
        dumpStopClockReq.chipletId = 0xff;
        sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                         (uint32_t*)&dumpStopClockReq, false );
        rc = sbeStopClocks_Wrap( istream, iv_oStream );
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC" Error from StopClocks on clockType[0x%02X], "
                      "chipUnitNum[0x%08X] ", iv_hdctRow->cmdStopClocks.tgtType,
                       iv_tocRow.tocHeader.chipUnitNum);
            break;
        }
        SBE_INFO(SBE_FUNC" clockTypeTgt[0x%04X], chipUnitNum[0x%08X] ",
                 dumpStopClockReq.targetType, iv_tocRow.tocHeader.chipUnitNum);
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
        iv_tocRow.tocHeader.preReq = PRE_REQ_NON_FUNCTIONAL;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_DEBUG("DUMP GETTRACEARRAY: NonFunctional Target UnitNum[0x%08X]",
                  (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }

    if( !checkScomAndScanStateForCore() )
    {
        // Update Core Stop Status DUMP header
        iv_tocRow.tocHeader.preReq = PRE_REQ_CORE_NOT_SCOM_SCAN_STATE;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_INFO("DUMP GETTRAEARRAY: StopStatus Enabled Target UnitNum[0x%08X]",
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

    // Skip stopping the tracearray for PERF dump.
    // It is handled by the HDCT.
    if( iv_dumpType != SBE_DUMP_TYPE_PERF)
    {
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
    // Skip resetting/restarting tracearray for PERF dump.
    // It is handled by the HDCT.
    if(iv_dumpType != SBE_DUMP_TYPE_PERF)
    {
        reqMsg.operation    = SBE_TA_RESET; // 0x0001: Reset Trace array

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                         " traceArrayId [0x%04X] operation [0x%04X]",
                           reqMsg.targetType, reqMsg.chipletId,
                           reqMsg.traceArrayId, reqMsg.operation);

        sbefifo_hwp_data_istream istreamReset( iv_fifoType, len,
                                               (uint32_t*)&reqMsg, false );

        rc = sbeControlTraceArrayWrap( istreamReset, iv_oStream );
        reqMsg.operation    = SBE_TA_RESTART; // 0x0002: Restart Trace array

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                         " traceArrayId [0x%04X] operation [0x%04X]",
                           reqMsg.targetType, reqMsg.chipletId,
                           reqMsg.traceArrayId, reqMsg.operation);

        sbefifo_hwp_data_istream istreamRestart( iv_fifoType, len,
                                                 (uint32_t*)&reqMsg, false );

        rc = sbeControlTraceArrayWrap( istreamRestart, iv_oStream );
    }
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

    bool pervTargetFunctionalState = true;
    if( (iv_tocRow.tocHeader.chipUnitType == CHIP_UNIT_TYPE_CHIP) ||
        (iv_tocRow.tocHeader.chipUnitType == CHIP_UNIT_TYPE_UNKNOWN) )
    {
        uint8_t chipletUnitNum = ( ( iv_hdctRow->cmdGetRing.ringAddr & 0xFF000000 ) >> 24 );
        plat_target_handle_t tgtHndl =
              plat_getTargetHandleByChipletNumber<TARGET_TYPE_PERV>(chipletUnitNum);
        pervTargetFunctionalState = tgtHndl.fields.functional;
        SBE_INFO("DUMP GETRING: Perv Target UnitNum[0x%02X], FunctionalState[0x%02X]",
                  chipletUnitNum, pervTargetFunctionalState);
        // PAU chiplets are always functional, but check the individual PAUs

        if((chipletUnitNum >= PAUC_CHIPLET_OFFSET) &&
           (chipletUnitNum < (PAUC_CHIPLET_OFFSET + PAUC_TARGET_COUNT)))
        {
           // functional state of the PAU targets [ PAU0, PAU3, PAU4 and PAU6 ]
           uint8_t pauRingNum = (iv_hdctRow->cmdGetRing.ringAddr & 0xF000)>>12;
           uint8_t pauChipId = (chipletUnitNum - PAUC_CHIPLET_OFFSET);
           static const uint8_t  pauc_pau_map[PAUC_TARGET_COUNT][PAU_PER_PAUC] = {
                      { PAU_TARGET_OFFSET + 0, 0 },
                      { PAU_TARGET_OFFSET + 1, 0 },
                      { PAU_TARGET_OFFSET + 2, PAU_TARGET_OFFSET + 3 },
                      { PAU_TARGET_OFFSET + 4, PAU_TARGET_OFFSET + 5 }
                  };
           if( (pauRingNum != 0x2) && (pauRingNum != 0x1) )
           {
               // Use PAUC funtional state: pau_func / pau_gptr / pau_repr
               // total 6 pau targets: pau0/pau3/pau4/pau5/pau6/pau7
               SBE_INFO("DUMP GETRING: PreCheck for Chiplet PAUC target");
           }
           else
           {
               // check pauUnitNum 0x02 bit for PAU0, PAU3, PAU4 and PAU6
               // check pauUnitNum 0x01 bit for PAU5 and PAU7
               uint8_t pauUnitNum = 0;
               if( pauRingNum == 0x1 )
               {
                   pauUnitNum = 1;
               }
               SBE_INFO("DUMP GETRING: PreCheck for PAUC[%d]PAU[%d]", pauChipId, pauUnitNum);
               uint8_t pauLogId = pauc_pau_map[pauChipId][pauUnitNum];
               pervTargetFunctionalState
                                =  G_vec_targets[pauLogId].getFunctional();
           }
        }
    }

    if((!iv_tocRow.tgtHndl.getFunctional()) || (!pervTargetFunctionalState) )
    {
        // Update non functional state in DUMP header
        iv_tocRow.tocHeader.preReq = PRE_REQ_NON_FUNCTIONAL;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_DEBUG("DUMP GETRING: NonFunctional Target UnitNum[0x%08X]",
                   (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }

    if( !checkScomAndScanStateForCore() )
    {
        // Update Core Stop Status DUMP header
        iv_tocRow.tocHeader.preReq = PRE_REQ_CORE_NOT_SCOM_SCAN_STATE;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_INFO("DUMP GETRING: StopStatus Enabled Target UnitNum[0x%08X]",
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

    uint32_t translatedAddress = 0;
    getAbsoluteAddressForRing(iv_tocRow.tgtHndl, iv_hdctRow->cmdGetRing.ringAddr, translatedAddress);

    l_reqMsg.ringAddr = translatedAddress;
    l_reqMsg.ringMode = 0x0001;
    l_reqMsg.ringLenInBits = bitlength;

    SBE_DEBUG(SBE_FUNC "Ring Address 0x%08X User Ring Mode 0x%04X "
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
            uint32_t startCount = iv_oStream.words_written();
            rc = sbeMemAccess_Wrap( istream, iv_oStream, true );
            uint32_t endCount = iv_oStream.words_written();
            uint32_t totalCount = 32; // 1024 bits == 128 Bytes == 32 words
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

            // Verify and modify all error rc and FFDC id's as per ChipOp RC
            if(iv_oStream.getFifoRc() == FAPI2_RC_SUCCESS)
            {
                iv_oStream.setFifoRc(fapiRc);
            }
            rc = SBE_SEC_INVALID_ADDRESS_PASSED;
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
            iv_tocRow.tocHeader.preReq = PRE_REQ_NON_FUNCTIONAL;
            iv_tocRow.tocHeader.dataLength = 0x00;
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
            SBE_DEBUG("DUMP GETSRAM: NonFunctional Target UnitNum[0x%08X]",
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
        if(!iv_tocRow.tgtHndl.getFunctional())
        {
            iv_tocRow.tocHeader.preReq = PRE_REQ_NON_FUNCTIONAL; 
            iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
            SBE_DEBUG("DUMP PUTSCOM: NonFunctional Target UnitNum[0x%08X]",
                     (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
            break;
        }
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
    iv_oStream.setFifoRc(FAPI2_RC_SUCCESS);
    iv_oStream.setPriSecRc(SBE_PRI_OPERATION_SUCCESSFUL);

    // Update address, length and stream header data vai FIFO
    iv_tocRow.tocHeader.address = iv_hdctRow->cmdGetScom.addr;
    uint32_t len = sizeof(iv_tocRow.tocHeader) / sizeof(uint32_t);
    if(!iv_tocRow.tgtHndl.getFunctional())
    {
        // Update non functional state DUMP header
        iv_tocRow.tocHeader.preReq = PRE_REQ_NON_FUNCTIONAL;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_DEBUG("DUMP GETSCOM: NonFunctional Target UnitNum[0x%08X]",
                   (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }
    if( !checkScomAndScanStateForCore() )
    {
        // Update Core Stop Status DUMP header
        iv_tocRow.tocHeader.preReq = PRE_REQ_CORE_NOT_SCOM_SCAN_STATE;
        iv_tocRow.tocHeader.dataLength = 0x00;
        iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
        SBE_INFO("DUMP GETSCOM: Core Stop State Target UnitNum[0x%08X]",
                  (uint32_t)iv_tocRow.tocHeader.chipUnitNum);
        return rc;
    }
    iv_tocRow.tocHeader.dataLength = 0x40; // 64 bits -or- 2 words
    iv_oStream.put(len, (uint32_t*)&iv_tocRow.tocHeader);
    uint64_t dumpData;
    if(iv_tocRow.tocHeader.chipUnitType == CHIP_UNIT_TYPE_OCMB)
    {
        // OCMB Scoms
        sbeGetHWRegReqMsg_t reqMsg = {};
        len  = sizeof(sbeGetHWRegReqMsg_t)/sizeof(uint32_t);
        reqMsg.targetType = TARGET_OCMB_CHIP;
        reqMsg.reserved = 0x00;
        reqMsg.targetInstance = iv_tocRow.tgtHndl.fields.chiplet_num;
        reqMsg.hiAddr = 0x00;
        reqMsg.lowAddr = iv_tocRow.tocHeader.address;
        SBE_INFO(SBE_FUNC "OCMB Address[0x%08X], targetType[0x%04X] "
                          "targetInstance number:[0x%02X]", reqMsg.lowAddr,
                           reqMsg.targetType, reqMsg.targetInstance);
        sbefifo_hwp_data_istream istream( iv_fifoType, len,
                                         (uint32_t*)&reqMsg, false );
        rc = sbeGetHWReg_Wrap( istream, iv_oStream );
        if( (rc != SBE_SEC_OPERATION_SUCCESSFUL) ||
            (iv_oStream.getFifoRc() != FAPI2_RC_SUCCESS) ||
            (iv_oStream.getPriSecRc() != SBE_SEC_OPERATION_SUCCESSFUL) )
        {
            SBE_ERROR(SBE_FUNC "sbeGetHWReg_Wrap failed for OCMB Instance");
            rc = SBE_SEC_INVALID_ADDRESS_PASSED;
            iv_oStream.put(FIFO_DOUBLEWORD_LEN, (uint32_t*)&dumpData);
        }       
    }
    else
    {
        // Proc Scoms
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
            SBE_DEBUG("getScom: address: 0x%08X, data HI: 0x%08X, data LO: 0x%08X ",
                   iv_tocRow.tocHeader.address, (dumpData >> 32),
                   static_cast<uint32_t>(dumpData & 0xFFFFFFFF));
        }
        iv_oStream.put(FIFO_DOUBLEWORD_LEN, (uint32_t*)&dumpData);
    }
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
        case CHIP_UNIT_TYPE_OCMB:
            {
                SBE_INFO(SBE_FUNC "OCMB Instance is %d", iv_OCMBInstance);
                Target<TARGET_TYPE_OCMB_CHIP> ocmbTgt = plat_getOCMBTargetHandleByInstance
                    <fapi2::TARGET_TYPE_OCMB_CHIP>(iv_OCMBInstance);
                SBE_DEBUG(SBE_FUNC "OCMB: [0x%08X]", ocmbTgt.get());
                o_targetList.push_back(ocmbTgt.get());
                break;
            }
        case CHIP_UNIT_TYPE_PERV:
            for( auto& target : procTgt.getChildren<fapi2::TARGET_TYPE_PERV>(
                static_cast<fapi2::TargetFilter>(
                        fapi2::TARGET_FILTER_TP |
                        fapi2::TARGET_FILTER_ALL_NEST |
                        fapi2::TARGET_FILTER_ALL_MC  |
                        fapi2::TARGET_FILTER_ALL_PCI  |
                        fapi2::TARGET_FILTER_ALL_PAU |
                        fapi2::TARGET_FILTER_ALL_IOHS |
                        fapi2::TARGET_FILTER_ALL_EQ),
                fapi2::TARGET_STATE_PRESENT))
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

    do
    {
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
        {
            SBE_ERROR(SBE_FUNC "Unsupported command types %d", (uint32_t)iv_tocRow.tocHeader.cmdType);
            break;
        }

        if((iv_tocRow.tocHeader.cmdType == CMD_GETFASTARRAY ) &&
           (iv_collectFastArray == 0))
        {
            SBE_INFO(SBE_FUNC "No need to collect fastarray");
            break;
        }

        if(iv_tocRow.tocHeader.cmdType == CMD_GETMEMPBA)
        {
            // HbDump only for Master Proc based on the
            // attribute ATTR_PROC_SBE_MASTER_CHIP value
            Target<TARGET_TYPE_PROC_CHIP> procTarget = plat_getChipTarget();
            fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type isMaster = false;
            FAPI_ATTR_GET(ATTR_PROC_SBE_MASTER_CHIP,procTarget,isMaster);
            if( !isMaster )
            {
                SBE_INFO(SBE_FUNC "HBMEM data will be collected only on master proc %d", isMaster);
                break;
            }
        }

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
            iv_tocRow.tocHeader.preReq = PRE_REQ_PASSED;
            if(iv_tocRow.tocHeader.chipUnitType == CHIP_UNIT_TYPE_PERV)
            {
                iv_tocRow.tocHeader.chipUnitNum = dumpRowTgtHnd.getChipletNumber();
            }
            else if(iv_tocRow.tocHeader.chipUnitType == CHIP_UNIT_TYPE_OCMB)
            {
                iv_tocRow.tocHeader.chipUnitNum = iv_OCMBInstance;
            }
            else
            {
                iv_tocRow.tocHeader.chipUnitNum = dumpRowTgtHnd.get().getTargetInstance();
            }
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
                    // Gather scom and scan state for all the cores.
                    populateAllCoresScomScanState();
                    break;
                }
                case CMD_GETMEMPBA:
                {
                    rc = writeGetMemPBAPacketToFifo();
                    break;
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
    }while(0);
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
        // Update PHB functional state
        updatePhbFunctionalState(); // Ignore return value

        // TODO: Gather clock state for all targets.

        // Gather scom and scan state for all the cores.
        populateAllCoresScomScanState();

        for(uint32_t coreNum = 0; coreNum < CORE_TARGET_COUNT; coreNum++)
        {
             SBE_DEBUG(SBE_FUNC "Core num:0x%02X L2 scom state:0x%02X L3 scom state:0x%02X", coreNum, iv_L2ScomState[coreNum], iv_L3ScomState[coreNum]);
             SBE_DEBUG(SBE_FUNC "L2 scan state:0x%02X L3 scan state:0x%02X MMA scan state:0x%02X", iv_L2ScanState[coreNum], iv_L3ScanState[coreNum], iv_MMAScanState[coreNum]);
        }

        // Write the dump header to FIFO
        uint32_t len = sizeof(dumpHeader_t)/ sizeof(uint32_t);
        rc = iv_oStream.put(len, (uint32_t*)&iv_dumpHeader);
        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Store the HDCT pointer for OCMB scoms.
        uint32_t hdctPointer = 0;
        while(sbeCollectDump::parserSingleHDCTEntry())
        {
            // OCMB scoms are in the last of the HDCT.txt
            // After OCMB scoms there will be no PROC entries.
            // Once we hit the OCMB entry, we will come out of this loop.
            if(iv_hdctRow->genericHdr.chipUnitType == CHIP_UNIT_TYPE_OCMB)
            {
                SBE_INFO(SBE_FUNC "Skip OCMB entries from the PROC DUMP BLOB");
                break;
            }
            // TODO: Verify and modify all error rc to handle all
            // primary/secondary error in DUMP chipOp
            rc = writeDumpPacketRowToFifo();
            if(rc)
            {
                SBE_ERROR(SBE_FUNC" Dump collection failed");
                rc = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                break;
            }
            hdctPointer = iv_hdctXipSecDetails.currAddr;
        }
        // Collect all OCMB entries for an instance.
        // Then repeat for all OCMB instances.
        // This has to be called for only clock on.
        if((iv_clockState == SBE_DUMP_CLOCK_ON) && (!rc))
        {
            for(iv_OCMBInstance = 0; iv_OCMBInstance < OCMB_TARGET_COUNT ; iv_OCMBInstance++)
            {
                iv_hdctXipSecDetails.currAddr = hdctPointer;
                SBE_INFO(SBE_FUNC "OCMB Instance is %d HDCT start Address is 0x%08X and End Address is 0x%08X",
                       iv_OCMBInstance, iv_hdctXipSecDetails.currAddr, iv_hdctXipSecDetails.endAddr);
                // OCMB indetifer as OCMB..01
                iv_oStream.put(OCMB_IDENTIFIER);

                // Convert Instance number into ASCII
                uint32_t ocmbInstance = 0;
                if(iv_OCMBInstance < 10)
                {
                    ocmbInstance = 0x20202000 | (0x30 + (uint8_t)iv_OCMBInstance);
                }
                else
                {
                    ocmbInstance = 0x20203100 | (0x30 + (uint8_t)iv_OCMBInstance - 10); 
                }
                SBE_INFO(SBE_FUNC "OCMB Instance is 0x%08X", ocmbInstance);
                iv_oStream.put(ocmbInstance);
                while(sbeCollectDump::parserSingleHDCTEntry())
                {
                    rc = writeDumpPacketRowToFifo();
                    if(rc)
                    {
                        SBE_ERROR(SBE_FUNC" Dump collection failed");
                        rc = SBE_PRI_GENERIC_EXECUTION_FAILURE;
                        break;
                    }
                }
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
