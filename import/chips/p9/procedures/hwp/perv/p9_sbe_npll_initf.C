/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_npll_initf.C $            */
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
/// @file  p9_sbe_npll_initf.C
///
/// @brief apply initfile for level 0 & 1 PLLs
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_npll_initf.H"

#include "p9_perv_sbe_cmn.H"


enum P9_SBE_NPLL_INITF_Private_Constants
{
    REGIONS_PLL_ONLY = 0x001,
    SCAN_TYPES_BNDY_FUNC = 0x808,
    SCAN_TYPES_GPTR = 0x200
};

fapi2::ReturnCode p9_sbe_npll_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    // Get the TPChiplet target
    for (auto it : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, it, l_attr_chip_unit_pos));

        if ((l_attr_chip_unit_pos == 0x01))/* TPChiplet */
        {
            FAPI_INF("Call Scan0 Module (scan region=PLL, scan types=GPTR)");
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(it, REGIONS_PLL_ONLY,
                                                  SCAN_TYPES_GPTR));
            FAPI_INF("Call Scan0 Module (scan region=PLL, scan types=GPTR)");
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(it, REGIONS_PLL_ONLY,
                                                  SCAN_TYPES_BNDY_FUNC));
            break;
        }
    }

    //TODO:Load Ring Module : Scan initialize PLL BNDY chain

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
