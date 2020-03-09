/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdregaccess.C $                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }
        if( false == regReqMsg.isValidRequest() )
        {
            SBE_ERROR(SBE_FUNC" Invalid request. core: 0x%02x threadNr:0x%x"
                      " regType:0x%01x numRegs:0x%02x", regReqMsg.coreChiplet,
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
#if 0
        uint8_t core = regReqMsg.coreChiplet;
#ifdef SEEPROM_IMAGE
        RamCore ramCore( plat_getTargetHandleByChipletNumber
                         <fapi2::TARGET_TYPE_CORE>(core),
                         regReqMsg.threadNr );
#endif

        SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_setup)
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" ram_setup failed. threadNr:0x%x"
                      "chipletId:0x%02x", (uint32_t)regReqMsg.threadNr, core);
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            ffdc.setRc(fapiRc);
            break;
        }
        fapi2::buffer<uint64_t> data64;
        uint64_t respData = 0;
        for( uint32_t regIdx = 0; regIdx < regReqMsg.numRegs; regIdx++ )
        {
            SBE_EXEC_HWP(fapiRc, ramCore.get_reg, getRegType(regReqMsg), reqData[regIdx],
                                      &data64, true )
            if( fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" get_reg failed. threadNr:0x%x"
                              "chipletId:0x%02x, regNr:%u regType:%u ",
                              regReqMsg.threadNr, core, reqData[regIdx],
                              regReqMsg.regType);
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
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
                       "chipletId:0x%02x", (uint32_t)regReqMsg.threadNr, core);
             respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
             ffdc.setRc(fapiRc);
         }
#endif
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
        if (rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }
        if( false == regReqMsg.isValidRequest() )
        {
            SBE_ERROR(SBE_FUNC" Invalid request. threadNr:0x%x"
                      " regType:0x%02x numRegs:0x%02x",
                      (uint32_t)regReqMsg.threadNr,
                      (uint32_t)regReqMsg.regType,
                      (uint32_t)regReqMsg.numRegs);
            respHdr.setStatus( SBE_PRI_INVALID_DATA,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
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
#if 0
        uint8_t core = regReqMsg.coreChiplet;
#ifdef SEEPROM_IMAGE
        RamCore ramCore( plat_getTargetHandleByChipletNumber
                         <fapi2::TARGET_TYPE_CORE>(core),
                         regReqMsg.threadNr );
#endif

        SBE_EXEC_HWP_NOARG(fapiRc, ramCore.ram_setup)
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" ram_setup failed. threadNr:0x%x"
                      "chipletId:0x%02x", (uint32_t)regReqMsg.threadNr, core);
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
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
                              "chipletId:0x%02x, regNr:%u regType:%u ",
                              regReqMsg.threadNr, core, regPkg[regIdx].regNr,
                              regReqMsg.regType);
                respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                   SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
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
             SBE_ERROR(SBE_FUNC" ram_cleanup failed. threadNr:0x%x"
                       " chipletId:0x%02x",
                       (uint32_t)regReqMsg.threadNr, core);
             respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
             ffdc.setRc(fapiRc);
         }
#endif
    }while(false);

    if ( SBE_SEC_OPERATION_SUCCESSFUL == rc )
    {
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

