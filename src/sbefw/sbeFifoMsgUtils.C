/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeFifoMsgUtils.C $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
 * @file: ppe/sbe/sbefw/sbeFifoMsgUtils.C
 *
 * @brief This file contains the SBE FIFO Access Common Utility Functions
 *
 */

#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "sbeerrorcodes.H"
#include "plat_hw_access.H"
#include "assert.h"
#include "sbeFFDC.H"
#include "hwp_error_info.H"

// If we can not perform FIFO operation ( FIFO FULL while writing
// or EMPTY while reading ) we will sleep for FIFO_WAIT_SLEEP_TIME
// ms so that FIFO can be ready.
static const uint32_t FIFO_WAIT_SLEEP_TIME = 1;
// Write this data to send EOT to DS FIFO. The register to send EOT
// is 32 bit only. But our scom operations are 64 bit. So set a bit
// in higher word to trigger EOT.
static const uint64_t DOWNSTREAM_EOT_DATA = 0x100000000ull;

using namespace fapi2;
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeUpFifoDeq_mult (uint32_t    &io_len,
                            uint32_t    *o_pData,
                            const bool  i_isEotExpected,
                            const bool  i_flush)
{
    #define SBE_FUNC " sbeUpFifoDeq_mult "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t l_len = 0;

    // If Caller didn't request flush operation
    // and passed a non-zero valid length, we
    // would expect a valid buffer
    if ((!i_flush) && (io_len > 0))
    {
        assert ( NULL != o_pData)
    }

    do
    {
        sbeFifoEntry_t l_data = {0};

        // Read Double word from the Upstream FIFO;
        // The DW data represents the first 32 bits of data word entry
        // followed by the status bits.

        // Bit 0-31    : Data
        // Bit 32      : Data valid flag
        // Bit 33      : EOT flag
        // Bit 34-63   : Status (2-31)
        // Valid : EOT
        //    1  : 0   -> data=message
        //    0  : 1   -> data=dummy_data of EOT operation
        //    0  : 0   -> data=dummy_data
        //    1  : 1   -> Not used

        l_rc = sbeUpFifoDeq ( reinterpret_cast<uint64_t*>(&l_data) );

        if (l_rc)
        {
            // Error while dequeueing from upstream FIFO
            SBE_ERROR(SBE_FUNC"sbeUpFifoDeq failed,"
                         "l_rc=[0x%08X]", l_rc);
            // @TODO RTC via : 132295
            //       RC refactoring - reserve 3 bits in SBE RC for PCBPIB
            l_rc = SBE_SEC_FIFO_ACCESS_FAILURE;
            break;
        }

        SBE_DEBUG(SBE_FUNC"sbeUpFifoDeq, "
                    "l_data.fifo_data=[0x%08X],",
                     l_data.fifo_data);

        // If FIFO reset is requested
        if(l_data.statusOrReserved.req_upfifo_reset)
        {
            // @TODO via RTC : 126147
            //       Review reset loop flow in here.
            //       Received a FIFO reset request
            l_rc = SBE_FIFO_RESET_RECEIVED;
            break;
        }

        // if EOT flag is set, clear EOT and
        // set the RC accordingly
        if (l_data.statusOrReserved.eot_flag)
        {
            l_rc = sbeUpFifoAckEot();
            if (l_rc)
            {
                // Error while ack'ing EOT in upstream FIFO
                SBE_ERROR(SBE_FUNC"sbeUpFifoAckEot failed,"
                          "l_rc=[0x%08X]", l_rc);

                // Collect FFDC and save off the l_rc
                l_rc = SBE_SEC_FIFO_ACCESS_FAILURE;
                break;
            }

            // Successfully Ack'ed the EOT in upstream FIFO
            if ( ((!i_isEotExpected) || (l_len != io_len))
                     && (!i_flush) )
            {
                if (l_len < io_len)
                {
                    // Unexpected EOT, got insufficient data
                    l_rc = SBE_SEC_UNEXPECTED_EOT_INSUFFICIENT_DATA ;
                }
                else
                {
                    // Unexpected EOT, got excess data
                    l_rc = SBE_SEC_UNEXPECTED_EOT_EXCESS_DATA ;
                }
            }
            break;
        }

        // if Upstream FIFO is empty,
        if (l_data.statusOrReserved.fifo_empty)
        {
            pk_sleep(PK_MILLISECONDS(FIFO_WAIT_SLEEP_TIME));
            continue;
        }

        if ((!i_flush) && (l_len < io_len))
        {
            o_pData[l_len] = l_data.fifo_data;
        }

        ++l_len;

    } while(i_flush || i_isEotExpected || (l_len < io_len));

    // Return the length of entries dequeued.
    io_len = l_len;
    return l_rc;

    #undef SBE_FUNC
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
uint32_t sbeDownFifoEnq_mult (uint32_t        &io_len,
                              const uint32_t *i_pData)
{
    #define SBE_FUNC " sbeDownFifoEnq_mult "
    uint32_t  l_rc   = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t   l_len = 0;

    do
    {
        sbeDownFifoStatusReg_t l_status = {0};

        // Read the down stream FIFO status
        l_rc = sbeDownFifoGetStatus (reinterpret_cast<uint64_t *>(&l_status));
        if (l_rc)
        {
            // Error while reading downstream FIFO status
            SBE_ERROR(SBE_FUNC"sbeDownFifoGetStatus failed, "
                      "l_rc=[0x%08X]", l_rc);
            l_rc = SBE_SEC_FIFO_ACCESS_FAILURE;
            break;
        }

        // Check if there was a FIFO reset request from SP
        if (l_status.downfifo_status.req_upfifo_reset)
        {
            // @TODO via RTC : 126147
            //       Review reset loop flow in here.
            //       Received an upstream FIFO reset request
            SBE_ERROR(SBE_FUNC"Received reset request");
            l_rc = SBE_FIFO_RESET_RECEIVED;
            break;
        }

        // Check if downstream FIFO is full
        if (l_status.downfifo_status.fifo_full)
        {
            // Downstream FIFO is full
            pk_sleep(PK_MILLISECONDS(FIFO_WAIT_SLEEP_TIME));
            continue;
        }

        // PIB write data format:
        // Bit 0 - 31  : Data
        // Bit 32 - 63 : Unused

        sbeFifoEntry_t l_data = {0};

        l_data.fifo_data   = *(i_pData+l_len);

        SBE_DEBUG(SBE_FUNC"Downstream fifo data entry[0x%08X]",
                                             l_data.fifo_data);

        // Write the data into the downstream FIFO
        uint64_t * tp = reinterpret_cast<uint64_t*>(&l_data);
        l_rc = sbeDownFifoEnq ( *tp );
        if (l_rc)
        {
            SBE_ERROR(SBE_FUNC"sbeDownFifoEnq failed, "
                              "l_rc[0x%08X]", l_rc);
            // @TODO RTC via : 132295
            //       RC refactoring - reserve 3 bits in SBE RC for PCBPIB
            l_rc = SBE_SEC_FIFO_ACCESS_FAILURE;
            break;
        }

        ++l_len;

    } while(l_len<io_len);

    io_len = l_len;
    return l_rc;
    #undef SBE_FUNC
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
uint32_t sbeDownFifoSignalEot (void)
{
    uint32_t l_rc = 0;
    #define SBE_FUNC "sbeDownFifoSignalEot "
    SBE_ENTER(SBE_FUNC);
    sbeDownFifoStatusReg_t l_status = {0};
    do
    {
        // Read the down stream FIFO status
        l_rc = sbeDownFifoGetStatus (reinterpret_cast<uint64_t *>(&l_status));
        if (l_rc)
        {
            // Error while reading downstream FIFO status
            SBE_ERROR(SBE_FUNC"sbeDownFifoGetStatus failed, "
                      "l_rc=[0x%08X]", l_rc);
            l_rc = SBE_SEC_FIFO_ACCESS_FAILURE;
            break;
        }

        // Check if downstream FIFO is full
        if (l_status.downfifo_status.fifo_full)
        {
            pk_sleep(PK_MILLISECONDS(FIFO_WAIT_SLEEP_TIME));
            continue;
        }
        l_rc = putscom_abs(SBE_DOWNSTREAM_FIFO_SIGNAL_EOT, DOWNSTREAM_EOT_DATA);
        break;
    } while(1);


    SBE_EXIT(SBE_FUNC);
    return l_rc;
    #undef SBE_FUNC
}


uint32_t sbeDsSendRespHdr(const sbeRespGenHdr_t &i_hdr,
                          sbeResponseFfdc_t *i_ffdc )
{
    #define SBE_FUNC "sbeDsSendRespHdr "
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        uint32_t distance = 1; //initialise by 1 for entry count itself.
        uint32_t len = sizeof( i_hdr )/sizeof(uint32_t);
        // sbeDownFifoEnq_mult.
        rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) &i_hdr);
        if (rc)
        {
            break;
        }
        distance += len;

        // If no ffdc , exit;
        if( (i_ffdc != NULL) && (i_ffdc->getRc() != FAPI2_RC_SUCCESS))
        {
            SBE_ERROR( SBE_FUNC" FAPI RC:0x%08X", i_ffdc->getRc());
            // making sure ffdc length is multiples of uint32_t
            assert((g_FfdcData.ffdcLength % sizeof(uint32_t)) == 0);
            uint32_t ffdcDataLenInWords = g_FfdcData.ffdcLength
                                            / sizeof(uint32_t);
            // Set failed command information
            // Sequence Id is 0 by default for Fifo interface
            i_ffdc->setCmdInfo(0, i_hdr.cmdClass, i_hdr.command);
            // Add HWP specific ffdc data length
            i_ffdc->lenInWords += ffdcDataLenInWords;
            len = sizeof(sbeResponseFfdc_t)/sizeof(uint32_t);
            rc = sbeDownFifoEnq_mult ( len, ( uint32_t *) i_ffdc);
            if (rc)
            {
                break;
            }
            distance += len;

            // Send HWP ffdc data
            rc = sbeDownFifoEnq_mult ( ffdcDataLenInWords,
                                        ( uint32_t *) &g_FfdcData.ffdcData);
            if (rc)
            {
                break;
            }
            distance += ffdcDataLenInWords;
        }

        // If there is a SBE internal failure
        if((i_hdr.primaryStatus != SBE_PRI_OPERATION_SUCCESSFUL) ||\
            (i_hdr.secondaryStatus != SBE_SEC_OPERATION_SUCCESSFUL))
        {
            SBE_ERROR( SBE_FUNC" primaryStatus:0x%08X secondaryStatus:0x%08X",
                       i_hdr.primaryStatus, i_hdr.secondaryStatus);
            //Add FFDC data as well.
            //Generate all the fields of FFDC package
            SbeFFDCPackage sbeFfdc;
            rc = sbeFfdc.sendOverFIFO(i_hdr,
                                      SBE_FFDC_ALL_DUMP,
                                      len);
            if (rc)
            {
                break;
            }
            distance += len;
        }

        len = sizeof(distance)/sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult ( len, &distance);
        if (rc)
        {
            break;
        }

    }while(0);
    return rc;
    #undef SBE_FUNC
}
