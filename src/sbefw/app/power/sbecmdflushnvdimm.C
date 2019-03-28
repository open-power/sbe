/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdflushnvdimm.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.C
 *
 * @brief This file contains the SBE istep chipOps
 *
 */
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
//#include "nvdimm_workarounds.H"
#include "sbecmdflushnvdimm.H"
#include "sbecmdcntlinst.H"

using namespace fapi2;


#ifdef __SBEFW_SEEPROM__

///////////////////////////////////////////////////////////////////////
// @brief sbeHandleFlushNVDIMM Sbe flush NVDIMM function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////

//mss::workarounds::nvdimm::p9_flush_nvdimm_FP_t p9_flush_nvdimm_hwp = &mss::workarounds::nvdimm::trigger_csave; 

uint32_t sbeHandleFlushNVDIMM(uint8_t *i_pArg)
{

    #define SBE_FUNC " sbeHandleFlushNVDIMM "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        fapiRc = stopAllCoreInstructions();
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "stop all core instructions was failed");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_HWP_FAILURE);
            ffdc.setRc(fapiRc);
            break;
        }
        fapiRc = flushNVDIMM();
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "p9_flush_NVDIMM_hwp failed");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_HWP_FAILURE);
            ffdc.setRc(fapiRc);
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
#endif
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------

fapi2::ReturnCode flushNVDIMM()
{
    #define SBE_FUNC "flushNVDIMM "
    SBE_ENTER(SBE_FUNC);
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
#if 0
    Target<TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    do
    {
        SBE_EXEC_HWP(fapiRc, p9_flush_nvdimm_hwp,  procTgt);
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "p9_flush_NVDIMM_hwp failed");
            break;
        }

    }while(0);
    SBE_EXIT(SBE_FUNC);
#endif
        return fapiRc;
    #undef SBE_FUNC
}

/* end PIBMEM CODE ----------------------------- */
#endif //#ifndef __SBEFW_SEEPROM__
