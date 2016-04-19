/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_chiplet_l3_dcc_setup.H $ */
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
/// @file  p9_hcd_cache_chiplet_l3_dcc_setup.H
///
/// @brief Setup L3 DCC, Drop L3 DCC bypass
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 1
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


#ifndef _P9_HCD_CACHE_CHIPLET_L3_DCC_SETUP_H_
#define _P9_HCD_CACHE_CHIPLET_L3_DCC_SETUP_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_hcd_cache_chiplet_l3_dcc_setup_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>&);

/// @brief * Setup L3 DCC (scan with setpulse, scan region = ANEP), attribute dependency Nimbus/Cumulus
/// * Drop L3 DCC bypass
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_EQ target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_hcd_cache_chiplet_l3_dcc_setup(const
            fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target_chiplet);
}

#endif
