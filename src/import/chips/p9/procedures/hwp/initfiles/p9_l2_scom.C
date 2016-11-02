/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/initfiles/p9_l2_scom.C $   */
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
#include "p9_l2_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;

constexpr auto literal_0 = 0;
constexpr auto literal_0x001 = 0x001;
constexpr auto literal_0b0000 = 0b0000;
constexpr auto literal_0b0001 = 0b0001;
constexpr auto literal_0b0100 = 0b0100;

fapi2::ReturnCode p9_l2_scom(const fapi2::Target<fapi2::TARGET_TYPE_EX>& TGT0,
                             const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>& TGT1)
{
    fapi2::ReturnCode l_rc = 0;

    do
    {
        fapi2::ATTR_PROC_FABRIC_ADDR_BAR_MODE_Type l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_ADDR_BAR_MODE, TGT1, l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_FABRIC_ADDR_BAR_MODE)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x1001080aull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x1001080aull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE == fapi2::ENUM_ATTR_PROC_FABRIC_ADDR_BAR_MODE_SMALL_SYSTEM))
                    {
                        constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_SYSMAP_SM_NOT_LG_SEL_ON = 0x1;
                        l_scom_buffer.insert<uint64_t> (l_EXP_L2_L2MISC_L2CERRS_CFG_SYSMAP_SM_NOT_LG_SEL_ON, 23, 1, 63 );
                    }
                    else if ((l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE == fapi2::ENUM_ATTR_PROC_FABRIC_ADDR_BAR_MODE_LARGE_SYSTEM))
                    {
                        constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_SYSMAP_SM_NOT_LG_SEL_OFF = 0x0;
                        l_scom_buffer.insert<uint64_t> (l_EXP_L2_L2MISC_L2CERRS_CFG_SYSMAP_SM_NOT_LG_SEL_OFF, 23, 1, 63 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x1001080aull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x1001080aull)");
                    break;
                }
            }
        }

        fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_TGT1_ATTR_SYSTEM_IPL_PHASE;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE, TGT1, l_TGT1_ATTR_SYSTEM_IPL_PHASE);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x1001080aull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x1001080aull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_SYSTEM_IPL_PHASE == fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_RUNTIME))
                    {
                        constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_HASH_L3_ADDR_EN_ON = 0x1;
                        l_scom_buffer.insert<uint64_t> (l_EXP_L2_L2MISC_L2CERRS_CFG_HASH_L3_ADDR_EN_ON, 21, 1, 63 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x1001080aull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x1001080aull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x1001080aull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x1001080aull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_SYSTEM_IPL_PHASE == ENUM_ATTR_SYSTEM_IPL_PHASE_HB_RUNTIME))
                    {
                        constexpr auto l_EXP_L2_L2MISC_L2CERRS_CFG_CAC_ERR_REPAIR_EN_ON = 0x1;
                        l_scom_buffer.insert<uint64_t> (l_EXP_L2_L2MISC_L2CERRS_CFG_CAC_ERR_REPAIR_EN_ON, 15, 1, 63 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x1001080aull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x1001080aull)");
                    break;
                }
            }
        }

        fapi2::ATTR_PROC_EPS_READ_CYCLES_T0_Type l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_READ_CYCLES_T0, TGT1, l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_EPS_READ_CYCLES_T0)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010810ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010810ull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0 != literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0, 0, 12, 52 );
                    }
                    else if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T0 == literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (literal_0x001, 0, 12, 52 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x10010810ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010810ull)");
                    break;
                }
            }
        }

        fapi2::ATTR_PROC_EPS_READ_CYCLES_T1_Type l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_READ_CYCLES_T1, TGT1, l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_EPS_READ_CYCLES_T1)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010810ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010810ull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1 != literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1, 12, 12, 52 );
                    }
                    else if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T1 == literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (literal_0x001, 12, 12, 52 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x10010810ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010810ull)");
                    break;
                }
            }
        }

        fapi2::ATTR_PROC_EPS_READ_CYCLES_T2_Type l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_READ_CYCLES_T2, TGT1, l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_EPS_READ_CYCLES_T2)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010810ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010810ull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2 != literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2, 24, 12, 52 );
                    }
                    else if ((l_TGT1_ATTR_PROC_EPS_READ_CYCLES_T2 == literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (literal_0x001, 24, 12, 52 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x10010810ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010810ull)");
                    break;
                }
            }
        }

        fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T1_Type l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T1, TGT1, l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_EPS_WRITE_CYCLES_T1)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010811ull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1 != literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1, 0, 12, 52 );
                    }
                    else if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T1 == literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (literal_0x001, 0, 12, 52 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010811ull)");
                    break;
                }
            }
        }

        fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T2_Type l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_EPS_WRITE_CYCLES_T2, TGT1, l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_EPS_WRITE_CYCLES_T2)");
            break;
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010811ull)");
                    break;
                }

                {
                    if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2 != literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2, 12, 12, 52 );
                    }
                    else if ((l_TGT1_ATTR_PROC_EPS_WRITE_CYCLES_T2 == literal_0))
                    {
                        l_scom_buffer.insert<uint64_t> (literal_0x001, 12, 12, 52 );
                    }
                }

                l_rc = fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010811ull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010811ull)");
                    break;
                }

                {
                    constexpr auto l_EXP_L2_L2MISC_L2CERRS_EPS_CNT_USE_L2_DIVIDER_EN_OFF = 0x0;
                    l_scom_buffer.insert<uint64_t> (l_EXP_L2_L2MISC_L2CERRS_EPS_CNT_USE_L2_DIVIDER_EN_OFF, 29, 1, 63 );
                }

                l_rc = fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010811ull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010811ull)");
                    break;
                }

                {
                    l_scom_buffer.insert<uint64_t> (literal_0b0000, 30, 4, 60 );
                }

                l_rc = fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010811ull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010811ull)");
                    break;
                }

                {
                    l_scom_buffer.insert<uint64_t> (literal_0b0001, 24, 4, 60 );
                }

                l_rc = fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010811ull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x10010811ull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x10010811ull)");
                    break;
                }

                {
                    constexpr auto l_EXP_L2_L2MISC_L2CERRS_EPS_MODE_SEL_MODE1 = 0x0;
                    l_scom_buffer.insert<uint64_t> (l_EXP_L2_L2MISC_L2CERRS_EPS_MODE_SEL_MODE1, 28, 1, 63 );
                }

                l_rc = fapi2::putScom(TGT0, 0x10010811ull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x10010811ull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x1001080bull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x1001080bull)");
                    break;
                }

                {
                    l_scom_buffer.insert<uint64_t> (literal_0b0001, 4, 4, 60 );
                }

                l_rc = fapi2::putScom(TGT0, 0x1001080bull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x1001080bull)");
                    break;
                }
            }
        }

        {
            fapi2::buffer<uint64_t> l_scom_buffer;
            {
                l_rc = fapi2::getScom( TGT0, 0x1001080bull, l_scom_buffer );

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: getScom (0x1001080bull)");
                    break;
                }

                {
                    l_scom_buffer.insert<uint64_t> (literal_0b0100, 8, 4, 60 );
                }

                l_rc = fapi2::putScom(TGT0, 0x1001080bull, l_scom_buffer);

                if (l_rc)
                {
                    FAPI_ERR("ERROR executing: putScom (0x1001080bull)");
                    break;
                }
            }
        }

    }
    while(0);

    return l_rc;
}
