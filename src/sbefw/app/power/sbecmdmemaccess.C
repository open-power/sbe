/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdmemaccess.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
 * @file: ppe/sbe/sbefw/sbecmdmemaccess.C
 *
 * @brief This file contains the SBE Memory Access chipOps
 *
 */

#include "sbecmdmemaccess.H"
#include "sbefifo.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sbeutil.H"
#include "sbeHostUtils.H"
#include "sbeglobals.H"
#include "sbeSecureMemRegionManager.H"

#include "fapi2.H"

#include "sbeMemAccessInterface.H"

using namespace fapi2;

// Buffer requirement for ADU and PBA on the stack
constexpr uint32_t MAX_ADU_BUFFER       = 5; // 40bytes
constexpr uint32_t MAX_PBA_BUFFER       = 32;

// Multiplier factor with respect to the FIFO length
constexpr uint32_t ADU_SIZE_MULTIPLIER_FOR_LEN_ALIGNMENT = 2;
constexpr uint32_t PBA_SIZE_MULTIPLIER_FOR_LEN_ALIGNMENT = 32;

/**
 * @brief static definition of parameters passed in adu chip-ops
 */
constexpr uint32_t SBE_ADU_LOCK_TRIES = 3;

#ifndef __SBEFW_SEEPROM__
///////////////////////////////////////////////////////////////////////
// @brief align4ByteWordLength - Internal Method to this file
//        Align the length passed and return number of words
//
// @param [in] i_len, length pass from user in Bytes
//
// @return  Number of words (number of 4byte length)
///////////////////////////////////////////////////////////////////////
inline uint32_t align4ByteWordLength(uint32_t i_len)
{
    if(i_len % 4 != 0)
    {
        i_len = i_len + (4 - (i_len % 4));
    }
    return(i_len/4);
}

///////////////////////////////////////////////////////////////////////
// @brief calInterAduLenForUpFifo - Internal Method to this file
//        Calculate Intermediate Adu Data length for Upstream Fifo
//
// @param [in] i_mod, Modulus number from user, (from numGranules % 4)
//                    could be any values from 1,2,3
// @param [in] i_itag, Itag flag
// @param [in] i_ecc, Ecc flag
//
// @return  length in bytes for intermediate ADU length
///////////////////////////////////////////////////////////////////////
inline uint32_t calInterAduLenForUpFifo(uint8_t i_mod, bool i_itag, bool i_ecc)
{
    //Default init length with either Ecc or Itag
    uint32_t l_len =
        ((sbeMemAccessInterface::ADU_GRAN_SIZE_BYTES + 1) * (1 + i_mod));
    // If ECC and iTag bit is also part of the buffer
    if(i_itag && i_ecc)
    {
        l_len = l_len + (1 + i_mod);
    }
    return (l_len);
}

///////////////////////////////////////////////////////////////////////
// @brief sbeAduLenInUpStreamFifo - Internal Method to this file
//        Calculate the Final Size which is write/read to/from HWP
//
// @param [in] i_numGranules, Number of granules read/write
// @param [in] i_granuleSize
// @param [in] i_itag, Itag flag
// @param [in] i_ecc, Ecc flag
//
// @return  Length in bytes for ADU to be put in Upstream FIFO
///////////////////////////////////////////////////////////////////////
inline uint32_t sbeAduLenInUpStreamFifo(uint32_t i_numGranules,
                                        uint32_t i_granuleSize,
                                        bool i_itag,
                                        bool i_ecc)
{
    uint32_t l_respLen = i_numGranules * i_granuleSize;
    if(i_itag)
    {
        // Add one byte for Itag for Each Granule Completed
        l_respLen = l_respLen + i_numGranules;
    }
    if(i_ecc)
    {
        // Add one byte for Ecc for Each Granule Completed
        l_respLen = l_respLen + i_numGranules;
    }
    return l_respLen;
}

