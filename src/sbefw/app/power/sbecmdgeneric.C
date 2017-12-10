/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdgeneric.C $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2018                        */
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
 * @file: ppe/sbe/sbefw/sbecmdgeneric.C
 *
 * @brief This file contains the SBE generic Commands
 *
 */

#include "sbecmdgeneric.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_build_info.H"
#include "sbeFifoMsgUtils.H"
#include "sbeFFDC.H"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"

#include "fapi2.H"
//#include "p9_xip_image.h"

using namespace fapi2;

#ifdef __SBEFW_SEEPROM__
// Forward declaration
sbeCapabilityRespMsg::sbeCapabilityRespMsg()
{
    verMajor= SBE_FW_MAJOR_VERSION;
    verMinor = SBE_FW_MINOR_VERSION;
    fwCommitId = SBE_COMMIT_ID;
    // Get hbbl section
    P9XipHeader *hdr = getXipHdr();
    for(uint32_t idx=0; idx<sizeof(hdr->iv_buildTag); idx++)
    {
        buildTag[idx] = hdr->iv_buildTag[idx];
    }

    // We can remove this for llop once all capabilities
    // are supported
    for(uint32_t idx = 0; idx < SBE_MAX_CAPABILITIES; idx++ )
    {
        capability[idx] = 0;
    }
    // @TODO via RTC : 160602
    // Update Capability flags based on lastes spec.
    capability[IPL_CAPABILITY_START_IDX] =
                                EXECUTE_ISTEP_SUPPPORTED |
                                SUSPEND_IO_SUPPPORTED;

    capability[SCOM_CAPABILITY_START_IDX] =
                                GET_SCOM_SUPPPORTED |
                                PUT_SCOM_SUPPPORTED |
                                MODIFY_SCOM_SUPPPORTED |
                                PUT_SCOM_UNDER_MASK_SUPPPORTED ;

    capability[GENERIC_CHIPOP_CAPABILITY_START_IDX] =
                                GET_SBE_FFDC_SUPPPORTED |
                                GET_SBE_CAPABILITIES_SUPPPORTED|
                                SBE_QUIESCE;

    capability[MEMORY_CAPABILITY_START_IDX] =
                                GET_MEMORY_SUPPPORTED |
                                PUT_MEMORY_SUPPPORTED |
                                GET_SRAM_OCC_SUPPPORTED |
                                PUT_SRAM_OCC_SUPPPORTED;

    capability[INSTRUCTION_CTRL_CAPABILITY_START_IDX] =
                                CONTROL_INSTRUCTIONS_SUPPPORTED;

    capability[REGISTER_CAPABILITY_START_IDX] =
                                GET_REGISTER_SUPPPORTED |
                                PUT_REGISTER_SUPPPORTED ;

    capability[RING_CAPABILITY_START_IDX] =
                                GET_RING_SUPPPORTED |
                                PUT_RING_SUPPPORTED;
}
#endif //__SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__
//----------------------------------------------------------------------------
uint32_t sbeReadMem( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeReadMem"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    sbeReadMemReq_t req = {};

    do
    {
        // Extract the request
        // and send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(req)/sizeof(uint64_t)),
                                    (uint64_t*)&req,
                                    true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC "Failed to extract SBE_HOST_PSU_MBOX_REG1 and "
                    "SBE_HOST_PSU_MBOX_REG2");
            break;
        }

        if(!( req.validateReq()) )
        {
            SBE_ERROR(SBE_FUNC"Invalid data. offset:0x%08X size:0x%08X",
                     req.offset, req.size );
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_USER_ERROR,
                                                    SBE_SEC_INVALID_PARAMS);
            break;
        }

        // Default EX Target Init. As its not LCO mode, ex does not matter.
        fapi2::Target<fapi2::TARGET_TYPE_EX> l_ex(
            fapi2::plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_EX>(
                    sbeMemAccessInterface::PBA_DEFAULT_EX_CHIPLET_ID));
        p9_PBA_oper_flag l_myPbaFlag;
        l_myPbaFlag.setOperationType(p9_PBA_oper_flag::INJ);

        sbeMemAccessInterface pbaInterface(
                                     SBE_MEM_ACCESS_PBA,
                                     req.responseAddr,
                                     &l_myPbaFlag,
                                     SBE_MEM_ACCESS_WRITE,
                                     sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES,
                                     l_ex);
        uint32_t len = req.size;
        uint64_t *seepromAddr = req.getEffectiveAddr();
        while( len > 0)
        {
            uint64_t *dataBuffer = static_cast<uint64_t*>
                                            (pbaInterface.getBuffer());
            for(size_t idx=0;
                idx < (sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES/
                                                    sizeof(uint64_t));
                idx++)
            {
                *dataBuffer = *seepromAddr;
                dataBuffer++;
                seepromAddr++;
            }

            fapi2::ReturnCode fapiRc = pbaInterface.accessGranule(
                           len == sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);
            if( fapiRc != fapi2::FAPI2_RC_SUCCESS)
            {
                break;
            }
            len = len - sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES;
        }
    } while(false);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
#endif //not __SBEFW_SEEPROM__

