/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdsram.C $                            */
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
#include "p10_pm_ocb_init.H"
#include "p10_pm_ocb_indir_setup_linear.H"
#include "p10_pm_ocb_indir_access.H"
#include "p9_perv_scom_addresses.H"

using namespace fapi2;

#ifdef SEEPROM_IMAGE
// Using Function pointer to force long call
p10_pm_ocb_indir_setup_linear_FP_t p10_ocb_setup_linear_access_hwp = &p10_pm_ocb_indir_setup_linear;
p10_pm_ocb_indir_access_FP_t p10_ocb_indirect_access_hwp = &p10_pm_ocb_indir_access;
#endif

///////////////////////////////////////////////////////////////////////
// @brief sbeOccSramAccess_Wrap Occ Sran Access Wrapper function
//
// @param [in] i_isGetFlag Flag to indicate the sram Access Type
//                 true  : GetOccSram ChipOp
//                 false : PutOccSram ChipOp
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeOccSramAccess_Wrap(const bool i_isGetFlag)
{
#if 0
    #define SBE_FUNC " sbeOccSramAccess_Wrap "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;

    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Total Returned length from the procedure
    uint32_t l_totalReturnLen = 0;

    // Create the req struct for the OCC Sram Chip-op
    sbeOccSramAccessReqMsgHdr_t l_req = {0};

    // Check if True - Get / False - Put
    ocb::PM_OCB_ACCESS_OP l_ocb_access =
            (i_isGetFlag)? ocb::OCB_GET : ocb::OCB_PUT;
    // Get the Req Struct Size Data from upstream Fifo
    uint32_t l_len2dequeue  = sizeof(l_req) / sizeof(uint32_t);

    do
    {
        l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                                  (uint32_t *)&l_req,
                                  i_isGetFlag);


        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        SBE_INFO("mode [0x%08X] addr[0x%08X] len[0x%08X]",
             (uint32_t)l_req.mode,
             (uint32_t)l_req.addr,
             (uint32_t)l_req.len);

        // Get the Proc Chip Target to be passed in to the procedure call
        Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc = plat_getChipTarget();

        // Do linear setup for indirect access HWP for Chan0, Chan2 and Chan3
        // For Circular Mode, Chan1 is assumed to be setup by default
        // Linear mode is setup with Linear streaming mode only

        // Sram Access condition to pass valid address during the first access
        bool l_validAddrForFirstAccess = true;

        // Channel Selection based on Mode as well as Fsp attchament
        ocb::PM_OCB_CHAN_NUM l_chan = ocb::OCB_CHAN0;
        switch(l_req.mode)
        {
            case NORMAL_MODE:
                if(false == SbeRegAccess::theSbeRegAccess().isFspSystem())
                {
                    l_chan = ocb::OCB_CHAN2;
                }
                break;

            case DEBUG_MODE:
                l_chan = ocb::OCB_CHAN3;
                break;

            case CIRCULAR_MODE:
                l_chan = ocb::OCB_CHAN1;
                l_validAddrForFirstAccess = false;
                break;

            default:
                SBE_ERROR(SBE_FUNC "Invalid Mode Passed by User");
                l_rc = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
                l_respHdr.setStatus( SBE_PRI_INVALID_DATA, l_rc);
                break;
        }
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        // Check if the access to the address is allowed
        // For read access no checking is required
        if( (l_validAddrForFirstAccess) && !( i_isGetFlag ))
        {
            uint16_t sbeRc = occSramSecRegionManager.isAccessAllowed(
                        {static_cast<uint64_t>(l_req.addr)&(0x00000000FFFFFFFFull),
                        l_req.len,
                        (i_isGetFlag? static_cast<uint8_t>(memRegionMode::READ):
                        static_cast<uint8_t>(memRegionMode::WRITE))});
            if(sbeRc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                l_respHdr.setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED,
                                    sbeRc);
                break;
            }
        }

        // Setup Needs to be called in Normal and Debug Mode only
        if( (l_req.mode == NORMAL_MODE) || (l_req.mode == DEBUG_MODE) )
        {
            SBE_EXEC_HWP(l_fapiRc, p10_ocb_setup_linear_access_hwp,l_proc, l_chan,
                                                    ocb::OCB_TYPE_LINSTR,
                                                    l_req.addr)
            if(l_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_pm_ocb_indir_setup_linear failed, "
                    "Channel[0x%02X] Addr[0x%08X]",
                    l_chan, l_req.addr);

                // Respond with HWP FFDC
                l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                         SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                l_ffdc.setRc(l_fapiRc);
                break;
            }
        }

        //Create a 128 Byte Buffer -  16 64-Bit buffer
        // This is our Granule size as well for this HWP
        uint32_t l_getBuf[SBE_OCC_SRAM_GRANULE/SBE_32BIT_ALIGN_FACTOR] = {};
        uint32_t l_remainingLen = l_req.len; // Initialize with Total Len
        uint32_t l_lenPassedToHwp = 0;
        uint32_t l_actLen = 0;  // Return Len from Hwp not used

        while(l_remainingLen)
        {
            if(l_remainingLen <= SBE_OCC_SRAM_GRANULE)
            {
                l_lenPassedToHwp = l_remainingLen;
            }
            else
            {
                l_lenPassedToHwp = SBE_OCC_SRAM_GRANULE;
            }
            l_remainingLen = l_remainingLen - l_lenPassedToHwp;

            // Fetch buffer from Upstream Fifo for the HWP if it is PutOCC Sram
            if(!i_isGetFlag)
            {
                l_len2dequeue = (l_lenPassedToHwp/SBE_32BIT_ALIGN_FACTOR);
                l_rc = sbeUpFifoDeq_mult ( l_len2dequeue,
                                           l_getBuf,
                                           false);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }

            // Don't need to put any check for Linear/Circular - It's the same
            // API for access, For circular valid address flag is false, Hwp
            // doesn't need the address field from us.
            SBE_EXEC_HWP(l_fapiRc,
                    p10_ocb_indirect_access_hwp,
                    l_proc,
                    l_chan,
                    l_ocb_access, // Get/Put
                    (l_lenPassedToHwp/SBE_64BIT_ALIGN_FACTOR), // 64-bit aligned
                    l_validAddrForFirstAccess, // If requested addr is valid
                    l_req.addr, // Requested Addr being passed
                    l_actLen, // O/p from hwp not used
                    (uint64_t *)l_getBuf) // O/p buffer
            if(l_fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "p10_pm_ocb_indir_access failed, "
                 "Channel[0x%02X] Addr[0x%08X] 64Bit Aligned Len[0x%08X]",
                 l_chan, l_req.addr, (l_lenPassedToHwp/SBE_64BIT_ALIGN_FACTOR));

                // Respond with HWP FFDC
                l_respHdr.setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                         SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                l_ffdc.setRc(l_fapiRc);
                break;
            }

            l_totalReturnLen = l_totalReturnLen + l_lenPassedToHwp;
            // Change this to false, so that Indirect Access Hwp doesn't
            // reset the Address to starting point.
            l_validAddrForFirstAccess = false;

            if(i_isGetFlag) // Get Occ Sram
            {
                l_len2dequeue = (l_lenPassedToHwp/SBE_32BIT_ALIGN_FACTOR);
                // Push this into the downstream FIFO
                l_rc = sbeDownFifoEnq_mult (l_len2dequeue, l_getBuf);
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
            // If there was a HWP failure for put sram occ request,
            // need to Flush out upstream FIFO, until EOT arrives
            if ( l_respHdr.primaryStatus() != SBE_PRI_OPERATION_SUCCESSFUL)
            {
                l_rc = sbeUpFifoDeq_mult(l_len2dequeue, NULL,
                                         true, true);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
            // For other success paths, just attempt to offload
            // the next entry, which is supposed to be the EOT entry
            else
            {
                l_rc = sbeUpFifoDeq_mult(l_len2dequeue, NULL, true);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
        }

        uint32_t l_len = 1;
        // first enqueue the length of data actually written
        l_rc = sbeDownFifoEnq_mult(l_len, (uint32_t *)(&l_totalReturnLen));

        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        l_rc = sbeDsSendRespHdr( l_respHdr, &l_ffdc);
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
#endif
    return 0;
}


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutOccSram (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutOccSram "
    return sbeOccSramAccess_Wrap (false);
    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetOccSram (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetOccSram "
    return sbeOccSramAccess_Wrap (true);
    #undef SBE_FUNC
}

