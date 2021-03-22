/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdtracearray.C $                  */
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
 * @file: ppe/sbe/sbefw/sbecmdtracearray.C
 *
 * @brief This file contains the SBE Control Tracearray chipOp
 *
 */
#include "sbecmdtracearray.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbeutil.H"
#include "sbefapiutil.H"
#include "fapi2.H"
#include "chipop_handler.H"
#include "p10_sbe_tracearray.H"

using namespace fapi2;

#if 0
constexpr uint32_t SBE_TRACE_GRANULE_NUM_ROWS = 1;
constexpr uint32_t SBE_TRACEARRAY_BYTES_PER_ROW =
                            (P10_TRACEARRAY_BITS_PER_ROW / 8);
constexpr uint32_t SBE_TRACE_GRANULE_NUM_WORDS =
     (SBE_TRACE_GRANULE_NUM_ROWS * SBE_TRACEARRAY_BYTES_PER_ROW) /
                                                        sizeof(uint32_t);
p10_sbe_tracearray_FP_t p10_sbe_tracearray_hwp = &p10_sbe_tracearray;
#endif


uint32_t sbeControlTraceArray(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeControlTraceArray"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);

    sbefifo_hwp_data_ostream ostream(type);
    sbefifo_hwp_data_istream istream(type);

    l_rc = sbeControlTraceArrayWrap( istream, ostream );
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

uint32_t sbeControlTraceArrayWrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                                  fapi2::sbefifo_hwp_data_ostream& i_putStream)
{
    #define SBE_FUNC " sbeControlTraceArrayWrap"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    sbeControlTraceArrayCMD_t l_req = {};
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t l_ffdc;
    ReturnCode l_fapiRc;
    uint32_t l_NumWordsRead = 0;
    uint32_t l_len = 0;

    do
    {
        l_len = sizeof(sbeControlTraceArrayCMD_t)/sizeof(uint32_t);
        l_rc = i_getStream.get(l_len, (uint32_t *)&l_req);

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]"
                         " traceArrayId [0x%04X] operation [0x%04X]",
                         l_req.targetType,
                         l_req.chipletId,
                         l_req.traceArrayId,
                         l_req.operation);

        // Construct a Target from Chiplet ID and Target Type
        fapi2::plat_target_handle_t l_tgtHndl;
        if(!sbeGetFapiTargetHandle(l_req.targetType, l_req.chipletId,
                                   l_tgtHndl))
        {
            SBE_ERROR(SBE_FUNC "Invalid target type [0x%04x]",
                                            (uint16_t)l_req.targetType);
            respHdr.setStatus(SBE_PRI_INVALID_DATA,
                                    SBE_SEC_INVALID_TARGET_TYPE_PASSED);
            break;
        }
        proc_gettracearray_args l_args = {};
        // Fill trace array Id
        l_args.trace_bus = (p10_tracearray_bus_id)l_req.traceArrayId;
        // Fill control arguments
        l_args.reset_post_dump      = (l_req.operation & SBE_TA_RESET);
        l_args.restart_post_dump    = (l_req.operation & SBE_TA_RESTART);
        l_args.stop_pre_dump        = (l_req.operation & SBE_TA_STOP);
        l_args.collect_dump         = (l_req.operation & SBE_TA_COLLECT_DUMP);
        l_args.ignore_mux_setting   = (l_req.operation &
                                                    SBE_TA_IGNORE_MUX_SETTING);

        uint64_t l_buffer[SBE_TRACE_GRANULE_NUM_WORDS/2] = {};
        for(uint32_t l_cur_row = 0; l_cur_row < P10_TRACEARRAY_NUM_ROWS;
                                                                l_cur_row++)
        {
            SBE_EXEC_HWP(l_fapiRc, p10_sbe_tracearray_hwp,
                         l_tgtHndl,
                         l_args,
                         l_buffer,
                         l_cur_row,
                         SBE_TRACE_GRANULE_NUM_ROWS);
            if(l_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR("p10_sbe_tracearray failed");
                // Respond with HWP FFDC
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
                l_ffdc.setRc(l_fapiRc);
                break;
            }

            // If dump is not requested, break from the loop
            if(!l_args.collect_dump)
                break;

            // Put the buffer onto Fifo
            SBE_DEBUG(SBE_FUNC " sending row [%d]", l_cur_row);
            l_len = SBE_TRACE_GRANULE_NUM_WORDS;
            l_rc = i_putStream.put(l_len, reinterpret_cast<uint32_t *>(l_buffer));
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            l_NumWordsRead += SBE_TRACE_GRANULE_NUM_WORDS;
        }

    } while(false);

    // Now build and enqueue response into downstream FIFO
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if ( (SBE_SEC_OPERATION_SUCCESSFUL == l_rc ) &&
         (i_putStream.isStreamRespHeader()) )
    {
        SBE_INFO(SBE_FUNC " l_NumWordsRead [%d]", l_NumWordsRead);
        l_len = sizeof(l_NumWordsRead)/sizeof(uint32_t);
        l_rc  = i_putStream.put(l_len);
        if(SBE_SEC_OPERATION_SUCCESSFUL == l_rc)
        {
            l_rc = sbeDsSendRespHdr( respHdr, &l_ffdc);
        }
    }
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
