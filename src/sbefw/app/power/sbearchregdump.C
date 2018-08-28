/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbearchregdump.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018                             */
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
#include "p9_query_core_access_state.H"
#include "p9_ram_core.H"
#include "sbeMemAccessInterface.H"
#include "sbecmdcntlinst.H"
#include "p9_thread_control.H"
#include "p9_perv_scom_addresses.H"

#include "fapi2.H"

using namespace fapi2;

#define SPR_LIST_SIZE 63
#define GPR_LIST_SIZE 32
// This is defined in Hostboot src/include/usr/sbeio/sbeioif.H, this is just a
// copy, so that SBE refers to the same count.
#define STASH_KEY_CNT_FOR_ARCH_DUMP_ADDR 0x4

#define SSHSRC_STOP_GATED_BIT 0

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
    sbeArchRegDumpFormat_t dump = {};
    sbe_pir_t pir = {};

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

        // Go for each core under this Proc
        for(auto &coreTgt : procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            bool doRamming = false;
            uint8_t chipUnitNum = 0;
            uint8_t procGrpId = 0;
            uint8_t procChipId = 0;
            // Required for PIR calculation
            FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, coreTgt, chipUnitNum);
            FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_GROUP_ID, procTgt, procGrpId);
            FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_CHIP_ID, procTgt, procChipId);

            // Fetch the core state
            uint8_t coreState = 0;
            fapi2::buffer<uint64_t> ppm_ssh_buf = 0;
            fapiRc = getscom_abs_wrap (&coreTgt, C_PPM_SSHSRC, &ppm_ssh_buf());
            if( fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC "Failed to read SSHSRC for Core[%d]",
                    chipUnitNum);
                continue;
            }
            //Check for Stop-gated (to check if corestate is valid)
            if (ppm_ssh_buf.getBit<SSHSRC_STOP_GATED_BIT>() == 1)
            {
                // CoreState may be 2/4/5/11
                ppm_ssh_buf.extractToRight<uint8_t>(coreState, 8, 4);
                // StopState Enabled, skip ramming, but need to send out
                // holes inplace of register/values.
                SBE_INFO(SBE_FUNC "StopGated Set for Core [%d] StopState[%d], "
                    "no ramming", chipUnitNum, coreState);
            }
            else // not stop gated
            {
                // CoreState may be 0/1, we need to identify which one.
                // Only for coreState 0, we need to enable ramming.
                // Double check the core isn't in stop 1
                auto exTgt = coreTgt.getParent<fapi2::TARGET_TYPE_EX>();
                fapi2::buffer<uint64_t> sisr_buf = 0;
                fapiRc = getscom_abs_wrap (&exTgt, EX_CME_LCL_SISR_SCOM, &sisr_buf());
                if( fapiRc != FAPI2_RC_SUCCESS )
                {
                    SBE_ERROR(SBE_FUNC "Failed to read SISR for Core[%d]",
                        chipUnitNum);
                    continue;
                }
                uint32_t pos = chipUnitNum % 2;
                if (pos == 0 && sisr_buf.getBit<EX_CME_LCL_SISR_PM_STATE_ACTIVE_C0>())
                {
                    sisr_buf.extractToRight<uint8_t>(coreState,
                                               EX_CME_LCL_SISR_PM_STATE_C0,
                                               EX_CME_LCL_SISR_PM_STATE_C0_LEN);
                }
                if (pos == 1 && sisr_buf.getBit<EX_CME_LCL_SISR_PM_STATE_ACTIVE_C1>())
                {
                    sisr_buf.extractToRight<uint8_t>(coreState,
                                               EX_CME_LCL_SISR_PM_STATE_C1,
                                               EX_CME_LCL_SISR_PM_STATE_C1_LEN);
                }
                if(coreState == 0)
                {
                    // Enable ramming for core state 0
                    doRamming = true;
                    SBE_INFO(SBE_FUNC "StopGated not set for Core[%d] StopState"
                        " [%d], so yes ramming", chipUnitNum);
                }
                else
                {
                    SBE_INFO(SBE_FUNC "StopGated not set for Core[%d] StopState"
                        " [%d], so no ramming", chipUnitNum, coreState);
                }
            }

            for(uint8_t thread = SMT4_THREAD0; thread < SMT4_THREAD_MAX; thread++)
            {
                RamCore ramCore( coreTgt, thread );
                if(doRamming == true)
                {
                    SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_setup)
                    if( fapiRc != FAPI2_RC_SUCCESS )
                    {
                        SBE_ERROR(SBE_FUNC" ram_setup failed. threadNr:0x%2X"
                            "coreChipletId:0x%02X Proc:0x%02X",
                            thread, chipUnitNum, procChipId);
                            // Skip this thread since ram setup failed, try
                            // to get data for the Next Thread, but what
                            // about this fapiRc
                            continue;
                    }
                }
                // If setup passes, then go for get_reg()
                Enum_RegType type = REG_SPR;
                // Construct PIR for the thread
                pir.procGrpId = procGrpId;
                pir.procChipId = procChipId;
                pir.chipUnitNum = chipUnitNum;
                pir.thread = thread;

                // Loop over the combined list of registers
                for( uint32_t regIdx=0; regIdx<(sizeof(SPR_GPR_list)/sizeof(uint16_t)); regIdx++ )
                {
                    // Switch to GPRs once SPRs are over in the list
                    if(regIdx >= SPR_LIST_SIZE)
                    {
                        type = REG_GPR;
                    }
                    // Start filling up the rest of data structure
                    dump.pir = pir;
                    dump.coreState = coreState;
                    dump.regNum = SPR_GPR_list[regIdx];
                    dump.regVal = 0;

                    if(doRamming == true)
                    {
                        fapi2::buffer<uint64_t> data64;
                        SBE_EXEC_HWP(fapiRc, ramCore.get_reg, type,
                                SPR_GPR_list[regIdx], &data64, true)
                        if( fapiRc != FAPI2_RC_SUCCESS )
                        {
                            SBE_ERROR(SBE_FUNC" get_reg failed. threadNr:0x%x "
                                "coreChipletId:0x%02x, regNr:%u regType:%u",
                                thread, chipUnitNum, SPR_GPR_list[regIdx], type);
                            // If get_reg fails,we need to indicate hostboot
                            // that data fetch failed, use this signature
                            dump.regVal = 0xDEADBEEFDEADBEEFULL;
                        }
                        else
                        {
                            dump.regVal = data64;
                        }
                    }

                    // PBA it to the stash address
                    fapiRc = PBAInterface.accessWithBuffer(
                                                &dump,
                                                sizeof(dump),
                                                false);
                    if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
                    {
                        SBE_ERROR(SBE_FUNC "failed in writing to hostboot");
                        break;
                    }
                }
                if(fapiRc)
                {
                    break;
                }
                // HWP team does not care about cleanup for failure case.i
                // So call cleaup only for success case.
                // Clean up the ram core setup
                if(doRamming == true)
                {
                    SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_cleanup)
                    if( fapiRc != FAPI2_RC_SUCCESS )
                    {
                        SBE_ERROR(SBE_FUNC" ram_cleanup failed. threadNr:0x%02X"
                            " coreChipletId:0x%02X", thread, chipUnitNum);
                        // Don't break, continue for the next thread
                    }
                }
            }
            if(fapiRc)
            {
                break;
            }
        }
        // Just see that we are pushing the last PBA Frame here so as to flush
        // anything which is stuck before of a non-aligned frame.
        sbeArchRegDumpFormat_t dump_dummy = {};
        fapiRc = PBAInterface.accessWithBuffer(&dump_dummy, sizeof(dump_dummy), true);
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "failed to write the last frame to hostboot");
            break;
        }
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

