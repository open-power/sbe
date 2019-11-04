/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdmpipl.C $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
#include "sbearchregdump.H"

#include "p9_hcd_core_stopclocks.H"
#include "p9_hcd_cache_stopclocks.H"
#include "p9_stopclocks.H"
#include "fapi2.H"

#include "core/ipl.H"

using namespace fapi2;

// Defines for stop clock
#define SBE_IS_EX0(chipletId) \
    (!(((chipletId - CORE_CHIPLET_OFFSET) & 0x0002) >> 1))

/* @brief Bitmapped enumeration to identify the stop clock HWP call
 */
enum stopClockHWPType
{
    SC_NONE     = 0x00,
    SC_PROC     = 0x01, // Call p9_stopclocks
    SC_CACHE    = 0x02, // Call p9_hcd_cache_stopclocks
    SC_CORE     = 0x04, // Call p9_hcd_core_stopclocks
};

#ifdef __SBEFW_SEEPROM__

#ifdef SEEPROM_IMAGE
    // Using function pointer to force long call.
p9_hcd_cache_stopclocks_FP_t p9_hcd_cache_stopclocks_hwp = &p9_hcd_cache_stopclocks;
p9_hcd_core_stopclocks_FP_t p9_hcd_core_stopclocks_hwp = &p9_hcd_core_stopclocks;
p9_stopclocks_FP_t p9_stopclocks_hwp = &p9_stopclocks;
#endif

