/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdfastarray.C $                       */
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
#include "sbecmdfastarray.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbeutil.H"
#include "sbefapiutil.H"
#include "fapi2.H"

#if 0
#include "p9_sbe_fastarray_setup.H"
#include "p9_sbe_fastarray_cleanup.H"
#include "p9_sbe_fastarray_abist_catchup.H"

#ifdef SEEPROM_IMAGE
// Using Function pointer to force long call
p9_sbe_fastarray_setup_FP_t p9_sbe_fastarray_setup_hwp =
                                            &p9_sbe_fastarray_setup;
p9_sbe_fastarray_cleanup_FP_t p9_sbe_fastarray_cleanup_hwp =
                                            &p9_sbe_fastarray_cleanup;
p9_sbe_fastarray_abist_catchup_FP_t p9_sbe_fastarray_abist_catchup_hwp =
                                            &p9_sbe_fastarray_abist_catchup;
#endif
#endif

using namespace fapi2;

uint32_t sbeControlFastArray(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeControlFastArray"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
#if 0
    sbeControlFastArrayCMD_t l_req = {};
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t l_ffdc;
    ReturnCode l_fapiRc;
    uint32_t l_len = 0;

    do
    {
        l_len = sizeof(sbeControlFastArrayCMD_t)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len, (uint32_t *)&l_req); //EOT fetch

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        SBE_INFO(SBE_FUNC" targetType [0x%04X] chipletId [0x%02X]",
                         static_cast<uint16_t>(l_req.targetType),
                         static_cast<uint8_t>(l_req.chipletId));
        // Construct a Target from Chiplet ID and Target Type
        plat_target_handle_t l_tgtHndl;
        if(!sbeGetFapiTargetHandle(l_req.targetType, l_req.chipletId,
                                   l_tgtHndl))
        {
            SBE_ERROR(SBE_FUNC "Invalid target - Type [0x%04x] Chiplet [0x%02X]",
                            static_cast<uint16_t>(l_req.targetType),
                            static_cast<uint8_t>(l_req.chipletId));
            respHdr.setStatus(SBE_PRI_INVALID_DATA,
                                    SBE_SEC_INVALID_TARGET_TYPE_PASSED);
            break;
        }

        // Relevant only when mode is set up
        uint64_t l_clock_regions = 0;
        // Relevant only when mode is skip cycles
        uint32_t l_clock_cycles = 0;
        switch(l_req.mode)
        {
            case FASTARRAY_SETUP:
                l_clock_regions = (MASK_ZERO_H32B_UINT64(l_req.hData) << 32)|
                                            l_req.lData;
                SBE_INFO(SBE_FUNC" setup:clockregion [0x%08X%08X]",
                    static_cast<uint32_t>(SBE::higher32BWord(l_clock_regions)),
                    static_cast<uint32_t>(SBE::lower32BWord(l_clock_regions)));
                SBE_EXEC_HWP(l_fapiRc,
                             p9_sbe_fastarray_setup_hwp,
                             l_tgtHndl,
                             l_clock_regions);
                break;
            case FASTARRAY_SKIPCYCLES:
                l_clock_cycles = l_req.lData;
                SBE_INFO(SBE_FUNC" abist catchup:skipcycles [0x%08X]",
                    static_cast<uint32_t>(l_clock_cycles));
                SBE_EXEC_HWP(l_fapiRc,
                             p9_sbe_fastarray_abist_catchup_hwp,
                             l_tgtHndl,
                             l_clock_cycles);
                break;
            case FASTARRAY_CLEANUP:
                SBE_INFO(SBE_FUNC" cleanup");
                SBE_EXEC_HWP(l_fapiRc,
                             p9_sbe_fastarray_cleanup_hwp,
                             l_tgtHndl);
                break;
        }
        if(l_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" HWP failure:mode [0x%02X]",
                      static_cast<uint8_t>(l_req.mode));
            SBE_ERROR(SBE_FUNC" targetType [0x%04X] "
                      "chipletId [0x%02X] data[0-31] [0x%08X] "
                      "data[0-31] [0x%08X]",
                      static_cast<uint16_t>(l_req.targetType),
                      static_cast<uint8_t>(l_req.chipletId),
                      static_cast<uint32_t>(l_req.hData),
                      static_cast<uint32_t>(l_req.lData));
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
#endif
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}
