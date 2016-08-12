/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/core/p9_hcd_core_initf.C $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
///
/// @file  p9_hcd_core_initf.C
/// @brief Core scan init
///
/// Procedure Summary:
///   Initfiles in procedure defined on VBU ENGD wiki (TODO add link)
///   Check for the presence of core FUNC override rings from image;
///   if found, apply;  if not, apply core base FUNC rings from image
///   Note:  FASTINIT ring (eg CMSK ring) is setup at this point to limit the
///   stumps that participate in FUNC ring scanning (this is new for P9).
///   (TODO to make sure the image build support is in place)
///   Note : if in fused mode, both core rings will be initialized to the same
///   values via multicast scans

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "p9_hcd_core_initf.H"
#ifdef P9_HCD_STOP_SKIP_SCAN
    #ifndef __PPE__
        #include <p9_core_scan.H>
    #endif
#endif

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Core scan init
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_initf(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_initf");

#ifndef P9_HCD_STOP_SKIP_SCAN

    FAPI_DBG("Scanning Core FUNC Rings");
    FAPI_TRY(putRing(i_target, EC_FUNC,
                     fapi2::RING_MODE_HEADER_CHECK));

fapi_try_exit:
#else
#ifndef __PPE__
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ReturnCode l_rc;

    FAPI_EXEC_HWP(l_rc, p9_core_scan, i_target, FAPI_SYSTEM);

    if (l_rc)
    {
        FAPI_ERR("Error from p9_core_scan (p9.core.scan.initfile)");
        fapi2::current_err = l_rc;
        goto fapi_try_exit;
    }

fapi_try_exit:
#endif
#endif

    FAPI_INF("<<p9_hcd_core_initf");
    return fapi2::current_err;
}
