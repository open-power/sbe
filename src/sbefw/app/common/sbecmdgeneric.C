/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdgeneric.C $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
#include "sbeXipUtils.H"
#include "sbeMemAccessInterface.H"
#include "sbeSecurity.H"
#include "chipop_handler.H"
#include "p10_lpc_test.H"

#include "fapi2.H"
//#include "p9_xip_image.h"

using namespace fapi2;

#ifdef __SBEFW_SEEPROM__

static const uint32_t SBE_CAPABILITES_LEN_FIFO  =
            sizeof(sbeCapabilityRespMsg_t) -
            (sizeof(uint32_t) *
             (SBE_MAX_CAPABILITIES - CAPABILITIES_LAST_INDEX_FIFO - 1));
static const uint32_t SBE_CAPABILITES_2_LEN_FIFO  =
            sizeof(sbeCapability2RespMsg_t) -
            (sizeof(uint32_t) *
             (SBE_MAX_CAPABILITIES_2 - CAPABILITIES_2_LAST_INDEX_FIFO - 1));
static const uint32_t SBE_CAPABILITES_LEN_PSU  =
            sizeof(sbeCapabilityRespMsg_t) -
            (sizeof(uint32_t) *
             (SBE_MAX_CAPABILITIES - CAPABILITIES_LAST_INDEX_PSU - 1));

p10_lpc_test_t lpcTestHwp = &p10_lpc_test;

