/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbecmdmemaccess.C $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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

#include "fapi2.H"

#include "p9_pba_setup.H"
#include "p9_adu_setup.H"
#include "p9_pba_access.H"
#include "p9_adu_access.H"


using namespace fapi2;

// Buffer requirement for ADU and PBA on the stack
static const uint32_t MAX_ADU_BUFFER       = 40;
static const uint32_t MAX_PBA_BUFFER       = 32;
// PBA / ADU Granule size as per the HWP Requirement
static const uint32_t PBA_GRAN_SIZE_BYTES  = 128;
static const uint32_t ADU_GRAN_SIZE_BYTES  = 8;

// Multiplier factor with respect to the FIFO length
static const uint32_t ADU_SIZE_MULTIPLIER_FOR_LEN_ALIGNMENT = 2;
static const uint32_t PBA_SIZE_MULTIPLIER_FOR_LEN_ALIGNMENT = 32;

//Default EX Target ChipletId to be used in PBA by default
static const uint32_t PBA_DEFAULT_EX_CHIPLET_ID = 7;

/**
 * @brief static definition of parameters passed in adu chip-ops
 */
static const uint32_t SBE_ADU_LOCK_TRIES = 3;

// Transaction size (choice is 1, 2, 4, or 8)
// 0b00: TSIZE_1
// 0b01: TSIZE_2
// 0b10: TSIZE_4
// 0b11: TSIZE_8
static const uint32_t SBE_ADU_TRANSACTION_SIZE = 3;
static const bool     SBE_ADU_LEAVE_DIRTY_BOOL = false;
static const bool     SBE_ADU_LOCK_PICK_BOOL   = false;

/**
 * @brief Mask used to build the Flag struct for ADU chip-op
 **/
static const uint32_t ADU_LOCK_TRIES_SHIFT        = 16;
static const uint32_t ADU_TRANSACTION_SIZE_SHIFT  = 20;
static const uint32_t ADU_ECC_OVERRIDE_BIT_SHIFT  = 22;
static const uint32_t ADU_ECC_REQUIRED_BIT_SHIFT  = 23;
static const uint32_t ADU_ITAG_REQUIRED_BIT_SHIFT = 24;
static const uint32_t ADU_FAST_MODE_SHIFT         = 25;
static const uint32_t ADU_LEAVE_DIRTY_SHIFT       = 26;
static const uint32_t ADU_LOCK_PICK_SHIFT         = 27;
static const uint32_t ADU_AUTO_INCR_SHIFT         = 28;
static const uint32_t CACHE_INHIBIT_MODE_SHIFT    = 29;