///////////////////////////////////////////////////////////////////////
// @brief flushUpstreamFifo - Internal Method to this file, to flush
//        out the upstream fifo
//
// @param [in] i_primaryStatus, Fapi RC
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
inline uint32_t flushUpstreamFifo (const uint32_t &i_primaryStatus)
{
    uint32_t l_len2dequeue = 0;
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    if ( i_primaryStatus != SBE_PRI_OPERATION_SUCCESSFUL)
    {
        l_rc = sbeUpFifoDeq_mult(l_len2dequeue, NULL,
                             true, true);
    }
    // For other success paths, just attempt to offload
    // the next entry, which is supposed to be the EOT entry
    else
    {
        l_rc = sbeUpFifoDeq_mult(l_len2dequeue, NULL, true);
    }
    return l_rc;
}

///////////////////////////////////////////////////////////////////////
// @brief processPbaRequest - Internal Method to this file,
//        To process the PBA Access request
//
// @param [in] i_hdr, Message Request Header
// @param [in] i_isFlagRead, Read/Write Flag
//
// @return  RC from the method
///////////////////////////////////////////////////////////////////////
uint32_t processPbaRequest(const sbeMemAccessReqMsgHdr_t &i_hdr,
                           const bool i_isFlagRead)
{
    #define SBE_FUNC " processPbaRequest "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Default for PBA
    uint32_t l_granuleSize = sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES;
    uint64_t l_addr = 0;
    // Keeps track of number of granules sent to HWP
    uint64_t l_granulesCompleted = 0;
    // Input Data length in alignment with PBA (128 Bytes)
    uint64_t l_lenCacheAligned = 0;

    do
    {
        // If not Host Pass through command, simply set the addr from the command
        if(!i_hdr.isPbaHostPassThroughModeSet())
        {
            l_addr = i_hdr.getAddr();
            // Check if the access to the address is allowed
            uint16_t sbeRc = mainStoreSecMemRegionManager.isAccessAllowed(
                                               {l_addr,
                                                i_hdr.len,
                 (i_isFlagRead ? static_cast<uint8_t>(memRegionMode::READ):
                                 static_cast<uint8_t>(memRegionMode::WRITE))});
            if(sbeRc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                l_respHdr.setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED,
                                    sbeRc);
                break;
            }
        }
        // If it is Host Pass through command, set the address using the Host pass
        // through address already set in the global and using the addr here in the
        // command as index to that address
        else
        {
            l_addr = SBE_GLOBAL->hostPassThroughCmdAddr.addr + i_hdr.getAddr();
            // Check if the size pass in the command is less than the size mentioned
            // in the Host Pass Through globals
            if((i_hdr.getAddr() + i_hdr.len) > SBE_GLOBAL->hostPassThroughCmdAddr.size)
            {
                // Break out, Invalid Size
                SBE_ERROR("User size[0x%08X] exceeds the Host Pass Through Mode "
                    "size[0x%08X] Start Index[0x%08X %08X]", 
                    i_hdr.len, SBE_GLOBAL->hostPassThroughCmdAddr.size,
                    SBE::higher32BWord(i_hdr.getAddr()),
                    SBE::lower32BWord(i_hdr.getAddr()));
                l_respHdr.setStatus( SBE_PRI_INVALID_DATA,
                                     SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            }
        }

        // Default EX Target Init..Not changing it for the time being
        Target<TARGET_TYPE_EX> l_ex(
                plat_getTargetHandleByChipletNumber<TARGET_TYPE_EX>(
                    sbeMemAccessInterface::PBA_DEFAULT_EX_CHIPLET_ID));

        p10_PBA_oper_flag l_myPbaFlag;
        // Determine the access flags
        // Fast mode flag
        if(i_hdr.isFastModeSet())
        {
            l_myPbaFlag.setFastMode(true);
            SBE_INFO(SBE_FUNC "Fast Mode is set");
        }

        // inject mode flag
        if(i_hdr.isPbaInjectModeSet())
        {
            l_myPbaFlag.setOperationType(p10_PBA_oper_flag::INJ); // Inject operation
            SBE_INFO(SBE_FUNC "inject Mode is set");
        }

        // By default, ex_chipletId printed below won't be used unless accompanied
        // by LCO_mode (LCO Mode for PBA-Put)
        if(i_hdr.isPbaLcoModeSet())
        {
            SBE_INFO(SBE_INFO "LCO Mode is set with Ex ChipletId[%d]",
                (i_hdr.coreChipletId)/2);
            //Derive the EX target from the input Core Chiplet Id
            //Core0/1 -> EX0, Core2/3 -> EX1, Core4/5 -> EX2, Core6/7 -> EX3
            //..so on
            l_ex = plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_EX>
                    (i_hdr.coreChipletId);
            l_myPbaFlag.setOperationType(p10_PBA_oper_flag::LCO); // LCO operation
        }

        l_lenCacheAligned = i_hdr.getDataLenCacheAlign();
        SBE_DEBUG(SBE_FUNC "Data Aligned Len / Number of data granules = %d",
            l_lenCacheAligned);

        sbeMemAccessInterface l_PBAInterface(SBE_MEM_ACCESS_PBA,
                                             l_addr,
                                             &l_myPbaFlag,
                                             (i_isFlagRead ?
                                              SBE_MEM_ACCESS_READ:
                                              SBE_MEM_ACCESS_WRITE),
                                             l_granuleSize,
                                             l_ex);

        while (l_granulesCompleted < l_lenCacheAligned)
        {
            // Breaking out here if invalid size
            if(l_respHdr.primaryStatus() != SBE_PRI_OPERATION_SUCCESSFUL)
            {
                break;
            }

            // If this is putmem request, read input data from the upstream FIFO
            if (!i_isFlagRead)
            {
                // l_sizeMultiplier * 4B Upstream FIFO = Granule size 128B
                uint32_t l_len2dequeue = sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES
                                                                / sizeof(uint32_t);
                l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                        (uint32_t *)l_PBAInterface.getBuffer(),
                        false);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }

            // Call the PBA HWP
            l_fapiRc = l_PBAInterface.accessGranule(
                                l_granulesCompleted==(l_lenCacheAligned-1));
            // if error
            if(l_fapiRc != FAPI2_RC_SUCCESS)
            {
                // Respond with HWP FFDC
                l_respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
                l_ffdc.setRc(l_fapiRc);
                break;
            }

            // If this is a getmem request,
            // need to push the data into the downstream FIFO
            if (i_isFlagRead)
            {
                // Number of 4Bytes to put, to align with Granule Size
                // l_len*4 = Granule Size
                uint32_t l_len = sbeMemAccessInterface::PBA_GRAN_SIZE_BYTES
                                                                / sizeof(uint32_t);
                l_rc = sbeDownFifoEnq_mult (l_len,
                                    (uint32_t *)l_PBAInterface.getBuffer());
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }

            l_granulesCompleted++;
        } // End..while (l_granulesCompleted < l_lenCacheAligned);
    } while(0);

    // Now build and enqueue response into downstream FIFO
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        // If there was a HWP failure for putmem request,
        // need to Flush out upstream FIFO, until EOT arrives
        if (!i_isFlagRead)
        {
            l_rc = flushUpstreamFifo(l_respHdr.primaryStatus());
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        }

        // first enqueue the length of data actually written
        uint32_t l_len = 1;
        uint32_t l_respLen = l_granulesCompleted * l_granuleSize;

        SBE_INFO(SBE_FUNC "Total length Pushed for ChipOp [%d]", l_respLen);
        l_rc = sbeDownFifoEnq_mult ( l_len, &l_respLen );
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        l_rc = sbeDsSendRespHdr( l_respHdr, &l_ffdc);
        // Indicate the host in put pass through mode via Interrupt
        if(!l_rc && i_hdr.isPbaHostPassThroughModeSet() && !i_isFlagRead)
        {
            l_rc = sbeSetSbe2PsuDbBitX(SBE_SBE2PSU_DOORBELL_SET_BIT4);
        }
    } while(false);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief processAduRequest - Internal Method to this file,