sbeCapabilityRespMsg::sbeCapabilityRespMsg() : capability{}
{
    verMajor= SBE_FW_MAJOR_VERSION;
    verMinor = SBE_FW_MINOR_VERSION;
    fwCommitId = SBE_COMMIT_ID;
    // Get xip header
    P9XipHeader *hdr = getXipHdr();
    for(uint32_t idx=0; idx<sizeof(hdr->iv_buildTag); idx++)
    {
        buildTag[idx] = hdr->iv_buildTag[idx];
    }

    capability[GENERIC_CAPABILTITY_START_IDX] =
                        HWP_FFDC_COLLECTION_SUPPPORTED |
                        SBE_FFDC_COLLECTION_SUPPPORTED |
                        ADDRESS_BLACKLISTING_SUPPPORTED |
                        FIFO_RESET_SUPPPORTED |
                        HOST_CMD_INTERFACE_SUPPORTED |
                        SP_LESS_MPIPL_SUPPORTED;
    capability[GENERIC_CAPABILTITY_START_IDX + 1] =
                        RESERVED_GENERIC_CAPABILITIES;
}
// Functions
//----------------------------------------------------------------------------
uint32_t sbePsuGetCapabilities(uint8_t *i_pArg)
{
    #define SBE_FUNC "sbePsuGetCapabilities "
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;

    sbePsuGetCapabilitiesReq_t req = {};

    sbeCapabilityRespMsg_t capMsg;
    updatePsuCapabilities(capMsg.capability);

    do
    {
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                  sizeof(req)/sizeof(uint64_t),
                                  (uint64_t*)&req,
                                  true);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_DEBUG(SBE_FUNC "at addr[0x%08X%08X] len[0x%08X%08X]",
                    SBE::higher32BWord(req.address),
                    SBE::lower32BWord(req.address),
                    SBE::higher32BWord(req.size),
                    SBE::lower32BWord(req.size));

        if((req.size < SBE_CAPABILITES_LEN_PSU) ||
                (req.size % sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES))
        {
            SBE_ERROR(SBE_FUNC " incorrect memory allocation");
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INVALID_DATA,
                                                    SBE_SEC_INVALID_PARAMS);
            break;
        }

        // Set MBOX response words
        uint64_t data = (uint64_t)SBE_CAPABILITES_LEN_PSU & 0x00000000FFFFFFFFull;
        rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG5,
                                   &data,
                                   sizeof(data)/sizeof(uint64_t));
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to write to "
                                "SBE_HOST_PSU_MBOX_REG5");
            break;
        }
        rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG6,
                                   (uint64_t*)&capMsg,
                                   1);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to write to "
                                "SBE_HOST_PSU_MBOX_REG6");
            break;
        }

        p10_PBA_oper_flag l_myPbaFlag;
        l_myPbaFlag.setOperationType(p10_PBA_oper_flag::INJ);

        sbeMemAccessInterface l_PBAInterface(
                                SBE_MEM_ACCESS_PBA,
                                req.address,
                                &l_myPbaFlag,
                                SBE_MEM_ACCESS_WRITE,
                                sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);
        l_fapiRc = l_PBAInterface.accessWithBuffer(
                                            &capMsg,
                                            SBE_CAPABILITES_LEN_PSU,
                                            true);
        if(l_fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "failed in writing to host memory");
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                                     SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            break;
        }
    } while(false);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, rc);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeGetCapabilities (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeGetCapabilities "
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeFifoType type;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeCapabilityRespMsg_t capMsg;
    updateFifoCapabilities(capMsg.capability);

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);
        // @TODO via RTC : 130575
        // Optimize both the RC handling and
        // FIFO operation infrastructure.
        if ( rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        len = SBE_CAPABILITES_LEN_FIFO / sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &capMsg, type);
        if (rc)
        {
            break;
        }

        rc = sbeDsSendRespHdr(respHdr, NULL, type);
    }while(0);

    if( rc )
    {
        SBE_ERROR( SBE_FUNC"Failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeGetCapabilities2 (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeGetCapabilities2 "
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeFifoType type;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeCapability2RespMsg_t capMsg;
    updateFifoCapabilities2(capMsg.capability);

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);
        // @TODO via RTC : 130575
        // Optimize both the RC handling and
        // FIFO operation infrastructure.
        if ( rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        len = SBE_CAPABILITES_2_LEN_FIFO / sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &capMsg, type);
        if (rc)
        {
            break;
        }

        rc = sbeDsSendRespHdr(respHdr, NULL, type);
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
    sbeFifoType type;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);

        if ( rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            // Let command processor routine to handle the RC
            break;
        }

        SbeFFDCPackage sbeFfdcPack;
        sbeResponseFfdc_t ffdc;

        // If need be, force collect HWP FFDC async to the real HWP fail.
        // Else, just send back what the SBE already has.
        sbeFfdcPack.collectAsyncHwpFfdc ();

        ffdc.setRc(g_FfdcData.fapiRc);
        SBE_INFO(SBE_FUNC"FAPI RC is %x", g_FfdcData.fapiRc);
        // If no ffdc , exit;
        if( (ffdc.getRc() != FAPI2_RC_SUCCESS) )
        {
            // making sure ffdc length is multiples of uint32_t
            assert((g_FfdcData.ffdcLength % sizeof(uint32_t)) == 0);
            uint32_t ffdcDataLenInWords = g_FfdcData.ffdcLength
                                            / sizeof(uint32_t);
            // Set failed command information
            // Sequence Id is 0 by default for Fifo interface
            ffdc.setCmdInfo(0, SBE_GLOBAL->failedCmdClass, SBE_GLOBAL->failedCmd);

            // Add HWP specific ffdc data length
            ffdc.lenInWords += ffdcDataLenInWords;
            len = sizeof(sbeResponseFfdc_t)/sizeof(uint32_t);
            rc = sbeDownFifoEnq_mult( len, (uint32_t *)&ffdc, type );
            if (rc)
            {
                break;
            }
            //Send HWP internal Data
            rc = sbeDownFifoEnq_mult( ffdcDataLenInWords,
                                      (uint32_t *)&g_FfdcData.ffdcData,
                                      type );
            if (rc)
            {
                break;
            }

        }
        //Send the FFDC data over FIFO.
        rc = sbeFfdcPack.sendOverFIFO(SBE_FFDC_ALL_DUMP, len, true, type);
        if (rc)
        {
            SBE_ERROR(SBE_FUNC "Failed in sendOverFIFO, rc[0x%08X]", rc);
            break;
        }
        rc = sbeDsSendRespHdr(respHdr, NULL, type);

        if (rc)
        {
            SBE_ERROR(SBE_FUNC "Failed in sbeDsSendRespHdr, rc[0x%08X]", rc);
            break;
        }
        // If we are able to send ffdc, turn off async ffdc bit
        (void)SbeRegAccess::theSbeRegAccess().updateAsyncFFDCBit(false);
        SBE_GLOBAL->asyncFfdcRC = FAPI2_RC_SUCCESS;

    }while(0);

    if( rc )
    {
        SBE_ERROR(SBE_FUNC "Failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}

//---------------------------------------------------------------------------
uint32_t sbeSetFFDCAddr(uint8_t *i_pArg)
{
#define SBE_FUNC "sbeSetFFDCAddr"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_fapiRc = FAPI2_RC_SUCCESS;
    sbeSetFFDCAddrReq_t l_req = {};

    do
    {
        // Extract the request
        // and send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(l_req)/sizeof(uint64_t)),
                                    (uint64_t*)&l_req,
                                    true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC "Failed to extract SBE_HOST_PSU_MBOX_REG1 and "
                    "SBE_HOST_PSU_MBOX_REG2");
            break;
        }

        l_req.getFFDCAddr(SBE_GLOBAL->hostFFDCAddr);
        l_req.getPassThroughCmdAddr(SBE_GLOBAL->hostPassThroughCmdAddr);

        SBE_INFO(SBE_FUNC" Global hostFFDCAddr size[0x%08X] Address[0x%08X%08X]",
             static_cast<uint32_t>(SBE_GLOBAL->hostFFDCAddr.size),
             static_cast<uint32_t>(SBE::higher32BWord(SBE_GLOBAL->hostFFDCAddr.addr)),
             static_cast<uint32_t>(SBE::lower32BWord(SBE_GLOBAL->hostFFDCAddr.addr)));
        SBE_INFO(SBE_FUNC" Global hostPassCmdAddr size[0x%08X] Address[0x%08X%08X]",
             static_cast<uint32_t>(SBE_GLOBAL->hostPassThroughCmdAddr.size),
             static_cast<uint32_t>(SBE::higher32BWord(SBE_GLOBAL->hostPassThroughCmdAddr.addr)),
             static_cast<uint32_t>(SBE::lower32BWord(SBE_GLOBAL->hostPassThroughCmdAddr.addr)));

    } while(false);
    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, rc);

    return rc;
    SBE_EXIT(SBE_FUNC);
#undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeStashKeyAddrPair( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeStashKeyAddrPair"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    do
    {
        stashMsg_t l_stashMsg;
        // Send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1, once both
        // key/addr is extracted out of MBOX_REG1 and MBOX_REG2
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                  (sizeof(stashMsg_t)/sizeof(uint64_t)),
                                  (uint64_t*)&l_stashMsg, true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract "
                "SBE_HOST_PSU_MBOX_REG1/SBE_HOST_PSU_MBOX_REG2");
            break;
        }

        SBE_INFO(SBE_FUNC "Key[0x%08X] Addr[0x%08X %08X]",
            l_stashMsg.key, SBE::higher32BWord(l_stashMsg.addr),
            SBE::lower32BWord(l_stashMsg.addr));

        // Update the Key-Addr Pair in local Memory
        bool update = SBE_GLOBAL->sbeKeyAddrPair.updatePair(l_stashMsg.key,
                                                            l_stashMsg.addr);
        if(false == update)
        {
            // Update RC to indicate Host that Stash memory is full
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                SBE_PRI_GENERIC_EXECUTION_FAILURE,
                SBE_SEC_INPUT_BUFFER_OVERFLOW);
            break;
        }
    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);

    SBE_EXIT(SBE_FUNC);
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

        PLAT_ATTR_INIT(fapi2::ATTR_SBE_SYS_CONFIG,
                       fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                       l_sysFabricMap);
    }while(0);
    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, l_fapiRc, l_rc);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeSecurityListBinDump( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeSecurityListBinDump"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;

    do
    {
        uint64_t dumpAddr = 0;
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(dumpAddr)/sizeof(uint64_t)),
                                    &dumpAddr, true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1");
            break;
        }
        SBE_INFO(SBE_FUNC "Security Dump Addr [0x%08X][%08X]",
                 SBE::higher32BWord(dumpAddr),
                 SBE::lower32BWord(dumpAddr));
        /// Initialise the PBA with the above address from stash,
        /// The access API would use it in auto-increment mode.
        p10_PBA_oper_flag pbaFlag;
        pbaFlag.setOperationType(p10_PBA_oper_flag::INJ);
        sbeMemAccessInterface PBAInterface(
                                SBE_MEM_ACCESS_PBA,
                                dumpAddr,
                               &pbaFlag,
                                SBE_MEM_ACCESS_WRITE,
                                sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);

        /// Send TOC of table header count and  list of _T1, _T2 and _T3 data
        fapiRc = SBE_SECURITY::sendSecurityListDumpToHB(&PBAInterface);
        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC"Failed to send security table data to hostboot");
            break;
        }
    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbeGetLpcAliveStatus(uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeGetLpcAliveStatus"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeFifoType type;
    sbeRespGenHdr_t respHdr;
    sbeResponseFfdc_t ffdc;
    respHdr.init();
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();

    uint32_t len2enqueue  = 0;
    uint32_t downFifoRespBuf = LPC_ACCESS_TIMEOUT_FALSE;

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);
        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_EXEC_HWP(fapiRc, lpcTestHwp, procTgt);

        if(fapiRc == (uint32_t)fapi2::RC_LPC_ACCESS_TIMEOUT)
        {
            SBE_ERROR(SBE_FUNC "LPC is not alive, FapiRc[0x%08X]", fapiRc);
            downFifoRespBuf = LPC_ACCESS_TIMEOUT_TRUE;
        }

        //Anything apart from RC_LPC_ACCESS_TIMEOUT
        if((fapiRc != FAPI2_RC_SUCCESS) && (fapiRc != (uint32_t)fapi2::RC_LPC_ACCESS_TIMEOUT))
        {
            SBE_ERROR(SBE_FUNC "LPC Error FapiRc[0x%08X]", fapiRc);
            respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE, SBE_SEC_LPC_ERROR);
            ffdc.setRc(fapiRc);
            break;
        }

        // Push the data into downstream FIFO
        len2enqueue = 1;
        rc = sbeDownFifoEnq_mult (len2enqueue, &downFifoRespBuf, type);
        if (rc)
        {
            // will let command processor routine handle the failure
            break;
        }

    }while(0);

    // Create the Response to caller
    // there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    if(SBE_SEC_OPERATION_SUCCESSFUL == rc)
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc, type);
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

