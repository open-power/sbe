/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_scominit.C $       */
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
///
/// @file  p9_hcd_cache_scominit.C
/// @brief  Cache Customization SCOMs
///
/// Procedure Summary:
///   Apply any SCOM initialization to the cache
///   Stop L3 configuration mode
///   Configure Trace Stop on Xstop
///   DTS Initialization sequense
///

// *HWP HWP Owner          : David Du      <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still    <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include <p9_l2_scom.H>
#include <p9_l3_scom.H>
#include <p9_ncu_scom.H>
#include "p9_hcd_cache_scominit.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Customization SCOMs
//------------------------------------------------------------------------------


fapi2::ReturnCode
p9_hcd_cache_scominit(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_scominit");
    fapi2::buffer<uint64_t> l_data64;

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    auto l_ex_targets = i_target.getChildren<fapi2::TARGET_TYPE_EX>();
    fapi2::ReturnCode l_rc;

    for (auto l_iter = l_ex_targets.begin(); l_iter != l_ex_targets.end(); l_iter++)
    {
        FAPI_EXEC_HWP(l_rc, p9_l2_scom, *l_iter, FAPI_SYSTEM);

        if (l_rc)
        {
            FAPI_ERR("Error from p9_l2_scom (p9.l2.scom.initfile)");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        FAPI_EXEC_HWP(l_rc, p9_l3_scom, *l_iter, FAPI_SYSTEM);

        if (l_rc)
        {
            FAPI_ERR("Error from p9_l3_scom (p9.l3.scom.initfile)");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        FAPI_EXEC_HWP(l_rc, p9_ncu_scom, *l_iter, FAPI_SYSTEM);

        if (l_rc)
        {
            FAPI_ERR("Error from p9_ncu_scom (p9.ncu.scom.initfile)");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }
    }

    /// @todo set the sample pulse count (bit 6:9)
    /// enable the appropriate loops
    /// (needs investigation with the Perv team on the EC wiring).
    FAPI_DBG("Enable DTS sampling via THERM_MODE_REG[5]");
    FAPI_TRY(getScom(i_target, EQ_THERM_MODE_REG, l_data64));
    FAPI_TRY(putScom(i_target, EQ_THERM_MODE_REG, DATA_SET(5)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_scominit");
    return fapi2::current_err;
}




