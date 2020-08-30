/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbearchregdump.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2020                        */
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

#include "sbefifo.H"
#include "sbeSpMsg.H"
#include "sbe_sp_intf.H"
#include "sbeHostMsg.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbecmdmpipl.H"
#include "sberegaccess.H"
#include "sbefapiutil.H"
#include "sbearchregdump.H"
#include "sbeglobals.H"
#include "sbeMemAccessInterface.H"
#include "sbecmdcntlinst.H"
#include "p10_thread_control.H"
#include "p10_ram_core.H"
#include "p10_scom_c_0.H"
#include "p10_scom_proc_b.H"
#include "sbeutil.H"
#include "fapi2_mem_access.H"
#include "p10_getmemproc.H"
#include "target_filters.H"
#include "sbeFFDC.H"

#include "fapi2.H"

using namespace fapi2;

#define SPR_LIST_SIZE  142
#define GPR_LIST_SIZE  32
#define TIMA_LIST_SIZE 8
#define GPR_REG_TYPE   0x1
#define SPR_REG_TYPE   0x2
#define TIMA_REG_TYPE  0x3

// This is defined in Hostboot src/include/usr/sbeio/sbeioif.H, this is just a
// copy, so that SBE refers to the same count.
#define STASH_KEY_CNT_FOR_ARCH_DUMP_ADDR 0x4

#define SSHSRC_STOP_GATED_BIT 0

#define INVALID_CORE_STATE 0x1

///////////////////////////////////////////////////////////////////////
// @brief sbeFetchRegDumpAddrFromStash
//
///////////////////////////////////////////////////////////////////////
uint64_t sbeFetchRegDumpAddrFromStash(void)
{
    #define SBE_FUNC " sbeFetchRegDumpAddrFromStash "
    SBE_ENTER(SBE_FUNC);
    uint64_t addr = 0;
    for(uint8_t cnt=0; cnt<MAX_ROW_COUNT; cnt++)
    {
        if(SBE_GLOBAL->sbeKeyAddrPair.keyValuePairfromHost.key[cnt] ==
                STASH_KEY_CNT_FOR_ARCH_DUMP_ADDR)
        {
            addr = SBE_GLOBAL->sbeKeyAddrPair.keyValuePairfromHost.addr[cnt];
            break;
        }
    }
    SBE_EXIT(SBE_FUNC);
    return addr;
    #undef SBE_FUNC
}

ReturnCode checkCoreStateForRam(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_coreTgt,
        bool  & o_isRamming,
        uint8_t & o_coreState )
{
    #define SBE_FUNC " checkCoreStateForRam "
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint8_t chipUnitNum = 0;
    o_isRamming = false;
    o_coreState = 0;
    FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_coreTgt, chipUnitNum);

    do
    {
        fapi2::buffer<uint64_t> datBuffer = 0;
        fapiRc = getscom_abs_wrap (&i_coreTgt, scomt::c::QME_SSH_SRC, &datBuffer());
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Failed to read SSHSRC for Core[%d]",
                    chipUnitNum);
            break;
        }
        //If stop state history register has QME_SSH_SRC_STOP_GATED bit set then
        //ramming cannot be performed.
        if(datBuffer.getBit<scomt::c::QME_SSH_SRC_STOP_GATED>())
        {
            SBE_DEBUG("core=0x%.8x QME_SSH_SRC value=0x%.8x%.8x, STOP_GATED(1),no ramming",
                     i_coreTgt.get(),(((uint64_t)datBuffer & 0xFFFFFFFF00000000ull) >> 32),
                     ((uint64_t)datBuffer & 0xFFFFFFFF));
            datBuffer.extractToRight<uint8_t>(o_coreState,scomt::c::QME_SSH_SRC_ACT_STOP_LEVEL,
                     scomt::c::QME_SSH_SRC_ACT_STOP_LEVEL_LEN);
            break;
        }
        //If Core is not STOP GATED, then derive the core state based on the Stop state
        //control register
        datBuffer = 0x0;
        fapiRc = getscom_abs_wrap (&i_coreTgt, scomt::c::QME_SCSR, &datBuffer());
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Failed to read scomt::c::QME_SCSR for Core[%d]",
                      chipUnitNum);
            break;
        }
        SBE_DEBUG("core=0x%.8x QME_SCSR value=0x%.8x%.8x",i_coreTgt.get(),
                 (((uint64_t)datBuffer & 0xFFFFFFFF00000000ull) >> 32),
                 ((uint64_t)datBuffer & 0xFFFFFFFF));

        //Read the QME_SCSR_PM_STATE_ACTIVE bit(59)
        //If bit 59 is set then bits 60..63 indicate the PM State
        if(datBuffer.getBit<scomt::c::QME_SCSR_PM_STATE_ACTIVE>())
        {
            datBuffer.extractToRight<uint8_t>(o_coreState,
                    scomt::c::QME_SCSR_PM_STATE ,scomt::c::QME_SCSR_PM_STATE_LEN);
            //If obtained core state is 0 then ramming is allowed
            if(!o_coreState)
            {
                o_isRamming = true;
            }
            break;
        }
        else
        {
            //If the QME_SCSR_PM_STATE_ACTIVE bit is not set then core is considered SCOMABLE
            SBE_DEBUG("In QME_SCSR , PM_STATE_ACTIVE not set, Ramming is allowed");
            o_isRamming = true;
            break;
        }

    }while(0);

    SBE_INFO(SBE_FUNC "Core[%d] StopState[%d] and isRamming[%d]",
             chipUnitNum, o_coreState, o_isRamming);

    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeDumpArchRegs Dump out the architected registers
