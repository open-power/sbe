/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdmpipl.C $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
/*
 * @file: ppe/src/sbefw/sbecmdmpipl.C
 *
 * @brief This file contains the SBE MPIPL chipOps
 *
 */

#include "sbefifo.H"
#include "sbeSpMsg.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbecmdmpipl.H"
#include "sberegaccess.H"
#include "sbefapiutil.H"
#include "sbecmdiplcontrol.H"
#include "fapi2_mem_access.H"
#include "p10_getmempba.H"
#include "sbeglobals.H"
#include "sbehandleresponse.H"
#include "p10_stopclocks.H"
#include "p10_hcd_core_stopclocks.H"
#include "p10_hcd_core_stopgrid.H"
#include <p10_hcd_mma_stopclocks.H>
#include "p10_hcd_eq_stopclocks.H"
#include "p10_hcd_cache_stopclocks.H"
#include "p10_hcd_cache_stopgrid.H"
#include "sbearchregdump.H"
#include "fapi2.H"
#include "core/ipl.H"
#include "sberegaccess.H"
#include "chipop_handler.H"
#include "sbearchregdump.H"
#include "sbeCollectDump.H"
#include "sbecmdcntlinst.H"
#include "p10_scom_pibms.H"
#include "p10_sbe_sync_quiesce_states.H"
#include "p10_scom_proc_9.H"

using namespace fapi2;

// Defines for stop clock
#define SBE_IS_EX0(chipletId) \
    (!(((chipletId - CORE_CHIPLET_OFFSET) & 0x0002) >> 1))

#ifdef __SBEFW_SEEPROM__

#ifdef SEEPROM_IMAGE
// Using function pointer to force long call.
p10_hcd_cache_stopclocks_FP_t p10_hcd_cache_stopclocks_hwp = &p10_hcd_cache_stopclocks;
p10_hcd_cache_stopgrid_FP_t p10_hcd_cache_stopgrid_hwp = &p10_hcd_cache_stopgrid;
p10_hcd_eq_stopclocks_FP_t p10_hcd_eq_stopclocks_hwp = &p10_hcd_eq_stopclocks;
p10_hcd_core_stopclocks_FP_t p10_hcd_core_stopclocks_hwp = &p10_hcd_core_stopclocks;
p10_hcd_core_stopgrid_FP_t p10_hcd_core_stopgrid_hwp = &p10_hcd_core_stopgrid;
p10_hcd_core_stopclocks_FP_t p10_hcd_mma_stopclocks_hwp = &p10_hcd_mma_stopclocks;
p10_stopclocks_FP_t p10_stopclocks_hwp = &p10_stopclocks;
p10_adu_utils_reset_adu_FP_t p10_adu_utils_reset_adu_hwp = &p10_adu_utils_reset_adu;
p10_adu_utils_cleanup_adu_FP_t p10_adu_utils_cleanup_adu_hwp = &p10_adu_utils_cleanup_adu;
#endif

fapi2::ReturnCode sbeSyncWithAllSbeUsingLQA(bool i_ignoreAduAddrError, uint8_t& o_quiesced_status);

static const uint32_t SBE_ISTEP_MPIPL_START         = 96;
static const uint32_t MPIPL_START_MAX_SUBSTEPS      = 9;
static const uint32_t SBE_ISTEP_MPIPL_CONTINUE      = 97;
static const uint32_t MPIPL_CONTINUE_MAX_SUBSTEPS   = 7;
static const uint32_t SBE_ISTEP4                    = 4;
static const uint32_t SBE_ISTEP5                    = 5;
static const uint32_t ISTEP_MINOR_START             = 1;
static const uint32_t ISTEP4_MAX_SUBSTEPS           = 20;
static const uint32_t ISTEP5_MAX_SUBSTEPS           = 3;

