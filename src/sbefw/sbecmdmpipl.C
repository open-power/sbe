/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdmpipl.C $                                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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

#include "p9_hcd_core_stopclocks.H"
#include "p9_hcd_cache_stopclocks.H"
#include "fapi2.H"

using namespace fapi2;


#ifdef SEEPROM_IMAGE
// Using function pointer to force long call.
p9_hcd_cache_stopclocks_FP_t p9_hcd_cache_stopclocks_hwp = &p9_hcd_cache_stopclocks;
p9_hcd_core_stopclocks_FP_t p9_hcd_core_stopclocks_hwp = &p9_hcd_core_stopclocks;
#endif

// TODO - RTC 133367
///////////////////////////////////////////////////////////////////////
// @brief sbeEnterMpipl Sbe enter MPIPL function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeEnterMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeEnterMpipl "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        l_rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        // TODO via  RTC:123696  MPIPL Related procedure/steps
        // Can send FFDC if MPIPL procedure fails
        l_rc = sbeDsSendRespHdr( l_respHdr );

        // set state to MPIPL Wait
        (void)SbeRegAccess::theSbeRegAccess().
               stateTransition(SBE_ENTER_MPIPL_EVENT);

    }while(0);
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

// TODO - RTC 133367
///////////////////////////////////////////////////////////////////////
// @brief sbeContinueMpipl Sbe Continue MPIPL function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeContinueMpipl(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeContinueMpipl "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        l_rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        l_rc = sbeDsSendRespHdr( l_respHdr);

        //TODO RTC-134278  Continue MPIPL Related procedure/steps

        // TODO - Once continue steps are over, it will trigger the
        // istep5.2 and transition to runtime will happen

    }while(0);
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeStopClocks Sbe Stop Clocks function
//
// @return  RC from the underlying FIFO utility
// RTC-161679 : Stop Clocks Chip-op to handle Proc Chip Target
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
                    l_reqMsg.targetType, l_reqMsg.chipletId);

        if(false == l_reqMsg.validateInputTargetType())
        {
            l_respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                 SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;
        }

        uint64_t l_clk_regions = p9hcd::CLK_REGION_ALL_BUT_PLL_REFR;
        uint8_t l_ex_select   = p9hcd::BOTH_EX;

        if( (l_reqMsg.chipletId == SMT4_ALL_CORES) ||
            (l_reqMsg.chipletId == EQ_ALL_CHIPLETS) )
        {
            Target<TARGET_TYPE_PROC_CHIP > l_procTgt = plat_getChipTarget();
            if(l_reqMsg.targetType == TARGET_CORE)
            {
                for (auto l_coreTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_CORE>())
                {
                    SBE_DEBUG(SBE_FUNC " Calling p9_hcd_core_stopclocks");
                    SBE_EXEC_HWP(l_fapiRc, p9_hcd_core_stopclocks_hwp, l_coreTgt)
                    if(l_fapiRc != FAPI2_RC_SUCCESS)
                    {
                        // break from internal for loop
                        break;
                    }
                }
            }
            else // Cache
            {
                for (auto l_eqTgt : l_procTgt.getChildren<fapi2::TARGET_TYPE_EQ>())
                {
                    SBE_DEBUG(SBE_FUNC " Calling p9_hcd_cache_stopclocks");
                    SBE_EXEC_HWP(l_fapiRc, p9_hcd_cache_stopclocks_hwp,
                                 l_eqTgt,
                                 (p9hcd::P9_HCD_CLK_CTRL_CONSTANTS)l_clk_regions,
                                 (p9hcd::P9_HCD_EX_CTRL_CONSTANTS)l_ex_select)
                    if(l_fapiRc != FAPI2_RC_SUCCESS)
                    {
                        // break from internal for loop
                        break;
                    }
                }
            }
        }
        else // for a single Core/Cache chiplet
        {
            // Construct the Target
            fapi2::plat_target_handle_t l_tgtHndl;
            // No Need to check the return here, it's already validated
            sbeGetFapiTargetHandle( l_reqMsg.targetType,
                                    l_reqMsg.chipletId,
                                    l_tgtHndl );

            if(l_reqMsg.targetType == TARGET_CORE)
            {
                SBE_DEBUG(SBE_FUNC " Calling p9_hcd_core_stopclocks");
                l_fapiRc = p9_hcd_core_stopclocks(l_tgtHndl);
            }
            else //Cache
            {
                SBE_DEBUG(SBE_FUNC " Calling p9_hcd_cache_stopclocks");
                l_fapiRc = p9_hcd_cache_stopclocks(l_tgtHndl,
                                (p9hcd::P9_HCD_CLK_CTRL_CONSTANTS)l_clk_regions,
                                (p9hcd::P9_HCD_EX_CTRL_CONSTANTS)l_ex_select);
            }
        }

        if( l_fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Stopclocks failed for TargetType [0x%04X] "
                "ChipletId [0x%02X]", l_reqMsg.targetType, l_reqMsg.chipletId);
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


