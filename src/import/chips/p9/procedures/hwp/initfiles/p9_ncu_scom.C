/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/initfiles/p9_ncu_scom.C $  */
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
#include "p9_ncu_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;

constexpr uint64_t literal_0xF = 0xF;
constexpr uint64_t literal_0x10 = 0x10;
constexpr uint64_t literal_0x4 = 0x4;
constexpr uint64_t literal_0b0001 = 0b0001;
constexpr uint64_t literal_0b1000 = 0b1000;
constexpr uint64_t literal_0x40 = 0x40;
constexpr uint64_t literal_0x8 = 0x8;
constexpr uint64_t literal_0b10 = 0b10;
constexpr uint64_t literal_6 = 6;
constexpr uint64_t literal_0 = 0;
constexpr uint64_t literal_15 = 15;
constexpr uint64_t literal_4 = 4;
constexpr uint64_t literal_0x01 = 0x01;
constexpr uint64_t literal_0x0F = 0x0F;

fapi2::ReturnCode p9_ncu_scom(const fapi2::Target<fapi2::TARGET_TYPE_EX>& TGT0,
                              const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>& TGT1, const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& TGT2)
{
    {
        fapi2::ATTR_EC_Type   l_chip_ec;
        fapi2::ATTR_NAME_Type l_chip_id;
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, TGT2, l_chip_id));
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, TGT2, l_chip_ec));
        fapi2::ATTR_PROC_FABRIC_PUMP_MODE_Type l_TGT1_ATTR_PROC_FABRIC_PUMP_MODE;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_PUMP_MODE, TGT1, l_TGT1_ATTR_PROC_FABRIC_PUMP_MODE));
        fapi2::ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID_Type l_TGT1_ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID, TGT1, l_TGT1_ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID));
        fapi2::ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID_Type l_TGT1_ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID, TGT1, l_TGT1_ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID));
        fapi2::ATTR_SMF_CONFIG_Type l_TGT1_ATTR_SMF_CONFIG;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SMF_CONFIG, TGT1, l_TGT1_ATTR_SMF_CONFIG));
        fapi2::ATTR_CHIP_EC_FEATURE_HW440920_Type l_TGT2_ATTR_CHIP_EC_FEATURE_HW440920;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW440920, TGT2, l_TGT2_ATTR_CHIP_EC_FEATURE_HW440920));
        fapi2::buffer<uint64_t> l_scom_buffer;
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x1001100aull, l_scom_buffer ));

            constexpr auto l_EXP_NC_NCMISC_NCSCOMS_SYSMAP_SM_NOT_LG_SEL_OFF = 0x0;
            l_scom_buffer.insert<9, 1, 63, uint64_t>(l_EXP_NC_NCMISC_NCSCOMS_SYSMAP_SM_NOT_LG_SEL_OFF );

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                if ((l_TGT1_ATTR_PROC_FABRIC_PUMP_MODE == fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_GROUP))
                {
                    constexpr auto l_EXP_NC_NCMISC_NCSCOMS_SKIP_GRP_SCOPE_EN_ON = 0x1;
                    l_scom_buffer.insert<51, 1, 63, uint64_t>(l_EXP_NC_NCMISC_NCSCOMS_SKIP_GRP_SCOPE_EN_ON );
                }
                else if ((l_TGT1_ATTR_PROC_FABRIC_PUMP_MODE == fapi2::ENUM_ATTR_PROC_FABRIC_PUMP_MODE_CHIP_IS_NODE))
                {
                    constexpr auto l_EXP_NC_NCMISC_NCSCOMS_SKIP_GRP_SCOPE_EN_OFF = 0x0;
                    l_scom_buffer.insert<51, 1, 63, uint64_t>(l_EXP_NC_NCMISC_NCSCOMS_SKIP_GRP_SCOPE_EN_OFF );
                }
            }

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                l_scom_buffer.insert<44, 4, 60, uint64_t>(l_TGT1_ATTR_FABRIC_ADDR_EXTENSION_GROUP_ID );
                l_scom_buffer.insert<48, 3, 61, uint64_t>(l_TGT1_ATTR_FABRIC_ADDR_EXTENSION_CHIP_ID );
            }

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                constexpr auto l_EXP_NC_NCMISC_NCSCOMS_TLBIE_PACING_CNT_EN_ON = 0x1;
                l_scom_buffer.insert<10, 1, 63, uint64_t>(l_EXP_NC_NCMISC_NCSCOMS_TLBIE_PACING_CNT_EN_ON );
            }

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                l_scom_buffer.insert<19, 8, 56, uint64_t>(literal_0xF );
            }

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                l_scom_buffer.insert<11, 8, 56, uint64_t>(literal_0x10 );
            }

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                l_scom_buffer.insert<27, 8, 56, uint64_t>(literal_0x4 );
            }

            FAPI_TRY(fapi2::putScom(TGT0, 0x1001100aull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x1001100bull, l_scom_buffer ));

            l_scom_buffer.insert<0, 4, 60, uint64_t>(literal_0b0001 );
            l_scom_buffer.insert<4, 4, 60, uint64_t>(literal_0b1000 );
            l_scom_buffer.insert<8, 10, 54, uint64_t>(literal_0x10 );
            l_scom_buffer.insert<26, 10, 54, uint64_t>(literal_0x40 );
            l_scom_buffer.insert<18, 4, 60, uint64_t>(literal_0x8 );
            l_scom_buffer.insert<22, 4, 60, uint64_t>(literal_0x8 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x1001100bull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x1001100cull, l_scom_buffer ));

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6)
                    && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                if ((l_TGT1_ATTR_SMF_CONFIG == fapi2::ENUM_ATTR_SMF_CONFIG_ENABLED))
                {
                    l_scom_buffer.insert<20, 2, 62, uint64_t>(literal_0b10 );
                }
            }

            if (((l_chip_id == 0x5) && (l_chip_ec == 0x20)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x21)) || ((l_chip_id == 0x5)
                    && (l_chip_ec == 0x22)) || ((l_chip_id == 0x5) && (l_chip_ec == 0x23)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x10))
                || ((l_chip_id == 0x6) && (l_chip_ec == 0x11)) || ((l_chip_id == 0x6) && (l_chip_ec == 0x12)) || ((l_chip_id == 0x6)
                        && (l_chip_ec == 0x13)) || ((l_chip_id == 0x7) && (l_chip_ec == 0x10)) )
            {
                constexpr auto l_EXP_NC_NCMISC_NCSCOMS_TLBIE_PACING_MST_DLY_EN_ON = 0x1;
                l_scom_buffer.insert<16, 1, 63, uint64_t>(l_EXP_NC_NCMISC_NCSCOMS_TLBIE_PACING_MST_DLY_EN_ON );
            }

            constexpr auto l_EXP_NC_NCMISC_NCSCOMS_TLBIE_STALL_EN_ON = 0x1;
            l_scom_buffer.insert<0, 1, 63, uint64_t>(l_EXP_NC_NCMISC_NCSCOMS_TLBIE_STALL_EN_ON );
            l_scom_buffer.insert<1, 3, 61, uint64_t>(literal_6 );

            if ((l_TGT2_ATTR_CHIP_EC_FEATURE_HW440920 != literal_0))
            {
                l_scom_buffer.insert<4, 4, 60, uint64_t>(literal_15 );
            }
            else if (( true ))
            {
                l_scom_buffer.insert<4, 4, 60, uint64_t>(literal_4 );
            }

            if ((l_TGT2_ATTR_CHIP_EC_FEATURE_HW440920 != literal_0))
            {
                l_scom_buffer.insert<8, 8, 56, uint64_t>(literal_0x01 );
            }
            else if (( true ))
            {
                l_scom_buffer.insert<8, 8, 56, uint64_t>(literal_0x0F );
            }

            FAPI_TRY(fapi2::putScom(TGT0, 0x1001100cull, l_scom_buffer));
        }

    };
fapi_try_exit:
    return fapi2::current_err;
}
