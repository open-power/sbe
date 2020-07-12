/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/initfiles/p10_core_scom.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
#include "p10_core_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;


fapi2::ReturnCode p10_core_scom(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& TGT0,
                                const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>& TGT1, const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& TGT2)
{
    {
        fapi2::ATTR_EC_Type   l_chip_ec;
        fapi2::ATTR_NAME_Type l_chip_id;
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, TGT2, l_chip_id));
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, TGT2, l_chip_ec));
        fapi2::ATTR_XSTOP_ON_SPATTN_Type l_TGT1_ATTR_XSTOP_ON_SPATTN;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_XSTOP_ON_SPATTN, TGT1, l_TGT1_ATTR_XSTOP_ON_SPATTN));
        fapi2::ATTR_FUSED_CORE_MODE_Type l_TGT1_ATTR_FUSED_CORE_MODE;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE, TGT1, l_TGT1_ATTR_FUSED_CORE_MODE));
        fapi2::buffer<uint64_t> l_scom_buffer;
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20020415ull, l_scom_buffer ));

            if ((l_TGT1_ATTR_XSTOP_ON_SPATTN == fapi2::ENUM_ATTR_XSTOP_ON_SPATTN_ENABLED))
            {
                constexpr auto l_ECP_EC_PC_PMU_SPRCOR_EN_ATTN_ON = 0x1;
                l_scom_buffer.insert<3, 1, 63, uint64_t>(l_ECP_EC_PC_PMU_SPRCOR_EN_ATTN_ON );
            }

            FAPI_TRY(fapi2::putScom(TGT0, 0x20020415ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20020443ull, l_scom_buffer ));

            if ((l_TGT1_ATTR_FUSED_CORE_MODE == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_UNFUSED))
            {
                constexpr auto l_ECP_EC_PC_FIR_MASK_PC_OTHER_CORE_CHIPLET_REC_ERROR_ON = 0x1;
                l_scom_buffer.insert<56, 1, 63, uint64_t>(l_ECP_EC_PC_FIR_MASK_PC_OTHER_CORE_CHIPLET_REC_ERROR_ON );
            }

            if ((l_TGT1_ATTR_FUSED_CORE_MODE == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_UNFUSED))
            {
                constexpr auto l_ECP_EC_PC_FIR_MASK_PC_OTHER_CORE_CHIPLET_XSTOP_ERROR_ON = 0x1;
                l_scom_buffer.insert<57, 1, 63, uint64_t>(l_ECP_EC_PC_FIR_MASK_PC_OTHER_CORE_CHIPLET_XSTOP_ERROR_ON );
            }

            if ((l_TGT1_ATTR_FUSED_CORE_MODE == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_UNFUSED))
            {
                constexpr auto l_ECP_EC_PC_FIR_MASK_PC_OTHER_CORE_CHIPLET_SYS_XSTOP_ERROR_ON = 0x1;
                l_scom_buffer.insert<58, 1, 63, uint64_t>(l_ECP_EC_PC_FIR_MASK_PC_OTHER_CORE_CHIPLET_SYS_XSTOP_ERROR_ON );
            }

            FAPI_TRY(fapi2::putScom(TGT0, 0x20020443ull, l_scom_buffer));
        }

    };
fapi_try_exit:
    return fapi2::current_err;
}
