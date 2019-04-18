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
    FAPI_TRY(fapi2::putScom(i_target, 0x00000008ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000028ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000028ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000018ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000000ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000020ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000010ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000010ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 5>(0xFull);
    l_data.insertFromRight<5, 5>(0xFull);
    l_data.insertFromRight<10, 5>(0xFull);
    l_data.insertFromRight<15, 5>(0xFull);
    l_data.insertFromRight<20, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107c5ull, l_data));
    l_data.insertFromRight<0, 5>(0xFull);
    l_data.insertFromRight<5, 5>(0xFull);
    l_data.insertFromRight<10, 5>(0xFull);
    l_data.insertFromRight<15, 5>(0xFull);
    l_data.insertFromRight<20, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c5ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 42>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107d1ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040008ull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040008ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 44>(0xFull);
    l_data.setBit<44>();
    l_data.insertFromRight<48, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050003ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050013ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00050013ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040009ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004001aull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x0004001aull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018207ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018222ull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00018242ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018265ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018265ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182c6ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c6ull, l_data));

    l_data.flush<0>();
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

    FAPI_TRY(fapi2::getScom(i_target, 0x0003c000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003c000ull, l_data));

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
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0018ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018225ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018225ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018247ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018286ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018286ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030012ull, l_data));
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

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00038000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00038000ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0003e000ull, l_data));

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
    l_data.setBit<12>();
    l_data.setBit<13>();
    l_data.insertFromRight<13, 10>(0xFull);
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
    l_data.setBit<23>();
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040004ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00040004ull, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x00000052ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000052ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000053ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000054ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000054ull, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x00040018ull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040018ull, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x00030028ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030028ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0019ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040007ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010406ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010406ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c8ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010545ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010545ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010587ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010602ull, l_data));
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

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010608ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010681ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010681ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000106c6ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c6ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010740ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010747ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010747ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010785ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003000aull, l_data));
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

    l_data.flush<0>();
    l_data.insertFromRight<0, 2>(0xFull);
    l_data.insertFromRight<2, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050036ull, l_data));
    l_data.insertFromRight<0, 3>(0xFull);
    l_data.insertFromRight<36, 2>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050036ull, l_data));

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
    l_data.setBit<0>();
    l_data.insertFromRight<1, 3>(0xFull);
    l_data.insertFromRight<4, 12>(0xFull);
    l_data.insertFromRight<16, 12>(0xFull);
    l_data.insertFromRight<28, 12>(0xFull);
    l_data.setBit<40>();
    l_data.insertFromRight<48, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00030004ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030020ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030020ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040016ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00039000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010405ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010405ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010480ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000104c7ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c7ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00010502ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010541ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010541ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010546ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010588ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010588ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010607ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010607ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c5ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010748ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010748ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010786ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000009ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000009ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000029ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000019ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000019ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000001ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000021ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000021ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000152ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000152ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000053ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000054ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x00000054ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c3ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107c6ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c6ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040013ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040019ull, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x00040002ull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040002ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0020ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0021ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0021ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0022ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0023ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0023ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0024ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0025ull, l_data));
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0025ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 6>(0xFull);
    l_data.setBit<6>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0026ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f002bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f002bull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018221ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018226ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018226ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018248ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018285ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018285ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e7ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e7ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00030011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003a000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003a000ull, l_data));

    l_data.flush<0>();
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

    FAPI_TRY(fapi2::getScom(i_target, 0x00010446ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010446ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010488ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010505ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010505ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000105c7ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010601ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010601ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010648ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010686ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010686ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    l_data.insertFromRight<24, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010707ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040000ull, l_data));
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

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0011ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00000055ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000055ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 4>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0029ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107cdull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040014ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00040010ull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040010ull, l_data));

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
    l_data.insertFromRight<1, 3>(0xFull);
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
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001full, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x0003d000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003d000ull, l_data));

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
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.insertFromRight<16, 16>(0xFull);
    l_data.insertFromRight<32, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050001ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005001bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005001bull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050014ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00050034ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050034ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018203ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018229ull, l_data));
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

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a4ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f0002ull, l_data));
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0002ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x000303feull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030008ull, l_data));
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

    FAPI_TRY(fapi2::getScom(i_target, 0x00010007ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010007ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040015ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000dull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000dull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000eull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000full, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000full, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00050032ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005003bull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003bull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018224ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018241ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018241ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018283ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a0ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a0ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a9ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000aull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000aull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000bull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004000cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0004000cull, l_data));

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
    l_data.insertFromRight<0, 24>(0xFull);
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
    l_data.insertFromRight<17, 2>(0xFull);
    l_data.setBit<19>();
    l_data.insertFromRight<20, 3>(0xFull);
    l_data.insertFromRight<23, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000107c0ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0004001bull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x0004001bull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00010005ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018249ull, l_data));
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

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018263ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182c4ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c4ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0004ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030030ull, l_data));
    l_data.setBit<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00030030ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0028ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010441ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010441ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00010489ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000104c3ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000104c3ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010580ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010584ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010584ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010603ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010649ull, l_data));
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

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010744ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f001cull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001cull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.insertFromRight<1, 4>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000303ffull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000003ffull, l_data));
    l_data.setBit<0>();
    l_data.insertFromRight<1, 4>(0xFull);
    l_data.insertFromRight<8, 8>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000003ffull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.setBit<60>();
    l_data.setBit<61>();
    l_data.setBit<62>();
    l_data.setBit<63>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00000155ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107d0ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x000107d0ull, l_data));

    l_data.flush<0>();
    l_data.setBit<0>();
    l_data.setBit<1>();
    l_data.setBit<2>();
    l_data.setBit<3>();
    l_data.setBit<4>();
    FAPI_TRY(fapi2::putScom(i_target, 0x00040017ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0005003cull, l_data));
    l_data.insertFromRight<0, 44>(0xFull);
    l_data.setBit<44>();
    FAPI_TRY(fapi2::putScom(i_target, 0x0005003cull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00018209ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018223ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018223ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018284ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182a2ull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x000182c2ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00030010ull, l_data));
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

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0000ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x0003b000ull, l_data));
    FAPI_TRY(fapi2::putScom(i_target, 0x0003b000ull, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x00000157ull, l_data));
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.insertFromRight<21, 11>(0xFull);
    l_data.insertFromRight<32, 5>(0xFull);
    l_data.insertFromRight<41, 11>(0xFull);
    l_data.insertFromRight<52, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000157ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<1, 11>(0xFull);
    l_data.insertFromRight<12, 5>(0xFull);
    l_data.insertFromRight<21, 11>(0xFull);
    l_data.insertFromRight<32, 5>(0xFull);
    l_data.insertFromRight<41, 11>(0xFull);
    l_data.insertFromRight<52, 5>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00000156ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000107ceull, l_data));
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
    FAPI_TRY(fapi2::putScom(i_target, 0x00040011ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x000f001eull, l_data));

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

    FAPI_TRY(fapi2::getScom(i_target, 0x00050000ull, l_data));
    l_data.insertFromRight<0, 16>(0xFull);
    l_data.insertFromRight<16, 16>(0xFull);
    l_data.insertFromRight<32, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050000ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 16>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00050022ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018201ull, l_data));
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018201ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00018204ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00018289ull, l_data));
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

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182a3ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000182e0ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000182e0ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<3, 3>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0003ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000f03feull, l_data));
    l_data.setBit<0>();
    l_data.setBit<1>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f03feull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0010ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010404ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010404ull, l_data));

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
    FAPI_TRY(fapi2::putScom(i_target, 0x00010509ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010543ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010543ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 32>(0xFull);
    l_data.insertFromRight<32, 10>(0xFull);
    l_data.insertFromRight<42, 9>(0xFull);
    l_data.setBit<51>();
    l_data.setBit<52>();
    l_data.setBit<53>();
    l_data.insertFromRight<54, 10>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010641ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x000106c4ull, l_data));
    l_data.insertFromRight<0, 24>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x000106c4ull, l_data));

    l_data.flush<0>();
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010780ull, l_data));

    FAPI_TRY(fapi2::getScom(i_target, 0x00010783ull, l_data));
    l_data.insertFromRight<0, 64>(0xFull);
    FAPI_TRY(fapi2::putScom(i_target, 0x00010783ull, l_data));

    l_data.flush<0>();
    FAPI_TRY(fapi2::putScom(i_target, 0x000f0017ull, l_data));

fapi_try_exit:
    return fapi2::current_err;
}
