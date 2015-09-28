/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_tp_chiplet_init2.C $      */
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
/// @file  p9_sbe_tp_chiplet_init2.C
///
/// @brief Run scan 0 module for pervasive
//------------------------------------------------------------------------------
// *HWP HWP Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HWP Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner         : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team             : Perv
// *HWP Level            : 2
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_chiplet_init2.H"

#include "p9_perv_sbe_cmn.H"


enum P9_SBE_TP_CHIPLET_INIT2_Private_Constants
{
    REGIONS_EXCEPT_PIB_NET_PLL = 0x4FE,
    SCAN_TYPES = 0xDCF
};

fapi2::ReturnCode p9_sbe_tp_chiplet_init2(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    auto l_perv_functional_vector =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_STATE_FUNCTIONAL);
    FAPI_DBG("Entering ...");

    FAPI_INF("Call Scan0 Module");

    // Get the TPChiplet target
    for (auto it : l_perv_functional_vector)
    {
        uint8_t l_attr_chip_unit_pos = 0; //actual value is read in FAPI_ATTR_GET below
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, it, l_attr_chip_unit_pos));

        if (l_attr_chip_unit_pos == 0x01)/* TPChiplet */
        {
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(it, REGIONS_EXCEPT_PIB_NET_PLL,
                                                  SCAN_TYPES));
            break;
        }
    }

    FAPI_DBG("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
