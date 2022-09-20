/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdfastarray.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2022                        */
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
#include "sbecmdfastarray.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbeutil.H"
#include "sbefapiutil.H"
#include "sbeglobals.H"
#include "fapi2.H"
#include "chipop_handler.H"
#include <hwp_data_stream.H>
#include <plat_hwp_data_stream.H>
#include "p10_sbe_fastarray.H"
#include "base_toc.H"

using namespace fapi2;

uint32_t sbeControlFastArrayWrap( fapi2::sbefifo_hwp_data_istream& i_getStream,
                                  fapi2::sbefifo_hwp_data_ostream& i_putStream )
{
    #define SBE_FUNC " sbeControlFastArrayWrap"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeControlFastArrayCMD_t req = {};
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;
    ReturnCode fapiRc;

    do
    {
        uint32_t len = SBE_GLOBAL->sbeFifoCmdHdr.len -
            sizeof(SBE_GLOBAL->sbeFifoCmdHdr)/sizeof(uint32_t);
        const uint32_t fifo_control_data_length = len -
            sizeof(req.hdr)/sizeof(uint32_t);

        if (len > sizeof(req)/sizeof(uint32_t))
        {
            SBE_ERROR(SBE_FUNC" Fastarray request is too large."
                      "request size: 0x%08x, max size: 0x%08x",
                      len, sizeof(req)/sizeof(uint32_t));
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            // flush the fifo
            rc = i_getStream.get(len, NULL, true, true);
            break;
        }

        rc = i_getStream.get(len, (uint32_t *)&req); //EOT fetch
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X] control_set [0x%02X] "
                 "custom_data_length [0x%08X]", req.hdr.targetType,
                 static_cast<uint8_t>(req.hdr.chipletId),
                 static_cast<uint8_t>(req.hdr.control_set),
                 req.hdr.custom_data_length);

        // Construct a Target from Chiplet ID and Target Type
        plat_target_handle_t tgtHndl;
        if(!sbeGetFapiTargetHandle(req.hdr.targetType, req.hdr.chipletId,
                                   tgtHndl))
        {
            SBE_ERROR(SBE_FUNC "Invalid target - Type [0x%04x] Chiplet [0x%02X]",
                            static_cast<uint16_t>(req.hdr.targetType),
                            static_cast<uint8_t>(req.hdr.chipletId));
            respHdr.setStatus(SBE_PRI_INVALID_DATA,
                              SBE_SEC_INVALID_TARGET_TYPE_PASSED);
            break;
        }

        if (req.hdr.control_set == SBE_FASTARRAY_CUSTOM_CONTROL_SET)
        {
            if (req.hdr.custom_data_length != fifo_control_data_length)
            {
                SBE_ERROR(SBE_FUNC" Control data size in request does not match "
                          "calculated size. length field: 0x%08x, calc length: 0x%08x",
                          req.hdr.custom_data_length, fifo_control_data_length);
                respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }

            //FIXME:Adding the below pice of code utilises a large amount of stack and
            //ultimately corrupts it.
            //We need to get rid off "req.custom_control_data" array.
#if 0
            hwp_array_istream istream(req.custom_control_data, req.hdr.custom_data_length);
            SBE_EXEC_HWP(fapiRc, p10_sbe_fastarray, tgtHndl, istream, i_putStream);
            if(i_putStream.isStreamRespHeader(respHdr.rcStatus(),ffdc.getRc()))
                i_putStream.put(i_putStream.words_written() * 4); //words_written needs to convert to number of bytes
#endif
        }
        else
        {
            if (req.hdr.custom_data_length != 0)
            {
                SBE_ERROR(SBE_FUNC" Control data size in request is nonzero, but "
                          "custom control data is not selected.");
                respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }
            if (req.hdr.isControlSetValid() != true)
            {
                SBE_ERROR(SBE_FUNC" Control Set not valid [%d]", (uint32_t)req.hdr.control_set);
                respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }

            uint32_t *control_data = NULL;
            size_t control_data_size = 0;
            if (req.hdr.control_set > 1)
            { 
                control_data = (uint32_t *)(BASE_IMG_TOC->fa_mma_start);
                control_data_size = BASE_IMG_TOC->fa_mma_size;
            }
            else
            {
                control_data = (uint32_t *)(BASE_IMG_TOC->fa_cl2_start);
                control_data_size = BASE_IMG_TOC->fa_cl2_size;
            }

            SBE_INFO( SBE_FUNC "Start Offset [0x%08X] Size [0x%08X]",
                control_data, control_data_size);

            seeprom_hwp_data_istream istream(control_data, control_data_size);
            SBE_EXEC_HWP(fapiRc, p10_sbe_fastarray, tgtHndl, istream, i_putStream);
            if(i_putStream.isStreamRespHeader(respHdr.rcStatus(),ffdc.getRc()))
                i_putStream.put(i_putStream.words_written() * 4); //words_written needs to convert to number of bytes
        }

        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" HWP failure:targetType [0x%04X] "
                      "chipletId [0x%02X] control_set [0x%02X] ",
                      static_cast<uint16_t>(req.hdr.targetType),
                      static_cast<uint8_t>(req.hdr.chipletId),
                      static_cast<uint32_t>(req.hdr.control_set));
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_HWP_FAILURE);
            ffdc.setRc(fapiRc);
        }

    } while(false);

    // Now build and enqueue response into downstream FIFO
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if( ( SBE_SEC_OPERATION_SUCCESSFUL == rc ) &&
        ( i_putStream.isStreamRespHeader(respHdr.rcStatus(),ffdc.getRc())) )
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc,
                               i_getStream.getFifoType() );
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeControlFastArray(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeControlFastArray "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);

    fapi2::sbefifo_hwp_data_ostream ostream(type);
    fapi2::sbefifo_hwp_data_istream istream(type);

    l_rc = sbeControlFastArrayWrap( istream, ostream );

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
