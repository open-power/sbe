/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdsram.C $                            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
 * @file: ppe/sbe/sbefw/sbecmdsram.C
 *
 * @brief This file contains the SBE Sram Access chipOps
 *
 */

#include "sbecmdsram.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sberegaccess.H"
#include "sbeSecureMemRegionManager.H"

#include "fapi2.H"
#include "p10_getsram.H"
#include "p10_putsram.H"
#include "p10_getputsram_utils.H"
#include "chipop_handler.H"
using namespace fapi2;

p10_getsram_FP_t p10_getsram_hwp = &p10_getsram;
p10_putsram_FP_t p10_putsram_hwp = &p10_putsram;

///////////////////////////////////////////////////////////////////////
// @brief sbeSramAccess_Wrap Sram Access Wrapper function
//
// @param[in]  i_getStream      up-stream fifo for chip-op /
//                              memory interface for dump
// @param[in]  i_putStream      down-stream fifo for chip-op /
//                              memory interface for dump
// @param[in]  i_isGetFlag      Flag to indicate the sram Access Type
//                              true  : GetSram ChipOp
//                              false : PutSram ChipOp
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeSramAccess_Wrap(fapi2::sbefifo_hwp_data_istream& i_getStream,
                            fapi2::sbefifo_hwp_data_ostream& i_putStream,
                            const bool i_isGetFlag )
{

    #define SBE_FUNC " sbeSramAccess_Wrap "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Total Returned length from the procedure
    uint32_t l_totalReturnLen = 0;

    // Create the req struct for the SRAM Chip-op
    sbeSramAccessReqMsgHdr_t l_req = {0};

    // Get the Req Struct Size Data from upstream Fifo
    uint32_t l_len2dequeue  = sizeof(l_req) / sizeof(uint32_t);

    do
    {
        l_rc = i_getStream.get( l_len2dequeue,
                                (uint32_t *)&l_req,
                                i_isGetFlag, false );

        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        SBE_INFO("ChipletId:[0x%.8x] McastAccess:[%d] mode [0x%.8X]",
                  (uint32_t)l_req.chipletId,(uint32_t)l_req.multicastAccess,(uint32_t)l_req.mode);
        SBE_INFO("Address:0x%.8x%.8x Length=0x%.8x",l_req.addressWord0,l_req.addressWord1,l_req.length);
        uint64_t addr = ( (uint64_t)l_req.addressWord0 << 32) | l_req.addressWord1;
        if(l_req.isAddr8ByteAligned() == false)
        {
            SBE_ERROR("Input address:0x%.8x%.8x is not 8 byte aligned ",l_req.addressWord0,l_req.addressWord1);
            l_respHdr.setStatus(SBE_PRI_INVALID_DATA, SBE_SEC_INVALID_ADDRESS_PASSED);
            break;
        }
        if(l_req.isLen8ByteAligned() == false)
        {
            SBE_ERROR("Input Length:0x%.8x is not 8 byte aligned ",l_req.length);
            l_respHdr.setStatus(SBE_PRI_INVALID_DATA,SBE_SEC_INVALID_PARAMS);
            break;
        } 
        // Get the Proc Chip Target to be passed in to the procedure call
        Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc = plat_getChipTarget();
        //Create a 128 Byte Buffer -  16 64-Bit buffer
        uint32_t dataBuffer[SBE_SRAM_GRANULE/SBE_32BIT_ALIGN_FACTOR] = {};
        uint32_t l_remainingLen = l_req.length; // Initialize with Total Len
        uint32_t l_lenPassedToHwp = 0;

       //Write or Read in the chunks of 128 bytes
        while(l_remainingLen)
        {
            if(l_remainingLen <= SBE_SRAM_GRANULE)
            {
                l_lenPassedToHwp = l_remainingLen;
            }
            else
            {
                l_lenPassedToHwp = SBE_SRAM_GRANULE;
            }
            l_remainingLen = l_remainingLen - l_lenPassedToHwp;


            uint64_t addrToHwp = addr;
            //For IO-PPE SRAM access,caller will pass valid offset/address to be accessed
            //in the lower 32 bits. Procedure expects valid offset/address information
            //to be in upper 32 bits.Updating the address as per the HWP requirement
            if( ((uint32_t)l_req.chipletId >= PAU0_PERV_CHIPLET_ID) && 
                ((uint32_t)l_req.chipletId <= PAU3_PERV_CHIPLET_ID) )
            {
                addrToHwp = ((uint64_t)addr << 32);
            }

            // Fetch buffer from Upstream Fifo for the HWP if it is Put SRAM
            if(!i_isGetFlag)
            {
                l_len2dequeue = (l_lenPassedToHwp/SBE_32BIT_ALIGN_FACTOR);
                l_rc = i_getStream.get( l_len2dequeue,
                                        dataBuffer,
                                        false, false );
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
                SBE_EXEC_HWP(l_fapiRc,p10_putsram_hwp,l_proc,
                             (uint32_t)l_req.chipletId,
                             l_req.multicastAccess,
                             (uint8_t)l_req.mode,addrToHwp,l_lenPassedToHwp,
                             (uint8_t *)dataBuffer);
                if(l_fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR("Failed in P10 PUTSRAM , ChipletId:[0x%.8x] McastAccess:[%d] mode [0x%.8X]",
                            (uint32_t)l_req.chipletId,(uint32_t)l_req.multicastAccess,(uint32_t)l_req.mode);
                    SBE_ERROR("Address:0x%.8x%.8x Length=0x%.8x",l_req.addressWord0,l_req.addressWord1,l_lenPassedToHwp);
                    // Respond with HWP FFDC
                    l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                        SBE_SEC_PUT_SRAM_FAILED);
                    l_ffdc.setRc(l_fapiRc);
                    break;
                }
            }
            else //GetSRAM
            {
                SBE_EXEC_HWP(l_fapiRc,p10_getsram_hwp,l_proc,(uint32_t)l_req.chipletId,
                             (uint8_t)l_req.mode,addrToHwp,l_lenPassedToHwp,(uint8_t *)dataBuffer);
                if(l_fapiRc != FAPI2_RC_SUCCESS)
                {
                    SBE_ERROR("Failed in P10 GETSRAM , ChipletId:[0x%.8x]  mode [0x%.8X]",
                            (uint32_t)l_req.chipletId ,(uint32_t)l_req.mode);
                    SBE_ERROR("Address:0x%.8x%.8x Length=0x%.8x",l_req.addressWord0,l_req.addressWord1,l_lenPassedToHwp);
                    // Respond with HWP FFDC
                    l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                        SBE_SEC_GET_SRAM_FAILED);
                    l_ffdc.setRc(l_fapiRc);
                    break;
                }

            }
            l_totalReturnLen = l_totalReturnLen + l_lenPassedToHwp;
            //Update the address offset 
            addr += l_lenPassedToHwp;
            if(i_isGetFlag) // Get Sram
            {
                l_len2dequeue = (l_lenPassedToHwp/SBE_32BIT_ALIGN_FACTOR);
                // Push this into the downstream FIFO
                l_rc = i_putStream.put(l_len2dequeue, dataBuffer);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
        } // End of while Put/Get from Hwp
    }while(0);

    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        l_len2dequeue = 0;
        if (!i_isGetFlag)
        {
            // If there was a HWP failure for put sram  request,
            // need to Flush out upstream FIFO, until EOT arrives
            if ( l_respHdr.primaryStatus() != SBE_PRI_OPERATION_SUCCESSFUL)
            {
                l_rc = i_getStream.get( l_len2dequeue, NULL,
                                         true, true );
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
            // For other success paths, just attempt to offload
            // the next entry, which is supposed to be the EOT entry
            else
            {
                l_rc = i_getStream.get( l_len2dequeue, NULL,
                                        true, false );
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
        }

        if(i_putStream.isStreamRespHeader())
        {
            uint32_t l_len = 1;
            // first enqueue the length of data actually written
            l_rc = i_putStream.put(l_len, &l_totalReturnLen);
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            l_rc = sbeDsSendRespHdr( l_respHdr, &l_ffdc,
                                     i_getStream.getFifoType() );
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutSram (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutSram "
    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);

    fapi2::sbefifo_hwp_data_ostream ostream(type);
    fapi2::sbefifo_hwp_data_istream istream(type);

    return sbeSramAccess_Wrap (istream,ostream,false);
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetSram (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetSram "
    chipOpParam_t* configStr = (struct chipOpParam*)i_pArg;
    sbeFifoType type = static_cast<sbeFifoType>(configStr->fifoType);

    fapi2::sbefifo_hwp_data_ostream ostream(type);
    fapi2::sbefifo_hwp_data_istream istream(type);

    return sbeSramAccess_Wrap (istream,ostream,true);
    #undef SBE_FUNC
}

