/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_npll_initf.C $            */
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

fapi2::ReturnCode p9_sbe_npll_initf(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    uint8_t l_read_attr = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    RingID ringID = perv_pll_bndy_bucket_1;
    FAPI_INF("Entering ...");

    FAPI_DBG("Get the attribute ATTR_NEST_PLL_BUCKET");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_NEST_PLL_BUCKET, FAPI_SYSTEM , l_read_attr));

    switch(l_read_attr)
    {

        case 1:
            ringID = perv_pll_bndy_bucket_1;
            break;

        case 2:
            ringID = perv_pll_bndy_bucket_2;
            break;

        case 3:
            ringID = perv_pll_bndy_bucket_3;
            break;

        case 4:
            ringID = perv_pll_bndy_bucket_4;
            break;

        case 5:
            ringID = perv_pll_bndy_bucket_5;
            break;

        default:
            FAPI_TRY(!(fapi2::FAPI2_RC_SUCCESS), "Invalid values of ATTR_NEST_PLL_BUCKET")
    }

    FAPI_TRY(fapi2::putRing(i_target_chip, ringID, fapi2::RING_MODE_SET_PULSE_NSL));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
