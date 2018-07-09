/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_nest_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2018                        */
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
//------------------------------------------------------------------------------
/// @file  p9_sbe_nest_initf.C
///
/// @brief Scan rings for Nest and Mc chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_nest_initf.H"
#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include "p9_const_common.H"
#include <p9_ring_id.h>

fapi2::ReturnCode
p9_sbe_nest_initf_sw430383_wa(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("Start");

    fapi2::ATTR_CHIP_EC_FEATURE_SW430383_Type l_sw430383;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_SW430383,
                           i_target,
                           l_sw430383),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_SW430383");

    if (l_sw430383)
    {
        fapi2::buffer<uint64_t> l_scan_region;
        fapi2::buffer<uint64_t> l_scan_data;

        // ring:
        //   n2_fure                             0x04035C0F   96269        N    Y     Y     Y NESTN2            OFF
        //
        // bits to set:
        //   1    4083   92185 0 PE2.PB2.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)
        //   1   45823   50445 0 PE1.PB1.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)
        //   1   70000   26268 0 PE0.PB0.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)

        // inject header
        l_scan_region.setBit<PERV_1_SCAN_REGION_TYPE_PERV>().  // PERV
        setBit<PERV_1_SCAN_REGION_TYPE_UNIT2>().           // PCIS0
        setBit<PERV_1_SCAN_REGION_TYPE_UNIT3>().           // PCIS1
        setBit<PERV_1_SCAN_REGION_TYPE_UNIT4>().           // PCIS2
        setBit<PERV_1_SCAN_REGION_TYPE_FUNC>().            // FUNC
        setBit<PERV_1_SCAN_REGION_TYPE_REGF>();            // REGF
        FAPI_TRY(fapi2::putScom(i_target, PERV_N2_SCAN_REGION_TYPE, l_scan_region));
        l_scan_data = 0xA5A5A5A5A5A5A5A5;
        FAPI_TRY(fapi2::putScom(i_target, PERV_N2_SCAN32, l_scan_data));

        // scan 0..4083 (37*110 + 13)
        for (auto ii = 0; ii < 37; ii++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x6E, l_scan_data));
        }

        FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x0D, l_scan_data));

        // flip PE2.PB2.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2
        FAPI_DBG("Flip PE2.PB2.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2");
        l_scan_data.setBit<0>();
        FAPI_TRY(fapi2::putScom(i_target, PERV_N2_SCAN32, l_scan_data));

        // scan 4083..45823 (379*110 + 50)
        for (auto ii = 0; ii < 379; ii++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x6E, l_scan_data));
        }

        FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x32, l_scan_data));

        // flip PE1.PB1.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)
        FAPI_DBG("Flip PE1.PB1.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)");
        l_scan_data.setBit<0>();
        FAPI_TRY(fapi2::putScom(i_target, PERV_N2_SCAN32, l_scan_data));

        // scan 45823..70000 (219*110 + 87)
        for (auto ii = 0; ii < 219; ii++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x6E, l_scan_data));
        }

        FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x57, l_scan_data));

        // flip PE0.PB0.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)
        FAPI_DBG("Flip PE0.PB0.PBCQ.PELDST.SPARE_LAT.SPARE_0.LATC.L2(0)");
        l_scan_data.setBit<0>();
        FAPI_TRY(fapi2::putScom(i_target, PERV_N2_SCAN32, l_scan_data));

        // scan 70000..96269 (238*110 + 89)
        for (auto ii = 0; ii < 238; ii++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x6E, l_scan_data));
        }

        FAPI_TRY(fapi2::getScom(i_target, PERV_N2_SCAN32 + 0x59, l_scan_data));

        // check header
        FAPI_ASSERT((l_scan_data == 0xA5A5A5A5A5A5A5A5),
                    fapi2::P9_PUTRING_CHECKWORD_DATA_MISMATCH().
                    set_TARGET(i_target).
                    set_CHIPLET_ID(0x02).
                    set_SCOM_ADDRESS(PERV_N2_SCAN32).
                    set_SCOM_DATA(l_scan_data()).
                    set_BITS_DECODED(0).
                    set_RINGID(n2_fure).
                    set_RINGMODE(fapi2::RING_MODE_HEADER_CHECK).
                    set_RETURN_CODE(0),
                    "Error rotating n2_fure for sw430383");
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}


fapi2::ReturnCode p9_sbe_nest_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("Entering ...");
    uint8_t l_attr_chip_unit_pos = 0;
    fapi2::buffer<uint16_t> l_read_attr;
#if 0
    bool mc01_iom01 = false;
    bool mc01_iom23 = false;
    bool mc23_iom01 = false;
    bool mc23_iom23 = false;
#endif

    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, l_chplt_trgt, l_read_attr));
        FAPI_DBG("ATTR_PG Value : %#04lx", l_read_attr);
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

#if 0

        if((l_attr_chip_unit_pos == MC01_CHIPLET_ID) && (!l_read_attr.getBit<6>()))
        {
            mc01_iom01 = true;
        }

        if((l_attr_chip_unit_pos == MC01_CHIPLET_ID) && (!l_read_attr.getBit<7>()))
        {
            mc01_iom23 = true;
        }

        if((l_attr_chip_unit_pos == MC23_CHIPLET_ID) && (!l_read_attr.getBit<6>()))
        {
            mc23_iom01 = true;
        }

        if((l_attr_chip_unit_pos == MC23_CHIPLET_ID) && (!l_read_attr.getBit<7>()))
        {
            mc23_iom23 = true;
        }

