/*
 * @file: ppe/sbe/sbefw/sbecmdcntlinst.C
 *
 * @brief This file contains the SBE Control Instruction chipOps
 *
 */

#include "sbecmdcntlinst.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"

#include "fapi2.H"
#include "p9_thread_control.H"

using namespace fapi2;

// This is used to find out the array index in g_control_reg_map in
// p9_thread_control.C
static const uint8_t SINGLE_THREAD_BIT_MASK = 0x08;
// TODO via RTC 152424
// Currently all proecdures in core directory are in seeprom.
// So we have to use function pointer to force a long call.
p9_thread_control_FP_t threadCntlhwp = &p9_thread_control;

/* @brief Map User Thread Command to Hwp ThreadCommands Enum */
ThreadCommands getThreadCommand(const sbeCntlInstRegMsgHdr_t & i_req)
{
    ThreadCommands l_cmd = PTC_CMD_START;
    switch(i_req.threadOps)
    {
        case THREAD_START_INS:  l_cmd = PTC_CMD_START;  break;
        case THREAD_STOP_INS:   l_cmd = PTC_CMD_STOP;   break;
        case THREAD_STEP_INS:   l_cmd = PTC_CMD_STEP;   break;
        case THREAD_SRESET_INS: l_cmd = PTC_CMD_SRESET; break;
    }
    return l_cmd;
}

/* @brief Map User Mode Command to Hwp Warn Check flag */
inline bool getWarnCheckFlag(const sbeCntlInstRegMsgHdr_t & i_req)
{
    bool l_warnCheck = false;
    if( EXIT_ON_FIRST_ERROR != i_req.mode )
    {
        l_warnCheck = true;
    }
    return l_warnCheck;
}

///////////////////////////////////////////////////////////////////////
// @brief sbeCntlInst Sbe control instructions function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeCntlInst(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeCntlInst "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Create the req struct for Control Instructions Chip-op
    sbeCntlInstRegMsgHdr_t l_req = {0};

    do
    {
        // Get the Req Struct Data sbeCntlInstRegMsgHdr_t from upstream Fifo
        uint32_t l_len2dequeue  = sizeof(l_req) / sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_req, true);

        // If FIFO failure
        if ( SBE_SEC_OPERATION_SUCCESSFUL != l_rc )
        {
            // Let command processor routine to handle the RC.
            break;
        }

        SBE_DEBUG("mode[0x%04X] coreChipletId[0x%08X] threadNum[0x%04X] "
            "threadOps[0x%04X] ", l_req.mode, l_req.coreChipletId,
            l_req.threadNum, l_req.threadOps);

        // Validate Input Args
        if( false == l_req.validateInputArgs())
        {
            SBE_ERROR(SBE_FUNC "ValidateAndMapInputArgs failed");
            l_respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                 SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            break;
        }

        // Fetch HWP mapped values
        bool l_warnCheck = getWarnCheckFlag(l_req);
        ThreadCommands l_cmd = getThreadCommand(l_req);

        // Default assignment not required since it is assigned below
        uint8_t l_core, l_coreCntMax;
        uint8_t l_threadCnt, l_threadCntMax;

        l_req.processInputDataToIterate(l_core, l_coreCntMax,
                                        l_threadCnt, l_threadCntMax);
        fapi2::buffer<uint64_t> l_data64;
        uint64_t l_state;
        do
        {
            fapi2::Target<fapi2::TARGET_TYPE_CORE> l_coreTgt(
                        plat_getTargetHandleByChipletNumber(l_core));
            if(!l_coreTgt.isFunctional())
            {
                continue;
            }

            uint8_t l_thread = l_threadCnt;
            do
            {
                // Call the Procedure
                l_fapiRc = threadCntlhwp(
                                    l_coreTgt,
                                    (SINGLE_THREAD_BIT_MASK >> l_thread),
                                    l_cmd, l_warnCheck,
                                    l_data64, l_state);

                if(l_fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR(SBE_FUNC "Failed for Core[%d] Thread [%d] "
                        "Cmd[%d] Mode[%d]", l_core, l_thread, l_req.threadOps,
                        l_req.mode);
                    if(IGNORE_HW_ERRORS == l_req.mode)
                    {
                        // No need to delete the l_fapiRc handle,it will get
                        // over-written
                        SBE_DEBUG(SBE_FUNC "Continuing in case of HW Errors"
                            " As user has passed to ignore errors.");
                        continue;
                    }
                    else
                    {
                        SBE_ERROR(SBE_FUNC "Breaking out, since User has "
                            "Selected the mode to exit on first error.");
                        l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                         SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                        l_ffdc.setRc(l_fapiRc);
                        break;
                    }
                }
            }while(++l_thread < l_threadCntMax);

            // If FapiRc from the inner loop (thread loop), just break here
            if ( l_fapiRc )
            {
                break; // From core while loop
            }
        }while(++l_core < l_coreCntMax);

    }while(0);

    // Create the Response to caller
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        if ( SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            break;
        }

        uint32_t l_dist2Hdr = 1;
        // Now enqueue the minimum response header
        uint32_t l_len = sizeof(l_respHdr) / sizeof(uint32_t);
        l_rc = sbeDownFifoEnq_mult(l_len, (uint32_t *)(&l_respHdr) );
        if ( SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            break;
        }

        l_dist2Hdr += l_len;

        // Enqueue FFDC data if there is one
        if( l_ffdc.getRc() )
        {
            l_len = sizeof(l_ffdc) / sizeof(uint32_t);
            l_rc = sbeDownFifoEnq_mult ( l_len, (uint32_t *)(&l_ffdc) );
            if ( SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
            {
                break;
            }

            l_dist2Hdr += l_len;
        }

        l_len = sizeof(l_dist2Hdr) / sizeof(uint32_t);
        l_rc = sbeDownFifoEnq_mult ( l_len, &l_dist2Hdr);
        if ( l_rc )
        {
            break;
        }
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

