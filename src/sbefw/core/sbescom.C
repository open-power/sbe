/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbescom.C $                                    */
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

#include <stdint.h>
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "plat_hw_access.H"
#include "plat_target.H"
#include "sbescom.H"
#include "sbeSecurity.H"
#include "sberegaccess.H"

using namespace fapi2;
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
// Allowed Address first 3 bits
// 0x000 - Allowed
// 0x10X  - Not Allowed
// 0x110 - Allowed
// 0x111 - Allowed
static const uint64_t SCOM_SBE_ADDR_MASK = 0xE0000000; // 0x01, 0x11, 0x00 is allowed
static const uint64_t SCOM_MASTER_ID_MASK = 0x00F00000;

// Scom types
enum sbeScomType
{
    SBE_SCOM_TYPE_DIRECT = 0,      // Direct scom
    SBE_SCOM_TYPE_INDIRECT1 = 1,  // Indirect scom. Old form
    SBE_SCOM_TYPE_INDIRECT_2 = 2,  // Indirect scom. New form
};

void checkIndirectAndDoScom( const bool i_isRead,
                             const uint64_t i_addr,
                             uint64_t & io_data,
                             sbeRespGenHdr_t *const o_hdr,
                             sbeResponseFfdc_t *const o_ffdc,
                             uint64_t i_mask )
{

    #define SBE_FUNC " checkIndirectAndDoScom "
    uint32_t elapsedIndScomTimeNs = 0;
    uint64_t tempBuffer = io_data;
    sbeScomType scomType = SBE_SCOM_TYPE_DIRECT;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    Target<TARGET_TYPE_PROC_CHIP> l_hndl = plat_getChipTarget();
    do
    {
        #ifndef __ALLOW_INVALID_SCOMS__
        // Do address validation
        // SBE throws data storage exception if Master ID field is not 0. Also
        // we halt sbe for SBE address space errors. So check these registers
        // at top level so that we do not halt SBE in these cases.

        // Override bit check for invalid Scom Addr check
        if(!SbeRegAccess::theSbeRegAccess().isSbeRegressionBit())
        {
            if( (( i_addr & SCOM_SBE_ADDR_MASK) == 0x80000000) ||
                ( i_addr & SCOM_MASTER_ID_MASK ))
            {
                SBE_ERROR(SBE_FUNC "Invalid scom");
                o_hdr->setStatus(SBE_PRI_USER_ERROR,
                        SBE_SEC_INVALID_ADDRESS_PASSED);
                break;
            }
        }
        #endif  // __ALLOW_INVALID_SCOMS__
        // If the indirect scom bit is 0, then doing a regular scom
        if( (i_addr & DIRECT_SCOM_ADDR_MASK) == 0)
        {
            SBE_INFO(SBE_FUNC "Performing Direct scom.");
            CHECK_SBE_SECURITY_RC_AND_BREAK_IF_NOT_SUCCESS(
                        static_cast<uint32_t>(i_addr),
                        (i_isRead ? SBE_SECURITY::READ : SBE_SECURITY::WRITE),
                        o_hdr, i_mask)
            if( i_isRead )
            {
                fapiRc = getscom_abs_wrap (&l_hndl, (uint32_t)i_addr,
                                              & io_data);
            }
            else
            {
                fapiRc = putscom_abs_wrap (&l_hndl, (uint32_t)i_addr,
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
                o_hdr->setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                                 SBE_SEC_INVALID_ADDRESS_PASSED);
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
        CHECK_SBE_SECURITY_RC_AND_BREAK_IF_NOT_SUCCESS(
                        static_cast<uint32_t>(tempAddr),
                        (i_isRead ? SBE_SECURITY::READ : SBE_SECURITY::WRITE),
                        o_hdr, i_mask)

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
        fapiRc = putscom_abs_wrap (&l_hndl, tempAddr, tempBuffer);

        if( ( fapiRc != FAPI2_RC_SUCCESS ) ||
                ( scomType == SBE_SCOM_TYPE_INDIRECT_2 ))
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
            fapiRc = getscom_abs_wrap (&l_hndl, tempAddr, &(scomout.data64));

            if( fapiRc != FAPI2_RC_SUCCESS) break;
            // if bit 32 is on indicating a complete bit
            //  or we saw an error, then we're done
            if (scomout.piberr)
            {

                SBE_ERROR(SBE_FUNC "pib error reading status register");
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

        if( fapiRc != FAPI2_RC_SUCCESS ) break;
        if( ! scomout.done)
        {
            SBE_ERROR(SBE_FUNC "Indirect scom timeout.");
            o_hdr->setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                             SBE_SEC_HW_OP_TIMEOUT);
            break;
        }

    }while(0);

    if  (fapiRc != FAPI2_RC_SUCCESS)
    {
        o_hdr->setStatus(SBE_PRI_GENERIC_EXECUTION_FAILURE,
                         SBE_SEC_PCB_PIB_ERR);
        if(o_ffdc) o_ffdc->setRc(fapiRc);
    }

    SBE_DEBUG(SBE_FUNC "fapiRc:%u o_hdr->secondaryStatus:0x%08X", fapiRc, o_hdr->secondaryStatus());
}

