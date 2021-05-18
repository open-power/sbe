/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdpmictelemetry.C $                  */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
 * @file: ppe/sbe/sbefw/sbecmdpmictelemetry.C
 *
 * @brief This file contains the SBE dump commands.
 *
 */

#include "sbefifo.H"
#include "sbetrace.H"
#include "sbeglobals.H"
#include "sbeFifoMsgUtils.H"
#include "sbeFFDC.H"
#include "chipop_handler.H"
#include "sbecmdpmictelemetry.H"
#include "pmic_n_mode_detect.H"
#include "plat_hwp_data_stream.H"

//----------------------------------------------------------------------------
uint32_t sbePmicHealthCheckWrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                                fapi2::sbefifo_hwp_data_ostream& i_putStream)
{
    #define SBE_FUNC " sbePmicHealthCheckWrap"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbePmicHealthCheckCmdHdr_t req = {};
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;
    ReturnCode fapiRc;

    do
    {
        uint32_t len = SBE_GLOBAL->sbeFifoCmdHdr.len -
                        sizeof(SBE_GLOBAL->sbeFifoCmdHdr)/sizeof(uint32_t);

        if (len > sizeof(req)/sizeof(uint32_t))
        {
            SBE_ERROR(SBE_FUNC" PMIC health check request is too large."
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

        SBE_INFO(SBE_FUNC" targetType [0x%04X] targetInstance [0x%02X] ",
                 req.targetType,
                 req.targetInstance);

        // Validate Target type.
        if(!req.validateTargetType())
        {
            SBE_ERROR(SBE_FUNC "Invalid target type[0x%04X] target instance[0x%02X]",
                      (uint32_t)req.targetType, (uint32_t)req.targetInstance);
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;
        }
        //Validate OCMB instance ID.
        if(!req.validateInstance())
        {
            SBE_ERROR(SBE_FUNC"Invalid OCMB Instance ID 0x%08X",
                (uint8_t)req.targetInstance);
            respHdr.setStatus(SBE_PRI_INVALID_DATA,
                            SBE_SEC_INVALID_OCMB_INSTANCE);
            break;
        }
        //Validate OCMB functional
        if(!req.validateFunctional())
        {
            SBE_ERROR(SBE_FUNC"Non functional target selected 0x%08X",
                (uint8_t)req.targetInstance);
            respHdr.setStatus(SBE_PRI_INVALID_DATA,
                            SBE_SEC_OCMB_TARGET_NOT_FUNCTIONAL);
            break;
        }
        Target<TARGET_TYPE_OCMB_CHIP> l_hndl = plat_getOCMBTargetHandleByInstance
                <fapi2::TARGET_TYPE_OCMB_CHIP>(req.targetInstance);
        SBE_DEBUG("OCMB target instance is %d and target is 0x%08X",req.targetInstance, l_hndl.get());

        fapiRc = pmic_n_mode_detect(l_hndl, i_putStream);
        if(i_putStream.isStreamRespHeader(respHdr.rcStatus(),ffdc.getRc()))
        {
            i_putStream.put(i_putStream.words_written() * 4); //words_written needs to convert to number of bytes
        }

        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" HWP failure:targetType [0x%04X] "
                    "targetInstance [0x%02X]",
                    (uint32_t)req.targetType,
                    (uint32_t)req.targetInstance);
            respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                            SBE_SEC_PMIC_HEALTH_CHECK_FAILED);
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

uint32_t sbePmicHealthCheck( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbePmicHealthCheck"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeFifoType type;

    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    type = static_cast<sbeFifoType>(configStr->fifoType);
    SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

    fapi2::sbefifo_hwp_data_ostream ostream(type);
    fapi2::sbefifo_hwp_data_istream istream(type);

    l_rc = sbePmicHealthCheckWrap(istream, ostream);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
