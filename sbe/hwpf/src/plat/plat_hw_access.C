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


};
