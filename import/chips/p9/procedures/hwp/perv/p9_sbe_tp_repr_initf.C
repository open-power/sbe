/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/perv/p9_sbe_tp_repr_initf.C $         */
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
/// @file  p9_sbe_tp_repr_initf.C
///
/// @brief Scan initialize REPR for PERV chiplet
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#include "p9_sbe_tp_repr_initf.H"

fapi2::ReturnCode p9_sbe_tp_repr_initf(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("Entering ...");

    FAPI_DBG("Scan perv repr ring.");
    FAPI_TRY(fapi2::putRing(i_target_chip, perv_repr));

    FAPI_DBG("Scan occ repr ring.");
    FAPI_TRY(fapi2::putRing(i_target_chip, occ_repr));

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
