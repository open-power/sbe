/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_chiplet_pll_initf.C $     */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015                                                         */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------
/// @file  p9_sbe_chiplet_pll_initf.C
///
/// @brief procedure for scan initializing PLL config bits for XBus, OBus, PCIe, MC Chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : srinivas naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_pll_initf.H"

#include "p9_perv_sbe_cmn.H"


enum P9_SBE_CHIPLET_PLL_INITF_Private_Constants
{
    REGIONS_PLL_ONLY = 0x001,
    SCAN_TYPES_GPTR = 0x200,
    SCAN_TYPES_BNDY_FUNC = 0x808
};

fapi2::ReturnCode p9_sbe_chiplet_pll_initf(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    FAPI_INF("Switch MC meshs to Nest mesh");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MC_SYNC_MODE, i_target_chip, l_read_attr));

    if ( l_read_attr )
    {
        for (auto l_chplt_trgt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
                  (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
                   || l_attr_chip_unit_pos == 0x0B
                   || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
                  (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
                   || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
                  (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
            {
                continue;
            }

            FAPI_INF("Call Scan0 Module (scan region=PLL, scan types=GPTR)");
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_chplt_trgt, REGIONS_PLL_ONLY,
                                                  SCAN_TYPES_GPTR));

            FAPI_INF("Call Scan0 Module (scan region=PLL, scan types=GPTR)");
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_chplt_trgt, REGIONS_PLL_ONLY,
                                                  SCAN_TYPES_BNDY_FUNC));

            //TODO:Load Ring Module : Scan initialize PLL BNDY chain
        }
    }
    else
    {
        for (auto l_chplt_trgt : l_perv_functional_vector)
        {
            uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_chplt_trgt,
                                   l_attr_chip_unit_pos));

            if (!((l_attr_chip_unit_pos == 0x07
                   || l_attr_chip_unit_pos == 0x08/* McChiplet */) ||
                  (l_attr_chip_unit_pos == 0x02 || l_attr_chip_unit_pos == 0x03
                   || l_attr_chip_unit_pos == 0x04
                   || l_attr_chip_unit_pos == 0x05/* NestChiplet */) ||
                  (l_attr_chip_unit_pos == 0x09 || l_attr_chip_unit_pos == 0x0A
                   || l_attr_chip_unit_pos == 0x0B
                   || l_attr_chip_unit_pos == 0x0C/* ObusChiplet */) ||
                  (l_attr_chip_unit_pos == 0x0D || l_attr_chip_unit_pos == 0x0E
                   || l_attr_chip_unit_pos == 0x0F/* PcieChiplet */) ||
                  (l_attr_chip_unit_pos == 0x06/* XbusChiplet */)))
            {
                continue;
            }

            FAPI_INF("Call Scan0 Module (scan region=PLL, scan types=GPTR)");
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_chplt_trgt, REGIONS_PLL_ONLY,
                                                  SCAN_TYPES_GPTR));

            FAPI_INF("Call Scan0 Module (scan region=PLL, scan types=GPTR)");
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_chplt_trgt, REGIONS_PLL_ONLY,
                                                  SCAN_TYPES_BNDY_FUNC));

            //TODO:Load Ring Module : Scan initialize PLL BNDY chain
        }
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
