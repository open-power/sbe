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

#include "fapi2.H"

#include "p9_pba_access.H"
#include "p9_pba_setup.H"

using namespace fapi2;

///////////////////////////////////////////////////////////////////////
// @brief sbeMemAccess_Wrap Memory Access Wrapper function
//
// @param [in] i_flagGetOrPut Flag to indicate the memory Access Type
//                 true  : GetMem ChipOp
//                 false : PutMem ChipOp
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeMemAccess_Wrap(const bool i_flagGetOrPut)
{
    #define SBE_FUNC " sbeMemAccess_Wrap "
    SBE_ENTER(SBE_FUNC);

    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;

    ReturnCode l_fapiRc = FAPI2_RC_SUCCESS;
    sbeRespGenHdr_t l_respHdr;
    l_respHdr.init();
    sbeResponseFfdc_t l_ffdc;

    // Create an instance of Memory Access ChipOp structure
    sbeMemAccessReqMsgHdr_t l_req = {0};

    // @TODO via RTC: 128982
    //       ADU support
    //       Handle ECC data

    // Offload the common header from
    // the Upstream FIFO
    bool l_eot = (i_flagGetOrPut)? true : false;
    uint32_t l_len2dequeue  = sizeof(l_req) / sizeof(uint32_t);
    l_rc = sbeUpFifoDeq_mult (l_len2dequeue, (uint32_t *)&l_req, l_eot);

    SBE_DEBUG(SBE_FUNC " flags[0x%08X], addrHi[0x%08X],"
                       " addrLo[0x%08X], len[[0x%08X]",
                       l_req.flags, l_req.addrHi,
                       l_req.addrLo, l_req.len);

    // Calculate the PBA address from the given input
    uint64_t l_pbaAddr = l_req.getPbaAddr();

    // @TODO via RTC: 128982
    //       Handle Invalid length (0) and Invalid Flags:
    //       Primary status code could be SBE_PRI_INVALID_DATA,
    //       but, what would be the secondary status code?

    // @TODO via RTC: 128982
    //       ADU support
    //       Auto Increment Flag is applicable only for ADU operations
    // bool l_isAutoIncr = l_req.isAutoIncrModeSet();

    // Determine the access flags
    bool l_isPBA = l_req.isPbaFlagSet();
    uint32_t l_isFastMode = l_req.isFastModeSet();

    uint32_t l_numGranules = 0;
    uint64_t l_numCacheLineCompleted = 0;

    // Input Data length in alignment with PBA Cacheline (128 Bytes)
    uint64_t l_dataLenCacheLineAligned = l_req.getDataLenPbaCacheAlign();

    Target<fapi2::TARGET_TYPE_PROC_CHIP> l_proc =
                              plat_getChipTarget();
    // @TODO via RTC: 128982
    //       Accept Ex target from input data
    Target<fapi2::TARGET_TYPE_EX > l_ex((uint64_t)7);

    while (l_numCacheLineCompleted < l_dataLenCacheLineAligned)
    {
        // If FIFO access failure
        if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            // Let command processor routine to handle the RC.
            break;
        }

        if (l_isPBA)
        {
            // Call the PBA setup HWP
            l_fapiRc = p9_pba_setup (l_proc,
                                     l_ex,
                                     l_pbaAddr,
                                     i_flagGetOrPut,
                                     l_isFastMode,
                                     l_numGranules);
        }
        else
        {
            // @TODO via RTC: 128982
            //       ADU support
            //       Call p9_*_setup for ADU operations
            l_rc = SBE_SEC_COMMAND_NOT_SUPPORTED;
            break;
        }

        // if p9_pba_setup returns error
        if( l_fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" p9_pba_setup Failed");

             // Respond with HWP FFDC
            l_respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                 SBE_SEC_GENERIC_FAILURE_IN_EXECUTION );
            l_ffdc.setRc(l_fapiRc);

            break;
        }

        SBE_DEBUG(SBE_FUNC "l_numGranules=[0x%08X]", l_numGranules);
        uint64_t l_numAccesses = 0;

        if ((l_numGranules != 0) &&
            (l_numGranules <
            (l_dataLenCacheLineAligned - l_numCacheLineCompleted)))
        {
            l_numAccesses = l_numGranules;
        }
        else
        {
            l_numAccesses =
                l_dataLenCacheLineAligned - l_numCacheLineCompleted;
        }

        uint64_t l_numCurrentAccess = 0;
        while (l_numCurrentAccess < l_numAccesses)
        {
            bool l_lastGranule = false;
            bool l_firstGranule = false;

            // @TODO via RTC: 128982
            //       ADU support
            //       Need 8Byte alignment for ADU operations
            uint32_t l_dataFifo[32] ; // 128Byte granule for PBA access

            if (l_numCurrentAccess == (l_numGranules - 1))
            {
                l_lastGranule = true;
            }

            if (l_numCurrentAccess == 0)
            {
                l_firstGranule = true;
            }

            // If this is putmem request,
            // read input data from the upstream FIFO
            if (!i_flagGetOrPut)
            {
                // @TODO via RTC: 128982
                //       Use a granule variable
                l_len2dequeue = 32;

                l_rc = sbeUpFifoDeq_mult (l_len2dequeue,
                                         (uint32_t *)&l_dataFifo,
                                          false);

                // If there was an underlying FIFO operation failure
                if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    // Let command processor routine to handle the RC.
                    break;
                }
            }

            // Call PBA access HWP for PBA write or read request
            l_fapiRc = p9_pba_access ( l_proc, l_pbaAddr,
                                       i_flagGetOrPut,
                                       l_isFastMode,
                                       l_firstGranule,
                                       l_lastGranule,
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
            if (i_flagGetOrPut)
            {
                // Break out on underlying FIFO operational error
                if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
                {
                    break;
                }

                uint32_t l_len = 32;
                l_rc = sbeDownFifoEnq_mult (l_len, (uint32_t *)&l_dataFifo);

                // If FIFO failure
                if (l_rc != SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    // Let command processor routine to handle the RC.
                    break;
                }
            }
            l_numCacheLineCompleted++;
            l_numCurrentAccess++;

        } // End inner while loop

        if ( (l_fapiRc != FAPI2_RC_SUCCESS) ||
             (l_rc != SBE_SEC_OPERATION_SUCCESSFUL) )
        {
            break;
        }

        // @TODO via RTC: 128982
        //       Use a granule variable
        l_pbaAddr += 128 * l_numCacheLineCompleted;

    } // End..while (l_numCacheLineCompleted < l_dataLenCacheLineAligned);

    // Now build and enqueue response into downstream FIFO
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            break;
        }

        l_len2dequeue = 0;
        // If there was a HWP failure for putmem request,
        // need to Flush out upstream FIFO, until EOT arrives
        if (!i_flagGetOrPut)
        {
            if ( l_fapiRc != FAPI2_RC_SUCCESS )
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
        }

        // Break out on underlying FIFO operational error
        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            break;
        }

        // first enqueue the length of data actually written
        uint32_t l_len = 1;
        uint32_t l_respLen = l_numCacheLineCompleted*8*16;
        l_rc = sbeDownFifoEnq_mult ( l_len, &l_respLen );

        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            break;
        }

        // Now enqueue the minimum response header
        uint32_t l_dist2Hdr = 1;
        l_len = sizeof(l_respHdr) / sizeof(uint32_t);
        l_rc = sbeDownFifoEnq_mult ( l_len, reinterpret_cast<uint32_t *>
                                                 (&l_respHdr) );

        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            break;
        }

        l_dist2Hdr += l_len;

        // Enqueue FFDC data if there is one
        if( l_ffdc.getRc() )
        {
            l_len = sizeof(l_ffdc) / sizeof(uint32_t);
            l_rc = sbeDownFifoEnq_mult ( l_len,
                        reinterpret_cast<uint32_t *>(&l_ffdc) );
            if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
            {
                break;
            }

            l_dist2Hdr += l_len;

            // @TODO via 129076:
            //       Need to add FFDC data as well.
        }

        l_len = sizeof(l_dist2Hdr) / sizeof(uint32_t);
        l_rc = sbeDownFifoEnq_mult ( l_len, &l_dist2Hdr);

        if ( l_rc != SBE_SEC_OPERATION_SUCCESSFUL )
        {
            break;
        }

    } while(false);

    return l_rc;
    #undef SBE_FUNC
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbePutMem (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbePutMem "
    SBE_ENTER(SBE_FUNC);

    return sbeMemAccess_Wrap (false);

    #undef SBE_FUNC
}

/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeGetMem (uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeGetMem "
    SBE_ENTER(SBE_FUNC);

    return sbeMemAccess_Wrap (true);

    #undef SBE_FUNC
}


