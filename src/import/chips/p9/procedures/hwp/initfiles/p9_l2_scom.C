/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/initfiles/p9_l2_scom.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
#include "p9_l2_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;

constexpr uint64_t literal_0b0001 = 0b0001;
constexpr uint64_t literal_0b0100 = 0b0100;
constexpr uint64_t literal_0 = 0;
constexpr uint64_t literal_0x001 = 0x001;
constexpr uint64_t literal_0b0000 = 0b0000;

fapi2::ReturnCode p9_l2_scom(const fapi2::Target<fapi2::TARGET_TYPE_EX>& TGT0,
                             const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>& TGT1)
{
    {
        fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_TGT1_ATTR_SYSTEM_IPL_PHASE;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, TGT1, l_TGT1_ATTR_SYSTEM_IPL_PHASE));
        fapi2::ATTR_PROC_EPS_READ_CYCLES_T0_Type l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_READ_CYCLES_T0, TGT1, l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0));
        fapi2::ATTR_PROC_EPS_READ_CYCLES_T1_Type l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_READ_CYCLES_T1, TGT1, l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1));
        fapi2::ATTR_PROC_EPS_READ_CYCLES_T2_Type l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_READ_CYCLES_T2, TGT1, l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2));
        fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T1_Type l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T1, TGT1, l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1));
        fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T2_Type l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T2, TGT1, l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2));
        fapi2::buffer<uint64_t> l_scom_buffer;
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x1001080aull, l_scom_buffer ));

            constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_SYSMAP_SM_NOT_LG_SEL_OFF = 0x0;
            l_scom_buffer.insert<23, 1, 63, uint64_t>(l_EXP_L2_L2MISC_L2CERRS_CFG_SYSMAP_SM_NOT_LG_SEL_OFF );

            if ((l_TGT1_ATTR_SYSTEM_IPL_PHASE == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_RUNTIME))
            {
                constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_HASH_L3_ADDR_EN_ON = 0x1;
                l_scom_buffer.insert<21, 1, 63, uint64_t>(l_EXP_L2_L2MISC_L2CERRS_CFG_HASH_L3_ADDR_EN_ON );
            }

            if ((l_TGT1_ATTR_SYSTEM_IPL_PHASE == ENUM_ATTR_SYSTEM_IPL_PHASE_HB_RUNTIME))
            {
                constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_CAC_ERR_REPAIR_EN_ON = 0x1;
                l_scom_buffer.insert<15, 1, 63, uint64_t>(l_EXP_L2_L2MISC_L2CERRS_CFG_CAC_ERR_REPAIR_EN_ON );
            }

            FAPI_TRY(fapi2::putScom(TGT0, 0x1001080aull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x1001080bull, l_scom_buffer ));

            l_scom_buffer.insert<4, 4, 60, uint64_t>(literal_0b0001 );
            l_scom_buffer.insert<8, 4, 60, uint64_t>(literal_0b0100 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x1001080bull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x10010810ull, l_scom_buffer ));

            if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0 != literal_0))
            {
                l_scom_buffer.insert<0, 12, 52, uint64_t>(l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0 );
            }
            else if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0 == literal_0))
            {
                l_scom_buffer.insert<0, 12, 52, uint64_t>(literal_0x001 );
            }

            if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1 != literal_0))
            {
                l_scom_buffer.insert<12, 12, 52, uint64_t>(l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1 );
            }
            else if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1 == literal_0))
            {
                l_scom_buffer.insert<12, 12, 52, uint64_t>(literal_0x001 );
            }

            if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2 != literal_0))
            {
                l_scom_buffer.insert<24, 12, 52, uint64_t>(l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2 );
            }
            else if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2 == literal_0))
            {
                l_scom_buffer.insert<24, 12, 52, uint64_t>(literal_0x001 );
            }

            FAPI_TRY(fapi2::putScom(TGT0, 0x10010810ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer ));

            if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1 != literal_0))
            {
                l_scom_buffer.insert<0, 12, 52, uint64_t>(l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1 );
            }
            else if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1 == literal_0))
            {
                l_scom_buffer.insert<0, 12, 52, uint64_t>(literal_0x001 );
            }

            if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2 != literal_0))
            {
                l_scom_buffer.insert<12, 12, 52, uint64_t>(l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2 );
            }
            else if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2 == literal_0))
            {
                l_scom_buffer.insert<12, 12, 52, uint64_t>(literal_0x001 );
            }

            constexpr auto l_EXP_L2_L2MISC_L2CERRS_EPS_CNT_USE_L2_DIVIDER_EN_OFF = 0x0;
            l_scom_buffer.insert<29, 1, 63, uint64_t>(l_EXP_L2_L2MISC_L2CERRS_EPS_CNT_USE_L2_DIVIDER_EN_OFF );
            l_scom_buffer.insert<30, 4, 60, uint64_t>(literal_0b0000 );
            l_scom_buffer.insert<24, 4, 60, uint64_t>(literal_0b0001 );
            constexpr auto l_EXP_L2_L2MISC_L2CERRS_EPS_MODE_SEL_MODE1 = 0x0;
            l_scom_buffer.insert<28, 1, 63, uint64_t>(l_EXP_L2_L2MISC_L2CERRS_EPS_MODE_SEL_MODE1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer));
        }

    };
fapi_try_exit:
    return fapi2::current_err;
}
