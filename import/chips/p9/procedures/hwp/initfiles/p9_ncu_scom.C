/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/initfiles/p9_ncu_scom.C $             */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2016                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#include "p9_ncu_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;

constexpr auto literal_0b0001 = 0b0001;
constexpr auto literal_0b0100 = 0b0100;
constexpr auto literal_0x8 = 0x8;
constexpr auto literal_0x10 = 0x10;

fapi2::ReturnCode p9_ncu_scom(const fapi2::Target<fapi2::TARGET_TYPE_EX>& TGT0,
                              const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>& TGT1)
{
    fapi2::ReturnCode l_rc = 0;

    do
    {
        fapi2::buffer<uint64_t> l_scom_buffer;
        fapi2::ATTR_PROC_FABRIC_ADDR_BAR_MODE_Type l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE;
        l_rc = FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_ADDR_BAR_MODE, TGT1, l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE);

        if (l_rc)
        {
            FAPI_ERR("ERROR executing: FAPI_ATTR_GET (ATTR_PROC_FABRIC_ADDR_BAR_MODE)");
            break;
        }

        {
            l_rc = fapi2::getScom( TGT0, 0x1001100aull, l_scom_buffer );

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: getScom (0x1001100aull)");
                break;
            }

            {
                if ((l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE == fapi2::ENUM_ATTR_PROC_FABRIC_ADDR_BAR_MODE_SMALL_SYSTEM))
                {
                    constexpr auto l_scom_buffer_ON = 0x1;
                    l_scom_buffer.insert<uint64_t> (l_scom_buffer_ON, 9, 1, 63 );
                }
                else if ((l_TGT1_ATTR_PROC_FABRIC_ADDR_BAR_MODE == fapi2::ENUM_ATTR_PROC_FABRIC_ADDR_BAR_MODE_LARGE_SYSTEM))
                {
                    constexpr auto l_scom_buffer_OFF = 0x0;
                    l_scom_buffer.insert<uint64_t> (l_scom_buffer_OFF, 9, 1, 63 );
                }
            }

            l_rc = fapi2::putScom(TGT0, 0x1001100aull, l_scom_buffer);

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: putScom (0x1001100aull)");
                break;
            }
        }

        {
            l_rc = fapi2::getScom( TGT0, 0x1001100bull, l_scom_buffer );

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: getScom (0x1001100bull)");
                break;
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0b0001, 0, 4, 60 );
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0b0100, 4, 4, 60 );
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0x8, 8, 10, 54 );
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0x10, 26, 10, 54 );
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0x8, 18, 4, 60 );
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0x8, 22, 4, 60 );
            }

            l_rc = fapi2::putScom(TGT0, 0x1001100bull, l_scom_buffer);

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: putScom (0x1001100bull)");
                break;
            }
        }
    }
    while (0);

    return l_rc;
}
