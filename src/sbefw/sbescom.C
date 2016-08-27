/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbescom.C $                                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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

#include <stdint.h>
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "ppe42_scom.h"

/**
 * @brief Indirect SCOM Status
 */
union IndirectScom_t
{
    uint64_t data64;
    struct
    {
        uint64_t :12;        //0:11
        uint64_t addr:20;    //12:31
        uint64_t done:1;     //32
        uint64_t piberr:3;   //33:35
        uint64_t userstat:4; //36:39
        uint64_t :8;         //40:47
        uint64_t data:16;    //48:63
    };

};

// Wait time slice to check indirect scom status register
static const uint32_t SBE_INDIRECT_SCOM_WAIT_TIME_NS = 10000;
// Indirect scom timeout
static const uint32_t MAX_INDSCOM_TIMEOUT_NS = 100000; //0.1 ns

static const uint64_t DIRECT_SCOM_ADDR_MASK = 0x8000000000000000;
static const uint64_t INDIRECT_SCOM_NEW_ADDR_MASK = 0x9000000000000000;

// Scom types
enum sbeScomType
{
    SBE_SCOM_TYPE_DIRECT = 0,      // Direct scom
    SBE_SCOM_TYPE_INDIRECT1 = 1,  // Indirect scom. Old form
    SBE_SCOM_TYPE_INDIRECT_2 = 2,  // Indirect scom. New form
};

uint32_t checkIndirectAndDoScom( const bool i_isRead,
                                 const uint64_t i_addr,
                                 uint64_t & io_data,
                                 uint32_t & o_pcbPibStatus )
{

    #define SBE_FUNC " checkIndirectAndDoScom "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t elapsedIndScomTimeNs = 0;
    uint64_t tempBuffer = io_data;
    sbeScomType scomType = SBE_SCOM_TYPE_DIRECT;
    do
    {
        // If the indirect scom bit is 0, then doing a regular scom
        if( (i_addr & DIRECT_SCOM_ADDR_MASK) == 0)
        {
            SBE_INFO(SBE_FUNC "Performing Direct scom");
            if( i_isRead )
            {
                o_pcbPibStatus = getscom_abs ( (uint32_t)i_addr,
                                              & io_data);
            }
            else
            {
                o_pcbPibStatus = putscom_abs ( (uint32_t)i_addr,
                                              io_data);
            }
            break;
        }
        // We are performing an indirect scom.
        if( ( i_addr & INDIRECT_SCOM_NEW_ADDR_MASK ) ==
                                        INDIRECT_SCOM_NEW_ADDR_MASK )
        {
            scomType = SBE_SCOM_TYPE_INDIRECT_2;
            if( i_isRead )
            {
                // Not allowed write on new format.
                SBE_ERROR(SBE_FUNC "Read not allowed in new form");
                l_rc = SBE_SEC_INVALID_ADDRESS_PASSED;
                break;
            }
            // Zero out the indirect address location.. leave the 52bits of data
            // Get the 12bit indirect scom address
            // OR in the 20bit indirect address
            tempBuffer = ( tempBuffer & 0x000FFFFFFFFFFFFF ) |
                                    ( ( i_addr & 0x00000FFF00000000) << 20 );
        }
        else
        {
            scomType = SBE_SCOM_TYPE_INDIRECT1;
            // Zero out the indirect address location.. leave the 16bits of data
            // Get the 20bit indirect scom address
            // OR in the 20bit indirect address
            tempBuffer = ( tempBuffer & 0x000000000000FFFF) |
                                    ( i_addr & 0x000FFFFF00000000 );
        }

        SBE_INFO(SBE_FUNC "Performing Indirect scom. Type :%u", scomType);

        // zero out the indirect address from the buffer..
        // bit 0-31 - indirect area..
        // bit 32 - always 0
        // bit 33-47 - bcast/chipletID/port
        // bit 48-63 - local addr
        uint64_t tempAddr = i_addr & 0x000000007FFFFFFF;

        // If we are doing a read. We need to do a write first..
        if( i_isRead)
        {
            // turn the read bit on.
            tempBuffer = tempBuffer | 0x8000000000000000;
        }
        else //write
        {
            // Turn the read bit off.
            tempBuffer = tempBuffer & 0x7FFFFFFFFFFFFFFF;

        } // end of write

        // perform write before the read with the new
        // IO_buffer with the imbedded indirect scom addr.
        o_pcbPibStatus = putscom_abs ( tempAddr, tempBuffer);

        if( ( o_pcbPibStatus ) || ( scomType == SBE_SCOM_TYPE_INDIRECT_2 ))
        {
            break;
        }

        // Need to check loop on read until we see done, error,
        //  or we timeout
        IndirectScom_t scomout;
        do
        {
            // Now perform the op requested using the passed in
            // IO_Buffer to pass the read data back to caller.
            o_pcbPibStatus = getscom_abs ( tempAddr, &(scomout.data64));

            if( o_pcbPibStatus ) break;
            // if bit 32 is on indicating a complete bit
            //  or we saw an error, then we're done
            if (scomout.piberr)
            {

                SBE_ERROR(SBE_FUNC "pib error reading status register");
                o_pcbPibStatus = scomout.piberr;
                break;
            }
            if (scomout.done )
            {
                io_data = scomout.data;
                break;
            }

            pk_sleep(PK_NANOSECONDS(SBE_INDIRECT_SCOM_WAIT_TIME_NS));
            elapsedIndScomTimeNs += SBE_INDIRECT_SCOM_WAIT_TIME_NS;

        }while ( elapsedIndScomTimeNs <= MAX_INDSCOM_TIMEOUT_NS);

        if( o_pcbPibStatus ) break;
        if( ! scomout.done)
        {
            SBE_ERROR(SBE_FUNC "Indirect scom timeout");
            l_rc = SBE_SEC_HW_OP_TIMEOUT;
            break;
        }

    }while(0);

    if  (o_pcbPibStatus ) l_rc = SBE_SEC_PCB_PIB_ERR;

    SBE_DEBUG(SBE_FUNC "o_pcbPibStatus:%u l_rc:0x%08X", o_pcbPibStatus, l_rc);
    return l_rc;
}

