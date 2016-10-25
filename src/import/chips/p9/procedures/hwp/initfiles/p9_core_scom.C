/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/initfiles/p9_core_scom.C $ */
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
#include "p9_core_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;

constexpr auto literal_0x4301D70000AB7696 = 0x4301D70000AB7696;
constexpr auto literal_0x0000000000000000 = 0x0000000000000000;
constexpr auto literal_0xA858009775100008 = 0xA858009775100008;

fapi2::ReturnCode p9_core_scom(const fapi2::Target<fapi2::TARGET_TYPE_CORE>& TGT0)
{
    fapi2::ReturnCode l_rc = 0;

    do
    {
        fapi2::buffer<uint64_t> l_scom_buffer;
        {
            l_rc = fapi2::getScom( TGT0, 0x20010a43ull, l_scom_buffer );

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: getScom (0x20010a43ull)");
                break;
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0x4301D70000AB7696, 0, 64, 0 );
            }

            l_rc = fapi2::putScom(TGT0, 0x20010a43ull, l_scom_buffer);

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: putScom (0x20010a43ull)");
                break;
            }
        }
        {
            l_rc = fapi2::getScom( TGT0, 0x20010a46ull, l_scom_buffer );

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: getScom (0x20010a46ull)");
                break;
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0x0000000000000000, 0, 64, 0 );
            }

            l_rc = fapi2::putScom(TGT0, 0x20010a46ull, l_scom_buffer);

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: putScom (0x20010a46ull)");
                break;
            }
        }
        {
            l_rc = fapi2::getScom( TGT0, 0x20010a47ull, l_scom_buffer );

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: getScom (0x20010a47ull)");
                break;
            }

            {
                l_scom_buffer.insert<uint64_t> (literal_0xA858009775100008, 0, 64, 0 );
            }

            l_rc = fapi2::putScom(TGT0, 0x20010a47ull, l_scom_buffer);

            if (l_rc)
            {
                FAPI_ERR("ERROR executing: putScom (0x20010a47ull)");
                break;
            }
        }
    }
    while (0);

    return l_rc;
}
