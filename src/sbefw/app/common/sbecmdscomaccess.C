/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdscomaccess.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2020                        */
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
 * @file: ppe/sbe/sbefw/sbecmdscomaccess.C
 *
 * @brief This file contains the SBE SCOM Access chipOps
 *
 */

#include "sbecmdscomaccess.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbescom.H"
#include "sbeutil.H"
#include "sbeFifoMsgUtils.H"
#include "plat_hw_access.H"
#include "sbeglobals.H"
#include "chipop_handler.H"

namespace SBE_COMMON
{
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeGetScomReqMsg_t msg;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    sbeFifoType type;

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Will attempt to dequeue two entries for the scom addresses plus
        // the expected EOT entry at the end
        uint32_t len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&msg, true, false, type);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint32_t len2enqueue  = 0;
        uint32_t downFifoRespBuf[2] = {0};

        uint64_t addr = ( (uint64_t)msg.hiAddr << 32) | msg.lowAddr;
        uint64_t scomData = 0;
        SBE_DEBUG(SBE_FUNC "sbeGetScom scomAddr[0x%08X%08X]",
            msg.hiAddr, msg.lowAddr);

        checkIndirectAndDoScom(true, addr, scomData, &hdr, &ffdc, type);

        // scom failed
        if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC"sbeGetScom failed, scomAddr[0x%08X%08X]",
                msg.hiAddr, msg.lowAddr);
            break;
        }
        else // successful scom
        {
            downFifoRespBuf[0] = (uint32_t)(scomData>>32);
            downFifoRespBuf[1] = (uint32_t)(scomData);

            SBE_DEBUG(SBE_FUNC"getscom succeeds, scomData[0x%08X%08X]",
                downFifoRespBuf[0], downFifoRespBuf[1]);

            // Push the data into downstream FIFO
            len2enqueue = 2;
            l_rc = sbeDownFifoEnq_mult (len2enqueue, &downFifoRespBuf[0], type);
            if (l_rc)
            {
                // will let command processor routine handle the failure
                break;
            }
        } // end successful scom

    } while(false);

    if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        l_rc = sbeDsSendRespHdr(hdr, &ffdc, type);
       // will let command processor routine handle the failure
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbePutScomReqMsg_t msg;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    sbeFifoType type;
    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Will attempt to dequeue four entries for the scom address
        // (two entries) and the corresponding data (two entries) plus
        // the expected EOT entry at the end

        uint32_t  len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&msg, true, false, type);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint64_t scomData = 0;
        // successfully dequeued two entries for
        // scom address followed by the EOT entry

        // Data entry 0 : Scom Register Address (0..31)
        // Data entry 1 : Scom Register Address (32..63)
        // Data entry 2 : Scom Register Data (0..31)
        // Data entry 3 : Scom Register Data (32..63)
        // For Direct SCOM, will ignore entry 0

        scomData = msg.getScomData();

        uint64_t addr = ( (uint64_t)  msg.hiAddr << 32) | msg.lowAddr;
        SBE_DEBUG(SBE_FUNC "sbePutScom scomAddr[0x%08X%08X]",
            msg.hiAddr, msg.lowAddr);

        checkIndirectAndDoScom(false, addr, scomData, &hdr, &ffdc, type);

        // scom failed
        if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC"sbePutScom failure data, scomAddr[0x%08X%08X], "
                "scomData[0x%08X%08X]", msg.hiAddr, msg.lowAddr,
                SBE::higher32BWord(scomData), SBE::lower32BWord(scomData));
            break;
        }

    } while(false);

    if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        l_rc = sbeDsSendRespHdr(hdr, &ffdc, type);
       // will let command processor routine handle the failure
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeModifyScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeModifyScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeModifyScomReqMsg_t msg;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    sbeFifoType type;

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Will attempt to dequeue the following entries:
        // Entry 1 : Operation Mode
        // Entry 2 : Scom Register Address (0..31)
        // Entry 3 : Scom Register Address (32..63)
        // Entry 4 : Modifying Data (0..31)
        // Entry 5 : Modifying Data (32..63)
        // Entry 6 : EOT entry at the end

        uint32_t  len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&msg, true, false, type);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        // Modifying Data
        uint64_t modifyData = msg.getModifyingData();

        SBE_DEBUG(SBE_FUNC"OpMode[0x%02X], modifyingData[0x%08X%08X]",
            msg.opMode, SBE::higher32BWord(modifyData),
            SBE::lower32BWord(modifyData));

        // The following steps need to be done as part of this command :
        //    1. Read Register Data (getscom)
        //    2. 'AND' the Mask with the data read from register
        //    3. 'OR' the modifying data with the result of step 2
        //    4. Write the result of step 3 into the register (putscom)
        do
        {
            // Check for a valid OpMode
            if ( (msg.opMode != SBE_MODIFY_MODE_OR)   &&
                 (msg.opMode != SBE_MODIFY_MODE_AND)  &&
                 (msg.opMode != SBE_MODIFY_MODE_XOR) )
            {
                // Invalid Data passed
                SBE_ERROR(SBE_FUNC "sbeModifyScom Invalid OpMode");
                hdr.setStatus(SBE_PRI_INVALID_DATA,
                                SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                break;
            }

            uint64_t addr = ( (uint64_t) msg.hiAddr << 32) | msg.lowAddr;
            uint64_t scomData = 0;
            SBE_DEBUG(SBE_FUNC "sbeModifyScom scomAddr[0x%08X%08X]",
                msg.hiAddr, msg.lowAddr);

            checkIndirectAndDoScom(true, addr, scomData, &hdr, &ffdc, type);

            // scom failed
            if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC "sbeModifyScom getscom failed,"
                    " ScomAddress[0x%08X %08X]", msg.hiAddr, msg.lowAddr);
                break;
            }

            if (msg.opMode == SBE_MODIFY_MODE_OR)
            {
                modifyData |= scomData;
            }
            else if (msg.opMode == SBE_MODIFY_MODE_AND)
            {
                modifyData &= scomData;
            }
            else
            {
                modifyData ^= scomData;
            }

            // Write the modified data
            checkIndirectAndDoScom(false, addr, modifyData, &hdr, &ffdc, type);

            // scom failed
            if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC "sbeModifyScom putscom failed,"
                    " ScomAddress[0x%08X%08X]", msg.hiAddr, msg.lowAddr);
                SBE_ERROR(SBE_FUNC"sbeModifyScom modifyData[0x%08X%08X]",
                    SBE::higher32BWord(modifyData), SBE::lower32BWord(modifyData));
                break;
            }
        } while (false);

        if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Build the response header packet
            l_rc = sbeDsSendRespHdr(hdr, &ffdc, type);
            if (l_rc)
            {
               // will let command processor routine handle the failure
               break;
            }
        }

    } while(false);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutScomUnderMask (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutScomUnderMask "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbePutScomUnderMaskReqMsg_t msg;
    sbeRespGenHdr_t hdr;
    hdr.init();
    sbeResponseFfdc_t ffdc;
    sbeFifoType type;

    do
    {
        chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
        type = static_cast<sbeFifoType>(configStr->fifoType);
        SBE_DEBUG(SBE_FUNC "Fifo Type is:[%02X]",type);

        // Will attempt to dequeue the following entries:
        // Entry 1 : Scom Register Address (0..31)
        // Entry 2 : Scom Register Address (32..63)
        // Entry 3 : Modifying Data (0..31)
        // Entry 4 : Modifying Data (32..63)
        // Entry 5 : Mask Data (0..31)
        // Entry 6 : Mask Data (32..63)
        // Entry 7 : EOT entry at the end

        uint32_t  len2dequeue  = sizeof(msg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (len2dequeue, (uint32_t *)&msg, true, false, type);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        SBE_DEBUG(SBE_FUNC "scomAddr[0x%08X%08X], modifyingData[0x%08X%08X]",
            msg.hiAddr, msg.lowAddr, msg.hiInputData, msg.lowInputData);
        SBE_INFO(SBE_FUNC "maskData[0x%08X%08X]", msg.hiMaskData, msg.lowMaskData);

        // PutScomUnderMask formula:
        // dest_reg = (dest_reg & ~input_mask) | (input_data & input_mask)

        do
        {
            uint64_t scomData = 0;
            uint64_t addr = ( (uint64_t) msg.hiAddr << 32) | msg.lowAddr;
            checkIndirectAndDoScom(true, addr, scomData, &hdr, &ffdc, type);

            // scom success
            if (hdr.secondaryStatus() == SBE_SEC_OPERATION_SUCCESSFUL)
            {
                msg.getScomData(scomData);

                // Write the modified data
                checkIndirectAndDoScom(false, addr, scomData, &hdr, &ffdc, type,
                                       msg.getInputMask());
            }

            // scom failed
            if (hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC"scom failed, ScomAddress[0x%08X%08X]",
                    msg.hiAddr, msg.lowAddr);
                SBE_ERROR(SBE_FUNC"modifyData[0x%08X%08X] maskData[0x%08X%08X]",
                    msg.hiInputData, msg.lowInputData, msg.hiMaskData, msg.lowMaskData);
                break;
            }
        } while (false);
    } while(false);

    if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        l_rc = sbeDsSendRespHdr(hdr, &ffdc, type);
       // will let command processor routine handle the failure
    }

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeMultiScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeMultiScom "
    return 0;
    #undef SBE_FUNC
}
} // namespace SBE_COMMON
