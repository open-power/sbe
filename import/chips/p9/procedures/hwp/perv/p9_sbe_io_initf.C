/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_io_initf.C $              */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_io_initf.C
///
/// @brief Initialize necessary latches in IP chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_io_initf.H"
#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"

fapi2::ReturnCode p9_sbe_io_initf(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("Entering ...");
    uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
    fapi2::buffer<uint64_t> l_data64;
    l_data64.setBit<PERV_1_CPLT_CTRL0_TC_UNIT_DETERMINISTIC_TEST_ENABLE_DC>();

    for( auto l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         ( fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0xD))/* PCI0 Chiplet */
        {
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_OR, l_data64));
            FAPI_DBG("Scan fure rings for PCI0 chiplets ");
            // FAPI_TRY(fapi2::putRing(i_target_chip, pci0_fure));
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_CLEAR, l_data64));
        }

        if ((l_attr_chip_unit_pos == 0xE))/* PCI1 Chiplet */
        {
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_OR, l_data64));
            FAPI_DBG("Scan fure rings for PCI1 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_fure));
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_CLEAR, l_data64));
        }

        if ((l_attr_chip_unit_pos == 0xF))/* PCI2 Chiplet */
        {
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_OR, l_data64));
            FAPI_DBG("Scan fure rings for PCI2 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_fure));
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_CLEAR, l_data64));
        }

        if ((l_attr_chip_unit_pos == 0x9))/* OBUS0 Chiplet */
        {
            FAPI_DBG("Scan fure rings for OBUS0 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_fure));
        }

        if ((l_attr_chip_unit_pos == 0xA))/* OBUS1 Chiplet */
        {
            FAPI_DBG("Scan fure rings for OBUS1 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob1_fure));
        }

        if ((l_attr_chip_unit_pos == 0xB))/* OBUS2 Chiplet */
        {
            FAPI_DBG("Scan fure rings for OBUS2 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob2_fure));
        }

        if ((l_attr_chip_unit_pos == 0xC))/* OBUS3 Chiplet */
        {
            FAPI_DBG("Scan fure rings for OBUS3 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_fure));
        }

        if ((l_attr_chip_unit_pos == 0x6))/* XBUS Chiplet */
        {
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_OR, l_data64));
            FAPI_DBG("Scan fure rings for XBUS chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_fure));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_fure));
            FAPI_TRY(fapi2::putScom(l_chplt_trgt, PERV_CPLT_CTRL0_CLEAR, l_data64));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_fure));
        }
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