#endif

        if (l_attr_chip_unit_pos == N0_CHIPLET_ID)/* N0 Chiplet */
        {

            if (!l_read_attr.getBit<6>()) //Check cxa0 is enable
            {
                FAPI_DBG("Scan n0_cxa_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_fure),
                         "Error from putRing (n0_cxa0_fure)");
            }

            FAPI_DBG("Scan n0_fure ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_fure),
                     "Error from putRing (n0_fure)");

            if (!l_read_attr.getBit<5>()) //Check nx is enable
            {
                FAPI_DBG("Scan n0_nx_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_fure),
                         "Error from putRing (n0_nx_fure)");
            }
        }

        if (l_attr_chip_unit_pos == N1_CHIPLET_ID)/* N1 Chiplet */
        {
            FAPI_DBG("Scan n1_fure ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_fure),
                     "Error from putRing (n1_fure)");

            if (!l_read_attr.getBit<7>()) //Check pbioo0 is enable
            {
                FAPI_DBG("Scan n1_ioo0_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_fure),
                         "Error from putRing (n1_ioo0_fure)");
            }

            if (!l_read_attr.getBit<8>()) //Check pbioo1 is enable
            {
                FAPI_DBG("Scan n1_ioo1_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_fure),
                         "Error from putRing (n1_ioo1_fure)");
            }

            if (!l_read_attr.getBit<9>()) //Check mcs23 is enable
            {
                FAPI_DBG("Scan n1_mcs23_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_fure),
                         "Error from putRing (n1_mcs23_fure)");
            }
        }

        if (l_attr_chip_unit_pos == N2_CHIPLET_ID)/* N2 Chiplet */
        {

            if (!l_read_attr.getBit<5>()) //Check cxa1 is enable
            {
                FAPI_DBG("Scan n2_cxa1_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_fure),
                         "Error from putRing (n2_cxa1_fure)");
            }

            FAPI_DBG("Scan n2_fure ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_fure),
                     "Error from putRing (n2_fure)");

            FAPI_TRY(p9_sbe_nest_initf_sw430383_wa(i_target_chip),
                     "Error from p9_sbe_nest_initf_sw430383_wa");

            if (!l_read_attr.getBit<9>()) //Check iopsi is enable
            {
                FAPI_DBG("Scan n2_psi_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n2_psi_fure),
                         "Error from putRing (n2_psi_fure)");
            }
        }

        if (l_attr_chip_unit_pos == N3_CHIPLET_ID)/* N3 Chiplet */
        {
            uint8_t l_hw388874 = 0;
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW388874, i_target_chip, l_hw388874),
                     "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_HW388874)");

            FAPI_DBG("Scan n3_fure ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_fure),
                     "Error from putRing (n3_fure)");

            if (!l_hw388874)
            {
                FAPI_DBG("Scan n3_br_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n3_br_fure),
                         "Error from putRing (n3_br_fure)");
            }

            if (!l_read_attr.getBit<10>()) //Check mcs01 is enable
            {
                FAPI_DBG("Scan n3_mcs01_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_fure),
                         "Error from putRing (n3_mcs01_fure)");
            }

            if (!l_read_attr.getBit<7>()) //Check npu is enable
            {
                FAPI_DBG("Scan n3_np_fure ring");
                FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_fure),
                         "Error from putRing (n3_np_fure)");
            }
        }
    }

    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_fure));
    }

#if 0

    // mcbist0
    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>
         (fapi2::TARGET_FILTER_MC_WEST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        {
            // MC IOMxx FURE rings require deterministic scan enable
            // no current plan to scan these during mainline IPL, but recipe is below if needed
            fapi2::buffer<uint64_t> l_data64;
            l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_DETERMINISTIC_TEST_ENABLE_DC>();
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_MC01_CPLT_CTRL0_OR, l_data64));

            if ( mc01_iom01 )
            {
                FAPI_DBG("Scan mc_iom01_fure ring");
                FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_fure),
                         "Error from putRing (mc_iom01_fure)");
            }

            if( mc01_iom23 )
            {
                FAPI_DBG("Scan mc_iom23_fure ring");
                FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_fure),
                         "Error from putRing (mc_iom23_fure)");
            }

            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_MC01_CPLT_CTRL0_CLEAR, l_data64));
        }
    }

    // mcbist1
    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>
         (fapi2::TARGET_FILTER_MC_EAST, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        {
            // MC IOMxx FURE rings require deterministic scan enable
            // no current plan to scan these during mainline IPL, but recipe is below if needed
            fapi2::buffer<uint64_t> l_data64;
            l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_DETERMINISTIC_TEST_ENABLE_DC>();
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_MC01_CPLT_CTRL0_OR, l_data64));

            if ( mc23_iom01 )
            {
                FAPI_DBG("Scan mc_iom01_fure ring");
                FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_fure),
                         "Error from putRing (mc_iom01_fure)");
            }

            if( mc23_iom23 )
            {
                FAPI_DBG("Scan mc_iom23_fure ring");
                FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_fure),
                         "Error from putRing (mc_iom23_fure)");
            }

            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_MC01_CPLT_CTRL0_CLEAR, l_data64));
        }
    }

#endif


fapi_try_exit:
    FAPI_INF("Exiting ...");
    return fapi2::current_err;
}
