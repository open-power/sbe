/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_repr_initf.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/// @file  p9_sbe_repr_initf.C
///
/// @brief Load Repair rings for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p9_sbe_repr_initf.H"
#include "p9_perv_scom_addresses.H"
#include <p9_ring_id.h>


fapi2::ReturnCode p9_sbe_repr_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_attr_chip_unit_pos = 0;
    FAPI_INF("p9_sbe_repr_initf: Entering ...");

    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_repr));
    }

    for (auto& l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

        if (l_attr_chip_unit_pos == 0x9)/* OBUS0 Chiplet */
        {
            FAPI_DBG("Scan ob0_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_repr),
                     "Error from putRing (ob0_repr)");
        }

        if (l_attr_chip_unit_pos == 0xA)/* OBUS1 Chiplet */
        {
            FAPI_DBG("Scan ob1_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob1_repr),
                     "Error from putRing (ob1_repr)");
        }

        if (l_attr_chip_unit_pos == 0xB)/* OBUS2 Chiplet */
        {
            FAPI_DBG("Scan ob2_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob2_repr),
                     "Error from putRing (ob2_repr)");
        }

        if (l_attr_chip_unit_pos == 0xC)/* OBUS3 Chiplet */
        {
            FAPI_DBG("Scan ob3_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_repr),
                     "Error from putRing (ob3_repr)");
        }

        if (l_attr_chip_unit_pos == 0x6)/* XBUS Chiplet */
        {
            FAPI_DBG("Scan xb_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_repr),
                     "Error from putRing (xb_repr)");
            FAPI_DBG("Scan xb_io1_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_repr),
                     "Error from putRing (xb_io1_repr)");
            FAPI_DBG("Scan xb_io2_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_repr),
                     "Error from putRing (xb_io2_repr)");
        }

        if (l_attr_chip_unit_pos == 0xD)/* PCI0 Chiplet */
        {
            FAPI_DBG("Scan pci0_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_repr),
                     "Error from putRing (pci0_repr)");
        }

        if (l_attr_chip_unit_pos == 0xE)/* PCI1 Chiplet */
        {
            FAPI_DBG("Scan pci1_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_repr),
                     "Error from putRing (pci1_repr)");
        }

        if (l_attr_chip_unit_pos == 0xF)/* PCI2 Chiplet */
        {
            FAPI_DBG("Scan pci2_repr_ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_repr),
                     "Error from putRing (pci2_repr)");
        }

        if (l_attr_chip_unit_pos == 0x2)/* N0 Chiplet */
        {
            FAPI_DBG("Scan n0_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_repr),
                     "Error from putRing (n0_repr)");
            FAPI_DBG("Scan n0_nx_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_repr),
                     "Error from putRing (n0_nx_repr)");
            FAPI_DBG("Scan n0_cxa0_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_repr),
                     "Error from putRing (n0_cxa0_repr)");
        }

        if (l_attr_chip_unit_pos == 0x3)/* N1 Chiplet */
        {
            FAPI_DBG("Scan n1_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_repr),
                     "Error from putRing (n1_repr)");
            FAPI_DBG("Scan n1_ioo0_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_repr),
                     "Error from putRing (n1_ioo0_repr)");
            FAPI_DBG("Scan n1_ioo1_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_repr),
                     "Error from putRing (n1_ioo1_repr)");
            FAPI_DBG("Scan n1_mcs23_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_repr),
                     "Error from putRing (n1_mcs23_repr)");
        }

        if (l_attr_chip_unit_pos == 0x4)/* N2 Chiplet */
        {
            FAPI_DBG("Scan n2_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_repr),
                     "Error from putRing (n2_repr)");
            FAPI_DBG("Scan n2_cxa1_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_repr),
                     "Error from putRing (n2_cxa1_repr)");
        }

        if (l_attr_chip_unit_pos == 0x5)/* N3 Chiplet */
        {
            FAPI_DBG("Scan n3_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_repr),
                     "Error from putRing (n3_repr)");
            FAPI_DBG("Scan n3_mcs01_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_repr),
                     "Error from putRing (n3_mcs01_repr)");
            FAPI_DBG("Scan n3_np_repr ring");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_repr),
                     "Error from putRing (n3_np_repr)");
        }
    }

fapi_try_exit:
    FAPI_INF("p9_sbe_repr_initf: Exiting ...");
    return fapi2::current_err;

}

