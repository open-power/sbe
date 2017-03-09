/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdgeneric.C $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sbeglobals.H"

#include "fapi2.H"

using namespace fapi2;

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
    SBE_ENTER(SBE_FUNC);
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
        sbeResponseFfdc_t l_ffdc ;
        l_ffdc.setRc(g_FfdcData.fapiRc);
        SBE_INFO(SBE_FUNC"FAPI RC is %x", g_FfdcData.fapiRc);
        // If no ffdc , exit;
        if( (l_ffdc.getRc() != FAPI2_RC_SUCCESS))
        {
            // making sure ffdc length is multiples of uint32_t
            assert((g_FfdcData.ffdcLength % sizeof(uint32_t)) == 0);
            uint32_t ffdcDataLenInWords = g_FfdcData.ffdcLength
                                            / sizeof(uint32_t);
            // Set failed command information
            // Sequence Id is 0 by default for Fifo interface
            // @TODO via RTC : 149074
            // primary and secondary status should be picked
            // from the globals.
            l_ffdc.setCmdInfo(0, respHdr.cmdClass, respHdr.command);
            // Add HWP specific ffdc data length
            l_ffdc.lenInWords += ffdcDataLenInWords;
            len = sizeof(sbeResponseFfdc_t)/sizeof(uint32_t);
            rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &l_ffdc);
            if (rc)
            {
                break;
            }
            //Send HWP internal Data
            rc = sbeDownFifoEnq_mult ( ffdcDataLenInWords,
                                        ( uint32_t *) &g_FfdcData.ffdcData);
            if (rc)
            {
                break;
            }

        }
        //Send the FFDC data over FIFO.
        // @TODO via RTC : 149074
        // primary and secondary status should be picked
        // from the globals.
        // Check for Primary and Secondary Status from Globals and then send 
        // internal FFDC.
        rc = sbeFfdcPack.sendOverFIFO(respHdr,
                                      SBE_FFDC_ALL_DUMP,
                                      len,
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

//----------------------------------------------------------------------------
uint32_t sbeFifoQuiesce( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeFifoQuiesce"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Set Quiesce State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                          SBE_QUIESCE_EVENT);

        rc = sbeDsSendRespHdr(respHdr);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC "sbeDsSendRespHdr failed");
            // Not Breaking here since we can't revert back on the set state
        }
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbePsuQuiesce( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbePsuQuiesce"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        // Send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        // This util method will check internally on the mbox0 register if
        // ACK is requested.
        rc = sbeAcknowledgeHost();
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                "SBE_SBE2PSU_DOORBELL_SET_BIT1");
            break;
        }

        // Set Quiesce State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                          SBE_QUIESCE_EVENT);

        rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG4,
                         (uint64_t*)(&SBE_GLOBAL->sbeSbe2PsuRespHdr),
                         (sizeof(SBE_GLOBAL->sbeSbe2PsuRespHdr)/sizeof(uint64_t)),
                         true);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC" Failed to write SBE_HOST_PSU_MBOX_REG4");
            // Not Breaking here since we can't revert back on the set state
        }
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeSetSystemFabricMap( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeSetSystemFabricMap"
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        uint64_t l_sysFabricMap = 0;
        //send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        l_rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(l_sysFabricMap)/sizeof(uint64_t)),
                                    &l_sysFabricMap, true);

        if(SBE_SEC_OPERATION_SUCCESSFUL != l_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1");
            break;
        }

        SBE_INFO(SBE_FUNC "Sytem Fabric Map [0x%08X][%08X]",
            SBE::higher32BWord(l_sysFabricMap),
            SBE::lower32BWord(l_sysFabricMap));

        FAPI_ATTR_SET(fapi2::ATTR_SBE_SYS_CONFIG,
                       fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                       l_sysFabricMap);
    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, l_rc);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

