/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdfastarray.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
#include <hwp_data_stream.H>

#include <plat_hwp_data_stream.H>

#include "p10_sbe_fastarray.H"

using namespace fapi2;

uint32_t sbeControlFastArray(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeControlFastArray"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeControlFastArrayCMD_t l_req = {};
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t l_ffdc;
    ReturnCode l_fapiRc;

    do
    {
        uint32_t l_len = SBE_GLOBAL->sbeFifoCmdHdr.len -
            sizeof(SBE_GLOBAL->sbeFifoCmdHdr)/sizeof(uint32_t);
        const uint32_t l_fifo_control_data_length = l_len -
            sizeof(l_req.hdr)/sizeof(uint32_t);

        if (l_len > sizeof(l_req)/sizeof(uint32_t))
        {
            SBE_ERROR(SBE_FUNC" Fastarray request is too large."
                      "request size: 0x%08x, max size: 0x%08x",
                      l_len, sizeof(l_req)/sizeof(uint32_t));
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            // flush the fifo
            l_rc = sbeUpFifoDeq_mult(l_len, NULL, true, true);
            break;
        }

        l_rc = sbeUpFifoDeq_mult(l_len, (uint32_t *)&l_req); //EOT fetch
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X] control_set [0x%02X] "
                 "custom_data_length [0x%08X]", l_req.hdr.targetType,
                 static_cast<uint8_t>(l_req.hdr.chipletId),
                 static_cast<uint8_t>(l_req.hdr.control_set),
                 l_req.hdr.custom_data_length);

        // Construct a Target from Chiplet ID and Target Type
        plat_target_handle_t l_tgtHndl;
        if(!sbeGetFapiTargetHandle(l_req.hdr.targetType, l_req.hdr.chipletId,
                                   l_tgtHndl))
        {
            SBE_ERROR(SBE_FUNC "Invalid target - Type [0x%04x] Chiplet [0x%02X]",
                            static_cast<uint16_t>(l_req.hdr.targetType),
                            static_cast<uint8_t>(l_req.hdr.chipletId));
            respHdr.setStatus(SBE_PRI_INVALID_DATA,
                              SBE_SEC_INVALID_TARGET_TYPE_PASSED);
            break;
        }

        if (l_req.hdr.control_set == SBE_FASTARRAY_CUSTOM_CONTROL_SET)
        {
            if (l_req.hdr.custom_data_length != l_fifo_control_data_length)
            {
                SBE_ERROR(SBE_FUNC" Control data size in request does not match "
                          "calculated size. length field: 0x%08x, calc length: 0x%08x",
                          l_req.hdr.custom_data_length, l_fifo_control_data_length);
                respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }
            hwp_array_istream l_istream(l_req.custom_control_data, l_req.hdr.custom_data_length);
            sbefifo_hwp_data_ostream l_ostream;
            SBE_EXEC_HWP(l_fapiRc, p10_sbe_fastarray, l_tgtHndl, l_istream, l_ostream);
        }
        else
        {
            if (l_req.hdr.custom_data_length != 0)
            {
                SBE_ERROR(SBE_FUNC" Control data size in request is nonzero, but "
                          "custom control data is not selected.");
                respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }
            if (l_req.hdr.isControlSetValid() != true)
            {
                SBE_ERROR(SBE_FUNC" Control Set not valid [%d]", (uint32_t)l_req.hdr.control_set);
                respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }

            // TODO: Determine control data location in SEEPROM
            uint32_t *l_control_data = (uint32_t*)l_req.hdr.getFastArrayXipOffset();
            size_t l_control_data_size = l_req.hdr.getFastArrayXipSize();
            SBE_INFO( SBE_FUNC "Start Offset [0x%08X] Size [0x%08X]",
                l_req.hdr.getFastArrayXipOffset(), l_req.hdr.getFastArrayXipSize());

            seeprom_hwp_data_istream l_istream(l_control_data, l_control_data_size);
            sbefifo_hwp_data_ostream l_ostream;
            SBE_EXEC_HWP(l_fapiRc, p10_sbe_fastarray, l_tgtHndl, l_istream, l_ostream);
        }

        if(l_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" HWP failure:targetType [0x%04X] "
                      "chipletId [0x%02X] control_set [0x%02X] ",
                      static_cast<uint16_t>(l_req.hdr.targetType),
                      static_cast<uint8_t>(l_req.hdr.chipletId),
                      static_cast<uint32_t>(l_req.hdr.control_set));
            l_ffdc.setRc(l_fapiRc);
        }

    } while(false);

    // Now build and enqueue response into downstream FIFO
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if ( SBE_SEC_OPERATION_SUCCESSFUL == l_rc )
    {
        l_rc = sbeDsSendRespHdr( respHdr, &l_ffdc);
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
