/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_ringspin.gen.H $ */
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
// *INDENT-OFF*
#ifndef P10_CONTAINED_RINGSPIN_GEN_H
#define P10_CONTAINED_RINGSPIN_GEN_H

namespace ec10 {
static inline fapi2::ReturnCode ec_l3_func_restore_active_l3(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV> &i_cplt,
    const uint64_t i_scan_region_type,
    const uint64_t i_ecp_l3_l3_misc_l3cerrs_backing_l3_ctl_reg,
    const uint64_t i_ecp_l3_l3_misc_l3cerrs_mode_reg1)
{
    fapi2::buffer<uint64_t> tmp = 0;

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // ---
    {
        FAPI_TRY(svs::rotate(i_cplt, 17052, tmp));
        tmp &= 0x0fffffffffffffffull;
        // ECP.L3.L3_MISC.L3CERRS.BACKING_L3_CTL_REG[2:3]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_backing_l3_ctl_reg & 0x3000000000000000) >> 60) ^ 0b00) << 62;
        // ECP.L3.L3_MISC.L3CERRS.BACKING_L3_CTL_REG[0:1]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_backing_l3_ctl_reg & 0xc000000000000000) >> 62) ^ 0b00;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 60) | (((x & 0x0000000000000001ull) >> 0) << 61);
        }
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
    }
    {
        FAPI_TRY(svs::rotate(i_cplt, 5588, tmp));
        tmp &= 0x000000ffffffffffull;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[7:7]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0100000000000000) >> 56) ^ 0b1) << 63;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[11:11]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0010000000000000) >> 52) ^ 0b1) << 62;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[12:13]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x000c000000000000) >> 50) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 60) | (((x & 0x0000000000000001ull) >> 0) << 61);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[17:17]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000400000000000) >> 46) ^ 0b1) << 59;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[6:6]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0200000000000000) >> 57) ^ 0b1) << 58;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[38:38]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000002000000) >> 25) ^ 0b0) << 57;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[16:16]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000800000000000) >> 47) ^ 0b1) << 56;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[22:22]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000020000000000) >> 41) ^ 0b1) << 55;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[19:19]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000100000000000) >> 44) ^ 0b1) << 54;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[23:24]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000018000000000) >> 39) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 52) | (((x & 0x0000000000000001ull) >> 0) << 53);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[29:29]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000400000000) >> 34) ^ 0b1) << 51;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[0:0]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x8000000000000000) >> 63) ^ 0b0) << 50;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[36:36]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000008000000) >> 27) ^ 0b1) << 49;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[9:9]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0040000000000000) >> 54) ^ 0b1) << 48;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[25:25]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000004000000000) >> 38) ^ 0b1) << 47;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[28:28]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000800000000) >> 35) ^ 0b1) << 46;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[33:34]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000060000000) >> 29) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 44) | (((x & 0x0000000000000001ull) >> 0) << 45);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[35:35]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000010000000) >> 28) ^ 0b1) << 43;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[8:8]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0080000000000000) >> 55) ^ 0b1) << 42;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[37:37]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000004000000) >> 26) ^ 0b1) << 41;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[32:32]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000080000000) >> 31) ^ 0b1) << 40;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
    }
    {
        FAPI_TRY(svs::rotate(i_cplt, 386, tmp));
        tmp &= 0x003e0fffffffffffull;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[27:27]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000001000000000) >> 36) ^ 0b1) << 63;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[18:18]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000200000000000) >> 45) ^ 0b1) << 62;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[30:31]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000300000000) >> 32) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 60) | (((x & 0x0000000000000001ull) >> 0) << 61);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[20:20]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000080000000000) >> 43) ^ 0b1) << 59;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[26:26]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000002000000000) >> 37) ^ 0b1) << 58;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[14:15]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0003000000000000) >> 48) ^ 0b11) << 56;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[21:21]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000040000000000) >> 42) ^ 0b1) << 55;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[10:10]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0020000000000000) >> 53) ^ 0b1) << 54;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[4:5]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0c00000000000000) >> 58) ^ 0b00) << 47;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[2:3]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x3000000000000000) >> 60) ^ 0b00) << 45;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[1:1]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x4000000000000000) >> 62) ^ 0b0) << 44;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
        FAPI_TRY(svs::rotate(i_cplt, 47293, tmp));
    }
    // ---

    if (tmp != 0x00BAD666BAD66600) {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}
}// namespace ec10

