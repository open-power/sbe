/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/examples/p10_raw.C $      */
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
#include "fapi2.H"
#include "p10_raw.H"

fapi2::ReturnCode p10_raw(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    fapi2::buffer<uint64_t> l_data;

    FAPI_TRY(fapi2::getScom(i_target, 0x00030006ull, l_data));
    l_data.insertFromRight<0, 2>(0xFull);
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.setBit<54>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030006ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030004ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000008ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000008ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000028ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000018ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000018ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000020ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000020ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107c2ull, l_data));
    l_data.insertFromRight<0, 5>(0xFull);
    l_data.insertFromRight<5, 5>(0xFull);
    l_data.insertFromRight<10, 5>(0xFull);
    l_data.insertFromRight<15, 5>(0xFull);
    l_data.insertFromRight<20, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c2ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 5>(0xFull);
    l_data.insertFromRight<5, 5>(0xFull);
    l_data.insertFromRight<10, 5>(0xFull);
    l_data.insertFromRight<15, 5>(0xFull);
    l_data.insertFromRight<20, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c5ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107d1ull, l_data));
    l_data.insertFromRight<0, 42>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107d1ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040088ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050003ull, l_data));
    l_data.insertFromRight<0, 44>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<48, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050003ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050013ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002824ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002824ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002890ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050020ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050020ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002887ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a1cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a1cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050087ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028c1ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c1ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b04ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500c1ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c1ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c30ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c30ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005010cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002940ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002940ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002d00ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050140ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050140ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002935ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cd4ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cd4ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050135ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002916ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002916ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c58ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050116ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050116ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002818ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002860ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002860ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050018ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002938ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002938ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002ce0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050138ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050138ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002918ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c60ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c60ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050118ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040004ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040004ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040044ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040064ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040064ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040054ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018207ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018207ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018222ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018242ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018242ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018265ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a8ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a8ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c6ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030001ull, l_data));
    l_data.insertFromRight<0, 4>(0xFull);
    l_data.insertFromRight<4, 4>(0xFull);
    l_data.insertFromRight<8, 4>(0xFull);
    l_data.insertFromRight<12, 8>(0xFull);
    l_data.insertFromRight<20, 12>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.insertFromRight<44, 2>(0xFull);
    l_data.insertFromRight<47, 5>(0xFull);
    l_data.insertFromRight<52, 12>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030001ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f007bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f004bull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f004bull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f005aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0062ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0062ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003c000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003c000ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000806eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000807full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000807full, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000801dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008024ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008024ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008030ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008039ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008039ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000803cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000805full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000805full, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.insertFromRight<18, 2>(0xFull);
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.insertFromRight<24, 9>(0xFull);
    l_data.insertFromRight<33, 7>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040003ull, l_data));
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 2>(0xFull);
    l_data.insertFromRight<16, 2>(0xFull);
    l_data.insertFromRight<18, 2>(0xFull);
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040003ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010407ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010481ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010481ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c5ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010540ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010540ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010548ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010586ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010586ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010605ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000106c7ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c7ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010742ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010746ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010746ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010782ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010788ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010788ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040040ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040060ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040060ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040050ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030002ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00030002ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.insertFromRight<8, 5>(0xFull);
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.insertFromRight<16, 4>(0xFull);
    l_data.insertFromRight<20, 4>(0xFull);
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030013ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010003ull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.insertFromRight<18, 13>(0xFull);
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.insertFromRight<35, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010003ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.insertFromRight<5, 2>(0xFull);
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050023ull, l_data));
    l_data.insertFromRight<0, 44>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<48, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050023ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050033ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005003aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000293cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000293cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005004full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002842ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002842ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002908ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050042ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050042ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002888ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a20ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a20ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050088ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028cbull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028cbull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b2cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500cbull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500cbull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002904ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c10ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c10ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050104ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002822ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002822ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002888ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050022ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050022ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028b8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028b8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005002eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002817ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002817ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000285cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050017ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050017ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000283aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028e8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028e8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0018ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0018ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018225ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018247ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018247ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018286ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e8ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e8ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<4, 5>(0xFull);
    l_data.insertFromRight<9, 5>(0xFull);
    l_data.insertFromRight<14, 5>(0xFull);
    l_data.insertFromRight<19, 5>(0xFull);
    l_data.insertFromRight<24, 5>(0xFull);
    l_data.insertFromRight<29, 5>(0xFull);
    l_data.insertFromRight<34, 5>(0xFull);
    l_data.insertFromRight<39, 5>(0xFull);
    l_data.insertFromRight<44, 5>(0xFull);
    l_data.insertFromRight<49, 5>(0xFull);
    l_data.insertFromRight<54, 5>(0xFull);
    l_data.insertFromRight<59, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030012ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010012ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010012ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0051ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0051ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00038000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003e000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003e000ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008001ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008064ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008064ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000800bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000801aull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000801aull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008027ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000802full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000802full, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000803aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008046ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008046ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008055ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010400ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010400ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010445ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010487ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010487ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010506ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c1ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c1ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010647ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010647ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010685ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010708ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010708ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<10>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<11>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<12>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.insertFromRight<13, 10>(0xFull);
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000100ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005002full, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 3>(0xFull);
    l_data.setBit<5>();
    l_data.insertFromRight<6, 4>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.insertFromRight<16, 4>(0xFull);
    l_data.insertFromRight<20, 3>(0xFull);
    l_data.insertFromRight<24, 3>(0xFull);
    l_data.setBit<28>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005002full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002930ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002930ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005004cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002845ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002845ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002914ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050045ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050045ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c9ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b24ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b24ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c9ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002903ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002903ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c0cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050103ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050103ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002827ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000289cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000289cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050027ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000282bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028acull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005002bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005002bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000281aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002868ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002868ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002810ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002810ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002840ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050010ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050010ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000283bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028ecull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028ecull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000281dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000281dull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002874ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005001dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018220ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018228ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018228ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018246ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018287ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018287ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c1ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e5ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e5ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010015ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0045ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0045ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0054ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0014ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0014ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000806bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008076ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008076ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008015ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008015ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008028ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008006ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008006ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008044ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000804dull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000804dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008052ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040002ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040002ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 60>(0xFull);
    l_data.insertFromRight<60, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040021ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040002ull, l_data));
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 2>(0xFull);
    l_data.insertFromRight<16, 2>(0xFull);
    l_data.insertFromRight<18, 2>(0xFull);
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040002ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010448ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010486ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010486ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010507ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c5ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c5ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010600ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010646ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010646ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010688ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010705ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010705ull, l_data));

    l_data.flush<0>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030009ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f000full, l_data));
    l_data.insertFromRight<36, 3>(0xFull);
    l_data.setBit<39>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f000full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000052ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000053ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000053ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000054ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040080ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040080ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005001aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001aull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 3>(0xFull);
    l_data.setBit<5>();
    l_data.insertFromRight<6, 4>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.insertFromRight<16, 4>(0xFull);
    l_data.insertFromRight<20, 3>(0xFull);
    l_data.insertFromRight<24, 3>(0xFull);
    l_data.setBit<28>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005000full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050020ull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.insertFromRight<16, 16>(0xFull);
    l_data.insertFromRight<32, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050020ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 2>(0xFull);
    l_data.insertFromRight<2, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050031ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000282cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028b0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050028ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050028ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002880ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a00ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a00ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050080ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000288bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a2cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005008bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005008bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c6ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b18ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b18ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c6ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000290full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c3cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005010full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005010full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002833ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028ccull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028ccull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050033ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050034ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050034ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002835ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028d4ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028d4ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050035ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002931ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002931ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cc4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050131ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050131ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002913ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c4cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c4cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050113ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018206ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018206ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018261ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018268ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018268ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018280ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018282ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018282ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a5ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182c7ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c7ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040003ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040003ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0001ull, l_data));
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0001ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0040ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0041ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0041ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0042ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0001001bull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001001bull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030028ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0019ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0019ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f004eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f005full, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f005full, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0067ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000806cull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000806cull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008075ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008078ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008078ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008013ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000801full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000801full, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008032ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008043ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008043ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000804eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008009ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008009ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000805cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040042ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040042ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040062ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040052ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040052ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010406ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000104c8ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c8ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010545ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010587ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010587ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010602ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010608ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010608ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010681ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000106c2ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c2ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c6ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010740ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010740ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010747ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010785ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010785ull, l_data));

    l_data.flush<0>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0034ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0034ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 2>(0xFull);
    l_data.insertFromRight<2, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050036ull, l_data));
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.insertFromRight<36, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050036ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002934ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002934ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005004dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002844ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002844ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002910ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050044ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050044ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b20ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b20ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002902ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002902ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c08ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050102ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050102ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000281bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000286cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000286cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002811ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002811ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002844ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050011ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050011ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002934ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cd0ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cd0ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050134ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000283cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000283cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028f0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005003cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003cull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018227ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018245ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018245ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018288ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e6ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e6ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0003ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0003ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0006ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0006ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.insertFromRight<4, 12>(0xFull);
    l_data.insertFromRight<16, 12>(0xFull);
    l_data.insertFromRight<28, 12>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<48, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030004ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010014ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010014ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030020ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0044ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0044ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0055ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0013ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0013ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000806aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008077ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008077ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008014ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008014ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008029ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008045ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008045ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008007ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000804cull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000804cull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008053ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040018ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040018ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.insertFromRight<8, 4>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.insertFromRight<16, 4>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.insertFromRight<24, 2>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000dull, l_data));
    l_data.insertFromRight<0, 60>(0xFull);
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010401ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010447ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010447ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010485ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010508ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010508ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c6ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c6ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010645ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010687ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010687ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010706ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003000cull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.insertFromRight<48, 12>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0020ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0022ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0022ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0021ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0023ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0023ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0025ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0024ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0024ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0026ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0028ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0028ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0027ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0029ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0029ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f002aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f002eull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030001ull, l_data));
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<8>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030001ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040081ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050019ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050019ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.setBit<44>();
    l_data.setBit<45>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050021ull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.insertFromRight<16, 16>(0xFull);
    l_data.insertFromRight<32, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050021ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002881ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002881ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a04ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050081ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050081ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a28ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a28ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005008aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028c7ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c7ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b1cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500c7ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c7ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c38ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c38ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005010eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002830ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002830ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050030ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050030ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002917ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c5cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c5cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050117ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018205ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018205ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018267ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a6ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a6ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0044ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0044ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0045ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0046ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0046ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001001aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f004full, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f004full, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f005eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0066ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0066ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00039000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000806dull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000806dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008074ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008079ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008079ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008012ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000801eull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000801eull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008033ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008042ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008042ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000804full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008008ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008008ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000805dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040022ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.insertFromRight<3, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040022ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010405ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010480ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010480ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c7ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010502ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010502ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010541ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010546ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010546ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010588ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c2ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c2ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010607ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000106c5ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c5ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010748ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010786ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010786ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0033ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f002full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0031ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0030ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0030ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000009ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000029ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000029ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000019ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000001ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000001ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000021ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000011ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000011ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000152ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000053ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000053ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000054ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107c3ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c3ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c6ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010001ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010001ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 31>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010008ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005001cull, l_data));
    l_data.insertFromRight<0, 44>(0xFull);
    l_data.setBit<44>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001cull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0018ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002886ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002886ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a18ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050086ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050086ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b00ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b00ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000290dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290dull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c34ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005010dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005010dull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002821ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002884ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002884ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050021ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000282dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282dull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028b4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005002dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005002dull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002941ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002d04ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002d04ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050141ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002912ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002912ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c48ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050112ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050112ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018208ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018260ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018260ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018266ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018281ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018281ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a7ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182c5ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c5ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040043ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040063ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040063ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040015ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040013ull, l_data));
    l_data.insertFromRight<1, 15>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040013ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f007aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f004aull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f004aull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f005bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0063ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0063ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000806full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000807eull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000807eull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000801cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008025ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008025ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008031ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008038ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008038ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000803dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000805eull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000805eull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 31>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 31>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040009ull, l_data));
    l_data.insertFromRight<0, 8>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    l_data.insertFromRight<16, 8>(0xFull);
    l_data.insertFromRight<24, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040009ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 8>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    l_data.insertFromRight<16, 8>(0xFull);
    l_data.insertFromRight<24, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040007ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.insertFromRight<22, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040007ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040029ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010408ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010408ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010442ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010482ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010482ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c6ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010547ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010547ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010585ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010606ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010606ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010680ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000106c8ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c8ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010741ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010745ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010745ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010787ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050012ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050012ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.insertFromRight<36, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050016ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050039ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050039ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.setBit<44>();
    l_data.setBit<45>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050030ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000284eull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002938ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005004eull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005004eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002843ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000290cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050043ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002889ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002889ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a24ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050089ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050089ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028caull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b28ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b28ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500caull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002905ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002905ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c14ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050105ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050105ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000292aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002ca8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002ca8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005012aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000292bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000292bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cacull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005012bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005012bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002930ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cc0ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cc0ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050130ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002816ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002816ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002858ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050016ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050016ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002839ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028e4ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028e4ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050039ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0020ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0020ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0021ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0022ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0022ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0023ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0024ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0024ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0025ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0026ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0026ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018221ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018221ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018226ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018248ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018248ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018285ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182c0ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c0ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e7ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030011ull, l_data));
    l_data.insertFromRight<4, 5>(0xFull);
    l_data.insertFromRight<9, 5>(0xFull);
    l_data.insertFromRight<14, 5>(0xFull);
    l_data.insertFromRight<19, 5>(0xFull);
    l_data.insertFromRight<24, 5>(0xFull);
    l_data.insertFromRight<29, 5>(0xFull);
    l_data.insertFromRight<34, 5>(0xFull);
    l_data.insertFromRight<39, 5>(0xFull);
    l_data.insertFromRight<44, 5>(0xFull);
    l_data.insertFromRight<49, 5>(0xFull);
    l_data.insertFromRight<54, 5>(0xFull);
    l_data.insertFromRight<59, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030011ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010013ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010001ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010001ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 53>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0050ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0050ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003a000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008000ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008000ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000800aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008065ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008065ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000801bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008026ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008026ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000802eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000803bull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000803bull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008047ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008054ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008054ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 35>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040012ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030000ull, l_data));
    l_data.insertFromRight<0, 4>(0xFull);
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<15>();
    l_data.insertFromRight<16, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030000ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.insertFromRight<10, 6>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 2>(0xFull);
    l_data.insertFromRight<19, 5>(0xFull);
    l_data.insertFromRight<24, 2>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040032ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.insertFromRight<50, 3>(0xFull);
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040032ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.insertFromRight<50, 3>(0xFull);
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040033ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040006ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 3>(0xFull);
    l_data.setBit<5>();
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 4>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.insertFromRight<16, 6>(0xFull);
    l_data.insertFromRight<22, 10>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040006ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040017ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000full, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 31>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 31>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000full, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010446ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010488ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010488ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010505ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c7ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c7ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010601ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010648ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010648ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010686ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010707ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010707ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030006ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00030006ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000055ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0029ull, l_data));
    l_data.insertFromRight<0, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0029ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 2>(0xFull);
    l_data.insertFromRight<16, 2>(0xFull);
    l_data.insertFromRight<18, 2>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.insertFromRight<34, 5>(0xFull);
    l_data.insertFromRight<39, 5>(0xFull);
    l_data.insertFromRight<44, 2>(0xFull);
    l_data.insertFromRight<46, 4>(0xFull);
    l_data.insertFromRight<50, 4>(0xFull);
    l_data.insertFromRight<54, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107cdull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040106ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040106ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010006ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010006ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.insertFromRight<24, 8>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000284bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000292cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005004bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005004bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002846ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002918ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002918ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050046ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028cfull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028cfull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b3cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500cfull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500cfull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002900ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c00ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c00ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050100ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002823ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002823ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050023ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050023ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028bcull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028bcull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005002full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002831ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002831ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050031ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050031ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002911ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c44ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c44ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050111ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002813ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002813ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050013ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050013ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000283eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028f8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028f8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018243ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018243ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018269ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e4ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e4ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.insertFromRight<21, 43>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010016ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010016ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010006ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00010006ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.insertFromRight<6, 32>(0xFull);
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<52>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.insertFromRight<56, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0046ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0046ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0057ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0011ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0011ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003d000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030005ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030005ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008068ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000800full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000800full, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008016ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000802bull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000802bull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000804aull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000804aull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008051ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010402ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010402ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010483ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000104c1ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c1ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000104c9ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c9ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010500ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c4ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c4ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010609ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010643ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010643ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010704ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003000eull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.insertFromRight<48, 12>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000004ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000024ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000024ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000014ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000151ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 4>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 11>(0xFull);
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 11>(0xFull);
    l_data.insertFromRight<44, 4>(0xFull);
    l_data.setBit<48>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000151ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040083ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050001ull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.insertFromRight<16, 16>(0xFull);
    l_data.insertFromRight<32, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050001ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050014ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050014ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050034ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002804ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002804ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002810ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050004ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050004ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002849ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002924ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002924ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050049ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000288cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a30ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005008cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005008cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002883ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a0cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a0cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050083ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028c5ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c5ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b14ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500c5ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c5ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002832ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028c8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050032ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000291bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000291bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c6cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005011bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005011bull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002937ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cdcull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cdcull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050137ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000283full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000283full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028fcull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005003full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003full, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018203ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018203ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018229ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a4ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a4ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010003ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010003ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001001cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010019ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010019ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f004dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f005cull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f005cull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0064ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008072ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008072ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000807bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008020ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008020ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008035ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008040ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008040ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000805bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004001dull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004001dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004001eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004001full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004001full, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010403ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010440ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010440ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010449ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010544ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010544ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010581ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010689ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010689ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c3ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010784ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010784ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000303feull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000303feull, l_data));

    l_data.flush<0>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030008ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030008ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00030008ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000003ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000023ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000023ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000013ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000058ull, l_data));
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000058ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000059ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107c1ull, l_data));
    l_data.insertFromRight<0, 8>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    l_data.insertFromRight<16, 8>(0xFull);
    l_data.insertFromRight<24, 8>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.insertFromRight<52, 4>(0xFull);
    l_data.insertFromRight<56, 4>(0xFull);
    l_data.insertFromRight<60, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c1ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 8>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    l_data.insertFromRight<16, 8>(0xFull);
    l_data.insertFromRight<24, 8>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.insertFromRight<52, 4>(0xFull);
    l_data.insertFromRight<56, 4>(0xFull);
    l_data.insertFromRight<60, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040084ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040084ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010007ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000288full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a3cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005008full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005008full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002884ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a10ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a10ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050084ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028c2ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c2ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b08ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500c2ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c2ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c2cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c2cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005010bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002908ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002908ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c20ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050108ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050108ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000293bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cecull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cecull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005013bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002943ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002943ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002d0cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050143ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050143ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002933ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cccull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cccull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050133ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002808ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002808ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002820ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050008ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050008ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018200ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018264ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018264ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c3ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e1ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e1ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e9ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0001001full, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001001full, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0078ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0048ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0048ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0059ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0061ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0061ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008071ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000807cull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000807cull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008023ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008036ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008036ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000803full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008059ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008059ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008060ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040030ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.insertFromRight<50, 3>(0xFull);
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040030ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 60>(0xFull);
    l_data.insertFromRight<60, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040020ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000104c4ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c4ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010583ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000105c9ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c9ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010604ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010640ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010640ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010709ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010743ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010743ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010781ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f03ffull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 4>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f03ffull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0032ull, l_data));
    l_data.insertFromRight<0, 26>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0032ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030007ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000057ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.insertFromRight<21, 11>(0xFull);
    l_data.insertFromRight<32, 5>(0xFull);
    l_data.insertFromRight<41, 11>(0xFull);
    l_data.insertFromRight<52, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000057ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.insertFromRight<21, 11>(0xFull);
    l_data.insertFromRight<32, 5>(0xFull);
    l_data.insertFromRight<41, 11>(0xFull);
    l_data.insertFromRight<52, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000056ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107cfull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.insertFromRight<22, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107cfull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040103ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040104ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040104ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040105ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050032ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050032ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f001eull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002841ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002904ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002904ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050041ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028ccull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028ccull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b30ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500ccull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500ccull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002907ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c1cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c1cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050107ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002826ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002826ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002898ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050026ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050026ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028a8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028a8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005002aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002814ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002814ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002850ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050014ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050014ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002914ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c50ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c50ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050114ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018224ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018224ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018241ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018283ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018283ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a9ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a9ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040100ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040101ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040101ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040102ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0001ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0001ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0004ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0004ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 6>(0xFull);
    l_data.insertFromRight<18, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001000eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010009ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.insertFromRight<13, 6>(0xFull);
    l_data.insertFromRight<19, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010009ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040001ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040001ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0052ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0052ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008067ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000800cull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000800cull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008002ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008019ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008019ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000802cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008049ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008049ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008056ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008063ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008063ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040012ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040012ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040013ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040014ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040014ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040015ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040016ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040016ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010444ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010503ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010503ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010542ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010549ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010549ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010582ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010684ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010684ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c1ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010700ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010700ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010789ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000002ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000002ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000022ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000012ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000012ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000101ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000003feull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000003feull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000159ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000051ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 4>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 11>(0xFull);
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 11>(0xFull);
    l_data.insertFromRight<44, 4>(0xFull);
    l_data.setBit<48>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000051ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.insertFromRight<3, 3>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.insertFromRight<17, 2>(0xFull);
    l_data.setBit<19>();
    l_data.insertFromRight<20, 3>(0xFull);
    l_data.insertFromRight<23, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040085ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040085ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002801ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002801ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002804ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050001ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050001ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000280bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000282cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000282cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005000bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002802ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002802ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002808ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050002ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050002ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002805ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002814ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002814ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000288eull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a38ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005008eull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005008eull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002885ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a14ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a14ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050085ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028c3ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c3ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b0cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500c3ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c3ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000290aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c28ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c28ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005010aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002909ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002909ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c24ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050109ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050109ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002825ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002894ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002894ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050025ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002829ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002829ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028a4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050029ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050029ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002836ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002837ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002837ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028d8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050036ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050036ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000291aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c68ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c68ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005011aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002942ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002942ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002d08ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050142ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050142ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000281eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002878ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002878ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040014ull, l_data));
    l_data.insertFromRight<1, 53>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040014ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018249ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018263ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018263ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0004ull, l_data));
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0004ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001001eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030030ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030030ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0028ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0079ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0079ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0049ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0058ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0058ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0060ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003f000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003f000ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008070ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000807dull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000807dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008022ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008037ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008037ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000803eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008058ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008058ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008061ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040001ull, l_data));
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 2>(0xFull);
    l_data.insertFromRight<16, 2>(0xFull);
    l_data.insertFromRight<18, 2>(0xFull);
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040001ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010441ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010489ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010489ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c3ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010580ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010580ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010584ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010603ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010603ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010649ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010744ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010744ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001cull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000303ffull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 4>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000303ffull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030009ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00030009ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 4>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000003ffull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000155ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000155ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000107d0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005003cull, l_data));
    l_data.insertFromRight<0, 44>(0xFull);
    l_data.setBit<44>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002806ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002818ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002818ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050006ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002840ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002840ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002900ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050040ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050040ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028cdull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b34ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b34ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500cdull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002906ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002906ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c18ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050106ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050106ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002910ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c40ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c40ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050110ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002815ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002815ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002854ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050015ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050015ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002936ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002cd8ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cd8ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050136ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002809ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002809ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002824ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050009ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050009ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002838ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028e0ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028e0ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050038ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018209ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018209ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018223ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018284ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018284ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182c2ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c2ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 4>(0xFull);
    l_data.insertFromRight<4, 5>(0xFull);
    l_data.insertFromRight<9, 5>(0xFull);
    l_data.insertFromRight<14, 5>(0xFull);
    l_data.insertFromRight<19, 5>(0xFull);
    l_data.insertFromRight<24, 5>(0xFull);
    l_data.insertFromRight<29, 5>(0xFull);
    l_data.insertFromRight<34, 5>(0xFull);
    l_data.insertFromRight<39, 5>(0xFull);
    l_data.insertFromRight<44, 5>(0xFull);
    l_data.insertFromRight<49, 5>(0xFull);
    l_data.insertFromRight<54, 5>(0xFull);
    l_data.insertFromRight<59, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010000ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.insertFromRight<4, 4>(0xFull);
    l_data.setBit<15>();
    l_data.insertFromRight<23, 3>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    l_data.insertFromRight<40, 8>(0xFull);
    l_data.insertFromRight<48, 8>(0xFull);
    l_data.insertFromRight<56, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010000ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0000ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040041ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040061ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040061ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040051ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0053ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0053ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003b000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008066ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008066ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000800dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008018ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008018ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008003ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000802dull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000802dull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008048ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008057ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008057ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008062ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040024ull, l_data));
    l_data.insertFromRight<0, 4>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040024ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040008ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000aull, l_data));
    l_data.insertFromRight<0, 4>(0xFull);
    l_data.insertFromRight<4, 4>(0xFull);
    l_data.insertFromRight<8, 5>(0xFull);
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.insertFromRight<16, 8>(0xFull);
    l_data.insertFromRight<24, 8>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    l_data.insertFromRight<40, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000aull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010409ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010443ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010443ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010501ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010501ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010504ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010642ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010642ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010682ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010683ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010683ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c9ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003000aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000aull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.insertFromRight<21, 11>(0xFull);
    l_data.insertFromRight<32, 5>(0xFull);
    l_data.insertFromRight<41, 11>(0xFull);
    l_data.insertFromRight<52, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000157ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000156ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.insertFromRight<21, 11>(0xFull);
    l_data.insertFromRight<32, 5>(0xFull);
    l_data.insertFromRight<41, 11>(0xFull);
    l_data.insertFromRight<52, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000156ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 2>(0xFull);
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.insertFromRight<48, 3>(0xFull);
    l_data.setBit<51>();
    l_data.insertFromRight<52, 3>(0xFull);
    l_data.setBit<55>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000107ceull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040107ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.setBit<50>();
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040107ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000281full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000287cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000287cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001full, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000284aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000284aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002928ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005004aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005004aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002847ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000291cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000291cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050047ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028ceull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028ceull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b38ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000500ceull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000500ceull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002901ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c04ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c04ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050101ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000293aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000293aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002ce8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005013aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005013aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002812ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002848ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002848ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050012ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002932ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002932ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002cc8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050132ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050132ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002915ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c54ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c54ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050115ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002928ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002928ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002ca0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050128ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050128ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000283dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000028f4ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000028f4ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018202ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018202ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018240ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018244ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018244ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018262ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a1ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a1ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c9ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e3ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e3ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 12>(0xFull);
    l_data.insertFromRight<12, 12>(0xFull);
    l_data.insertFromRight<24, 12>(0xFull);
    l_data.insertFromRight<36, 12>(0xFull);
    l_data.setBit<49>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.insertFromRight<55, 2>(0xFull);
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.insertFromRight<59, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030003ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010017ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010017ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<3>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010008ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0047ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0047ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0056ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0012ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0012ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008069ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000800eull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000800eull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008017ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008004ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008004ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000802aull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000804bull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000804bull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008050ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f001eull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 8>(0xFull);
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001eull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040023ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040004ull, l_data));
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.setBit<3>();
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 2>(0xFull);
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 2>(0xFull);
    l_data.insertFromRight<16, 2>(0xFull);
    l_data.insertFromRight<18, 2>(0xFull);
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040004ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 4>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.insertFromRight<16, 4>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.insertFromRight<26, 2>(0xFull);
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.insertFromRight<32, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040027ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 8>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040027ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010484ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010589ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010589ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c3ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010644ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010644ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010701ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010701ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 8>(0xFull);
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.insertFromRight<12, 2>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010702ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010703ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010703ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010749ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003000dull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.insertFromRight<48, 12>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000dull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<48>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003000bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0008ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0008ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000005ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000005ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000025ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000015ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000015ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000158ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040082ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040082ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.insertFromRight<16, 16>(0xFull);
    l_data.insertFromRight<32, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050022ull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050022ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002803ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000280cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0000280cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050003ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002848ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002848ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002920ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050048ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050048ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0000288dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002a34ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a34ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005008dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002882ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002882ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002a08ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050082ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050082ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000028c4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002b10ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002b10ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000500c4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002920ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002920ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c80ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050120ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050120ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002921ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c84ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c84ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050121ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002922ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002922ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c88ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050122ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050122ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002923ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c8cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c8cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050123ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002924ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002924ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c90ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050124ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050124ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002925ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c94ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c94ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050125ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002926ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002926ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c98ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050126ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050126ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00002927ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00002c9cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00002c9cull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050127ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f001aull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001aull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018201ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018204ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018204ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00018289ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a3ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a3ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0003ull, l_data));
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0003ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0001001dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010018ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010018ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f03feull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f004cull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f004cull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f005dull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0065ull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.setBit<4>();
    l_data.insertFromRight<5, 3>(0xFull);
    l_data.setBit<8>();
    l_data.insertFromRight<9, 3>(0xFull);
    l_data.setBit<12>();
    l_data.insertFromRight<13, 3>(0xFull);
    l_data.setBit<16>();
    l_data.insertFromRight<17, 3>(0xFull);
    l_data.setBit<20>();
    l_data.insertFromRight<21, 3>(0xFull);
    l_data.setBit<24>();
    l_data.insertFromRight<25, 3>(0xFull);
    l_data.setBit<28>();
    l_data.insertFromRight<29, 3>(0xFull);
    l_data.setBit<32>();
    l_data.insertFromRight<33, 3>(0xFull);
    l_data.setBit<36>();
    l_data.insertFromRight<37, 3>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<41, 3>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<45, 3>(0xFull);
    l_data.setBit<48>();
    l_data.insertFromRight<49, 3>(0xFull);
    l_data.setBit<52>();
    l_data.insertFromRight<53, 3>(0xFull);
    l_data.setBit<56>();
    l_data.insertFromRight<57, 3>(0xFull);
    l_data.setBit<60>();
    l_data.insertFromRight<61, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0065ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008073ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000807aull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000807aull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008021ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00008034ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008034ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00008041ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0000805aull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x0000805aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040031ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    l_data.setBit<5>();
    l_data.setBit<6>();
    l_data.setBit<7>();
    l_data.setBit<8>();
    l_data.setBit<9>();
    l_data.setBit<10>();
    l_data.setBit<11>();
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.setBit<14>();
    l_data.setBit<15>();
    l_data.setBit<16>();
    l_data.setBit<17>();
    l_data.setBit<18>();
    l_data.setBit<19>();
    l_data.setBit<20>();
    l_data.setBit<21>();
    l_data.setBit<22>();
    l_data.setBit<23>();
    l_data.setBit<24>();
    l_data.setBit<25>();
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.setBit<28>();
    l_data.setBit<29>();
    l_data.setBit<30>();
    l_data.setBit<31>();
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    l_data.setBit<37>();
    l_data.setBit<38>();
    l_data.setBit<39>();
    l_data.setBit<40>();
    l_data.setBit<41>();
    l_data.setBit<42>();
    l_data.setBit<43>();
    l_data.setBit<44>();
    l_data.setBit<45>();
    l_data.setBit<46>();
    l_data.setBit<47>();
    l_data.setBit<48>();
    l_data.setBit<49>();
    l_data.insertFromRight<50, 3>(0xFull);
    l_data.setBit<53>();
    l_data.setBit<54>();
    l_data.setBit<55>();
    l_data.setBit<56>();
    l_data.setBit<57>();
    l_data.setBit<58>();
    l_data.setBit<59>();
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040031ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010404ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010509ull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.insertFromRight<2, 2>(0xFull);
    l_data.insertFromRight<4, 2>(0xFull);
    l_data.insertFromRight<6, 2>(0xFull);
    l_data.insertFromRight<8, 2>(0xFull);
    l_data.insertFromRight<10, 4>(0xFull);
    l_data.insertFromRight<14, 4>(0xFull);
    l_data.insertFromRight<18, 4>(0xFull);
    l_data.insertFromRight<22, 4>(0xFull);
    l_data.setBit<26>();
    l_data.setBit<27>();
    l_data.insertFromRight<28, 4>(0xFull);
    l_data.setBit<32>();
    l_data.setBit<33>();
    l_data.setBit<34>();
    l_data.setBit<35>();
    l_data.setBit<36>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010509ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010543ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010641ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010641ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010780ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010780ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010783ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0017ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0017ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 53>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00040010ull, l_data));

fapi_try_exit:
    return fapi2::current_err;
}