//
///////////////////////////////////////////////////////////////////////
ReturnCode sbeDumpArchRegs()
{
    #define SBE_FUNC " sbeDumpArchRegs "
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint64_t dumpAddr = 0;
    sbeArchRegDumpProcHdr_t dumpProcHdr = {};
    sbeArchRegDumpThreadHdr_t dumpThreadHdr = {};
    sbeArchRegDumpEntries_t dumpRegData = {};

    //TODO: Add below SPRs to the list after confirming the 
    //requirement 496,497,505,506,507,511
    //
    // Combined list of SPRs GPRs TIMAs
    static const uint16_t SPR_GPR_TIMA_list[] = {
        // List for TIMA 0-7 to be collected in MPIPL path
        0, 1, 2, 3, 4, 5, 6, 7,
        // List for SPRs to be collected in MPIPL path
        1 ,3 ,8 ,9 ,13 ,17 ,18 ,19 ,22 ,26 ,27 ,28 ,29 ,48 ,61 ,128 ,129 ,
        130 ,131 ,136 ,152 ,153 ,157 ,158 ,159 ,176 ,180 ,181 ,186 ,187 ,
        188 ,189 ,190 ,256 ,259 ,268 ,269 ,272 ,273 ,274 ,275 ,276 ,277 ,
        284 ,285 ,286 ,287 ,304 ,305 ,306 ,307 ,308 ,309 ,310 ,313 ,314 ,
        315 ,317 ,318 ,319 ,336 ,337 ,338 ,339 ,349 ,446 ,455 ,464 ,471 ,
        736 ,737 ,738 ,752 ,753 ,754 ,768 ,769 ,770 ,771 ,772 ,773 ,774 ,
        775 ,776 ,779 ,780 ,781 ,782 ,784 ,785 ,786 ,787 ,788 ,789 ,790 ,
        791 ,792 ,795 ,796 ,797 ,798 ,799 ,800 ,801 ,802 ,803 ,804 ,805 ,
        806 ,812 ,815 ,816 ,823 ,828 ,848 ,849 ,850 ,851 ,853 ,855 ,861 ,
        880 ,881 ,882 ,884 ,885 ,895 ,896 ,898 ,921 ,922 ,1006,1008,1023,
        2000,2001,2002,2003,2004,2005,2006,2007,
        // List for GPRs to be collected in MPIPL path
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    };

    do
    {
        Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
        dumpAddr = sbeFetchRegDumpAddrFromStash();
        if(dumpAddr == 0xFFFFFFFFFFFFFFFFULL || dumpAddr == 0)
        {
            SBE_ERROR(SBE_FUNC "Invalid Dump address from Stash [0x%08X %08X]",
                      SBE::higher32BWord(dumpAddr),SBE::lower32BWord(dumpAddr));
            break;
        }
        SBE_INFO(SBE_FUNC "Stash Dump Addr = [0x%08X %08X] ",
                     SBE::higher32BWord(dumpAddr), SBE::lower32BWord(dumpAddr));

        // Initialise the PBA with the above address from stash,
        // The access API would use it in auto-increment mode.
        p10_PBA_oper_flag pbaFlag;
        pbaFlag.setOperationType(p10_PBA_oper_flag::INJ);
        sbeMemAccessInterface PBAInterface(
                SBE_MEM_ACCESS_PBA,
                dumpAddr,
                &pbaFlag,
                SBE_MEM_ACCESS_WRITE,
                sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);

        //Build the Proc specific header and send to the host
        dumpProcHdr.ownerId = 0; //Indicates SBE
        dumpProcHdr.version = DUMP_STRUCT_VERSION_ID;
        dumpProcHdr.core_cnt = 
                          procTgt.getChildren<fapi2::TARGET_TYPE_CORE>().size();
        dumpProcHdr.thread_count = (dumpProcHdr.core_cnt * SMT4_THREAD_MAX);
        dumpProcHdr.reg_cnt = (sizeof(SPR_GPR_TIMA_list)/sizeof(uint16_t));

        fapiRc = PBAInterface.accessWithBuffer(&dumpProcHdr,
                 sizeof(dumpProcHdr),false);
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed to write Proc header to hostboot");
            break;
        }

        //Interate through all the Cores under the Proc chips
        for(auto &coreTgt : procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            bool doRamming = false;
            uint8_t coreState = 0;
            uint8_t chipUnitNum = 0;
            uint8_t procGrpId = 0;
            uint8_t procChipId = 0;

            // Required for PIR calculation
            FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, coreTgt, chipUnitNum);
            fetchEffectiveGroupAndChipId(procGrpId, procChipId);
            fapiRc = checkCoreStateForRam(coreTgt, doRamming, coreState);
            if( fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC "Failed in checkCoreStateForRam Core[%d],"
                    "GroupId[%d], ChipId[%d]",chipUnitNum,procGrpId,procChipId);
                //Mark the coreState to be bad and disable ramming.
                coreState = INVALID_CORE_STATE;
                doRamming = false;
            }

            for(uint8_t thread = SMT4_THREAD0;thread < SMT4_THREAD_MAX;thread++)
            {
                //Build the thread header for each thread related to the core.
                dumpThreadHdr.pir.procGrpId = procGrpId;
                dumpThreadHdr.pir.procChipId = procChipId;
                dumpThreadHdr.pir.chipUnitNum = chipUnitNum;
                dumpThreadHdr.pir.thread = thread;
                dumpThreadHdr.coreState = coreState;

                //Check if ram setup passes on this thread before sending the
                //thread header content.
                RamCore ramCore( coreTgt, thread );
                if(doRamming)
                {
                    //Do the ramming setup
                    SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_setup)
                    if( fapiRc != FAPI2_RC_SUCCESS )
                    {
                        SBE_ERROR(SBE_FUNC" ram_setup failed. threadNr:0x%2X"
                                "coreChipletId:0x%02X Proc:0x%02X",thread, chipUnitNum,
                                procChipId);

                        //Mark the coreState to be bad for this thread in the
                        //thread header to indicate the non-existence of register
                        //data.
                        dumpThreadHdr.coreState = INVALID_CORE_STATE;
                        doRamming = false;
                    }
                    SBE_DEBUG("RAM SETUP: coreTgt=0x%.8x thread=%d",coreTgt.get(),thread);
                }

                //Send the thread header content
                fapiRc = PBAInterface.accessWithBuffer(&dumpThreadHdr,
                                                   sizeof(dumpThreadHdr),false);
                if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC "Failed to write thread header to hostboot");
                    break;
                }

                uint32_t totalRegisters=(sizeof(SPR_GPR_TIMA_list)/sizeof(uint16_t));
                if(!doRamming)
                {
                    // Do attempt to read only TIMA register data
                    totalRegisters = TIMA_LIST_SIZE;
                }
                // Loop over the combined list of registers
                for( uint32_t regIdx=0; regIdx < totalRegisters; regIdx++ )
                {
                    dumpRegData = {};
                    Enum_RegType type=REG_GPR;
                    // Start filling up the rest of data structure
                    if(regIdx < TIMA_LIST_SIZE)
                    {
                        dumpRegData.regType = TIMA_REG_TYPE;
                    }
                    else if((regIdx >= TIMA_LIST_SIZE) &&
                           (regIdx < (TIMA_LIST_SIZE+SPR_LIST_SIZE)))
                    {
                        type = REG_SPR;
                        dumpRegData.regType = SPR_REG_TYPE;
                    }
                    // Switch to GPRs once SPRs are over in the list
                    else
                    {
                        type = REG_GPR;
                        dumpRegData.regType = GPR_REG_TYPE;
                    }

                    dumpRegData.regNum = SPR_GPR_TIMA_list[regIdx];
                    if((regIdx+1) == totalRegisters)
                    {
                        dumpRegData.isLastReg = true;
                    }
                    else
                    {
                        dumpRegData.isLastReg = false;
                    }

                    fapi2::buffer<uint64_t> data64;
                    //Do not attempt to read the SPR or GPR register data related to
                    //current core / Thread as the core status is not valid
                    if( dumpRegData.regType != TIMA_REG_TYPE )
                    {
                        SBE_EXEC_HWP(fapiRc, ramCore.get_reg, type,
                                     SPR_GPR_TIMA_list[regIdx], &data64, true)
                        if(fapiRc!=FAPI2_RC_SUCCESS)
                        {
                             SBE_ERROR("Failed to read SPR/GPR Register:threadNr:0x%x"
                            "coreChipletId:0x%02x, regNr:%u regType:%u",thread,
                            chipUnitNum, SPR_GPR_TIMA_list[regIdx], dumpRegData.regType);
                            dumpRegData.isRegDataValid = false;//Invalid data
                            dumpRegData.regVal = (uint64_t)fapiRc; //Save FAPI_RC
                            dumpRegData.isLastReg = true;//No more registers fetched
                            dumpRegData.isFfdcPresent = true;
                        }
                    }
                    else
                    {
                        //TODO:Add logic to fetch TIMA offsets
                        data64 = 0xDEADBEEFBEEFDEAD;
                    }
                    if(fapiRc == FAPI2_RC_SUCCESS)
                    {
                        dumpRegData.regVal = data64;
                        dumpRegData.isRegDataValid = true; //Valid Data
                    }

                    //Send the Register data to the Hostboot using PBA
                    fapiRc = PBAInterface.accessWithBuffer(
                            &dumpRegData, sizeof(dumpRegData),false);
                    if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                    {
                        SBE_ERROR(SBE_FUNC "failed in register content to hostboot");
                        break;
                    }

                    //Add the FAPI FFDC if fetching the register content had
                    //failed
                    if(dumpRegData.isFfdcPresent)
                    {
                        //g_FfdcData Will have FFDC data collected on failure
                        SBE_DEBUG("FAPI_RC=0x%.8x and FFDC Length =0x%.8x",
                                  g_FfdcData.fapiRc,g_FfdcData.ffdcLength);

                        //Write Length of FFDC
                        fapiRc = PBAInterface.accessWithBuffer(
                                 &g_FfdcData.ffdcLength,4,false);
                        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                        {
                           SBE_ERROR(SBE_FUNC "failed to write FFDC length");
                           break;
                        }
                        //Write the FFDC bytes
                        fapiRc = PBAInterface.accessWithBuffer(
                                 &g_FfdcData.ffdcData,g_FfdcData.ffdcLength,false);
                        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                        {
                           SBE_ERROR(SBE_FUNC "failed to write failure FFDC to hostboot");
                           break;
                        }
                    }
                    if(dumpRegData.isLastReg)
                    {
                        //Stop reading reset of the register data and exit out
                        //of register loop and continue for next thread.
                        break;
                    }
                }//End of Register loop
                 if(fapiRc)
                {
                    break;
                }

                if(doRamming)
                {
                    SBE_DEBUG("Attempting RAM Cleanup core=0x%.8x Thread=%d",coreTgt.get(),thread);
                    //Clean up the ram core setup
                    SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_cleanup)
                    if( fapiRc != FAPI2_RC_SUCCESS )
                    {
                        SBE_ERROR(SBE_FUNC" ram_cleanup failed. threadNr:0x%02X"
                                " coreChipletId:0x%02X", thread, chipUnitNum);
                         // Don't break, continue for the next thread
                        continue;
                    }
                }
            } //End of Thread Loop
            if(fapiRc)
            {
                break;
            }
        }//End of Core loop

        if(fapiRc == FAPI2_RC_SUCCESS)
        {
            // Just see that we are pushing the last PBA Frame here so as to flush
            // anything which is stuck before of a non-aligned frame.
            sbeArchRegDumpThreadHdr_t dump_dummy = {};
            fapiRc = PBAInterface.accessWithBuffer(&dump_dummy, sizeof(dump_dummy), true);
            if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "failed to write the last frame to hostboot");
                break;
            }
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////////////////////////////////////
void fetchEffectiveGroupAndChipId(uint8_t &o_procGroupId,
                                      uint8_t &o_chipId)
{
#define SBE_FUNC " fetchGroupAndChipId "
    SBE_ENTER(SBE_FUNC);

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    uint8_t topologyMode = 0;
    uint8_t topologyId = 0;
    FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_EFF_TOPOLOGY_ID, procTgt, topologyId);
    FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_MODE, FAPI_SYSTEM, topologyMode);
    //If Mode = 0 , Topology ID: GGG_C
    //If Mode = 1 , Topology ID: GG_CC
    if(topologyMode == fapi2::ENUM_ATTR_PROC_FABRIC_TOPOLOGY_MODE_MODE0) //GGG_C
    {
       o_chipId = topologyId & 0x1;
       o_procGroupId = (topologyId & 0xE) >> 1;
    }
    else //GG_CC
    {
        o_chipId = topologyId & 0x3;
        o_procGroupId = (topologyId & 0xC) >> 2;
    }
    SBE_INFO("topologyMode=%d , topologyId=0x%.8x , o_procGroupId=0x%.8x and  o_chipId = 0x%x",
               topologyMode,topologyId, o_procGroupId, o_chipId);
    SBE_EXIT(SBE_FUNC);
#undef SBE_FUNC
}