namespace ec10 {
static inline fapi2::ReturnCode ec_l3_func_restore_backing_l3(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV> &i_cplt,
    const uint64_t i_scan_region_type,
    const uint64_t i_ecp_l3_l3_misc_l3cerrs_mode_reg0,
    const uint64_t i_ecp_l3_l3_misc_l3cerrs_mode_reg1)
{
    fapi2::buffer<uint64_t> tmp = 0;

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // ---
    {
        FAPI_TRY(svs::rotate(i_cplt, 22143, tmp));
        tmp &= 0x0000000007ffffffull;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[1:1]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x4000000000000000) >> 62) ^ 0b1) << 63;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[33:33]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000040000000) >> 30) ^ 0b0) << 62;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[13:13]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0004000000000000) >> 50) ^ 0b0) << 61;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[9:9]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0040000000000000) >> 54) ^ 0b1) << 60;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[34:34]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000020000000) >> 29) ^ 0b0) << 59;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[26:27]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000003000000000) >> 36) ^ 0b00) << 57;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[29:29]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000400000000) >> 34) ^ 0b0) << 56;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[22:22]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000020000000000) >> 41) ^ 0b0) << 55;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[20:21]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x00000c0000000000) >> 42) ^ 0b00) << 53;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[18:19]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000300000000000) >> 44) ^ 0b10) << 51;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[24:24]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000008000000000) >> 39) ^ 0b0) << 50;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[12:12]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0008000000000000) >> 51) ^ 0b0) << 49;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[25:25]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000004000000000) >> 38) ^ 0b0) << 48;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[28:28]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000800000000) >> 35) ^ 0b0) << 47;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[23:23]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000010000000000) >> 40) ^ 0b0) << 46;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[35:35]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000010000000) >> 28) ^ 0b0) << 45;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[32:32]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000080000000) >> 31) ^ 0b0) << 44;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[10:10]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0020000000000000) >> 53) ^ 0b0) << 43;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[15:15]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0001000000000000) >> 48) ^ 0b0) << 42;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[17:17]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000400000000000) >> 46) ^ 0b0) << 41;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[5:5]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0400000000000000) >> 58) ^ 0b0) << 40;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[7:8]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0180000000000000) >> 55) ^ 0b00) << 38;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[31:31]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000100000000) >> 32) ^ 0b0) << 37;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[2:3]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x3000000000000000) >> 60) ^ 0b00;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 35) | (((x & 0x0000000000000001ull) >> 0) << 36);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[11:11]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0010000000000000) >> 52) ^ 0b0) << 34;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[4:4]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0800000000000000) >> 59) ^ 0b0) << 33;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[36:36]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000008000000) >> 27) ^ 0b0) << 32;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[16:16]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000800000000000) >> 47) ^ 0b0) << 31;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[30:30]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0000000200000000) >> 33) ^ 0b0) << 30;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[6:6]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0200000000000000) >> 57) ^ 0b0) << 29;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[14:14]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x0002000000000000) >> 49) ^ 0b1) << 28;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG0[0:0]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg0 & 0x8000000000000000) >> 63) ^ 0b0) << 27;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
    }
    {
        FAPI_TRY(svs::rotate(i_cplt, 497, tmp));
        tmp &= 0x000000ffffffffffull;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[7:7]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0100000000000000) >> 56) ^ 0b1) << 63;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[11:11]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0010000000000000) >> 52) ^ 0b1) << 62;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[12:13]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x000c000000000000) >> 50) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 60) | (((x & 0x0000000000000001ull) >> 0) << 61);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[17:17]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000400000000000) >> 46) ^ 0b1) << 59;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[6:6]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0200000000000000) >> 57) ^ 0b1) << 58;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[38:38]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000002000000) >> 25) ^ 0b0) << 57;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[16:16]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000800000000000) >> 47) ^ 0b1) << 56;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[22:22]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000020000000000) >> 41) ^ 0b1) << 55;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[19:19]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000100000000000) >> 44) ^ 0b1) << 54;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[23:24]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000018000000000) >> 39) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 52) | (((x & 0x0000000000000001ull) >> 0) << 53);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[29:29]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000400000000) >> 34) ^ 0b1) << 51;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[0:0]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x8000000000000000) >> 63) ^ 0b0) << 50;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[36:36]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000008000000) >> 27) ^ 0b1) << 49;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[9:9]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0040000000000000) >> 54) ^ 0b1) << 48;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[25:25]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000004000000000) >> 38) ^ 0b1) << 47;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[28:28]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000800000000) >> 35) ^ 0b1) << 46;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[33:34]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000060000000) >> 29) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 44) | (((x & 0x0000000000000001ull) >> 0) << 45);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[35:35]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000010000000) >> 28) ^ 0b1) << 43;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[8:8]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0080000000000000) >> 55) ^ 0b1) << 42;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[37:37]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000004000000) >> 26) ^ 0b1) << 41;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[32:32]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000080000000) >> 31) ^ 0b1) << 40;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
    }
    {
        FAPI_TRY(svs::rotate(i_cplt, 386, tmp));
        tmp &= 0x003e0fffffffffffull;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[27:27]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000001000000000) >> 36) ^ 0b1) << 63;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[18:18]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000200000000000) >> 45) ^ 0b1) << 62;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[30:31]
        {
            uint64_t x = ((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000000300000000) >> 32) ^ 0b11;
            tmp |= (((x & 0x0000000000000002ull) >> 1) << 60) | (((x & 0x0000000000000001ull) >> 0) << 61);
        }
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[20:20]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000080000000000) >> 43) ^ 0b1) << 59;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[26:26]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000002000000000) >> 37) ^ 0b1) << 58;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[14:15]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0003000000000000) >> 48) ^ 0b11) << 56;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[21:21]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0000040000000000) >> 42) ^ 0b1) << 55;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[10:10]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0020000000000000) >> 53) ^ 0b1) << 54;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[4:5]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x0c00000000000000) >> 58) ^ 0b00) << 47;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[2:3]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x3000000000000000) >> 60) ^ 0b00) << 45;
        // ECP.L3.L3_MISC.L3CERRS.MODE_REG1[1:1]
        tmp |= (((i_ecp_l3_l3_misc_l3cerrs_mode_reg1 & 0x4000000000000000) >> 62) ^ 0b0) << 44;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
        FAPI_TRY(svs::rotate(i_cplt, 47293, tmp));
    }
    // ---

    if (tmp != 0x00BAD666BAD66600) {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}
}// namespace ec10