//        To process the ADU Access request
//
// @param [in] i_hdr, Message Request Header
// @param [in] i_isFlagRead, Read/Write Flag
//
// @return  RC from the method
///////////////////////////////////////////////////////////////////////
#define IS_ONE_BIT_SET(x)               ((x & (x-1)) == 0)
#define GET_8_BYTE_ALIGNED_OFFSET(x)    ((x & 0x07) * 8)
uint32_t processAduRequest(const sbeMemAccessReqMsgHdr_t &i_hdr,
                           const bool i_isFlagRead)
{
    #define SBE_FUNC " processAduRequest "
    SBE_ENTER(SBE_FUNC);
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Default for ADU
    uint32_t l_sizeMultiplier = ADU_SIZE_MULTIPLIER_FOR_LEN_ALIGNMENT;
    uint32_t l_granuleSize = sbeMemAccessInterface::ADU_GRAN_SIZE_BYTES;

    // Keeps track of number of granules sent to HWP
    uint64_t l_granulesCompleted = 0;
    adu_operationFlag l_aduFlag;
    // For local Use
    bool l_isEccMode = i_hdr.isEccFlagSet();
    bool l_isItagMode = i_hdr.isItagFlagSet();
    uint64_t l_addr = i_hdr.getAddr();

    do
    {
        l_aduFlag.setTransactionSize(adu_operationFlag::TSIZE_8);
        // For len lesser than 8, only 1,2 and 4 lengths are allowed
        if(i_hdr.len < 8)
        {
            if(IS_ONE_BIT_SET(i_hdr.len) == false)
            {
                l_respHdr.setStatus(SBE_PRI_INVALID_DATA,
                                    SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
                SBE_ERROR(SBE_FUNC"Invalid length[%d] - "
                                  "supported values[1/2/4/multiples of 8]",
                                  i_hdr.len);
                break;
            }
            l_sizeMultiplier = 1;
            l_granuleSize = i_hdr.len;
            l_aduFlag.setTransactionSize((adu_operationFlag::Transaction_size_t)(i_hdr.len));
        }
        //Default Operation Type is DMA_PARTIAL
        l_aduFlag.setOperationType(adu_operationFlag::DMA_PARTIAL);
        l_aduFlag.setLockControl(false);
        l_aduFlag.setOperFailCleanup(true);
        l_aduFlag.setNumLockAttempts(SBE_ADU_LOCK_TRIES);

        // Fast Mode / Ecc mode / Cache Inhibit Mode / Auto Increment
        // required in ADU operations.
        if(i_hdr.isFastModeSet())
        {
            l_aduFlag.setFastMode(true);
        }
        if(i_hdr.isItagFlagSet())
        {
            l_aduFlag.setItagMode(true);
        }
        if(i_hdr.isAutoIncrModeSet())
        {
            l_aduFlag.setAutoIncrement(true);
        }

        if(!i_isFlagRead) // ECC override in write mode
        {
            if(i_hdr.isEccOverrideFlagSet())
            {
                l_aduFlag.setEccItagOverrideMode(true);
                l_aduFlag.setEccMode(true);
            }
        }
        else // ECC required in read mode
        {
            if(i_hdr.isEccFlagSet())
            {
                l_aduFlag.setEccMode(true);
            }
        }

        i_hdr.setADUOperationType(l_aduFlag);

        // Input Data length in alignment with ADU
        // 1 for 1/2/4 Bytes else number of multiples of 8 Bytes
        uint64_t l_lenCacheAligned = i_hdr.getDataLenCacheAlign();
        SBE_DEBUG(SBE_FUNC "User length [%d], Data Aligned Len / "
                "Number of data granules = %d",
                i_hdr.len, l_lenCacheAligned);

        sbeMemAccessInterface l_ADUInterface(SBE_MEM_ACCESS_ADU,
                                             l_addr,
                                             &l_aduFlag,
                                             (i_isFlagRead ?
                                              SBE_MEM_ACCESS_READ :
                                              SBE_MEM_ACCESS_WRITE),
                                    sbeMemAccessInterface::ADU_GRAN_SIZE_BYTES);
        if(false == i_hdr.isTrustedOp())
        {
            // Check if the access to the address is allowed
            uint16_t sbeRc = mainStoreSecMemRegionManager.isAccessAllowed(
                                               {l_addr,
                                                i_hdr.len,
                 (i_isFlagRead ? static_cast<uint8_t>(memRegionMode::READ):
                                 static_cast<uint8_t>(memRegionMode::WRITE))});
            if(sbeRc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                l_respHdr.setStatus(SBE_PRI_UNSECURE_ACCESS_DENIED,
                                    sbeRc);
                break;
            }
        }
        // 8Byte granule for ADU access
        uint64_t l_dataFifo[MAX_ADU_BUFFER] = {0};
        while (l_granulesCompleted < l_lenCacheAligned)
        {
            // With ECC or ITAG the output length of a granule will become
            // 9 bytes instead of 8, To align it we will merge 4 output granule
            // before putting it in the Downstream FIFO i.e. 9*4 = 36Bytes
            // which is 4Bytes aligned size.
            // With Both ECC and ITag, the o/p length of a granule will become
            // 10Bytes instead of 8, To align it we will merge 4 output granule
            // before putting it in the Downstream FIFO i.e. 10*4 = 40bytes
            // So in ADU Read case we will use the same buffer with 10Bytes
            // offset to fill the 40bytes.

            // Both Ecc and ITag Present = 40Bytes is the alignment length
            /* D[00] D[01] D[02] D[03] D[04] D[05] D[06] D[07] -> 8 Data Bytes
             * D[08-Itag] D[09-Ecc] D[0a] D[0b] D[0c] D[0d] D[0e] D[0f] -> 6D B
             * D[10] D[11] D [12-Itag] D[13-Ecc] D[14] D[15] D[16] D[17]
             * D[18] D[19] D[1a] D[1b] D[1c-Itag] D[1d-Ecc] D[1e] D[1f]
             * D[20] D[21] D[22] D[23] D[24] D[25] D[26-Itag] D[27-Ecc]
             */
            // Only Ecc Present = 36 Bytes is the alignment length
            /* D[00] D[01] D[02] D[03] D[04] D[05] D[06] D[07] -> 8 Data Bytes
             * D[08-Ecc] D[09] D[0a] D[0b] D[0c] D[0d] D[0e] D[0f] -> 7D B
             * D[10] D[11-Ecc] D [12] D[13] D[14] D[15] D[16] D[17]
             * D[18] D[19] D[1a-Ecc] D[1b] D[1c] D[1d] D[1e] D[1f]
             * D[20] D[21] D[22] D[23-Ecc]
             */
            // Only ITag Present = 36 Bytes is the alignment length
            /* D[00] D[01] D[02] D[03] D[04] D[05] D[06] D[07] -> 8 Data Bytes
             * D[08-Itag] D[09] D[0a] D[0b] D[0c] D[0d] D[0e] D[0f] -> 7D B
             * D[10] D[11-Itag] D [12] D[13] D[14] D[15] D[16] D[17]
             * D[18] D[19] D[1a-Itag] D[1b] D[1c] D[1d] D[1e] D[1f]
             * D[20] D[21] D[22] D[23-Itag]
             */
            uint8_t l_bufIdx = 0;

            // If this is putmem request, read input data from the upstream FIFO
            if (!i_isFlagRead)
            {
                // l_sizeMultiplier * 4B Upstream FIFO = Granule size 8B
                uint32_t l_len2dequeue = l_sizeMultiplier;
                l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                        (uint32_t *)&l_dataFifo,
                        false);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
                SBE_DEBUG("l_dataFifo#1 0x%08x%08x", SBE::higher32BWord(l_dataFifo[0]),
                                                    SBE::lower32BWord(l_dataFifo[0]));
                // For lengths 1,2 and 4, data needs to be shift-aligned to
                // 8-byte address boundary.
                l_dataFifo[0] >>= GET_8_BYTE_ALIGNED_OFFSET(l_addr);
                SBE_DEBUG("l_dataFifo#2 0x%08x%08x", SBE::higher32BWord(l_dataFifo[0]),
                                                    SBE::lower32BWord(l_dataFifo[0]));

                // Insert the ECC if ECC Mode is set
                if(l_isEccMode)
                {
                    uint8_t l_eccPos = 8;
                    if(l_isItagMode)
                    {
                        l_eccPos = 9;
                    }
                    ((uint8_t*)&l_dataFifo)[l_eccPos] = i_hdr.eccByte;
                }
            }
            else
            {
                //Adu Read Mode - with either ECC or ITag or Both
                // Calculate the MODULUS
                uint8_t l_mod = (l_granulesCompleted % 4);
                if( (l_mod) && ((l_isEccMode) || (l_isItagMode)) )
                {
                    // Default Init it for 1byte extra
                    l_bufIdx = (sbeMemAccessInterface::ADU_GRAN_SIZE_BYTES
                                * l_mod) + l_mod;
                    if((l_isEccMode) && (l_isItagMode))
                    {
                        l_bufIdx = l_bufIdx + l_mod;
                    }
                }
            }
            l_fapiRc = l_ADUInterface.accessWithBuffer(
                                &(((uint8_t *)&(l_dataFifo))[l_bufIdx]),
                                sbeMemAccessInterface::ADU_GRAN_SIZE_BYTES,
                                (l_granulesCompleted == (l_lenCacheAligned-1)));
            // if error
            if( (l_fapiRc != FAPI2_RC_SUCCESS) )
            {
                // Respond with HWP FFDC
                l_respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                     SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
                l_ffdc.setRc(l_fapiRc);
                break;
            }

            // If this is a getmem request,
            // need to push the data into the downstream FIFO
            if (i_isFlagRead)
            {
                // Number of 4Bytes to put, to align with Granule Size
                uint32_t l_len = l_sizeMultiplier; // l_len*4 = Granule Size

                //Enter the below 'if' if ADU Read Mode with Either Ecc or ITag
                //or both set. During non-aligned Transaction (but not the last)
                //then set the len as zero so as to skip the unalign byte send,
                //during next transaction when the data is aligned it will take
                //care of sending all granules.
                //If the below condition is not met then for ADU Read Mode will
                //happen to write on DownStream FIFO for each granule.

                if((l_isEccMode) || (l_isItagMode))
                {
                    //Calculate the MODULUS
                    uint8_t l_mod = (l_granulesCompleted % 4);
                    if( (l_mod == 3) ||
                            ((l_granulesCompleted+1) == l_lenCacheAligned) )
                    {
                        l_len = calInterAduLenForUpFifo(l_mod,l_isItagMode,
                                                        l_isEccMode);
                        l_len = align4ByteWordLength(l_len);
                    }
                    else
                    {
                        // If it is not the last granule or on the 4th entry
                        // into the data buffer, need not send it to Upstream
                        // Fifo
                        l_len = 0;
                    }
                }
                if(l_len)
                {
                    SBE_DEBUG("l_dataFifo#3 0x%08x%08x", SBE::higher32BWord(l_dataFifo[0]),
                                                        SBE::lower32BWord(l_dataFifo[0]));
                    // For lengths 1,2 and 4, data needs to be extracted from
                    // shift-aligned 8-byte address boundary.
                    l_dataFifo[0] <<= GET_8_BYTE_ALIGNED_OFFSET(l_addr);
                    SBE_DEBUG("l_dataFifo#4 0x%08x%08x", SBE::higher32BWord(l_dataFifo[0]),
                                                        SBE::lower32BWord(l_dataFifo[0]));

                    l_rc = sbeDownFifoEnq_mult (l_len, (uint32_t *)&l_dataFifo);
                    CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
                }
            }
            l_addr += l_granuleSize;
            l_granulesCompleted++;
        } // End..while (l_granulesCompleted < l_lenCacheAligned);
    } while(false);

    // Now build and enqueue response into downstream FIFO
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        // If there was a HWP failure for putmem request,
        // need to Flush out upstream FIFO, until EOT arrives
        if (!i_isFlagRead)
        {
            l_rc = flushUpstreamFifo(l_respHdr.primaryStatus());
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        }

        // first enqueue the length of data actually written
        uint32_t l_len = 1;
        uint32_t l_respLen = sbeAduLenInUpStreamFifo(
                                       l_granulesCompleted,
                                       l_granuleSize,
                                       l_isItagMode,
                                       l_isEccMode);

        SBE_INFO(SBE_FUNC "Total length Pushed for ChipOp [%d]", l_respLen);
        l_rc = sbeDownFifoEnq_mult ( l_len, &l_respLen );
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

        l_rc = sbeDsSendRespHdr( l_respHdr, &l_ffdc);
    } while(false);

    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeMemAccess_Wrap Memory Access Wrapper function
