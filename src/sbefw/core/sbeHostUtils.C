/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeHostUtils.C $                               */
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
/*
 * @file: ppe/sbe/sbefw/sbeHostUtils.C
 *
 * @brief This file contains the PSU Access Utility Functions
 *
 */

#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeHostUtils.H"
#include "sbeHostMsg.H"
#include "sbe_host_intf.H"
#include "sbeerrorcodes.H"
#include "assert.h"
#include "fapi2.H"
#include "sbeglobals.H"
#include "sbeMemAccessInterface.H"
#include "sbeFFDC.H"
#include "hwp_error_info.H"
#include "sberegaccess.H"

///////////////////////////////////////////////////////////////////
// PSU->SBE register access utilities
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeReadPsu2SbeMbxReg (uint32_t       i_addr,
                               const uint8_t  i_count,
                               uint64_t       *o_pData,
                               bool i_isFinalRead)
{
    #define SBE_FUNC " sbeReadPsu2SbeMbxReg "
    SBE_DEBUG(SBE_FUNC"i_count[0x%02X], i_addr=[0x%08X]", i_count, i_addr);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint8_t  l_count = 0;

    assert((i_count >0 ) && (NULL != o_pData))
    assert( (SBE_HOST_PSU_MBOX_REG0 <= i_addr) &&
            (SBE_HOST_PSU_MBOX_REG3 >= (i_addr + i_count - 1)) )

    while (l_count < i_count)
    {
        l_rc = getscom_abs ( i_addr,
                    reinterpret_cast<uint64_t*>(&o_pData[l_count]) );

        if (l_rc)
        {
            // Error while reading from PSU->SBE mbx register
            SBE_ERROR(SBE_FUNC"getscom_abs failed,"
                         "l_rc=[0x%08X], i_addr=[0x%08X]",
                         l_rc, i_addr);
            break;
        }

        SBE_DEBUG(SBE_FUNC"l_data=[0x%08X%08X]", 
                    SBE::higher32BWord(o_pData[l_count]),
                    SBE::lower32BWord(o_pData[l_count]));
        ++l_count;
        ++i_addr;
    }

    // Set the Ack bit in SBE->PSU DB register
    // if the message requires ack and if its a final read operation
    if ((i_isFinalRead) && (SBE_SEC_OPERATION_SUCCESSFUL == l_rc))
    {
        l_rc = sbeAcknowledgeHost();
        if (l_rc)
        {
            SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                    "SBE_SBE2PSU_DOORBELL_SET_BIT1");
        }
    }
    return l_rc;

    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////
// SBE->PSU register access utilities
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeIntrHostUponRespWaiting ()
{
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // Interrupt the host by setting bit0 in SBE->PSU DB register
    // if the caller requested for it.
    if (SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_RESP_REQUIRED)
    {
        l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT0);
    }
    return l_rc;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeAcknowledgeHost ()
{
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    // Set the Ack bit in SBE->PSU DB register
    // if the caller requested for it.
    if (SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_ACK_REQUIRED)
    {
        l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT1);
    }
    return l_rc;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
uint32_t sbeWriteSbe2PsuMbxReg (uint32_t        i_addr,
                                const uint64_t  *i_pData,
                                const uint8_t   i_count,
                                bool            i_setBit0ToIntrHB)
{
    #define SBE_FUNC " sbeWriteSbe2PsuMbxReg "
    SBE_DEBUG(SBE_FUNC"i_count[0x%02X], i_addr=[0x%08X]", i_count, i_addr);
    uint32_t  l_rc   = SBE_SEC_OPERATION_SUCCESSFUL;
    uint8_t   l_count = 0;

    assert( (i_count >0 ) && (NULL != i_pData) )
    assert( (SBE_HOST_PSU_MBOX_REG4 <= i_addr) &&
            (SBE_HOST_PSU_MBOX_REG7 >= (i_addr + i_count - 1)) )

    while (l_count < i_count)
    {
        SBE_DEBUG(SBE_FUNC"l_data=[0x%08X%08X]",
                        SBE::higher32BWord(*(i_pData+l_count)),
                        SBE::lower32BWord(*(i_pData+l_count)));

        l_rc = putscom_abs ( i_addr, *(i_pData+l_count) );
        if (l_rc)
        {
            // Error while reading from PSU->SBE mbx register
            SBE_ERROR(SBE_FUNC"putscom_abs failed,"
                    "l_rc=[0x%08X], i_addr=[0x%08X]",
                    l_rc, i_addr);
            break;
        }

        ++l_count;
        ++i_addr;
    }

    if( SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & SBE_PSU_FLAGS_RESP_REQUIRED )
    {
        if( (i_setBit0ToIntrHB) && (SBE_SEC_OPERATION_SUCCESSFUL == l_rc) )
        {
            // indicate the Host via Bit SBE_SBE2PSU_DOORBELL_SET_BIT0
            l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT0);
            if(l_rc)
            {
                SBE_ERROR(SBE_FUNC " Failed to Sent Ack to Host over "
                        "SBE_SBE2PSU_DOORBELL_SET_BIT0");
            }
        }
    }
    return l_rc;

    #undef SBE_FUNC
}

/* @brief - Send PSU Chip Op response
 *
 * @param[in] - i_sbe2PsuRespHdr - Response header
 * @param[in] - i_fapiRc - fapi rc of the relevant hwp call
 * @param[in/out] - io_rc - rc status of the PSU access utility
 *
 * @return - void
 */
