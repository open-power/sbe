/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdregaccess.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2021                        */
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
 * @file: ppe/sbe/sbefw/sbecmdregaccess.C
 *
 * @brief This file contains the SBE Reg Access chipOps
 *
 */

#include "sbecmdregaccess.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "p10_ram_core.H"
#include "chipop_handler.H"
#include "plat_target.H"
#include "sbescom.H"
#include "plat_i2c_access.H"
#include "sbe_sp_intf.H"
#include "sbeutil.H"
#include "sbeSecurity.H"

// PSU ops
#include "sbeHostUtils.H"
#include "sbeglobals.H"

using namespace fapi2;

Enum_RegType getRegType( const sbeRegAccessMsgHdr_t &regReq)
{
    Enum_RegType type = REG_GPR;
    switch( regReq.regType )
    {
        case SBE_REG_ACCESS_SPR:
            type = REG_SPR;
            break;

        case SBE_REG_ACCESS_FPR:
            type = REG_FPR;
            break;
    }
    return type;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetReg(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetReg "
    SBE_ENTER(SBE_FUNC);

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeRegAccessMsgHdr_t regReqMsg;
    uint32_t reqData[SBE_MAX_REG_ACCESS_REGS];
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;
    ReturnCode fapiRc;

    do
    {
        // Get the reg access header
        uint32_t len  = sizeof(sbeRegAccessMsgHdr_t)/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult (len, (uint32_t *)&regReqMsg, false);

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        if( false == regReqMsg.isValidRequest() )
        {
            SBE_ERROR(SBE_FUNC" Invalid request. coreId: 0x%02x threadNr:0x%02x"
                      " regType:0x%01x numRegs:0x%02x", regReqMsg.coreId,
                      regReqMsg.threadNr, regReqMsg.regType, regReqMsg.numRegs);

            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            break;
        }
        len  = regReqMsg.numRegs;
        rc = sbeUpFifoDeq_mult (len, reqData, true);
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            break;
        }
        uint8_t core = regReqMsg.coreId;
        RamCore ramCore( plat_getTargetHandleByInstance
                         <fapi2::TARGET_TYPE_CORE>(core),
                         regReqMsg.threadNr );
        SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_setup)
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" ram_setup failed. threadNr:0x%02x"
                      "coreId:0x%02x", regReqMsg.threadNr, core);
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_RAM_CORE_SETUP_FAILED );
            ffdc.setRc(fapiRc);
            break;
        }
        fapi2::buffer<uint64_t> data64;
        uint64_t respData = 0;
        for( uint32_t regIdx = 0; regIdx < regReqMsg.numRegs; regIdx++ )
        {
            SBE_EXEC_HWP(fapiRc, ramCore.get_reg, getRegType(regReqMsg),
                         reqData[regIdx], &data64, true )
            if( fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" get_reg failed. threadNr:0x%02x "
                    "coreId:0x%02x, regNr:%u regType:%u ", regReqMsg.threadNr,
                    core, reqData[regIdx], regReqMsg.regType);
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_RAM_CORE_ACCESS_FAILED );
                ffdc.setRc(fapiRc);
                break;
            }
            respData = data64;
            // Now enqueue into the downstream FIFO
            len = sizeof( respData )/sizeof(uint32_t);
            rc = sbeDownFifoEnq_mult (len, ( uint32_t *)&respData);
            if (rc)
            {
               break;
            }
         }
         // HWP team does not care about cleanup for failure case.
         // So call cleaup only for success case.
         if( ffdc.getRc() )
         {
             break;
         }
         SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_cleanup)
         if( fapiRc != FAPI2_RC_SUCCESS )
         {
             SBE_ERROR(SBE_FUNC" ram_cleanup failed. threadNr:0x%x"
                 "coreId:0x%02x", regReqMsg.threadNr, core);
             respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                SBE_SEC_RAM_CORE_CLEANUP_FAILED );
             ffdc.setRc(fapiRc);
         }
    }while(false);

    if ( SBE_SEC_OPERATION_SUCCESSFUL == rc )
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutReg(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutReg "
    SBE_ENTER(SBE_FUNC);

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeRegAccessMsgHdr_t regReqMsg;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;
    ReturnCode fapiRc;

    do
    {
        // Get the reg access header
        uint32_t len  = sizeof(sbeRegAccessMsgHdr_t)/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult (len, (uint32_t *)&regReqMsg, false);

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        if( false == regReqMsg.isValidRequest() )
        {
            SBE_ERROR(SBE_FUNC" Invalid request. coreId 0x%02x threadNr:0x%x"
                   " regType:0x%02x numRegs:0x%02x", regReqMsg.coreId,
                   regReqMsg.threadNr, regReqMsg.regType, regReqMsg.numRegs);
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            break;
        }
        sbeRegAccessPackage_t regPkg[SBE_MAX_REG_ACCESS_REGS];
        len  = (sizeof(sbeRegAccessPackage_t)/sizeof(uint32_t)) *
                                                    regReqMsg.numRegs;
        rc = sbeUpFifoDeq_mult (len, (uint32_t *) regPkg,true );
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            break;
        }
        uint8_t core = regReqMsg.coreId;
        RamCore ramCore( plat_getTargetHandleByInstance
                         <fapi2::TARGET_TYPE_CORE>(core),
                         regReqMsg.threadNr );
        SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_setup)
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" ram_setup failed. threadNr:0x%02x"
                "coreId:0x%02x", regReqMsg.threadNr, core);
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_RAM_CORE_SETUP_FAILED );
            ffdc.setRc(fapiRc);
            break;
        }
        fapi2::buffer<uint64_t> data64;
        for( uint32_t regIdx = 0; regIdx < regReqMsg.numRegs; regIdx++ )
        {
            data64 = regPkg[regIdx].getData();
            SBE_EXEC_HWP(fapiRc, ramCore.put_reg, getRegType(regReqMsg),
                                      regPkg[regIdx].regNr,
                                      &data64, true )
            if( fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" get_reg failed. threadNr:0x%x"
                              "coreId:0x%02x, regNr:%u regType:%u ",
                              regReqMsg.threadNr, core, regPkg[regIdx].regNr,
                              regReqMsg.regType);
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_RAM_CORE_ACCESS_FAILED );
                ffdc.setRc(fapiRc);
                break;
            }
         }
         // HWP team does not care about cleanup for failure case.
         // So call cleaup only for success case.
         if( ffdc.getRc() )
         {
             break;
         }
         SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_cleanup)
         if( fapiRc )
         {
             SBE_ERROR(SBE_FUNC" ram_cleanup failed. threadNr:0x%02x"
                 " coreId:0x%02x", regReqMsg.threadNr, core);
             respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                SBE_SEC_RAM_CORE_CLEANUP_FAILED );
             ffdc.setRc(fapiRc);
         }
    }while(false);

    if ( SBE_SEC_OPERATION_SUCCESSFUL == rc )
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetHWReg_Wrap( fapi2::sbefifo_hwp_data_istream& i_getStream,
                           fapi2::sbefifo_hwp_data_ostream& i_putStream )
{
    #define SBE_FUNC " sbeGetHWReg_Wrap "
    SBE_ENTER(SBE_FUNC);

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeGetHWRegReqMsg_t msg;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    uint32_t pibRc = 0;

    do
    {
        // Will attempt to dequeue three entries for the scom addresses plus
        // the expected EOT entry at the end
        uint32_t len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        rc = i_getStream.get(len2dequeue, (uint32_t *)&msg, true, false);

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        uint32_t len2enqueue  = 0;
        uint32_t downFifoRespBuf[2] = {0};

        uint64_t addr = msg.getScomAddr();
        uint64_t scomData = 0;
        SBE_DEBUG(SBE_FUNC " scomAddr[0x%08X %08X]",
            msg.hiAddr, msg.lowAddr);

        //Fetch Target type.
        if(msg.targetType == TARGET_OCMB_CHIP)
        {
            SBE_DEBUG(SBE_FUNC "OCMB GET SCOM");
            //Validate OCMB instance ID.
            if(!msg.validateOCMBInstance())
            {
                SBE_ERROR(SBE_FUNC"Invalid OCMB Instance ID 0x%08X",
                    (uint8_t)msg.targetInstance);
                hdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                              SBE_SEC_INVALID_OCMB_INSTANCE);
                break;
            }
            Target<TARGET_TYPE_OCMB_CHIP> l_hndl = plat_getOCMBTargetHandleByInstance
                  <fapi2::TARGET_TYPE_OCMB_CHIP>(msg.targetInstance);
            if(!l_hndl.get().fields.present)
            {
                SBE_ERROR(SBE_FUNC"OCMB Instance ID 0x%08X is not present",
                    (uint8_t)msg.targetInstance);
                hdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                              SBE_SEC_OCMB_TARGET_NOT_PRESENT);
                break;
            }
            if(!l_hndl.get().fields.functional)
            {
                SBE_ERROR(SBE_FUNC"OCMB Instance ID 0x%08X is not functional",
                    (uint8_t)msg.targetInstance);
                hdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                              SBE_SEC_OCMB_TARGET_NOT_FUNCTIONAL);
                break;
            }

            SBE_DEBUG("OCMB target instance is %d and target is 0x%08X",msg.targetInstance, l_hndl.get());

            //Check if access is allowed.
            uint64_t tempAddr = addr & 0x000000007FFFFFFF;
            sbeRespGenHdr_t *rsp = &hdr;
            CHECK_SBE_OCMB_READ_SECURITY_RC_AND_BREAK_IF_NOT_SUCCESS(
                                        static_cast<uint32_t>(tempAddr),
                                        rsp, i_getStream.getFifoType())

            pibRc = i2cGetScom(&l_hndl, addr, &scomData);
            if(pibRc != 0)
            {
                SBE_ERROR(SBE_FUNC"i2cGetScom failed, scomAddr[0x%08X %08X]",
                    msg.hiAddr, msg.lowAddr);
                hdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                              SBE_SEC_OCMB_SCOM_FAILED);
                ffdc.setRc(pibRc);
                break;
            }
        }
        else if(msg.targetType == TARGET_PROC_CHIP)
        {
            SBE_DEBUG(SBE_FUNC "PROC or Its chiplet GET SCOM");
            checkIndirectAndDoScom( true, addr, scomData, &hdr,
                                   &ffdc, i_getStream.getFifoType());
        }
        else
        {
            SBE_ERROR(SBE_FUNC "Invalid target type[0x%04X] target instance[0x%02X]"
                      (uint32_t)msg.targetType, (uint32_t)msg.targetInstance);
            hdr.setStatus( SBE_PRI_INVALID_DATA,
                           SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;

        }

        // scom failed
        if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC"sbeGetScom failed, scomAddr[0x%08X %08X]",
                msg.hiAddr, msg.lowAddr);
            break;
        }
        else // successful scom
        {
            downFifoRespBuf[0] = (uint32_t)(scomData>>32);
            downFifoRespBuf[1] = (uint32_t)(scomData);

            SBE_DEBUG(SBE_FUNC"getscom succeeds, scomData[0x%08X %08X]",
                downFifoRespBuf[0], downFifoRespBuf[1]);

            // Push the data into downstream FIFO
            len2enqueue = 2;
            rc = i_putStream.put(len2enqueue, &downFifoRespBuf[0]);
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        } // end successful scom

    } while(false);

    if(i_putStream.isStreamRespHeader(hdr.rcStatus(),ffdc.getRc()))
    {
        if(rc == SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Build the response header packet
            rc = sbeDsSendRespHdr(hdr, &ffdc, i_getStream.getFifoType());
            // will let command processor routine handle the failure
        }
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetHWReg(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetHWReg "
    SBE_ENTER(SBE_FUNC);

    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);
    fapi2::sbefifo_hwp_data_ostream ostream(type);
    fapi2::sbefifo_hwp_data_istream istream(type);

    uint32_t rc = sbeGetHWReg_Wrap (istream,ostream);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutHWReg(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutHWReg "
    SBE_ENTER(SBE_FUNC);

    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbePutHWRegReqMsg_t msg;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    sbeFifoType type;
    uint32_t pibRc = 0;

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_ENTER(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Will attempt to dequeue five entries for the scom address
        // (two entries) and the corresponding data (two entries) plus
        // the expected EOT entry at the end

        uint32_t  len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&msg, true, false, type);

        // If FIFO access failure
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        uint64_t scomData = 0;
        scomData = msg.getScomData();

        uint64_t addr = msg.hwRegMsg.getScomAddr();
        SBE_DEBUG(SBE_FUNC " scomAddr[0x%08X %08X]",
            msg.hwRegMsg.hiAddr, msg.hwRegMsg.lowAddr);

        SBE_DEBUG(SBE_FUNC " scomData[0x%08X %08X]",
            msg.hiInputData, msg.lowInputData);

        //Fetch Target type.
        if(msg.hwRegMsg.targetType == TARGET_OCMB_CHIP)
        {
            SBE_DEBUG(SBE_FUNC "OCMB PUT SCOM");
            //Validate OCMB instance ID.
            if(!msg.hwRegMsg.validateOCMBInstance())
            {
                SBE_ERROR(SBE_FUNC"Invalid OCMB Instance ID 0x%08X",
                    (uint8_t)msg.hwRegMsg.targetInstance);
                hdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                              SBE_SEC_INVALID_OCMB_INSTANCE);
                break;
            }
            Target<TARGET_TYPE_OCMB_CHIP> l_hndl = plat_getOCMBTargetHandleByInstance
                  <fapi2::TARGET_TYPE_OCMB_CHIP>(msg.hwRegMsg.targetInstance);
            SBE_DEBUG("OCMB target instance is %d and target is 0x%08X",msg.hwRegMsg.targetInstance, l_hndl.get());

            //Check if access is allowed.
            uint64_t tempAddr = addr & 0x000000007FFFFFFF;
            sbeRespGenHdr_t *rsp = &hdr;
            CHECK_SBE_SECURITY_RC_AND_BREAK_IF_NOT_SUCCESS(
                            static_cast<uint32_t>(tempAddr),
                            SBE_SECURITY::WRITE,
                            rsp, type, 0x00)

            pibRc = i2cPutScom(&l_hndl, addr, scomData);
            if(pibRc != 0)
            {
                SBE_ERROR(SBE_FUNC"i2cPutScom failed, scomAddr[0x%08X %08X]",
                    msg.hwRegMsg.hiAddr, msg.hwRegMsg.lowAddr);
                hdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_OCMB_SCOM_FAILED);
                ffdc.setRc(pibRc);
                break;
            }
        }
        else if(msg.hwRegMsg.targetType == TARGET_PROC_CHIP)
        {
            SBE_DEBUG(SBE_FUNC "PROC or Its chiplet PUT SCOM");
            checkIndirectAndDoScom(true, addr, scomData, &hdr, &ffdc, type);
        }
        else
        {
            SBE_ERROR(SBE_FUNC "Invalid target type[0x%04X] target instance[0x%02X]"
                      (uint32_t)msg.hwRegMsg.targetType, (uint32_t)msg.hwRegMsg.targetInstance);
            hdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;

        }
        // scom failed
        if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC"sbePutHWReg failure data, scomAddr[0x%08X %08X], "
                "scomData[0x%08X %08X]", msg.hwRegMsg.hiAddr, msg.hwRegMsg.lowAddr,
                SBE::higher32BWord(scomData), SBE::lower32BWord(scomData));
            break;
        }
    } while(false);
    if(rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        rc = sbeDsSendRespHdr(hdr, &ffdc, type);
       // will let command processor routine handle the failure
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

uint32_t sbePsuGetHWReg(uint8_t *i_pArg)
{
#define SBE_FUNC " sbePsuGetHWReg "
    SBE_ENTER(SBE_FUNC);
#if HOST_INTERFACE_AVAILABLE
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeGetHWRegReqMsg_t req = {};
    uint32_t fapiRc;
    do
    {
        // Extract the request
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
                                 ((sizeof(req) + sizeof(uint64_t) - 1) /sizeof(uint64_t)),
                                 (uint64_t*)&req,
                                 true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != rc)
        {
            SBE_ERROR(SBE_FUNC "Failed to extract SBE_HOST_PSU_MBOX_REG1 and "
                    "SBE_HOST_PSU_MBOX_REG2");
            break;
        }
        uint64_t addr = req.getScomAddr();
        uint64_t register_data = 0;
        SBE_INFO(SBE_FUNC " getHwReg Addr[0x%08X %08X]",
            req.hiAddr, req.lowAddr);

        //Fetch Target type.
        if(req.targetType == TARGET_OCMB_CHIP)
        {
            Target<TARGET_TYPE_OCMB_CHIP> l_hndl = plat_getOCMBTargetHandleByInstance
                  <fapi2::TARGET_TYPE_OCMB_CHIP>(req.targetInstance);
            SBE_DEBUG("OCMB target instance is %d and target is 0x%08X",req.targetInstance, l_hndl.get());
            fapiRc = i2cGetScom(&l_hndl, addr, &register_data);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                // Primary/Secondary response / FFDC will be set in sbePSUSendResponse
                SBE_ERROR(SBE_FUNC"i2cGetScom failed with RC 0x%8x , scomAddr[0x%08X %08X]",
                    fapiRc, req.hiAddr, req.lowAddr);
                break;
            }
            SBE_DEBUG("data returned is 0x%08x_%08x ",
                     (uint32_t)((register_data) >> 32 ),
                     (uint32_t)(register_data));
        }
        else
        {
            SBE_ERROR(SBE_FUNC "Invalid target type[0x%04X] target instance[0x%02X]"
                      (uint32_t)req.targetType, (uint32_t)req.targetInstance);
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_INVALID_DATA,
                                                    SBE_SEC_INVALID_TARGET_TYPE_PASSED );
            break;
        }

        // get hw reigster operation failed
        if (SBE_GLOBAL->sbeSbe2PsuRespHdr.secStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC"get hw reigster operation failed, hwRegAddr[0x%08X %08X]",
                req.hiAddr, req.lowAddr);
            break;
        }
        else // successful scom
        {
            rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG5,
                                      &register_data,
                                      sizeof(register_data)/sizeof(uint64_t));

            if (rc)
            {
                // will let command processor routine handle the failure
                break;
            }
        }
    }while(0);
    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
#endif
    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}

