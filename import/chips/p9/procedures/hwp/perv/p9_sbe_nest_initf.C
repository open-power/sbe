/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/perv/p9_sbe_nest_initf.C $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_nest_initf.H"
#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"

fapi2::ReturnCode p9_sbe_nest_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("Entering ...");
    uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
    fapi2::buffer<uint64_t> l_data64;
    l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_DETERMINISTIC_TEST_ENABLE_DC>();

    for( auto l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         ( fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0x02))/* N0 Chiplet */
        {
            FAPI_DBG("Scan N0 chiplet rings");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_fure));
        }

        if ((l_attr_chip_unit_pos == 0x03))/* N1 Chiplet */
        {
            FAPI_DBG("Scan N1 chiplet rings");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_fure));
        }

        if ((l_attr_chip_unit_pos == 0x04))/* N2 Chiplet */
        {
            FAPI_DBG("Scan N2 chiplet rings");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_psi_fure));
        }

        if ((l_attr_chip_unit_pos == 0x05))/* N3 Chiplet */
        {
            FAPI_DBG("Scan N3 chiplet rings");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_fure));
        }
    }

    for( auto l_chplt_trgt :  i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>
         (fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Scan MC chiplet rings");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_fure));
        FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_OR, l_data64));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_fure));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_fure));
        FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_OR, l_data64));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
