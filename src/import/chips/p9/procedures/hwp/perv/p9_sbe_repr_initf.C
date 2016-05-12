/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_repr_initf.C $ */
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

fapi2::ReturnCode p9_sbe_repr_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET belo

    FAPI_INF("Entering ...");

    for (auto l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>
         (fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_DBG("Scan repr rings for mc chiplets ");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_repr));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_repr));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_repr));
    }

    for( auto l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         ( fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0x09))/* OBUS0 Chiplet */
        {
            FAPI_DBG("Scan repr rings for obus chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_repr));
        }

        //Rings need to be scanned for OBUS 2 and 3 incase of Cumulus
        if ((l_attr_chip_unit_pos == 0xC))/* OBUS3 Chiplet */
        {
            FAPI_DBG("Scan repr rings for obus chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_repr));
        }

        if ((l_attr_chip_unit_pos == 0x06))/* XBUS Chiplet */
        {
            FAPI_DBG("Scan repr rings for xbus chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io0_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_repr));
        }

        if ((l_attr_chip_unit_pos == 0xD))/* PCI0 Chiplet */
        {
            FAPI_DBG("Scan repr rings for pci0 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_repr));
        }

        if ((l_attr_chip_unit_pos == 0xE))/* PCI1 Chiplet */
        {
            FAPI_DBG("Scan repr rings for pci1 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_repr));
        }

        if ((l_attr_chip_unit_pos == 0xF))/* PCI2 Chiplet */
        {
            FAPI_DBG("Scan repr rings for pci2 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_repr));
        }

        if ((l_attr_chip_unit_pos == 0x02))/* N0 Chiplet */
        {
            FAPI_DBG("Scan repr rings for n0 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_repr));
        }

        if ((l_attr_chip_unit_pos == 0x03))/* N1 Chiplet */
        {
            FAPI_DBG("Scan repr rings for n1 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_repr));
        }

        if ((l_attr_chip_unit_pos == 0x04))/* N2 Chiplet */
        {
            FAPI_DBG("Scan repr rings for n2 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_repr));
        }

        if ((l_attr_chip_unit_pos == 0x05))/* N3 Chiplet */
        {
            FAPI_DBG("Scan repr rings for n3 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_repr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_repr));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