static const uint32_t SBE_ISTEP_MPIPL_START         = 96;
static const uint32_t MPIPL_START_MAX_SUBSTEPS      = 8;
static const uint32_t SBE_ISTEP_MPIPL_CONTINUE      = 97;
static const uint32_t MPIPL_CONTINUE_MAX_SUBSTEPS   = 8;
static const uint32_t SBE_ISTEP4                    = 4;
static const uint32_t SBE_ISTEP5                    = 5;
static const uint32_t ISTEP_MINOR_START             = 1;
static const uint32_t ISTEP4_MAX_SUBSTEPS           = 34;
static const uint32_t ISTEP5_MAX_SUBSTEPS           = 2;

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
            SBE_ERROR(SBE_FUNC "Failed in StartMpipl Minor Isteps[%d]", minor);
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

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        fapiRc = startMpiplIstepsExecute();
        bool checkstop = isSystemCheckstop();
        if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
        {
            SBE_ERROR(SBE_FUNC "Failed in Mpipl Start in ChipOp Mode");
            if(checkstop)
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_SYSTEM_CHECKSTOP);
            }
            else
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                ffdc.setRc(fapiRc);
            }
            // reset attribute. We do not want to reset register, so do not
            // use setMpIplMode
            uint8_t isMpipl = 0;
            PLAT_ATTR_INIT(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), isMpipl);
            break;
        }
    }while(0);

    // Create the Response to caller
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
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

    sbeResponseFfdc_t ffdc;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Refresh SBE role and proc chip mem attribute
        // to handle failover of FSP and with it the switchover of
        // master proc SBE
        fapi2::plat_AttrInit();
        SbeRegAccess::theSbeRegAccess().init(true);

        g_sbeRole = SbeRegAccess::theSbeRegAccess().isSbeSlave() ?
                    SBE_ROLE_SLAVE : SBE_ROLE_MASTER;

        fapiRc = continueMpiplIstepsExecute(g_sbeRole);
        bool checkstop = isSystemCheckstop();
        if((fapiRc != FAPI2_RC_SUCCESS) || checkstop)
        {
            SBE_ERROR(SBE_FUNC "Failed in Continue Mpipl in ChipOp Mode, "
                "SBE Role[%d]", g_sbeRole);
            if(checkstop)
            {
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     SBE_SEC_SYSTEM_CHECKSTOP);
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

    // reset attribute. We do not want to reset register, so do not
    // use setMpIplMode
    uint8_t isMpipl = 0;
    PLAT_ATTR_INIT(ATTR_IS_MPIPL, Target<TARGET_TYPE_SYSTEM>(), isMpipl);
    // Create the Response to caller
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if(SBE_SEC_OPERATION_SUCCESSFUL == rc)
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeCollectDumpMpipl: Sbe Collect Dump in MPIPL function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeCollectDumpMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeCollectDumpMpipl "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;

    sbeResponseFfdc_t ffdc;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Collect Architected Register Dump
        fapiRc = sbeDumpArchRegs();
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "Failed sbeDumpArchRegs while sbeCollectDumpMpipl in progress");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                        SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            ffdc.setRc(fapiRc);
            break;
        }
    }while(0);
    // Create the Response to caller
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if(SBE_SEC_OPERATION_SUCCESSFUL == rc)
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
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
    p9_stopclocks_flags flags;

    p9hcd::P9_HCD_CLK_CTRL_CONSTANTS clk_regions =
        p9hcd::CLK_REGION_ALL_BUT_PLL_REFR;
    p9hcd::P9_HCD_EX_CTRL_CONSTANTS ex_select = p9hcd::BOTH_EX;

    flags.clearAll();
    flags.sync_stop_quad_clks = false;
    flags.stop_core_clks = true; 
    flags.stop_cache_clks = true;
    
    SBE_EXEC_HWP(fapiRc, p9_stopclocks_hwp,
            plat_getChipTarget(),
            flags,
            clk_regions,
            ex_select);
    if(fapiRc != FAPI2_RC_SUCCESS)
    {
        SBE_ERROR(SBE_FUNC "Failed in StopClock S0S1 Interface");
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
 *
 * @return Bitmapped stopClockHWPType enum values
 */
///////////////////////////////////////////////////////////////////////
static inline uint32_t getStopClockHWPType(uint32_t i_targetType,
                                           uint32_t i_chipletId)
{
    uint32_t l_rc = SC_NONE;
    TargetType l_fapiTarget = sbeGetFapiTargetType(
                                                i_targetType,
                                                i_chipletId);
    if((l_fapiTarget == TARGET_TYPE_PROC_CHIP) ||
       (l_fapiTarget == TARGET_TYPE_PERV) ||
       ((i_targetType == TARGET_CORE) && (i_chipletId == SMT4_ALL_CORES))||
       ((i_targetType == TARGET_EQ) && (i_chipletId == EQ_ALL_CHIPLETS)) ||
       ((i_targetType == TARGET_EX) && (i_chipletId == EX_ALL_CHIPLETS)))
    {
           l_rc |= SC_PROC;
    }
    if((l_fapiTarget == TARGET_TYPE_CORE) ||
       (l_fapiTarget == TARGET_TYPE_EX))
    {
        l_rc |= SC_CORE;
    }
    if((l_fapiTarget == TARGET_TYPE_EQ) ||
       (l_fapiTarget == TARGET_TYPE_EX))
    {
        l_rc |= SC_CACHE;
    }
    return l_rc;
}

///////////////////////////////////////////////////////////////////////
/* @brief Prepare Stop clock flags base on Target Type
 *
 * @param[in] i_targetType  SBE chip-op target Type
 *
 * @return p9_stopclocks_flags
 */
///////////////////////////////////////////////////////////////////////
static inline p9_stopclocks_flags getStopClocksFlags(uint32_t i_targetType)
{
    p9_stopclocks_flags l_flags;

    if(i_targetType != TARGET_PROC_CHIP)
    {
        // Clear default flags - only in case the target is not PROC_CHIP
        // Otherwise, for a PROC_CHIP target, we want to keep default flags
        l_flags.clearAll();
    }
    if(i_targetType == TARGET_PERV)
    {
        // Keep only tp as true
        l_flags.stop_tp_clks  = true;
    }
    else if(i_targetType == TARGET_CORE)
    {
        // Keep only core flag as true
        l_flags.stop_core_clks = true;
    }
    else if(i_targetType == TARGET_EQ)
    {
        // Keep only cache flag as true
        l_flags.stop_cache_clks = true;
    }
    else if(i_targetType == TARGET_EX)
    {
        // Keep only cache and core as true
        l_flags.stop_cache_clks = true;
        l_flags.stop_core_clks = true;
    }

    return l_flags;
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
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    uint32_t l_len = 0;
    sbeResponseFfdc_t l_ffdc;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeStopClocksReqMsgHdr_t l_reqMsg = {0};

    do
    {
        // Get the TargetType and ChipletId from the command message
        l_len  = sizeof(sbeStopClocksReqMsgHdr_t)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len, (uint32_t *)&l_reqMsg); // EOT fetch
        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        SBE_INFO(SBE_FUNC "TargetType 0x%04X ChipletId 0x%02X",
                    (uint16_t)l_reqMsg.targetType,
                    (uint8_t)l_reqMsg.chipletId);

        fapi2::plat_target_handle_t l_tgtHndl;
        // Keep these default values in sync with p9_stopclocks.H
        p9hcd::P9_HCD_CLK_CTRL_CONSTANTS l_clk_regions
                                    = p9hcd::CLK_REGION_ALL_BUT_PLL_REFR;
        p9hcd::P9_HCD_EX_CTRL_CONSTANTS l_ex_select   = p9hcd::BOTH_EX;

        // Get the type of stopclocks procedure to call
        // based on target and chiplet id
        uint32_t l_hwpType = getStopClockHWPType(l_reqMsg.targetType,
                                                        l_reqMsg.chipletId);
        if(l_hwpType == SC_NONE)
        {
            // Error in target and chiplet id combination
            SBE_ERROR(SBE_FUNC "Invalid TargetType[0x%04X] ChipletId[0x%02X]",
                    (uint32_t)l_reqMsg.targetType,
                    (uint32_t)l_reqMsg.chipletId);
            l_respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                 SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;
        }
        // All Core/All Cache/All Ex & Perv & Proc are handled here
        if(l_hwpType & SC_PROC)
        {
            SBE_DEBUG(SBE_FUNC " Calling p9_stopclocks");
            p9_stopclocks_flags l_flags = getStopClocksFlags(
                                                        l_reqMsg.targetType);
            if(l_reqMsg.targetType == TARGET_EX)
            {
                l_clk_regions = static_cast<p9hcd::P9_HCD_CLK_CTRL_CONSTANTS>
                                (p9hcd::CLK_REGION_EX0_REFR |
                                                    p9hcd::CLK_REGION_EX1_REFR);
            }
            l_flags.sync_stop_quad_clks = false;
            SBE_EXEC_HWP(l_fapiRc, p9_stopclocks_hwp,
                          plat_getChipTarget(),
                          l_flags,
                          l_clk_regions,
                          l_ex_select);
        }
        // Specific CORE/EX
        if(l_hwpType & SC_CORE)
        {
            SBE_DEBUG(SBE_FUNC " Calling p9_hcd_core_stopclocks");
            sbeGetFapiTargetHandle(l_reqMsg.targetType,
                                   l_reqMsg.chipletId,
                                   l_tgtHndl);
            if(l_reqMsg.targetType == TARGET_EX)
            {
                Target<TARGET_TYPE_EX> l_exTgt(l_tgtHndl);
                for(auto &l_childCore :
                                    l_exTgt.getChildren<TARGET_TYPE_CORE>())
                {
                    SBE_EXEC_HWP(l_fapiRc,
                                 p9_hcd_core_stopclocks_hwp,
                                 l_childCore,
                                 false);
                }
            }
            else
            {
                SBE_EXEC_HWP(l_fapiRc, p9_hcd_core_stopclocks_hwp, l_tgtHndl, false);
            }
        }
        // Specific EQ/EX
        if(l_hwpType & SC_CACHE)
        {
            SBE_DEBUG(SBE_FUNC " Calling p9_hcd_cache_stopclocks");
            if(l_reqMsg.targetType == TARGET_EX)
            {
                // Modify l_clk_regions based on chiplet Id
                l_clk_regions = SBE_IS_EX0(l_reqMsg.chipletId) ?
                        p9hcd::CLK_REGION_EX0_REFR : p9hcd::CLK_REGION_EX1_REFR;
                // Modify l_ex_select based on chiplet ID
                l_ex_select = SBE_IS_EX0(l_reqMsg.chipletId) ?
                                            p9hcd::EVEN_EX : p9hcd::ODD_EX;
                Target<TARGET_TYPE_EX> l_ex_target(l_tgtHndl);
                l_tgtHndl = l_ex_target.getParent<TARGET_TYPE_EQ>();
            }
            SBE_EXEC_HWP(l_fapiRc, p9_hcd_cache_stopclocks_hwp,
                         l_tgtHndl,
                         (p9hcd::P9_HCD_CLK_CTRL_CONSTANTS)l_clk_regions,
                         (p9hcd::P9_HCD_EX_CTRL_CONSTANTS)l_ex_select,
                         false);
        }

        if( l_fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Stopclocks failed for TargetType [0x%04X] "
                "ChipletId [0x%02X]",
                (uint16_t)l_reqMsg.targetType,
                (uint8_t)l_reqMsg.chipletId);
            l_respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                 SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            l_ffdc.setRc(l_fapiRc);
            break;
        }
    }while(0);

    // Create the Response to caller
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if(SBE_SEC_OPERATION_SUCCESSFUL == l_rc)
    {
        l_rc = sbeDsSendRespHdr( l_respHdr, &l_ffdc);
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

#endif //__SBEFW_SEEPROM__