#endif //__SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__
//----------------------------------------------------------------------------
uint32_t sbeFifoQuiesce( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeFifoQuiesce"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t len = 0;
    sbeFifoType type;
    sbeRespGenHdr_t respHdr;
    respHdr.init();

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL, true, false, type);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Suspend async task
        int pkRc = pk_thread_suspend(&SBE_GLOBAL->sbeAsyncCommandProcessor_thread);
        if (pkRc != PK_OK)
        {
            SBE_ERROR(SBE_FUNC "Async thread suspend failed. pkRc:%u", (-pkRc));
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_OS_FAILURE);
            break;
        }

        // Set Quiesce State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(SBE_QUIESCE_EVENT);

    }while(0);

    rc = sbeDsSendRespHdr(respHdr, NULL, type);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC"sbeDsSendRespHdr failed. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbePsuQuiesce( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbePsuQuiesce"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;

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

        // Suspend async task
        int pkRc = pk_thread_suspend(&SBE_GLOBAL->sbeAsyncCommandProcessor_thread);
        if (pkRc != PK_OK)
        {
            SBE_ERROR(SBE_FUNC "async thread suspend failed. pkRc:%u", (-pkRc));
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                    SBE_PRI_GENERIC_EXECUTION_FAILURE,
                    SBE_SEC_OS_FAILURE);
            break;
        }

        // Set Quiesce State
        (void)SbeRegAccess::theSbeRegAccess().stateTransition(
                                          SBE_QUIESCE_EVENT);

    }while(0);

    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC"Failed to write SBE_HOST_PSU_MBOX_REG4. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------