ReturnCode startMpiplIstepsExecute(void)
{
    #define SBE_FUNC " startMpiplIstepsExecute "
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;

    uint32_t minor = 1;
    do
    {
        fapiRc = sbeExecuteIstep(SBE_ISTEP_MPIPL_START, minor);
        bool checkstop = isSystemCheckstop();
        if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
        {
            SBE_ERROR(SBE_FUNC "Failed in StartMpipl Minor Isteps[%d],"
                               " SystemCheckstop=%d", minor,checkstop);
            break;
        }
        ++minor;
    }while(minor<=MPIPL_START_MAX_SUBSTEPS);

    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

static const uint8_t g_continuempipl_isteps[3][3] = {
    // Major Num,              Minor Start,       Minor End
    {SBE_ISTEP_MPIPL_CONTINUE, ISTEP_MINOR_START, MPIPL_CONTINUE_MAX_SUBSTEPS},
    {SBE_ISTEP4,               ISTEP_MINOR_START, ISTEP4_MAX_SUBSTEPS},
    {SBE_ISTEP5,               ISTEP_MINOR_START, ISTEP5_MAX_SUBSTEPS}};


ReturnCode continueMpiplIstepsExecute(const sbeRole i_sbeRole)
{
    #define SBE_FUNC " continueMpiplIstepsExecute "
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;

    // Loop through isteps
    for( auto istep : g_continuempipl_isteps )
    {
        bool checkstop = false;
        for(uint8_t minor = istep[1]; minor <= istep[2]; minor++)
        {
            fapiRc = sbeExecuteIstep(istep[0], minor);
            checkstop = isSystemCheckstop();
            if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
            {
                SBE_ERROR(SBE_FUNC "Failed in Master ContinueMpipl Isteps "
                    "Major[%d] Minor[%d]", istep[0], minor);
                break;
            }
        }
        if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
        {
            break;
        }
        if(i_sbeRole == SBE_ROLE_SLAVE)
        {
            (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                    SBE_RUNTIME_EVENT);
            break;
        }
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief Collect HW Dump contents in MPIPL path
///////////////////////////////////////////////////////////////////////
ReturnCode collectMPIPLHWDumpEntries()
{
#define SBE_FUNC " collectMPIPLHWDumpEntries"
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint64_t HWDumpAllocAddr = 0;
    uint64_t HWDumpAllocSize = 0;
    bool stopClockReq = true;

    do
    {
        uint64_t metaDataAddr = 0;
        //Fetch the MetaData related to the MPIPL
        metaDataAddr = sbeFetchRegDumpAddrFromStash();
        if(metaDataAddr == 0xFFFFFFFFFFFFFFFFULL || metaDataAddr == 0)
        {
            SBE_ERROR(SBE_FUNC "Invalid Metadata address from Stash [0x%08X %08X]",
                    SBE::higher32BWord(metaDataAddr),SBE::lower32BWord(metaDataAddr));
            break;
        }
        SBE_INFO(SBE_FUNC "Metadata Address = [0x%08X %08X] ",
                SBE::higher32BWord(metaDataAddr), SBE::lower32BWord(metaDataAddr));

        //Read metadata and fetch the address to collect HW dump contents
        sbeArchHWDumpMetaData_t metadata;
        {//Keeping below memory access object to limited scope
            p10_PBA_oper_flag pbaFlag;
            pbaFlag.setOperationType(p10_PBA_oper_flag::INJ);
            sbeMemAccessInterface PBAInterForMetadata(
                    SBE_MEM_ACCESS_PBA,
                    metaDataAddr, 
                    &pbaFlag,
                    SBE_MEM_ACCESS_READ,
                    sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);

            fapiRc = PBAInterForMetadata.accessWithBuffer(&metadata,
                    sizeof(sbeArchHWDumpMetaData_t),true);
        }
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed to fetch the Dump metadata at the offset:0x%.8x%.8x!!",
                    SBE::higher32BWord(metaDataAddr), SBE::lower32BWord(metaDataAddr));
            break;
        }

        HWDumpAllocAddr = metadata.hwDataMemoryAddr;
        HWDumpAllocSize = metadata.hwDataMemAllocSize;
        SBE_INFO(SBE_FUNC "PHYP allocated HW Dump adress=0x%.8x%.8x and Size=0x%.8x%.8x",
                SBE::higher32BWord(HWDumpAllocAddr),SBE::lower32BWord(HWDumpAllocAddr),
                SBE::higher32BWord(HWDumpAllocSize),SBE::lower32BWord(HWDumpAllocSize));

        if(!metadata.canCollectHwDumpData())
        {
            SBE_ERROR(SBE_FUNC "PHYP has not allocated memory for collection of HW Dump Data!"
                      "Collection of HW dump content is skipped,clocks will be stopped");
            break;
        }
        //Remove the 0th Bit while accessing the address sent by PHYP
        HWDumpAllocAddr = HWDumpAllocAddr & 0x7FFFFFFFFFFFFFFF;
        uint32_t bytesCollected = 0;
        //Skip stopping of the clocks as collectMpiplHwDump() will do it
        //internally
        uint32_t rc = collectMpiplHwDump(HWDumpAllocAddr,bytesCollected);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC "Failed in collectMpiplHwDump().MPIPL will continue");
            break;
        }
        stopClockReq = false; //HDCT would have taken care of stopping required clocks
        SBE_INFO(SBE_FUNC "MURU:Bytes collected=0x%.8x",bytesCollected);
        metadata.hwDataMemCapturedSize = bytesCollected;

        //Update the Metadata
        {//Keeping below memory access object to limited scope
            p10_PBA_oper_flag pbaFlag;
            pbaFlag.setOperationType(p10_PBA_oper_flag::INJ);
            sbeMemAccessInterface PBAInterForMetadata(
                    SBE_MEM_ACCESS_PBA,
                    metaDataAddr, 
                    &pbaFlag,
                    SBE_MEM_ACCESS_WRITE,
                    sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);

            fapiRc = PBAInterForMetadata.accessWithBuffer(&metadata,
                    sizeof(sbeArchHWDumpMetaData_t),true);
        }
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed to update the Dump metadata at the offset:0x%.8x%.8x!!",
                    SBE::higher32BWord(metaDataAddr), SBE::lower32BWord(metaDataAddr));
            break;
        }

    }while(0);

    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Internal Error in collectMPIPLHWDumpEntries() MPIPL will continue!");
        fapiRc = FAPI2_RC_SUCCESS;
    }

    //Stop the Clocks if clocks where not stopped by collectMpiplHwDump()
    if(stopClockReq)
    {
        SBE_INFO(SBE_FUNC "Clocks not stopped by collectMpiplHwDump(),Attempt Stop "
                "clocks for all Core and cache, for MPIPL to continue");
        fapiRc = stopClockS0();
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in Core/Cache StopClock");
        }
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
#undef SBE_FUNC

}


