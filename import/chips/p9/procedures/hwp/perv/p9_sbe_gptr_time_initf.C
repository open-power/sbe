/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_gptr_time_initf.C $       */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2016                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
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

fapi2::ReturnCode p9_sbe_gptr_time_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below

    FAPI_INF("Entering ...");

    for (auto l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_MCBIST>
         (fapi2::TARGET_STATE_FUNCTIONAL))
    {

        FAPI_DBG("Scan gptr rings for MC chiplets ");
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_gptr));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_gptr));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_gptr));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_pll_gptr));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_time));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom01_time));
        FAPI_TRY(fapi2::putRing(l_chplt_trgt, mc_iom23_time));
    }

    for( auto l_chplt_trgt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         ( fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt, l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0x09))/* OBUS0 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for obus0 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_pll_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, ob0_time));

        }

        //Rings need to be scanned for OBUS 2 and 3 incase of Cumulus
        if ((l_attr_chip_unit_pos == 0xC))/* OBUS3 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for obus3 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_pll_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, ob3_time));
        }

        if ((l_attr_chip_unit_pos == 0x06))/* XBUS Chiplet */
        {
            FAPI_DBG("Scan gptr rings for xbus chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io0_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_pll_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io0_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io1_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, xb_io2_time));
        }

        if ((l_attr_chip_unit_pos == 0xD))/* PCI0 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for pci0 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, pci0_time));
        }

        if ((l_attr_chip_unit_pos == 0xE))/* PCI1 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for pci1 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, pci1_time));
        }

        if ((l_attr_chip_unit_pos == 0xF))/* PCI2 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for pci2 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, pci2_time));
        }

        if ((l_attr_chip_unit_pos == 0x02))/* N0 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for n0 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_nx_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n0_cxa0_time));
        }

        if ((l_attr_chip_unit_pos == 0x03))/* N1 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for n1 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo0_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_ioo1_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n1_mcs23_time));
        }

        if ((l_attr_chip_unit_pos == 0x04))/* N2 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for n2 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n2_cxa1_time));
        }

        if ((l_attr_chip_unit_pos == 0x05))/* N3 Chiplet */
        {
            FAPI_DBG("Scan gptr rings for n3 chiplets ");
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_gptr ));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_gptr ));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_gptr));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_mcs01_time));
            FAPI_TRY(fapi2::putRing(i_target_chip, n3_np_time));
        }

    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