uint32_t sbeReadMem( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeReadMem"
    SBE_ENTER(SBE_FUNC);
#if HOST_INTERFACE_AVAILABLE
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

        p10_PBA_oper_flag l_myPbaFlag;
        l_myPbaFlag.setOperationType(p10_PBA_oper_flag::INJ);

        uint8_t l_coreId = 0;
        FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,plat_getChipTarget(),l_coreId);
        fapi2::Target<fapi2::TARGET_TYPE_CORE> l_core =
              plat_getTargetHandleByInstance<fapi2::TARGET_TYPE_CORE>(l_coreId);
        sbeMemAccessInterface pbaInterface(
                                     SBE_MEM_ACCESS_PBA,
                                     req.responseAddr,
                                     &l_myPbaFlag,
                                     SBE_MEM_ACCESS_WRITE,
                                     sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES,
                                     l_core);
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
#else  //HOST_INTERFACE_AVAILABLE
    uint32_t rc = SBE_SEC_COMMAND_NOT_SUPPORTED;
#endif
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeUpdateOCMBTarget( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeUpdateOCMBTarget"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;

    sbePsuUpdateOcmbTargetReq_t req = {};

    do
    {
        // Extract the request.
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                  (sizeof(req)/sizeof(uint64_t)),
                                  (uint64_t*)&req,
                                   true);

        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC "Failed to extract SBE_HOST_PSU_MBOX_REG1, "
                    "SBE_HOST_PSU_MBOX_REG2 and SBE_HOST_PSU_MBOX_REG3 %d", rc);
            break;
        }

        //update the G_vec list with the req message.
        plat_OCMBTargetsInit(reinterpret_cast<uint8_t *>(&req));

    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC"Failed to write SBE_HOST_PSU_MBOX_REG4. rc[0x%X]", rc);
    }
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
uint32_t sbeHostSyncFabTopologyId( uint8_t *i_pArg )
{
    #define SBE_FUNC "sbeHostSyncFabTopologyId"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    do
    {
        uint64_t topoTabInfoAddr = 0;
        //send Ack to Host via SBE_SBE2PSU_DOORBELL_SET_BIT1
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                    (sizeof(topoTabInfoAddr)/sizeof(uint64_t)),
                                    &topoTabInfoAddr, true);

        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to extract SBE_HOST_PSU_MBOX_REG1"
            " Cannot sync fabric topology table");
            break;
        }

        SBE_INFO(SBE_FUNC "Address to fetch Fabric Topology Table ID values [0x%08X][%08X]",
            SBE::higher32BWord(topoTabInfoAddr),
            SBE::lower32BWord(topoTabInfoAddr));

        //Fetch the Topology ID Table values and update the Attribute
        fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID_TABLE_Type topoTable;
        //Do a PBA read to fetch the topology ID
        p10_PBA_oper_flag pbaFlag;
        pbaFlag.setOperationType(p10_PBA_oper_flag::INJ);
        sbeMemAccessInterface PBAInterForMetadata(
                SBE_MEM_ACCESS_PBA,
                topoTabInfoAddr,
                &pbaFlag,
                SBE_MEM_ACCESS_READ,
                sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES);
        fapiRc = PBAInterForMetadata.accessWithBuffer(&topoTable,
                 sizeof(topoTable),true);

        if(fapiRc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed to fetch the Fabric System Topology ID,fapiRc =0x%.8x"
                      fapiRc);
            break;
        }
        PLAT_ATTR_INIT(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID_TABLE, fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(), topoTable);

    }while(0);
    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}


#endif //not __SBEFW_SEEPROM__