///////////////////////////////////////////////////////////////////////
// @brief sbeEnterMpipl Sbe enter MPIPL function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeEnterMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeEnterMpipl "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;

    uint32_t len = 0;
    sbeResponseFfdc_t ffdc;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    size_t timeOut = SBE_DELAY_2SEC_TIMEOUT_LOOP;
    bool ignoreAddrError = false;
    uint8_t status = 0;

    do
    {
        if (!SBE::isMpiplReset())
        {
            // Dequeue the EOT entry as no more data is expected.
            rc = sbeUpFifoDeq_mult (len, NULL);
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

            // Stop all the instructions before going into Mpipl reset path.
            // Phyp might be performing some operation on TPM Sequencer.
            rc = stopAllCoreInstructions();
            if(rc)
            {
                SBE_ERROR(SBE_FUNC "Stop all core instructions failed before"
                    " Mpipl Reset, RC=[0x%08X]", rc);
                break;
            }
            ignoreAddrError = false;
            status = 0;
            // Perform Sync up with other SBEs in the system
            rc = sbeSyncWithAllSbeUsingLQA(ignoreAddrError, status);
            if(!status || rc)
            {
                SBE_ERROR(SBE_FUNC "p10_sbe_sync_quiesce_states LQA SBE System Quiesce "
                    "failed, Either System Quiesce Achieved not true or procedure failed "
                    "before Mpipl Reset, RC=[0x%08X] Status[0x%02X]", rc, status);

                SBE_ERROR(SBE_FUNC "Forced the System Checkstop to indicate sync_quiesce "
                    "failure before Mpipl Reset !!");

                //Force System Checkstop
                fapi2::buffer<uint64_t> data(0);
                data.setBit<scomt::proc::TP_TCN1_N1_LOCAL_FIR_IN57>();
                rc = putscom_abs_wrap(&procTgt, scomt::proc::TP_TCN1_N1_LOCAL_FIR_WO_OR, data());
                if(rc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR("Failed to force system checkstop to indicate failure in"
                              "p10_sbe_sync_quiesce_states HWP before Mpipl Reset");
                }
                break;
            }

            // Hard-Reset SPI TPM engine.
            SBE::hardResetTPMSpiEngine();

            // Create MPIPL Reset request for the Otprom to execute
            // Set bit 14 in 0xc0002040 and jump to otprom addr 0x18040
            SBE::setMpiplReset();

            // one or more SBEs may run faster and can go to the xscom disable in
            // measurement path which will block the above sync path on other SBEs.
            // This timeout will give enough time to other SBEs to finish the sync.
            timeOut = SBE_DELAY_2SEC_TIMEOUT_LOOP;
            while(timeOut)
            {
                fapi2::delay(SBE_DELAY_HW_1MSEC, SBE_DELAY_SIM_CYCLES);
                timeOut--;
            }
            SBE::runSystemReset();
            // There is no execution after this.. SBE is taking a reset.
        }

        // Call the Sync again here before you execute mpipl procedure.
        ignoreAddrError = true;
        status = 0;
        SBE_INFO(SBE_FUNC " LQA SBE System Quiesce After Mpipl Reset before Procedures - Starting");
        rc = sbeSyncWithAllSbeUsingLQA(ignoreAddrError, status);
        if(!status || rc)
        {
            SBE_ERROR(SBE_FUNC "p10_sbe_sync_quiesce_states LQA SBE System Quiesce "
                    "failed, Either System Quiesce Achieved not true or procedure failed "
                    "After Mpipl Reset before Procedures, RC=[0x%08X] Status[0x%02X]", rc, status);

            SBE_ERROR(SBE_FUNC "Forced the System Checkstop to indicate sync_quiesce "
                    "failure after Mpipl Reset !!");

            //Force System Checkstop
            fapi2::buffer<uint64_t> data(0);
            data.setBit<scomt::proc::TP_TCN1_N1_LOCAL_FIR_IN57>();
            rc = putscom_abs_wrap(&procTgt, scomt::proc::TP_TCN1_N1_LOCAL_FIR_WO_OR, data());
            if(rc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed to force system checkstop to indicate failure in"
                        "p10_sbe_sync_quiesce_states HWP After Mpipl Reset before Procedures");
            }
            break;
        }

        SBE_INFO(SBE_FUNC " Starting Mpipl Procedures");
        fapiRc = startMpiplIstepsExecute();
        bool checkstop = isSystemCheckstop();
        if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
        {
            SBE_ERROR(SBE_FUNC "Failed in Mpipl Start in ChipOp Mode,"
                               " System Checkstop=%d",checkstop);
            if(checkstop)
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_SYSTEM_CHECKSTOP);
                //If we have Valid FAPI RC
                if(fapiRc!=FAPI2_RC_SUCCESS)
                {
                    ffdc.setRc(fapiRc);
                }
            }
            else
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_ENTER_MPIPL_FAILED);
                ffdc.setRc(fapiRc);
            }
            // reset attribute. We do not want to reset register, so do not
            // use setMpIplMode
            uint8_t isMpipl = 0;
            PLAT_ATTR_INIT(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), isMpipl);
            break;
        }

        //HW Dump related HDCT entries. Below interface also takes care of
        //stopping the clocks
        fapiRc = collectMPIPLHWDumpEntries();
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR("Failed in collectMPIPLHWDumpEntries(),cannot continue MPIPL!");
            break;
        }

        //WORKAROUND:Force TOD RUNNING status to be OFF
        //TODO:Remove this once TOD procedure is updated.
        fapiRc = putscom_abs_wrap(&procTgt, 0x40024, 0x1000000000000000);
        if(fapiRc)
        {
            SBE_ERROR("Failed in applying TOD workaround in MPIPL Path");
            break;
        }
        SBE_INFO("Workaround applied : TOD Status Forced to be OFF");

    }while(0);

    // Create the Response to caller
    do
    {
        // Clear MPIPL Reset. System is ready for sbeEnterMpipl Chip-Op call
        SBE::clearMpiplReset();

        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        rc = sbeDsSendRespHdr(respHdr, &ffdc);
        if(SBE::isMpiplResetDone())
        {
            sbeHandleFifoResponse (rc, SBE_FIFO);
            SBE_GLOBAL->sbeIntrSource.clearIntrSource(SBE_ALL_HANDLER,SBE_INTERFACE_FIFO);
            pk_irq_enable(SBE_IRQ_SBEFIFO_DATA);
            pk_irq_enable(SBE_IRQ_SBEFIFO_RESET);
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeContinueMpipl Sbe Continue MPIPL function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeContinueMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeContinueMpipl "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeFifoType type;
    sbeResponseFfdc_t ffdc;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Refresh SBE role and proc chip mem attribute
        // to handle failover of FSP and with it the switchover of
        // master proc SBE
        fapi2::plat_AttrInit();
        SbeRegAccess::theSbeRegAccess().init(true);

        g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeMaster() ?
                    SBE_ROLE_MASTER : SBE_ROLE_SLAVE;

        fapiRc = continueMpiplIstepsExecute(g_sbeRole);
        bool checkstop = isSystemCheckstop();
        if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
        {
            SBE_ERROR(SBE_FUNC "Failed in Continue Mpipl in ChipOp Mode, "
            "SBE Role[%d],System Checkstop=%d", g_sbeRole,checkstop);
            if(checkstop)
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     SBE_SEC_SYSTEM_CHECKSTOP);
                //If FAPIRC is valid
                if(fapiRc != FAPI2_RC_SUCCESS)
                {
                    ffdc.setRc(fapiRc);
                }
            }
            else
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                        SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                ffdc.setRc(fapiRc);
                // Async Response to be stored
            }
            break;
        }
    }while(0);

    //Clear the MPIPL attribute for secondary SBEs. For the primary SBE
    //this attribute will be cleared post deadman timer chipOp.
    if(g_sbeRole == SBE_ROLE_SLAVE)
    {
        uint8_t isMpipl = 0;
        PLAT_ATTR_INIT(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), isMpipl);
    }

    // Create the Response to caller
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if(SBE_SEC_OPERATION_SUCCESSFUL == rc)
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc,type);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

