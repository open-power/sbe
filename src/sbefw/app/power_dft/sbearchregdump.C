/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbearchregdump.C $                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2020                        */
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

#include "fapi2.H"

using namespace fapi2;

#define SPR_LIST_SIZE 63
#define GPR_LIST_SIZE 32
#define GPR_REG_TYPE 0x1
#define SPR_REG_TYPE 0x2
// This is defined in Hostboot src/include/usr/sbeio/sbeioif.H, this is just a
// copy, so that SBE refers to the same count.
#define STASH_KEY_CNT_FOR_ARCH_DUMP_ADDR 0x4

#define SSHSRC_STOP_GATED_BIT 0

#define INVALID_CORE_STATE 0x1;
#define VALID_CORE_STATE_FOR_RAM 0x0;
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

#if 0
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
    o_coreState = VALID_CORE_STATE_FOR_RAM;
    FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, i_coreTgt, chipUnitNum);

    do
    {
        fapi2::buffer<uint64_t> ppm_ssh_buf = 0;
        fapiRc = getscom_abs_wrap (&i_coreTgt, C_PPM_SSHSRC, &ppm_ssh_buf());
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Failed to read SSHSRC for Core[%d]",
                    chipUnitNum);
            break;
        }
        //Check for Stop-gated (to check if corestate is valid)
        if (ppm_ssh_buf.getBit<SSHSRC_STOP_GATED_BIT>() == 1)
        {
            // CoreState may be 2/4/5/11
            ppm_ssh_buf.extractToRight<uint8_t>(o_coreState, 8, 4);
            // StopState Enabled, skip ramming,
            SBE_INFO(SBE_FUNC "StopGated Set for Core [%d] StopState[%d], "
                    "no ramming", chipUnitNum, o_coreState);
            break;
        }

        //Not Stop Gated
        // CoreState may be 0/1, we need to identify which one.
        // Only for coreState 0, we need to enable ramming.
        // Double check the core isn't in stop 1
        auto exTgt = i_coreTgt.getParent<fapi2::TARGET_TYPE_EX>();
        fapi2::buffer<uint64_t> sisr_buf = 0;
        fapiRc = getscom_abs_wrap (&exTgt, EX_CME_LCL_SISR_SCOM, &sisr_buf());
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Failed to read SISR for Core[%d]",
                    chipUnitNum);
            break;
        }
        uint32_t pos = chipUnitNum % 2;
        if (pos == 0 && sisr_buf.getBit<EX_CME_LCL_SISR_PM_STATE_ACTIVE_C0>())
        {
            sisr_buf.extractToRight<uint8_t>(o_coreState,
                    EX_CME_LCL_SISR_PM_STATE_C0,
                    EX_CME_LCL_SISR_PM_STATE_C0_LEN);
        }
        if (pos == 1 && sisr_buf.getBit<EX_CME_LCL_SISR_PM_STATE_ACTIVE_C1>())
        {
            sisr_buf.extractToRight<uint8_t>(o_coreState,
                    EX_CME_LCL_SISR_PM_STATE_C1,
                    EX_CME_LCL_SISR_PM_STATE_C1_LEN);
        }
        if(o_coreState == 0)
        {
            // Enable ramming for core state 0
            o_isRamming = true;
            SBE_INFO(SBE_FUNC "StopGated not set for Core[%d] StopState"
                    " [%d], so yes ramming", chipUnitNum,o_coreState);
        }
        else
        {
            SBE_INFO(SBE_FUNC "StopGated set for Core[%d] StopState"
                    " [%d], so no ramming", chipUnitNum, o_coreState);
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}
#endif
///////////////////////////////////////////////////////////////////////
// @brief sbeDumpArchRegs Dump out the architected registers
//
///////////////////////////////////////////////////////////////////////
ReturnCode sbeDumpArchRegs()
{
    #define SBE_FUNC " sbeDumpArchRegs "
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
#if 0
    uint64_t dumpAddr = 0;
    sbeArchRegDumpProcHdr_t dumpProcHdr = {};
    sbeArchRegDumpThreadHdr_t dumpThreadHdr = {};
    sbeArchRegDumpEntries_t dumpRegData = {};

    // Combined list of SPRs GPRs
    static const uint16_t SPR_GPR_list[] = {
        // List for SPRs to be collected in MPIPL path
        2001,2000, 2002, 8,    9,   815,  28,  1,   26,  27,  19,  18,
        314, 315,  307,  306,  152, 48,   144, 272, 273, 274, 275, 304,
        305, 153,  190,  176,  29,  349,  157, 128, 129, 130, 268, 849,
        22,  310,  309,  308,  317, 885,  336, 337, 313, 318, 319, 464,
        25,  180,  188,  17,   339, 1008, 338, 884, 921, 922, 256, 813,
        814, 186,  855,
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
            // Return failure
            SBE_ERROR(SBE_FUNC "Invalid Dump address from Stash [0x%08X %08X]",
                SBE::higher32BWord(dumpAddr), SBE::lower32BWord(dumpAddr));
            break;
        }
        SBE_INFO(SBE_FUNC "Stash Dump Addr = [0x%08X %08X] ",
            SBE::higher32BWord(dumpAddr), SBE::lower32BWord(dumpAddr));

        // Initialise the PBA with the above address from stash,
        // The access API would use it in auto-increment mode.
        p9_PBA_oper_flag pbaFlag;
        pbaFlag.setOperationType(p9_PBA_oper_flag::INJ);
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
        dumpProcHdr.reg_cnt = (sizeof(SPR_GPR_list)/sizeof(uint16_t));


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
            FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, procTgt, procGrpId);
            FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, procTgt,procChipId); 
            fapiRc = checkCoreStateForRam(coreTgt, doRamming, coreState);
            if( fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC "Failed in checkCoreStateForRam Core[%d]",
                    chipUnitNum);
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
                       //thread header to indicae the non-existence of register
                       //data.
                       dumpThreadHdr.coreState = INVALID_CORE_STATE;
                       doRamming = false;
                    }
                }

                //Send the thread header content
                fapiRc = PBAInterface.accessWithBuffer(&dumpThreadHdr,
                        sizeof(dumpThreadHdr),false);
                if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC "Failed to write thread header to hostboot");
                    break;
                }

                if(!doRamming)
                {
                    //Do not attempt to read the register data related to
                    //current core / Thread as the core status is not valid
                    continue;
                }

                uint32_t totalRegisters=(sizeof(SPR_GPR_list)/sizeof(uint16_t));
                // Loop over the combined list of registers
                for( uint32_t regIdx=0; regIdx < totalRegisters; regIdx++ )
                {
                    // Start filling up the rest of data structure
                    Enum_RegType type = REG_SPR;
                    dumpRegData.regType = SPR_REG_TYPE;
                    // Switch to GPRs once SPRs are over in the list
                    if(regIdx >= SPR_LIST_SIZE)
                    {
                        type = REG_GPR;
                        dumpRegData.regType = GPR_REG_TYPE;
                    }
                    dumpRegData.regNum = SPR_GPR_list[regIdx];
                    if((regIdx+1) == totalRegisters)
                    {
                        dumpRegData.isLastReg = true;
                    }
                    else
                    {
                        dumpRegData.isLastReg = false;
                    }

                    fapi2::buffer<uint64_t> data64;
                    SBE_EXEC_HWP(fapiRc, ramCore.get_reg, type,
                            SPR_GPR_list[regIdx], &data64, true)

                    if( fapiRc != FAPI2_RC_SUCCESS )
                    {
                        SBE_ERROR(SBE_FUNC" get_reg failed. threadNr:0x%x "
                                "coreChipletId:0x%02x, regNr:%u regType:%u",
                              thread, chipUnitNum, SPR_GPR_list[regIdx], type); 
                        dumpRegData.isRegDataValid = false;//Invalid data
                        dumpRegData.regVal = (uint64_t)fapiRc; //Save FAPI_RC
                        dumpRegData.isFfdcPresent = true; //Indicate FFDC pressence
                        dumpRegData.isLastReg = true;//No more registers fetched
                    }
                    else
                    {
                        dumpRegData.regVal = data64;
                        dumpRegData.isRegDataValid = true; //Valid Data
                        dumpRegData.isFfdcPresent = false; //No FFDC
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
                        /* 128 Bytes FAPI FFDC which will be added to the data
                         * shared with the hostboot  when accessing the
                         * SPR/GPR data fails.*/
                        uint32_t FFDCData[128] = {0xFF};

                        //Send the Register data to the Hostboot using PBA
                        fapiRc = PBAInterface.accessWithBuffer(
                                &FFDCData, sizeof(FFDCData),false);
                        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                        {
                            SBE_ERROR(SBE_FUNC "failed to write failure FFDC to hostboot");
                            break;
                        }

                        //Stop reading reset of the register data and exit out
                        //of register loop and continue for next thread.
                        break;
                    }

                }//End of Register loop

                if(fapiRc)
                {
                    break;
                }

                //Clean up the ram core setup
                SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_cleanup)
                if( fapiRc != FAPI2_RC_SUCCESS )
                {
                    SBE_ERROR(SBE_FUNC" ram_cleanup failed. threadNr:0x%02X"
                            " coreChipletId:0x%02X", thread, chipUnitNum);
                    // Don't break, continue for the next thread
                    continue;
                }

            } //End of Thread Loop
            if(fapiRc)
            {
                break;
            }
        }//End of Core loop

        // Just see that we are pushing the last PBA Frame here so as to flush
        // anything which is stuck before of a non-aligned frame.
        sbeArchRegDumpThreadHdr_t dump_dummy = {};
        fapiRc = PBAInterface.accessWithBuffer(&dump_dummy, sizeof(dump_dummy), true);
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "failed to write the last frame to hostboot");
            break;
        }

    }while(0);
    SBE_EXIT(SBE_FUNC);
#endif
    return fapiRc;
    #undef SBE_FUNC
}

