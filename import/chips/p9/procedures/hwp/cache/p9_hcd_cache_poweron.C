/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_poweron.C $        */
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
///
/// @file  p9_hcd_cache_poweron.C
/// @brief Cache Chiplet Power-on
///
// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2
//
// Procedure Summary:
//   Command the cache PFET controller to power-on
//   Check for valid power on completion
//   Polled Timeout:  100us
//

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>
#include "p9_hcd_cache_poweron.H"
#include "p9_common_poweronoff.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Chiplet Power-on
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_poweron(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_TRY(p9_common_poweronoff(i_target, p9power::POWER_ON));

fapi_try_exit:
    return fapi2::current_err;

} // Procedure
