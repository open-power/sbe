/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmdscomaccess.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2018                        */
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

namespace SBE_COMMON
{
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetScom (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetScom "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    sbeGetScomReqMsg_t l_getScomReqMsg;
    sbeRespGenHdr_t l_hdr;
    l_hdr.init();
    sbeResponseFfdc_t l_ffdc;

    do
    {
        // Will attempt to dequeue two entries for
        // the scom addresses plus the expected
        // EOT entry at the end

        uint32_t l_len2dequeue  = sizeof(l_getScomReqMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_getScomReqMsg);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint32_t l_len2enqueue  = 0;
        uint32_t l_sbeDownFifoRespBuf[2] = {0};

        uint64_t l_addr = ( (uint64_t)l_getScomReqMsg.hiAddr << 32) |
                                      l_getScomReqMsg.lowAddr;
        uint64_t l_scomData = 0;
        SBE_DEBUG(SBE_FUNC"scomAddr[0x%08X%08X]",
                            l_getScomReqMsg.hiAddr, l_getScomReqMsg.lowAddr);
        checkIndirectAndDoScom(true, l_addr,
                               l_scomData, &l_hdr,
                               &l_ffdc);

        if (l_hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL) // scom failed
        {
            SBE_ERROR(SBE_FUNC"getscom failed, "
                "scomAddr[0x%08X%08X]",
                l_getScomReqMsg.hiAddr, l_getScomReqMsg.lowAddr);
            break;
        }
        else // successful scom
        {
            SBE_DEBUG(SBE_FUNC"getscom succeeds, l_scomData[0x%016X]",
                                   l_scomData);

            l_sbeDownFifoRespBuf[0] = (uint32_t)(l_scomData>>32);
            l_sbeDownFifoRespBuf[1] = (uint32_t)(l_scomData);

            // Push the data into downstream FIFO
            l_len2enqueue = 2;
            l_rc = sbeDownFifoEnq_mult (l_len2enqueue,
                                      &l_sbeDownFifoRespBuf[0]);
            if (l_rc)
            {
                // will let command processor routine
                // handle the failure
                break;
            }
        } // end successful scom

    } while(false);

    if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        l_rc = sbeDsSendRespHdr(l_hdr, &l_ffdc);
       // will let command processor routine
       // handle the failure
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
    sbePutScomReqMsg_t l_putScomReqMsg;
    sbeRespGenHdr_t l_hdr;
    l_hdr.init();
    sbeResponseFfdc_t l_ffdc;

    do
    {
        // Will attempt to dequeue four entries for
        // the scom address (two entries) and the
        // corresponding data (two entries) plus
        // the expected EOT entry at the end

        uint32_t  l_len2dequeue  = sizeof(l_putScomReqMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_putScomReqMsg);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        uint64_t l_scomData = 0;
        // successfully dequeued two entries for
        // scom address followed by the EOT entry

        // Data entry 0 : Scom Register Address (0..31)
        // Data entry 1 : Scom Register Address (32..63)
        // Data entry 2 : Scom Register Data (0..31)
        // Data entry 3 : Scom Register Data (32..63)
        // For Direct SCOM, will ignore entry 0

        l_scomData = l_putScomReqMsg.getScomData();

        uint64_t l_addr = ( (uint64_t)  l_putScomReqMsg.hiAddr << 32) |
                                        l_putScomReqMsg.lowAddr;
        SBE_DEBUG(SBE_FUNC"scomAddr[0x%08X%08X]",
                            l_putScomReqMsg.hiAddr, l_putScomReqMsg.lowAddr);
        checkIndirectAndDoScom(false, l_addr,
                               l_scomData, &l_hdr, &l_ffdc);

        if (l_hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL) // scom failed
        {
            SBE_ERROR(SBE_FUNC"putscom failure data, "
                          "scomAddr[0x%08X%08X], "
                          "scomData[0x%08X%08X]",
                          l_putScomReqMsg.hiAddr,
                          l_putScomReqMsg.lowAddr,
                          SBE::higher32BWord(l_scomData),
                          SBE::lower32BWord(l_scomData));
            break;
        }

    } while(false);

    if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        l_rc = sbeDsSendRespHdr(l_hdr, &l_ffdc);
       // will let command processor routine
       // handle the failure
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

    sbeModifyScomReqMsg_t l_modifyScomMsg;
    sbeRespGenHdr_t l_hdr;
    l_hdr.init();
    sbeResponseFfdc_t l_ffdc;

    do
    {
        // Will attempt to dequeue the following entries:
        // Entry 1 : Operation Mode
        // Entry 2 : Scom Register Address (0..31)
        // Entry 3 : Scom Register Address (32..63)
        // Entry 4 : Modifying Data (0..31)
        // Entry 5 : Modifying Data (32..63)
        // Entry 6 : EOT entry at the end

        uint32_t  l_len2dequeue  = sizeof(l_modifyScomMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_modifyScomMsg);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        // Modifying Data
        uint64_t l_modifyingData = l_modifyScomMsg.getModifyingData();

        SBE_DEBUG(SBE_FUNC"OpMode[0x%02X], modifyingData[0x%016X]",
                    l_modifyScomMsg.opMode,
                    SBE::higher32BWord(l_modifyingData),
                    SBE::lower32BWord(l_modifyingData));

        // The following steps need to be done as part of this command :
        //    1. Read Register Data (getscom)
        //    2. 'AND' the Mask with the data read from register
        //    3. 'OR' the modifying data with the result of step 2
        //    4. Write the result of step 3 into the register (putscom)
        do
        {
                // Check for a valid OpMode
                if ( (l_modifyScomMsg.opMode != SBE_MODIFY_MODE_OR)   &&
                     (l_modifyScomMsg.opMode != SBE_MODIFY_MODE_AND)  &&
                     (l_modifyScomMsg.opMode != SBE_MODIFY_MODE_XOR) )
                {
                    // Invalid Data passed
                    SBE_ERROR(SBE_FUNC"Invalid OpMode");
                    l_hdr.setStatus(SBE_PRI_INVALID_DATA,
                                    SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                    break;
                }

                uint64_t l_addr = ( (uint64_t) l_modifyScomMsg.hiAddr << 32) |
                                               l_modifyScomMsg.lowAddr;
                uint64_t l_scomData = 0;
                SBE_DEBUG(SBE_FUNC"scomAddr[0x%08X%08X]",
                              l_modifyScomMsg.hiAddr, l_modifyScomMsg.lowAddr);
                checkIndirectAndDoScom(true, l_addr,
                                      l_scomData, &l_hdr, &l_ffdc);

                if (l_hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL) // scom failed
                {
                    SBE_ERROR(SBE_FUNC"getscom failed,"
                        " ScomAddress[0x%08X %08X]",
                        l_modifyScomMsg.hiAddr, l_modifyScomMsg.lowAddr);
                    break;
                }

                if (l_modifyScomMsg.opMode == SBE_MODIFY_MODE_OR)
                {
                    l_modifyingData |= l_scomData;
                }
                else if (l_modifyScomMsg.opMode == SBE_MODIFY_MODE_AND)
                {
                    l_modifyingData &= l_scomData;
                }
                else
                {
                    l_modifyingData ^= l_scomData;
                }

                // Write the modified data
                checkIndirectAndDoScom(false, l_addr,
                                      l_modifyingData, &l_hdr, &l_ffdc);

                if (l_hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL) // scom failed
                {
                    SBE_ERROR(SBE_FUNC"putscom failed,"
                        " ScomAddress[0x%08X%08X]",
                        l_modifyScomMsg.hiAddr, l_modifyScomMsg.lowAddr);
                    SBE_ERROR(SBE_FUNC"modifyingData[0x%08X%08X]",
                              SBE::higher32BWord(l_modifyingData),
                              SBE::lower32BWord(l_modifyingData));
                    break;
                }
        } while (false);

        if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Build the response header packet
            l_rc = sbeDsSendRespHdr(l_hdr, &l_ffdc);
            if (l_rc)
            {
               // will let command processor routine
               // handle the failure
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
    sbePutScomUnderMaskReqMsg_t l_putScomUmaskMsg;
    sbeRespGenHdr_t l_hdr;
    l_hdr.init();
    sbeResponseFfdc_t l_ffdc;

    do
    {
        // Will attempt to dequeue the following entries:
        // Entry 1 : Scom Register Address (0..31)
        // Entry 2 : Scom Register Address (32..63)
        // Entry 3 : Modifying Data (0..31)
        // Entry 4 : Modifying Data (32..63)
        // Entry 5 : Mask Data (0..31)
        // Entry 6 : Mask Data (32..63)
        // Entry 7 : EOT entry at the end

        uint32_t  l_len2dequeue  = sizeof(l_putScomUmaskMsg)/sizeof(uint32_t);
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                                  (uint32_t *)&l_putScomUmaskMsg);

        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        SBE_DEBUG(SBE_FUNC"scomAddr[0x%08X%08X],"
                     "modifyingData[0x%08X%08X]",
                      l_putScomUmaskMsg.hiAddr,
                      l_putScomUmaskMsg.lowAddr,
                      l_putScomUmaskMsg.hiInputData,
                      l_putScomUmaskMsg.lowInputData);
        SBE_INFO(SBE_FUNC"maskData[0x%08X%08X]",
                      l_putScomUmaskMsg.hiMaskData,
                      l_putScomUmaskMsg.lowMaskData);

        // PutScomUnderMask formula:
        // dest_reg = (dest_reg & ~input_mask) | (input_data & input_mask)

        do
        {
            uint64_t l_scomData = 0;

            uint64_t l_addr = ( (uint64_t) l_putScomUmaskMsg.hiAddr << 32) |
                                           l_putScomUmaskMsg.lowAddr;
            checkIndirectAndDoScom(true, l_addr,
                                   l_scomData, &l_hdr, &l_ffdc);

            if (l_hdr.secondaryStatus() == SBE_SEC_OPERATION_SUCCESSFUL) // scom success
            {
                l_putScomUmaskMsg.getScomData(l_scomData);

                // Write the modified data
                checkIndirectAndDoScom(false, l_addr,
                                       l_scomData, &l_hdr, &l_ffdc);
            }

            if (l_hdr.secondaryStatus() != SBE_SEC_OPERATION_SUCCESSFUL) // scom failed
            {
                SBE_ERROR(SBE_FUNC"scom failed, "
                    "ScomAddress[0x%08X%08X]",
                    l_putScomUmaskMsg.hiAddr,
                    l_putScomUmaskMsg.lowAddr);
                SBE_ERROR(SBE_FUNC"modifyingData[0x%08X%08X]"
                    "maskData[0x%08X%08X]",
                    l_putScomUmaskMsg.hiInputData,
                    l_putScomUmaskMsg.lowInputData,
                    l_putScomUmaskMsg.hiMaskData,
                    l_putScomUmaskMsg.lowMaskData);

                break;
            }
        } while (false);
    } while(false);

    if(l_rc == SBE_SEC_OPERATION_SUCCESSFUL)
    {
        // Build the response header packet
        l_rc = sbeDsSendRespHdr(l_hdr, &l_ffdc);
       // will let command processor routine
       // handle the failure
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