#ifdef _S0_
///////////////////////////////////////////////////////////////////////
// @brief stopClockS0 Sbe StopClock S0 interface function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
ReturnCode stopClockS0()
{
#define SBE_FUNC "stopClockS0"
    SBE_ENTER(SBE_FUNC);
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    p10_stopclocks_flags flags;
    flags.clearAll();
    flags.stop_core_clks = true; 
    flags.stop_cache_clks = true;
    
    SBE_EXEC_HWP(fapiRc, p10_stopclocks_hwp,
                 plat_getChipTarget(), flags);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Failed in stopClockS0():,fapiRc=0x%.8x",fapiRc);
    }
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
#undef SBE_FUNC
}
#endif
///////////////////////////////////////////////////////////////////////
/* @brief Deduce the type of stop clock procedure to call based on
 *        target and chiplet id combination
 *
 * @param[in] i_targetType  SBE chip-op target type
 * @param[in] i_chipletId   Chiplet id
 * @return Bitmapped stopClockHWPType enum values
 */
///////////////////////////////////////////////////////////////////////
static inline uint32_t getStopClockHWPType(uint32_t i_targetType,
                                           uint32_t i_chipletId)
{
    uint32_t rc = SC_NONE;
    TargetType fapiTarget = sbeGetFapiTargetType(
                                                i_targetType,
                                                i_chipletId);

    if((fapiTarget == TARGET_TYPE_PROC_CHIP) ||
       (fapiTarget == TARGET_TYPE_PERV))
    {
        rc |= SC_PROC;
    }
    if((fapiTarget == TARGET_TYPE_CORE) && (i_targetType == TARGET_CORE))
    {
        rc |= SC_CORE;
    }
    if((fapiTarget == TARGET_TYPE_CORE) && (i_targetType == TARGET_CACHE))
    {
        rc |= SC_CACHE;
    }
    if(fapiTarget == TARGET_TYPE_EQ)
    {
        rc |= SC_EQ;
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
/* @brief Prepare Stop clock flags base on Target Type
 *
 * @param[in] i_targetType  SBE chip-op target Type
 * @param[in] i_chipletId  Target chiplet
 *
 * @return p9_stopclocks_flags
 */
///////////////////////////////////////////////////////////////////////
static inline p10_stopclocks_flags getStopClocksFlags(uint32_t i_targetType,
                                                      uint32_t i_chipletId)
{
    p10_stopclocks_flags flags;

    do
    {
        if(i_targetType == TARGET_PROC_CHIP)
        {
            // We need the default Proc Flags
            SBE_INFO(SBE_FUNC "In Proc Target");
            break;
        }

        // Clear default flags - only in case the target is not PROC_CHIP
        // Otherwise, for a PROC_CHIP target, we want to keep default flags
        flags.clearAll();

        if(i_targetType == TARGET_PERV)
        {
            if((i_chipletId >= NEST_CHIPLET_OFFSET) &&
               (i_chipletId < (NEST_CHIPLET_OFFSET + NEST_TARGET_COUNT)))
            {
                flags.stop_nest_clks = true;
            }
            else if((i_chipletId >= PEC_CHIPLET_OFFSET) &&
                    (i_chipletId < (PEC_CHIPLET_OFFSET + PEC_TARGET_COUNT)))
            {
                flags.stop_pcie_clks = true;
            }
            else if((i_chipletId >= MC_CHIPLET_OFFSET) &&
                i_chipletId < (MC_CHIPLET_OFFSET + MC_TARGET_COUNT))
            {
                flags.stop_mc_clks = true;
            }
            else if((i_chipletId >= PAUC_CHIPLET_OFFSET) &&
                    (i_chipletId < (PAUC_CHIPLET_OFFSET + PAUC_TARGET_COUNT)))
            {
                flags.stop_pau_clks = true;
            }
            else if((i_chipletId >= IOHS_CHIPLET_OFFSET) &&
                    (i_chipletId < (IOHS_CHIPLET_OFFSET + IOHS_TARGET_COUNT)))
            {
                flags.stop_axon_clks = true;
            }
            else
            {
                SBE_ERROR( SBE_FUNC "Unsupported Perv TargetType=[0x%08X] "
                    "Chiplet Id=[0x%08X]",i_targetType, i_chipletId);
            }
            break;
        }
        else if(i_targetType == TARGET_CORE)
        {
            // Keep only core flag as true
            flags.stop_core_clks = true;
        }
        else if(i_targetType == TARGET_EQ)
        {
            // Keep only cache flag as true
            flags.stop_cache_clks = true;
        }
        else
        {
            SBE_ERROR( SBE_FUNC "Unsupported TargetType=[0x%08X] "
                "ChipletId=[0x%08X]",i_targetType, i_chipletId);
        }
    }while(0);
    return flags;
}

///////////////////////////////////////////////////////////////////////
// @brief sbeStopClocks_Wrap Sbe Stop Clocks function
// @param[in]  i_getStream      up-stream fifo for chip-op /
//                              memory interface for dump
// @param[in]  i_putStream      down-stream fifo for chip-op /
//                              memory interface for dump
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeStopClocks_Wrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                            fapi2::sbefifo_hwp_data_ostream& i_putStream )
{
    #define SBE_FUNC " sbeStopClocks_Wrap"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeResponseFfdc_t ffdc;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeStopClocksReqMsgHdr_t reqMsg = {0};

    do
    {
        // Get the TargetType and ChipletId from the command message
        len  = sizeof(sbeStopClocksReqMsgHdr_t)/sizeof(uint32_t);
        rc = i_getStream.get(len, (uint32_t *)&reqMsg); // EOT fetch
        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_INFO(SBE_FUNC "TargetType 0x%04X ChipletId 0x%02X",
                    (uint16_t)reqMsg.targetType,
                    (uint8_t)reqMsg.chipletId);

        fapi2::plat_target_handle_t tgtHndl;
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
        // Get the type of stopclocks procedure to call
        // based on target and chiplet id
        uint32_t hwpType = getStopClockHWPType(reqMsg.targetType,
                                               reqMsg.chipletId);

        // Bad input
        if(hwpType == SC_NONE)
        {
            // Error in target and chiplet id combination
            SBE_ERROR(SBE_FUNC "Invalid TargetType[0x%04X] ChipletId[0x%02X]",
            (uint32_t)reqMsg.targetType, (uint32_t)reqMsg.chipletId);
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;
        }

        // All Eq/All Cache/All Core/All & Perv & Proc are handled here
        if(hwpType & SC_PROC)
        {
            SBE_INFO(SBE_FUNC "Calling p10_stopclocks HWP with Proc Target FLag ALL");
            p10_stopclocks_flags flags = getStopClocksFlags(reqMsg.targetType, reqMsg.chipletId);
            SBE_EXEC_HWP(fapiRc, p10_stopclocks_hwp, proc, flags);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_stopclocks(), Proc, Flag ALL, fapiRc=0x%.8x",fapiRc);
                break;
            }
        }
        // Specific CORE or all Cores
        // p10_hcd_core_stopclocks() Stops CORE+L2 clocks. Does not alter the L3
        // and MMA.
        if(hwpType & SC_CORE)
        {
            SBE_INFO(SBE_FUNC " Calling p10_hcd_core_stopclocks, Core");
            if(reqMsg.chipletId == SMT4_ALL_CORES)
            {
                //Request is for All cores , create a multicast target and call
                //the procedure.
                fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,fapi2::MULTICAST_OR> mc_cores;
                mc_cores = proc.getMulticast<fapi2::MULTICAST_OR>(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
                tgtHndl = mc_cores;
            }
            else //Specific core Instance number
            {
                sbeGetFapiTargetHandle(reqMsg.targetType,reqMsg.chipletId,tgtHndl);
            }
            //Execute the Core Stop Clock HWP
            SBE_EXEC_HWP(fapiRc, p10_hcd_core_stopclocks_hwp, tgtHndl);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_hcd_core_stopclocks(), fapiRc=0x%.8x, Target=0x%.8x",
                          fapiRc,tgtHndl);
                break;
            }
            //Execute the Core Stop Grid HWP
            SBE_EXEC_HWP(fapiRc, p10_hcd_core_stopgrid_hwp, tgtHndl);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_hcd_core_stopgrid(), fapiRc=0x%.8x, Target=0x%.8x",
                          fapiRc,tgtHndl);
                break;
            }

            //Execute the MMA Stop Clock HWP
            SBE_EXEC_HWP(fapiRc, p10_hcd_mma_stopclocks_hwp, tgtHndl);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_hcd_mma_stopclocks(), fapiRc=0x%.8x, Target=0x%.8x",
                          fapiRc,tgtHndl);
                break;
            }
        }
        // Specific EQ or All EQs
        // p10_hcd_eq_stopclocks: ECL2, L3 and MMA clocks are stopped.
        if(hwpType & SC_EQ)
        {
            SBE_INFO(SBE_FUNC " Calling p10_hcd_eq_stopclocks");
            if(reqMsg.chipletId == EQ_ALL_CHIPLETS)
            {
                //Request is for All EQs , create a multicast target and call
                //the procedure.
                fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST,fapi2::MULTICAST_AND> mc_eqs;
                mc_eqs = proc.getMulticast<fapi2::TARGET_TYPE_EQ>(fapi2::MCGROUP_GOOD_EQ);
                tgtHndl = mc_eqs;
            }
            else //Specific eq chiplet
            {
                sbeGetFapiTargetHandle(reqMsg.targetType,reqMsg.chipletId,tgtHndl);
            }
            //Execute the EQ Stop Clock procedure
            SBE_EXEC_HWP(fapiRc, p10_hcd_eq_stopclocks_hwp, tgtHndl);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_hcd_eq_stopclocks(), fapiRc=0x%.8x,Target=0x%.8x",
                           fapiRc,tgtHndl);
                break;
            }
        }

        // Specific CACHE or all Caches
        // p10_hcd_cache_stopclocks() Stops Core L3 clocks
        if(hwpType & SC_CACHE)
        {
            SBE_INFO(SBE_FUNC " Calling p10_hcd_cache_stopclocks, Core");
            if(reqMsg.chipletId == SMT4_ALL_CORES)
            {
                //Request is for All cores , create a multicast target and call
                //the procedure.
                fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,fapi2::MULTICAST_OR> mc_cores;
                mc_cores = proc.getMulticast<fapi2::MULTICAST_OR>(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
                tgtHndl = mc_cores;
            }
            else //Specific core Instance number
            {
                sbeGetFapiTargetHandle(reqMsg.targetType,reqMsg.chipletId,tgtHndl);
            }
            //Execute the Cache Stop Clock HWP
            SBE_EXEC_HWP(fapiRc, p10_hcd_cache_stopclocks_hwp, tgtHndl);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_hcd_cache_stopclocks(), fapiRc=0x%.8x, Target=0x%.8x",
                          fapiRc,tgtHndl);
                break;
            }
            //Execute the Cache Stop Grid HWP
            SBE_EXEC_HWP(fapiRc, p10_hcd_cache_stopgrid_hwp, tgtHndl);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("Failed in p10_hcd_cache_stopgrid(), fapiRc=0x%.8x, Target=0x%.8x",
                          fapiRc,tgtHndl);
                break;
            }
        }

    }while(0);

    if( fapiRc != FAPI2_RC_SUCCESS )
    {
        SBE_ERROR(SBE_FUNC" Stopclocks failed for TargetType [0x%04X] "
                "ChipletId [0x%02X]", (uint16_t)reqMsg.targetType,
                (uint8_t)reqMsg.chipletId);
        respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                           SBE_SEC_STOP_CLOCK_FAILED );
        ffdc.setRc(fapiRc);
    }

    // Create the Response to caller
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if(i_putStream.isStreamRespHeader(respHdr.rcStatus(),ffdc.getRc()))
    {
        if(SBE_SEC_OPERATION_SUCCESSFUL == rc)
        {
            rc = sbeDsSendRespHdr( respHdr, &ffdc, i_getStream.getFifoType());
        }
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeStopClocks Sbe Stop Clocks function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeStopClocks(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeStopClocks"
    SBE_ENTER(SBE_FUNC);

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);

    fapi2::sbefifo_hwp_data_ostream ostream(type);
    fapi2::sbefifo_hwp_data_istream istream(type);
    rc = sbeStopClocks_Wrap (istream,ostream);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetTIInfo (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetTIInfo "
    SBE_ENTER(SBE_FUNC);

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    uint32_t flags = fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA |
                     fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON |
                     fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE;
    uint32_t bytesRead = 0;
    do
    {
        //Will attempt to dequeue for the expected EOT entry at the end.
        uint32_t len2dequeue  = 0;
        rc = sbeUpFifoDeq_mult (len2dequeue, NULL);

        // If FIFO access failure
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }
        //Read the Scratch register to get the TI data location.
        //First get the master core for the proc. The scratch reg is
        //for the master core.
        uint8_t coreId = 0;
        Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,procTgt,coreId);
        fapi2::Target<fapi2::TARGET_TYPE_CORE >
        coreTgt(plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(coreId));

        uint64_t tiDataLoc = 0;
        fapiRc = getscom_abs_wrap(&coreTgt, CORE_SCRATCH_REG0, &tiDataLoc);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "GetScom failed for address 0x20028486");
            hdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                           SBE_SEC_TI_CORE_SCRATCH_READ_FAILED);
            ffdc.setRc(fapiRc);
            break;
        }
        SBE_INFO("tiDataLoc is 0x%08X%08X and core target is 0x%08X",
                  SBE::higher32BWord(tiDataLoc), SBE::lower32BWord(tiDataLoc),
                  coreTgt.get());
        //Bit 0 of the core scratch reg meant to ignore hrmor.
        tiDataLoc = tiDataLoc & 0x7FFFFFFFFFFFFFFF;
        //Now we got the TI data location. Read TI_DATA_LEN bytes from
        //that location.
        uint32_t bytesRemaining = TI_DATA_LEN;
        do
        {
            uint8_t tiData[PBA_GRAN_SIZE] = {0};
            SBE_EXEC_HWP(fapiRc, p10_getmempba, procTgt, (tiDataLoc + bytesRead), PBA_GRAN_SIZE,
                         reinterpret_cast<uint8_t*>(tiData), flags);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in p10_getmempba() ,Addr[0x%08X%08X] "
                "bytes[0x%08X]",SBE::higher32BWord(tiDataLoc),
                SBE::lower32BWord(tiDataLoc), flags);
                hdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_TI_DATA_READ_FAILED);
                ffdc.setRc(fapiRc);
                break;
            }
            for(uint32_t i = 0; i < PBA_GRAN_SIZE; i++)
                SBE_DEBUG("TI data[%d] is 0x%08X ", i, tiData[i]);

            uint32_t len2enqueue  = 0;
            len2enqueue = PBA_GRAN_SIZE/sizeof(uint32_t);
            //Create the response for TI Data.

            rc = sbeDownFifoEnq_mult (len2enqueue, reinterpret_cast<uint32_t *>(tiData));
            if (rc)
            {
               // will let command processor routine handle the failure
               break;
            }
            bytesRemaining = bytesRemaining - PBA_GRAN_SIZE;
            bytesRead = bytesRead + PBA_GRAN_SIZE;
        }while(bytesRemaining > 0);
    }while(0);
    // Create the Response to caller
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    do
    {
        // Build the response header packet
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        //TI data is sent. Now send the TI length.
        uint32_t len2enqueue = sizeof(bytesRead)/sizeof(uint32_t);;
        SBE_INFO("Length of data sent through FIFO 0x%08X", bytesRead);
        rc = sbeDownFifoEnq_mult (len2enqueue, &bytesRead);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        rc = sbeDsSendRespHdr(hdr, &ffdc);
       // will let command processor routine handle the failure
    }while(0);
    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
    return 0;
}

