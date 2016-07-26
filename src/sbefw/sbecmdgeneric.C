/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdgeneric.C $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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

// Forward declaration
sbeCapabilityRespMsg::sbeCapabilityRespMsg()
{
    verMajor= SBE_FW_MAJOR_VERSION;
    verMinor = SBE_FW_MINOR_VERSION;
    fwCommitId = SBE_COMMIT_ID;
    // We can remove this for llop once all capabilities
    // are supported
    for(uint32_t idx = 0; idx < SBE_MAX_CAPABILITIES; idx++ )
    {
        capability[idx] = 0;
    }
    // @TODO via RTC : 160602
    // Update Capability flags based on lastes spec.
    capability[IPL_CAPABILITY_START_IDX] =
                                EXECUTE_ISTEP_SUPPPORTED;

    capability[SCOM_CAPABILITY_START_IDX] =
                                GET_SCOM_SUPPPORTED |
                                PUT_SCOM_SUPPPORTED |
                                MODIFY_SCOM_SUPPPORTED |
                                PUT_SCOM_UNDER_MASK_SUPPPORTED ;

    capability[GENERIC_CHIPOP_CAPABILITY_START_IDX] =
                                GET_SBE_FFDC_SUPPPORTED |
                                GET_SBE_CAPABILITIES_SUPPPORTED;

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
                                GET_RING_SUPPPORTED;
}
// Functions
//----------------------------------------------------------------------------
uint32_t sbeGetCapabilities (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeGetCapabilities "
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeCapabilityRespMsg_t capMsg;

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        // @TODO via RTC : 130575
        // Optimize both the RC handling and
        // FIFO operation infrastructure.
        if ( rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        len = sizeof(capMsg)/sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &capMsg);
        if (rc)
        {
            break;
        }

        rc = sbeDsSendRespHdr(respHdr);
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}


// Functions
//----------------------------------------------------------------------------
uint32_t sbeGetFfdc (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeGetFfdc "
    SBE_DEBUG(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);

        if ( rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        SbeFFDCPackage sbeFfdcPack;
        len = 0;
        //Send the FFDC data over FIFO.
        // @TODO via RTC : 149074
        // primary and secondary status should be picked
        // from the globals.
        rc = sbeFfdcPack.sendOverFIFO(respHdr.primaryStatus,
                                   respHdr.secondaryStatus,
                                   SBE_FFDC_ALL_DUMP,len,
                                   true);
        if (rc)
        {
            break;
        }
        rc = sbeDsSendRespHdr(respHdr);

    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}
