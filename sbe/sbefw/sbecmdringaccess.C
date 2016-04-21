/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/sbefw/sbecmdringaccess.C $                                */
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
 * @file: ppe/sbe/sbefw/sbecmdcntrldmt.C
 *
 * @brief This file contains Put Ring From Image Command handler
 *
 */
#include "sbecmdringaccess.H"
#include "sbetrace.H"
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sbeutil.H"
#include "fapi2.H"

using namespace fapi2;
/*@brief - Map sbe ring access modes to fapi ring access modes
 *
 * @param[in] - i_ringMode - sbe ring access mode
 *
 * @return - l_fapiRingMode - fapi ring mode
 */
uint16_t sbeToFapiRingMode(uint16_t i_ringMode)
{
    uint16_t l_fapiRingMode = RING_MODE_HEADER_CHECK;

    if(i_ringMode & SBE_RING_MODE_SET_PULSE_NO_OPCG_COND)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_NO_OPCG_COND;
    }
    if(i_ringMode & SBE_RING_MODE_NO_HEADER_CHECK)
    {
        l_fapiRingMode |= RING_MODE_NO_HEADER_CHECK;
    }
    if(i_ringMode & SBE_RING_MODE_SET_PULSE_NSL)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_NSL;
    }
    if(i_ringMode & SBE_RING_MODE_SET_PULSE_SL)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_SL;
    }
    if(i_ringMode & SBE_RING_MODE_SET_PULSE_ALL)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_ALL;
    }
    return l_fapiRingMode;
}

/*@brief - create fapi target handle for the target type
 *
 * @param[in] - i_taretType - sbe ring target type
 * @param[in] - i_chipletId - chiplet id
 * @param[out] - o_tgtHndl - fapi target handle
 *
 * @return - true - on success
 *           false - on failure
 */
bool sbeGetFapiTargetHandle(uint16_t i_targetType,
                            uint16_t i_chipletId,
                            fapi2::plat_target_handle_t &o_tgtHndl)
{
    bool l_rc = true;
    switch(i_targetType)
    {
        case TARGET_CORE:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_CORE>(i_chipletId);
            break;
        case TARGET_EX:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_EX>(i_chipletId);
            break;
        case TARGET_PERV:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_PERV>(i_chipletId);
            break;
        case TARGET_PROC_CHIP:
            o_tgtHndl = plat_getChipTarget().get();
            break;
        default:
            l_rc = false;
            break;
    }
    return l_rc;
}

uint32_t sbePutRingFromImagePSU (uint8_t *i_pArg)
{
#define SBE_FUNC " sbePutRingFromImagePSU "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    sbePSUPutRingCMD_t l_cmd = {0};
    do
    {
        // Fetch Ring ID, Ring mode and pervasive chiplet id from the message
        // and also send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(l_cmd)/sizeof(uint64_t)),
                                    (uint64_t*)&l_cmd,
                                    true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC "Failed to extract SBE_HOST_PSU_MBOX_REG1 and "
                    "SBE_HOST_PSU_MBOX_REG2");
            break;
        }

        // Construct a Target from Chiplet ID and Target Type
        fapi2::plat_target_handle_t l_tgtHndl = {0};
        if(!sbeGetFapiTargetHandle(l_cmd.TargetType, l_cmd.ChipletID,
                                   l_tgtHndl))
        {
            SBE_ERROR(SBE_FUNC "Invalid target type [0x%04x]",
                                            (uint16_t)l_cmd.TargetType);
            g_sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INVALID_DATA,
                                    SBE_SEC_INVALID_TARGET_TYPE_PASSED);
            break;
        }
        fapi2::Target<TARGET_TYPE_ALL> l_Tgt(l_tgtHndl);

        // Initialize with HEADER CHECK mode
        uint16_t l_ringMode = sbeToFapiRingMode(l_cmd.RingMode);

        l_fapiRc = (uint32_t)putRing(l_Tgt, (RingID)l_cmd.RingID,
                                                          (RingMode)l_ringMode);
        if(l_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "putRing HWP failure - "
                        "MSG - Target Type [0x%04x] Ring ID [0x%04x]"
                        " Ring Mode [0x%04x] Chiplet ID [0x%04x]",
                        l_cmd.TargetType,
                        l_cmd.RingID,
                        l_cmd.RingMode,
                        l_cmd.ChipletID);
            break;
        }
    }while(0); // End of do-while

    // Send the response
    sbePSUSendResponse(g_sbeSbe2PsuRespHdr, l_fapiRc, l_rc);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
#undef SBE_FUNC
}
