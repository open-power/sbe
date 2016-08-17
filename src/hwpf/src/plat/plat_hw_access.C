/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/src/plat/plat_hw_access.C $                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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

#include <fapi2.H>
#include "plat_hw_access.H"
#include "p9_perv_scom_addresses.H"

namespace fapi2
{

    ReturnCode getRing_setup(const uint32_t i_ringAddress,
                             const RingMode i_ringMode)
    {
        return FAPI2_RC_SUCCESS;
    }

    ReturnCode getRing_granule_data(const uint32_t i_ringAddress,
                                    uint64_t *o_data,
                                    const uint32_t i_bitShiftValue)
    {
        return FAPI2_RC_SUCCESS;
    }


    ReturnCode getRing_verifyAndcleanup(const uint32_t i_ringAddress,
                                        const RingMode i_ringMode)
    {
        return FAPI2_RC_SUCCESS;
    }

    uint32_t getscom_abs_wrap(const uint32_t i_addr, uint64_t *o_data)
    {
        uint32_t l_rc = 0;
        FAPI_INF("getScom: address: 0x%08X", i_addr);
        l_rc = getscom_abs(i_addr, o_data);
        if( PIB_NO_ERROR != l_rc )
        {
            l_rc = p9_pibErrRetry( i_addr, o_data, l_rc, true);
        }
        FAPI_INF("getScom: returned rc: 0x%08X, data HI: 0x%08X, "
                 "data LO: 0x%08X", l_rc, (*o_data >> 32),
                 static_cast<uint32_t>(*o_data & 0xFFFFFFFF));
        return l_rc;
    }

    uint32_t putscom_abs_wrap(const uint32_t i_addr, uint64_t i_data)
    {
        uint32_t l_rc = 0;
        FAPI_INF("putScom: address: 0x%08X, data HI: 0x%08X, data LO: 0x%08X",
                 i_addr, (i_data >> 32),
                 static_cast<uint32_t>(i_data & 0xFFFFFFFF));
        l_rc = putscom_abs(i_addr, i_data);
        if( PIB_NO_ERROR != l_rc )
        {
            l_rc = p9_pibErrRetry( i_addr, &i_data, l_rc, false);
        }
        FAPI_INF("putScom: returned rc: 0x%08X", l_rc);
        return l_rc;
    }

uint32_t p9_pibErrRetry( const uint32_t i_addr, uint64_t *io_data,
                         const uint8_t i_pibErr, const bool i_isRead)
{
    FAPI_INF("Entering p9_pibErrRetry");
    static const uint8_t MAX_RETRIES = 2;
    static const uint64_t REG_BIT0 = (uint64_t)(0x1)<<63;
    uint8_t l_retryCount = 0;
    uint32_t pibErr = i_pibErr;

    do
    {
        // Only retry for parity and timeout errors
        if (( i_pibErr != PIB_PARITY_ERROR )
            && ( i_pibErr !=  PIB_TIMEOUT_ERROR ))
        {
            break;
        }
        for(l_retryCount = 0; l_retryCount < MAX_RETRIES; l_retryCount++)
        {
            // RESET_PCB: Reset all PCB elements outside of the standby domain
            pibErr = putscom_abs( PERV_PIB_RESET_REG, REG_BIT0 );
            if( pibErr ) break;
            // RESET_PCB: clear it again
            pibErr = putscom_abs( PERV_PIB_RESET_REG, 0);
            if( pibErr ) break;

            FAPI_DBG( "*** Retry %i ***", l_retryCount );

            if ( i_isRead )
            {
                pibErr = getscom_abs(i_addr, io_data);
            }
            else
            {
                pibErr = putscom_abs(i_addr, *io_data);
            }

            if( PIB_NO_ERROR == pibErr )
            {
                FAPI_INF("Read/Write Retry Successful");
                break;
            }
            if ( pibErr != i_pibErr ) break;
        }
    }while(0);
    FAPI_INF("Exiting p9_pibErrRetry");
    return pibErr;
}


};
