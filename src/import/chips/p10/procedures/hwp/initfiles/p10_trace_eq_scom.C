/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/initfiles/p10_trace_eq_scom.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
#include "p10_trace_eq_scom.H"
#include <stdint.h>
#include <stddef.h>
#include <fapi2.H>

using namespace fapi2;

constexpr uint64_t literal_0b1 = 0b1;

fapi2::ReturnCode p10_trace_eq_scom(const fapi2::Target<fapi2::TARGET_TYPE_EQ>& TGT0,
                                    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& TGT1)
{
    {
        fapi2::ATTR_EC_Type   l_chip_ec;
        fapi2::ATTR_NAME_Type l_chip_id;
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_NAME, TGT1, l_chip_id));
        FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, TGT1, l_chip_ec));
        fapi2::buffer<uint64_t> l_scom_buffer;
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20018202ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x20018202ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20018222ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x20018222ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20018242ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x20018242ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20018262ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x20018262ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x20018282ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x20018282ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x200182a2ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x200182a2ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x200182c2ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x200182c2ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x200182e2ull, l_scom_buffer ));

            l_scom_buffer.insert<23, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x200182e2ull, l_scom_buffer));
        }
        {
            FAPI_TRY(fapi2::getScom( TGT0, 0x200183e0ull, l_scom_buffer ));

            l_scom_buffer.insert<13, 1, 63, uint64_t>(literal_0b1 );
            l_scom_buffer.insert<26, 1, 63, uint64_t>(literal_0b1 );
            FAPI_TRY(fapi2::putScom(TGT0, 0x200183e0ull, l_scom_buffer));
        }

    };
fapi_try_exit:
    return fapi2::current_err;
}