// Fast Mode bit shift for PBA
static const uint32_t PBA_FAST_MODE_SHIFT         = 31;

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
        ((ADU_GRAN_SIZE_BYTES + 1) * (1 + i_mod));
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
// @param [in] i_itag, Itag flag
// @param [in] i_ecc, Ecc flag
//
// @return  Length in bytes for ADU to be put in Upstream FIFO
///////////////////////////////////////////////////////////////////////
inline uint32_t sbeAduLenInUpStreamFifo(uint32_t i_numGranules,
                                        bool i_itag,
                                        bool i_ecc)
{
    uint32_t l_respLen = i_numGranules * ADU_GRAN_SIZE_BYTES;
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
// @param [in] i_fapiRc, Fapi RC
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
inline uint32_t flushUpstreamFifo (const uint32_t &i_fapiRc)
{
    uint32_t l_len2dequeue = 0;
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    if ( i_fapiRc != FAPI2_RC_SUCCESS )
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
// @brief constructAduFlag - Internal Method to this file, to construct
//        ADU flag as the HWP expects
//
// @param [in] i_hdr, Message Request Header
// @param [in] i_isFlagRead, Read/Write Flag
//
// @return Constructed ADU Flag
///////////////////////////////////////////////////////////////////////
uint32_t constructAduFlag(const sbeMemAccessReqMsgHdr_t & i_hdr,
                          const bool i_isFlagRead)
{
    #define SBE_FUNC " constructAduFlag"

    // Fast Mode / Ecc mode / Cache Inhibit Mode / Auto Increment
    // required in ADU operations.
    bool l_isFastMode = i_hdr.isFastModeSet();
    bool l_isCacheInhibitMode = i_hdr.isCacheInhibitModeFlagSet();
    bool l_isItagBit = i_hdr.isItagFlagSet();
    bool l_isAutoIncr = i_hdr.isAutoIncrModeSet();
    bool l_isEccRequiredMode = false;
    bool l_isEccOverrideMode = false;

    if(!i_isFlagRead) // ECC override in write mode
    {
        l_isEccOverrideMode = i_hdr.isEccOverrideFlagSet();
        if(l_isEccOverrideMode)
        {
            l_isEccRequiredMode = true;
        }
    }
    else // ECC required in read mode
    {
        l_isEccRequiredMode = i_hdr.isEccFlagSet();
    }

    // Construct the flag required for adu setup
    uint32_t l_aduSetupFlag =
            ( (l_isCacheInhibitMode << CACHE_INHIBIT_MODE_SHIFT) |
                    // 3-bit of Cache mode placed in 31-30-29 bits
              (l_isAutoIncr << ADU_AUTO_INCR_SHIFT) |
                    // 1-bit Auto increment placed at 28th bit
              (SBE_ADU_LOCK_PICK_BOOL << ADU_LOCK_PICK_SHIFT) |
                    // 1-bit pick lock placed at 27th bit
              (SBE_ADU_LEAVE_DIRTY_BOOL << ADU_LEAVE_DIRTY_SHIFT) |
                    // 1-bit leave dirty placed at 26th bit
              (l_isFastMode << ADU_FAST_MODE_SHIFT) |
                    // 1-bit Fast mode placed at 25th bit
              (l_isItagBit << ADU_ITAG_REQUIRED_BIT_SHIFT) |
                    // 1-bit itag placed at 24th bit
              (l_isEccRequiredMode << ADU_ECC_REQUIRED_BIT_SHIFT) |
                    // 1-bit ecc required at 23rd bit
              (l_isEccOverrideMode << ADU_ECC_OVERRIDE_BIT_SHIFT) |
                    // 1-bit ecc override  at 22nd bit
              (SBE_ADU_TRANSACTION_SIZE << ADU_TRANSACTION_SIZE_SHIFT) |
                    // 2-bit Transcation size at 21-20th bits
              (SBE_ADU_LOCK_TRIES << ADU_LOCK_TRIES_SHIFT) );
                    // 4-bit Lock Tries at 19-18-17-16 bits

    SBE_INFO(SBE_FUNC "Cache[%d] Itag[%d] AutoIncr[%d] FastMode[%d] ",
        l_isCacheInhibitMode,l_isItagBit,l_isAutoIncr,l_isFastMode);
    SBE_INFO(SBE_FUNC "EccRequiredMode[%d] EccOverrideMode[%d] EccOverrideByte"
        "[0x%02X] AduSetupFlag[0x%04X]",l_isEccRequiredMode,l_isEccOverrideMode,
        i_hdr.eccByte, l_aduSetupFlag);

    return (l_aduSetupFlag);
    #undef SBE_FUNC
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
    uint32_t l_sizeMultiplier = PBA_SIZE_MULTIPLIER_FOR_LEN_ALIGNMENT;
    uint32_t l_granuleSize = PBA_GRAN_SIZE_BYTES;
    uint64_t l_addr = i_hdr.getAddr();

    // Proc Chip Target
    Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc = plat_getChipTarget();
    // Default EX Target Init..Not changing it for the time being
    Target<fapi2::TARGET_TYPE_EX > l_ex((uint64_t)PBA_DEFAULT_EX_CHIPLET_ID);

    // Determine the access flags
    // Fast mode flag
    bool l_isFastMode = i_hdr.isFastModeSet();
    //LCO Mode for PBA-Put
    bool l_isLcoMode = i_hdr.isPbaLcoModeSet();
    if(l_isLcoMode)
    {
        //Derive the EX target from the input Core Chiplet Id
        //Core0/1 -> EX0, Core2/3 -> EX1, Core4/5 -> EX2, Core6/7 -> EX3
        //..so on
        l_ex = plat_getTargetHandleByChipletNumber<fapi2::TARGET_TYPE_EX>
                (i_hdr.coreChipletId);
    }
    // By default, ex_chipletId printed below won't be used unless accompanied
    // by LCO_mode.
    SBE_INFO(SBE_FUNC "FAST_Mode[%d] LCO_Mode[%d] EX_ChipletId[%d]",
        l_isFastMode, l_isLcoMode, (i_hdr.coreChipletId)/2);

    // The max granule size for which the ADU/PBA interface if configured
    uint32_t l_numGranules = 0;
    // Keeps track of number of granules sent to HWP
    uint64_t l_granulesCompleted = 0;

    // Input Data length in alignment with PBA (128 Bytes)
    uint64_t l_lenCacheAligned = i_hdr.getDataLenCacheAlign();
    SBE_DEBUG(SBE_FUNC "Data Aligned Len / Number of data granules = %d",
        l_lenCacheAligned);

    while (l_granulesCompleted < l_lenCacheAligned)
    {
        // Call the PBA setup HWP
        l_fapiRc = p9_pba_setup(
                        l_proc,
                        l_ex,
                        l_addr,
                        i_isFlagRead,
                        ((l_isFastMode) ? (1<<PBA_FAST_MODE_SHIFT) : 0),
                        l_numGranules);

        // if p9_pba_setup returns error
        if(l_fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" p9_pba_setup Failed");
            // Respond with HWP FFDC
            l_respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                 SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            l_ffdc.setRc(l_fapiRc);
            break;
        }

        // Assumption is Hwp won't return zero for Num Granules
        assert(0 != l_numGranules);

        SBE_INFO(SBE_FUNC "Hwp returned l_numGranules=[0x%08X]",l_numGranules);
        uint64_t l_numAcc = 0;

        if (l_numGranules < (l_lenCacheAligned - l_granulesCompleted))
        {
            l_numAcc = l_numGranules;
        }
        else
        {
            l_numAcc = l_lenCacheAligned - l_granulesCompleted;
        }

        // 128Byte granule for PBA access
        uint32_t l_dataFifo[MAX_PBA_BUFFER] = {0};

        uint64_t l_numCurrAcc = 0;
        while (l_numCurrAcc < l_numAcc)
        {
            bool l_lastGran =  (l_numCurrAcc == (l_numAcc-1)) ? true : false;
            bool l_firstGran = (l_numCurrAcc == 0) ? true : false;

            // If this is putmem request, read input data from the upstream FIFO
            if (!i_isFlagRead)
            {
                // l_sizeMultiplier * 4B Upstream FIFO = Granule size 128B
                uint32_t l_len2dequeue = l_sizeMultiplier;
                l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                        (uint32_t *)&l_dataFifo,
                        false);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
            // Call PBA access
            l_fapiRc = p9_pba_access(
                           l_proc,
                           l_addr,
                           i_isFlagRead,
                           ((l_isFastMode) ? (1<<PBA_FAST_MODE_SHIFT) : 0),
                           l_firstGran,
                           l_lastGran,
                           (uint8_t *)&l_dataFifo);

            // if p9_pba_access returns error
            if( l_fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" p9_pba_access Failed");
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
                l_rc = sbeDownFifoEnq_mult (l_len, (uint32_t *)&l_dataFifo);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
            }
            l_granulesCompleted++;
            l_numCurrAcc++;
        } // End inner while loop

        if ( (l_fapiRc != FAPI2_RC_SUCCESS) ||
             (l_rc != SBE_SEC_OPERATION_SUCCESSFUL) )
        {
            break;
        }
        l_addr += l_granuleSize * l_granulesCompleted;
    } // End..while (l_granulesCompleted < l_lenCacheAligned);

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
            l_rc = flushUpstreamFifo(l_fapiRc);
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        }

        // first enqueue the length of data actually written
        uint32_t l_len = 1;
        uint32_t l_respLen = l_granulesCompleted * l_granuleSize;

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
// @brief processAduRequest - Internal Method to this file,
//        To process the ADU Access request
//
// @param [in] i_hdr, Message Request Header
// @param [in] i_isFlagRead, Read/Write Flag
//
// @return  RC from the method
///////////////////////////////////////////////////////////////////////
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
    uint32_t l_granuleSize = ADU_GRAN_SIZE_BYTES;

    // Adu Setup Flag
    uint32_t l_aduSetupFlags = constructAduFlag(i_hdr, i_isFlagRead);

    // For local Use
    bool l_isEccMode = i_hdr.isEccFlagSet();
    bool l_isItagMode = i_hdr.isItagFlagSet();
    uint64_t l_addr = i_hdr.getAddr();

    // Proc Chip Target
    Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc = plat_getChipTarget();

    // The max granule size for which the ADU/PBA interface if configured
    uint32_t l_numGranules = 0;
    // Keeps track of number of granules sent to HWP
    uint64_t l_granulesCompleted = 0;

    // Input Data length in alignment with PBA (128 Bytes)
    uint64_t l_lenCacheAligned = i_hdr.getDataLenCacheAlign();
    SBE_DEBUG(SBE_FUNC "Data Aligned Len / Number of data granules = %d",
        l_lenCacheAligned);

    while (l_granulesCompleted < l_lenCacheAligned)
    {
        // Call the ADU setup HWP
        l_fapiRc = p9_adu_setup (l_proc,
                                 l_addr,
                                 i_isFlagRead,
                                 l_aduSetupFlags,
                                 l_numGranules);
        // if p9_adu_setup returns error
        if( (l_fapiRc != FAPI2_RC_SUCCESS) )
        {
            SBE_ERROR(SBE_FUNC" p9_adu_setup Failed");
            // Respond with HWP FFDC
            l_respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                 SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            l_ffdc.setRc(l_fapiRc);
            break;
        }
        // Assumption is Hwp won't return zero for Num Granules
        assert(0 != l_numGranules);

        SBE_INFO(SBE_FUNC "Hwp returned l_numGranules=[0x%08X]",l_numGranules);
        uint64_t l_numAcc = 0;

        if (l_numGranules < (l_lenCacheAligned - l_granulesCompleted))
        {
            l_numAcc = l_numGranules;
        }
        else
        {
            l_numAcc = l_lenCacheAligned - l_granulesCompleted;
        }

        // 8Byte granule for ADU access
        uint32_t l_dataFifo[MAX_ADU_BUFFER] = {0};

        uint64_t l_numCurrAcc = 0;
        while (l_numCurrAcc < l_numAcc)
        {
            bool l_lastGran =  (l_numCurrAcc == (l_numAcc-1)) ? true : false;
            bool l_firstGran = (l_numCurrAcc == 0) ? true : false;

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
                // l_sizeMultiplier * 4B Upstream FIFO = Granule size 128B
                uint32_t l_len2dequeue = l_sizeMultiplier;
                l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                        (uint32_t *)&l_dataFifo,
                        false);
                CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);

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
                uint8_t l_mod = (l_numCurrAcc % 4);
                if( (l_mod) && ((l_isEccMode) || (l_isItagMode)) )
                {
                    // Default Init it for 1byte extra
                    l_bufIdx = (ADU_GRAN_SIZE_BYTES * l_mod) + l_mod;
                    if((l_isEccMode) && (l_isItagMode))
                    {
                        l_bufIdx = l_bufIdx + l_mod;
                    }
                }
            }

            // Call ADU access HWP for ADU write/read request
            l_fapiRc = p9_adu_access (
                            l_proc,
                            l_addr,
                            i_isFlagRead,
                            l_aduSetupFlags,
                            l_firstGran,
                            l_lastGran,
                            &(((uint8_t *)&(l_dataFifo))[l_bufIdx]));
            // if p9_adu_access returns error
            if( l_fapiRc != FAPI2_RC_SUCCESS )
            {
                SBE_ERROR(SBE_FUNC" p9_adu_access Failed");
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

                //Calculate the MODULUS
                uint8_t l_mod = (l_numCurrAcc % 4);
                if((l_isEccMode) || (l_isItagMode))
                {
                    if( (l_mod == 3) || (l_lastGran) )
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
                    l_rc = sbeDownFifoEnq_mult (l_len, (uint32_t *)&l_dataFifo);
                    CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
                }
            }
            l_granulesCompleted++;
            l_numCurrAcc++;
        } // End inner while loop

        if ( (l_fapiRc != FAPI2_RC_SUCCESS) ||
             (l_rc != SBE_SEC_OPERATION_SUCCESSFUL) )
        {
            break;
        }

        l_addr += l_granuleSize * l_granulesCompleted;
    } // End..while (l_granulesCompleted < l_lenCacheAligned);

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
            l_rc = flushUpstreamFifo(l_fapiRc);
            CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(l_rc);
        }

        // first enqueue the length of data actually written
        uint32_t l_len = 1;
        uint32_t l_respLen = sbeAduLenInUpStreamFifo(
                                       l_granulesCompleted,
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

