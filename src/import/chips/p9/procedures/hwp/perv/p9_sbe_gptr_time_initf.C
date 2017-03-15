/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_gptr_time_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
/// @file  p9_sbe_gptr_time_initf.C
///
/// @brief Load time and GPTR rings for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p9_sbe_gptr_time_initf.H"

#include "p9_perv_scom_addresses.H"
#include <p9_ring_id.h>


fapi2::ReturnCode p9_sbe_gptr_time_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below

    FAPI_INF("p9_sbe_gptr_time_initf: Entering ...");

    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>
         (fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Scan mc_gptr ring");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_gptr),
                 "Error from putRing (mc_gptr)");
        FAPI_DBG("Scan mc_iom01_gptr ring");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_gptr),
                 "Error from putRing (mc_iom01_gptr)");
        FAPI_DBG("Scan mc_iom23_gptr ring");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_gptr),
                 "Error from putRing (mc_iom23_gptr)");
        FAPI_DBG("Scan mc_pll_gptr ring");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_pll_gptr),
                 "Error from putRing (mc_pll_gptr)");
        FAPI_DBG("Scan mc_time ring");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_time),
                 "Error from putRing (mc_time)");
    }

    for( auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         ( fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0x9))/* OBUS0 Chiplet */
        {
            FAPI_DBG("Scan ob0_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_gptr),
                     "Error from putRing (ob0_gptr)");
            FAPI_DBG("Scan ob0_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_pll_gptr),
                     "Error from putRing (ob0_pll_gptr)");
            FAPI_DBG("Scan ob0_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_time),
                     "Error from putRing (ob0_time)");
        }

        if ((l_attr_chip_unit_pos == 0xA))/* OBUS1 Chiplet */
        {
            FAPI_DBG("Scan ob1_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob1_gptr),
                     "Error from putRing (ob1_gptr)");
            FAPI_DBG("Scan ob1_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob1_pll_gptr),
                     "Error from putRing (ob1_pll_gptr)");
            FAPI_DBG("Scan ob1_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob1_time),
                     "Error from putRing (ob1_time)");
        }

        if ((l_attr_chip_unit_pos == 0xB))/* OBUS2 Chiplet */
        {
            FAPI_DBG("Scan ob2_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob2_gptr),
                     "Error from putRing (ob2_gptr)");
            FAPI_DBG("Scan ob2_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob2_pll_gptr),
                     "Error from putRing (ob2_pll_gptr)");
            FAPI_DBG("Scan ob2_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob2_time),
                     "Error from putRing (ob2_time)");
        }

        if ((l_attr_chip_unit_pos == 0xC))/* OBUS3 Chiplet */
        {
            FAPI_DBG("Scan ob3_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_gptr),
                     "Error from putRing (ob3_gptr)");
            FAPI_DBG("Scan ob3_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_pll_gptr),
                     "Error from putRing (ob3_pll_gptr)");
            FAPI_DBG("Scan ob3_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_time),
                     "Error from putRing (ob3_time)");
        }

        if ((l_attr_chip_unit_pos == 0x6))/* XBUS Chiplet */
        {
            FAPI_DBG("Scan xb_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_gptr),
                     "Error from putRing (xb_gptr)");
            FAPI_DBG("Scan xb_io1_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_gptr),
                     "Error from putRing (xb_io1_gptr)");
            FAPI_DBG("Scan xb_io2_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_gptr),
                     "Error from putRing (xb_io2_gptr)");
            FAPI_DBG("Scan xb_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_pll_gptr),
                     "Error from putRing (xb_pll_gptr)");
            FAPI_DBG("Scan xb_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_time),
                     "Error from putRing (xb_time)");
            FAPI_DBG("Scan xb_io1_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_time),
                     "Error from putRing (xb_io1_time)");
            FAPI_DBG("Scan xb_io2_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_time),
                     "Error from putRing (xb_io2_time)");
        }

        if ((l_attr_chip_unit_pos == 0xD))/* PCI0 Chiplet */
        {
            FAPI_DBG("Scan pci0_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_gptr),
                     "Error from putRing (pci0_gptr)");
            FAPI_DBG("Scan pci0_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_pll_gptr),
                     "Error from putRing (pci0_pll_gptr)");
            FAPI_DBG("Scan pci0_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_time),
                     "Error from putRing (pci0_time)");
        }

        if ((l_attr_chip_unit_pos == 0xE))/* PCI1 Chiplet */
        {
            FAPI_DBG("Scan pci1_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_gptr),
                     "Error from putRing (pci1_gptr)");
            FAPI_DBG("Scan pci1_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_pll_gptr),
                     "Error from putRing (pci1_pll_gptr)");
            FAPI_DBG("Scan pci1_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_time),
                     "Error from putRing (pci1_time)");
        }

        if ((l_attr_chip_unit_pos == 0xF))/* PCI2 Chiplet */
        {
            FAPI_DBG("Scan pci2_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_gptr),
                     "Error from putRing (pci2_gptr)");
            FAPI_DBG("Scan pci2_pll_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_pll_gptr),
                     "Error from putRing (pci2_pll_gptr)");
            FAPI_DBG("Scan pci2_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_time),
                     "Error from putRing (pci2_time)");
        }

        if ((l_attr_chip_unit_pos == 0x2))/* N0 Chiplet */
        {
            FAPI_DBG("Scan n0_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_gptr),
                     "Error from putRing (n0_gptr)");
            FAPI_DBG("Scan n0_nx_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_gptr),
                     "Error from putRing (n0_nx_gptr)");
            FAPI_DBG("Scan n0_cxa0_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_gptr),
                     "Error from putRing (n0_cxa0_gptr)");
            FAPI_DBG("Scan n0_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_time),
                     "Error from putRing (n0_time)");
            FAPI_DBG("Scan n0_nx_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_time),
                     "Error from putRing (n0_nx_time)");
            FAPI_DBG("Scan n0_cxa0_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_time),
                     "Error from putRing (n0_cxa0_time)");
        }

        if ((l_attr_chip_unit_pos == 0x3))/* N1 Chiplet */
        {
            FAPI_DBG("Scan n1_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_gptr),
                     "Error from putRing (n1_gptr)");
            FAPI_DBG("Scan n1_ioo0_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_gptr),
                     "Error from putRing (n1_ioo0_gptr)");
            FAPI_DBG("Scan n1_ioo1_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_gptr),
                     "Error from putRing (n1_ioo1_gptr)");
            FAPI_DBG("Scan n1_mcs23_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_gptr),
                     "Error from putRing (n1_mcs23_gptr)");
            FAPI_DBG("Scan n1_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_time),
                     "Error from putRing (n1_time)");
            FAPI_DBG("Scan n1_ioo0_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_time),
                     "Error from putRing (n1_ioo0_time)");
            FAPI_DBG("Scan n1_ioo1_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_time),
                     "Error from putRing (n1_ioo1_time)");
            FAPI_DBG("Scan n1_mcs23_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_time),
                     "Error from putRing (n1_mcs23_time)");
        }

        if ((l_attr_chip_unit_pos == 0x4))/* N2 Chiplet */
        {
            FAPI_DBG("Scan n2_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_gptr),
                     "Error from putRing (n2_gptr)");
            FAPI_DBG("Scan n2_psi_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_psi_gptr),
                     "Error from putRing (n2_psi_gptr)");
            FAPI_DBG("Scan n2_cxa1_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_gptr),
                     "Error from putRing (n2_cxa1_gptr)");
            FAPI_DBG("Scan n2_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_time),
                     "Error from putRing (n2_time)");
            FAPI_DBG("Scan n2_cxa1_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_time),
                     "Error from putRing (n2_cxa1_time)");
        }

        if ((l_attr_chip_unit_pos == 0x5))/* N3 Chiplet */
        {
            FAPI_DBG("Scan n3_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_gptr),
                     "Error from putRing (n3_gptr)");
            FAPI_DBG("Scan n3_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_time),
                     "Error from putRing (n3_time)");
            FAPI_DBG("Scan n3_np_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_gptr),
                     "Error from putRing (n3_np_gptr)");
            FAPI_DBG("Scan n3_mcs01_gptr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_gptr),
                     "Error from putRing (n3_mcs01_gptr)");
            FAPI_DBG("Scan n3_mcs01_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_time),
                     "Error from putRing (n3_mcs01_time)");
            FAPI_DBG("Scan n3_np_time ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_time),
                     "Error from putRing (n3_np_time)");
        }
    }

fapi_try_exit:
    FAPI_INF("p9_sbe_gptr_time_initf: Exiting ...");
    return fapi2::current_err;

}