namespace ec10 {
static inline fapi2::ReturnCode ec_cl2_mode_disable_core_sreset(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV> &i_cplt,
    const uint64_t i_scan_region_type,
    const uint64_t i_ecp_ec_pc_pmc_decr_extended_func,
    const uint64_t i_ecp_ec_pc_thrctl_tctlcom_decr_extended_func)
{
    fapi2::buffer<uint64_t> tmp = 0;

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // ---
    {
        FAPI_TRY(svs::rotate(i_cplt, 4606, tmp));
        tmp &= 0x7fffffffffffffffull;
        // ECP.EC.PC.THRCTL.TCTLCOM.DECR_EXTENDED_FUNC[0:0]
        tmp |= (((i_ecp_ec_pc_thrctl_tctlcom_decr_extended_func & 0x8000000000000000) >> 63) ^ 0b0) << 63;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
    }
    {
        FAPI_TRY(svs::rotate(i_cplt, 5631, tmp));
        tmp &= 0x7fffffffffffffffull;
        // ECP.EC.PC.PMC.DECR_EXTENDED_FUNC[0:0]
        tmp |= (((i_ecp_ec_pc_pmc_decr_extended_func & 0x8000000000000000) >> 63) ^ 0b0) << 63;
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));
        FAPI_TRY(svs::rotate(i_cplt, 5744, tmp));
    }
    // ---

    if (tmp != 0x00BAD666BAD66600) {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}
}// namespace ec10

#endif//P10_CONTAINED_RINGSPIN_GEN_H
// *INDENT-ON*