fapi2::ReturnCode sbeSyncWithAllSbeUsingLQA(bool i_ignoreAduAddrError, uint8_t& o_quiesced_status)
{
    #define SBE_FUNC " SBE_SYNC_WITH_ALL_SBE_USING_LQA "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    size_t timeOut = SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP;
    o_quiesced_status = 0;
    // Use the LQA bit to indicate the other chips
    uint64_t data = 0x1000000000000000ULL; //Set bit3
    // Set bit3(SECURITY_SWITCH_LOCAL_QUIESCE_ACHIEVED) on 0x00010005
    rc = putscom_abs_wrap(&procTgt, OTP_SECURITY_SWITCH, data);
    if(rc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "PutScom failed for OTP_SECURITY_SWITCH(0x10005) RC=[0x%08X]", rc);
    }
    else
    {
        // Sync with all the chips before invoking measurement Seeprom sequence
        // of TPM Reset. This will make sure no thread from any chip is accessing
        // TPM SPI
        timeOut = SBE_SYSTEM_QUIESCE_TIMEOUT_LOOP;
        while(timeOut)
        {
            rc = p10_sbe_sync_quiesce_states(procTgt, o_quiesced_status);
            if(rc != FAPI2_RC_SUCCESS)
            {
                if((rc == RC_P10_ADU_STATUS_REG_ADDRESS_ERR) && (i_ignoreAduAddrError == true))
                {
                    SBE_INFO(SBE_FUNC " Adu Address Error, Retry with Adu Reset and Cleanup, FapiRC[0x%08X]", rc);
                    rc= FAPI2_RC_SUCCESS;
                    o_quiesced_status = 0;
                    SBE_EXEC_HWP(rc, p10_adu_utils_reset_adu_hwp, procTgt);
                    if(rc != FAPI2_RC_SUCCESS)
                    {
                        SBE_ERROR(SBE_FUNC " Adu reset Error, rc[0x%08X]", rc);
                        break;
                    }
                    SBE_EXEC_HWP(rc, p10_adu_utils_cleanup_adu_hwp, procTgt);
                    if(rc != FAPI2_RC_SUCCESS)
                    {
                        SBE_ERROR(SBE_FUNC " Adu reset Error, rc[0x%08X]", rc);
                        break;
                    }
                }
                else
                {
                    SBE_ERROR(SBE_FUNC "p10_sbe_sync_quiesce_states failed before MPIPL Reset, RC=[0x%08X]",rc);
                    break;
                }
            }
            if(o_quiesced_status)
            {
                SBE_INFO(SBE_FUNC "p10_sbe_sync_quiesce_states LQA SBE System "
                        "Quiesce done before Mpipl Reset");
                break;
            }
            else
            {
                timeOut--;
                // delay prior to repeating the above
                fapi2::delay(SBE_LQA_DELAY_HW_US, SBE_LQA_DELAY_SIM_CYCLES);
            }
        }
    }

    SBE_EXIT(SBE_FUNC);
    #undef SBE_FUNC
    return rc;
}


#endif //__SBEFW_SEEPROM__