//
// @param [in] i_isFlagRead Flag to indicate the memory Access Type
//                 true  : GetMem ChipOp
//                 false : PutMem ChipOp
//
// @return  RC from the method
///////////////////////////////////////////////////////////////////////
uint32_t sbeMemAccess_Wrap(const bool i_isFlagRead)
{
    #define SBE_FUNC " sbeMemAccess_Wrap "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    // Create an instance of Memory Access ChipOp structure
    sbeMemAccessReqMsgHdr_t l_req = {0};

    // Offload the common header from the Upstream FIFO
    uint32_t l_len2dequeue  = sizeof(l_req) / sizeof(uint32_t);
    l_rc = sbeUpFifoDeq_mult(l_len2dequeue, (uint32_t *)&l_req, i_isFlagRead);

    if(!l_rc)
    {
        // Calculate the PBA/ADU address from the given input
        SBE_INFO(SBE_FUNC "Address Upper[0x%08X] Lower[0x%08X] Flags[0x%08X] "
            "Length[0x%08X]", ((l_req.getAddr()>>32) & 0xFFFFFFFF),
            (l_req.getAddr() & 0xFFFFFFFF), l_req.flags, l_req.len);

        // PBA
        bool l_isPBA = l_req.isPbaFlagSet();
        if(l_isPBA)
        {
            l_rc = processPbaRequest(l_req, i_isFlagRead);
            if(l_rc)
            {
                SBE_ERROR(SBE_FUNC "processPbaRequest failed");
            }
        }
        // ADU
        else
        {
            l_rc = processAduRequest(l_req, i_isFlagRead);
            if(l_rc)
            {
                SBE_ERROR(SBE_FUNC "processAduRequest failed");
            }
        }
    }
    // If there was a FIFO error, will skip sending the response,
    // instead give the control back to the command processor thread
    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutMem (uint8_t *i_pArg)
{
    return sbeMemAccess_Wrap (false);
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetMem (uint8_t *i_pArg)
{
    return sbeMemAccess_Wrap (true);
}
#endif //not __SBEFW_SEEPROM__
#ifdef __SBEFW_SEEPROM__

uint32_t sbeUpdateMemAccessRegion (uint8_t *i_pArg)
{
    #define SBE_FUNC "sbeManageMemAccessRegion"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_GENERIC_FAILURE_IN_EXECUTION;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    sbeMemRegionReq_t req = {};

    do
    {
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

        SBE_INFO(SBE_FUNC" Addr[0x%08x%08x] size[0x%08x] flags[0x%04x]",
                  SBE::higher32BWord(req.startAddress),
                  SBE::lower32BWord(req.startAddress),
                  req.size,
                  SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags);

        uint16_t mode = SBE_GLOBAL->sbePsu2SbeCmdReqHdr.flags & 0x00FF;
        uint16_t sbeRc = SBE_SEC_OPERATION_SUCCESSFUL;
        if(mode == SBE_MEM_REGION_CLOSE)
        {
            sbeRc = mainStoreSecMemRegionManager.remove(req.startAddress);
        }
        else
        {
            uint8_t memMode = 0;
            if(mode == SBE_MEM_REGION_OPEN_RO)
            {
                memMode = static_cast<uint8_t>(memRegionMode::READ);
            }
            else if(mode == SBE_MEM_REGION_OPEN_RW)
            {
                memMode = static_cast<uint8_t>(memRegionMode::READ) |
                          static_cast<uint8_t>(memRegionMode::WRITE);
            }
            sbeRc = mainStoreSecMemRegionManager.add(req.startAddress,
                                        req.size,
                                        memMode);
        }
        if(sbeRc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(SBE_PRI_USER_ERROR,
                                                    sbeRc);
        }
    } while(false);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
#endif //__SBEFW_SEEPROM__