void sbePSUSendResponse(sbeSbe2PsuRespHdr_t &i_sbe2PsuRespHdr,
                        uint32_t &i_fapiRc,
                        uint32_t &io_rc)
{

#define SBE_FUNC "sbePSUSendResponse"
    SBE_ENTER(SBE_FUNC);
    #if HOST_INTERFACE_AVAILABLE
    do
    {
        // Making sure the PSU access utility is functional
        if(io_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            break;
        }

        uint32_t l_allocatedSize = SBE_GLOBAL->hostFFDCAddr.size;
        bool l_is_lastAccess = false;
        // Default EX Target Init..Not changing it for the time being
        fapi2::Target<fapi2::TARGET_TYPE_EX> l_ex(
            fapi2::plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_EX>(
                    sbeMemAccessInterface::PBA_DEFAULT_EX_CHIPLET_ID));
        p10_PBA_oper_flag l_myPbaFlag;
        l_myPbaFlag.setOperationType(p10_PBA_oper_flag::INJ);

        sbeMemAccessInterface l_PBAInterface(
                                     SBE_MEM_ACCESS_PBA,
                                     SBE_GLOBAL->hostFFDCAddr.addr,
                                     &l_myPbaFlag,
                                     SBE_MEM_ACCESS_WRITE,
                                     sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES,
                                     l_ex);

        bool l_internal_ffdc_present = ((i_sbe2PsuRespHdr.primStatus() !=
                                         SBE_PRI_OPERATION_SUCCESSFUL) ||
                                        (i_sbe2PsuRespHdr.secStatus() !=
                                         SBE_SEC_OPERATION_SUCCESSFUL));

        // If no ffdc , exit;
        sbeResponseFfdc_t l_ffdc;
        l_ffdc.setRc(i_fapiRc);
        if(l_ffdc.getRc() != fapi2::FAPI2_RC_SUCCESS)
        {
            // Clear global fapi2::current_err so that
            // FFDC can be sent over PBA interface.
            // We are good with HWP ffdc, as
            // g_FfdcData.fapiRc is a copy of current_err
            fapi2::current_err = fapi2::FAPI2_RC_SUCCESS;

            i_sbe2PsuRespHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                      SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            l_internal_ffdc_present = true;

            SBE_ERROR( SBE_FUNC" FAPI RC:0x%08X", l_ffdc.getRc());
            SBE_INFO(SBE_FUNC" FFDC memory - addr[0x%08X%08X] size[%d]bytes",
                     static_cast<uint32_t>(SBE::higher32BWord(SBE_GLOBAL->hostFFDCAddr.addr)),
                     static_cast<uint32_t>(SBE::lower32BWord(SBE_GLOBAL->hostFFDCAddr.addr)),
                     SBE_GLOBAL->hostFFDCAddr.size);
            uint32_t ffdcDataLenInWords = fapi2::g_FfdcData.ffdcLength
                                            / sizeof(uint32_t);
            // Set failed command information
            l_ffdc.setCmdInfo(i_sbe2PsuRespHdr.seqID(),
                              i_sbe2PsuRespHdr.cmdClass(),
                              i_sbe2PsuRespHdr.command());
            // Add HWP specific ffdc data length
            l_ffdc.lenInWords += ffdcDataLenInWords;

            uint32_t len = sizeof(sbeResponseFfdc_t);
            MEM_AVAILABLE_CHECK(l_allocatedSize, len, l_is_lastAccess);
            fapi2::ReturnCode l_fapiRc = l_PBAInterface.accessWithBuffer(
                                                        &l_ffdc,
                                                        len,
                                                        l_is_lastAccess);
            if(l_fapiRc != fapi2::FAPI2_RC_SUCCESS)
            {
                io_rc = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                break;
            }
            MEM_AVAILABLE_CHECK(l_allocatedSize,
                                ffdcDataLenInWords,
                                l_is_lastAccess);
            l_is_lastAccess = l_is_lastAccess ||
                              !l_internal_ffdc_present ||
                    !SbeRegAccess::theSbeRegAccess().isSendInternalFFDCSet();
            l_fapiRc = l_PBAInterface.accessWithBuffer(
                                                &fapi2::g_FfdcData.ffdcData,
                                                ffdcDataLenInWords,
                                                l_is_lastAccess);
            if(l_fapiRc != fapi2::FAPI2_RC_SUCCESS)
            {
                io_rc = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                break;
            }
        }

        // Send SBE internal ffdc if there is enough memory allocated
        if(l_internal_ffdc_present)
        {
            SBE_ERROR( SBE_FUNC" primaryStatus:0x%08X secondaryStatus:0x%08X",
                       (uint32_t)i_sbe2PsuRespHdr.primStatus(),
                       (uint32_t)i_sbe2PsuRespHdr.secStatus());
            // SBE internal FFDC package
            SbeFFDCPackage sbeFfdc;
            //Generate all the fields of FFDC package
            io_rc = sbeFfdc.sendOverHostIntf(SBE_FFDC_ALL_DUMP,
                                             &l_PBAInterface,
                                             l_allocatedSize);
            if (io_rc)
            {
                break;
            }
        }

        // Send the response header
        io_rc = sbeWriteSbe2PsuMbxReg(SBE_HOST_PSU_MBOX_REG4,
                        (uint64_t*)(&i_sbe2PsuRespHdr),
                        (sizeof(i_sbe2PsuRespHdr)/sizeof(uint64_t)),
                        true);
        if(SBE_SEC_OPERATION_SUCCESSFUL != io_rc)
        {
            SBE_ERROR(SBE_FUNC" Failed to write to "
                    "SBE_HOST_PSU_MBOX_REG4");
        }
    } while(0);
    #else
    io_rc = SBE_SEC_COMMAND_NOT_SUPPORTED;
    #endif
#undef SBE_FUNC
}
