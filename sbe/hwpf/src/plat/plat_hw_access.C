/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/hwpf/src/plat/plat_hw_access.C $                          */
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
        FAPI_INF("putScom: returned rc: 0x%08X", l_rc);
        return l_rc;
    }
};
