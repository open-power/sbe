/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/core/p9_hcd_core_arrayinit.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_hcd_core_arrayinit.C
/// @brief Core Initialize arrays
///
/// Procedure Summary:
///   Use ABIST engine to zero out all arrays
///   Upon completion, scan0 flush all rings except Vital,Repair,GPTR,and TIME

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_core_arrayinit.H"
#include <p9_ring_id.h>

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------
// simGETFAC B0.C0.S0.P0.E9.TP.TCEC14.CORE.EPS.CC.COMP.OPCG.LPCNTQ.LATC.L2 43 -ox
//0x000000012B8
// simGETFAC B0.C0.S0.P0.E9.TP.TCEC14.CORE.EPS.CTRL.COMP.CPLT_STATQ.LATC.L2 24 -ox
//0x804000

enum P9_HCD_CORE_ARRAYINIT_Private_Constants
{
    REGIONS_EXCEPT_VITAL = 0x7FF,
    LOOP_COUNTER =       0x00000000000012B8, // Parallel mode must be set!
    SELECT_SRAM = 0x1,
    SELECT_EDRAM = 0x0,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//------------------------------------------------------------------------------
// Procedure: Core Initialize arrays
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_arrayinit(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_arrayinit");
    fapi2::buffer<uint64_t>                     l_data64;

    // To get proc DD level
    uint8_t l_parallel_abist_nimbus_dd1_workaround = 0;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

#if not defined(P9_HCD_STOP_SKIP_FLUSH) || not defined(P9_HCD_STOP_SKIP_ARRAYINIT)
    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();
#endif

    FAPI_DBG("Assert sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY(putScom(i_target, C_CPLT_CONF0_OR, MASK_SET(34)));

#ifndef P9_HCD_STOP_SKIP_ARRAYINIT

    // Only set ABIST engine to parallel mode for DD1 level
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_NDD1_ABIST_PARALLEL, l_chip,
                           l_parallel_abist_nimbus_dd1_workaround));

    if (l_parallel_abist_nimbus_dd1_workaround != 0)
    {
        FAPI_DBG("Scan ec_abst ring");
        FAPI_TRY(fapi2::putRing(i_target, ec_abst),
                 "Error from putRing (ec_abst)");
    }

    FAPI_DBG("Arrayinit all regions except vital");
    FAPI_TRY(p9_perv_sbe_cmn_array_init_module(l_perv,
             REGIONS_EXCEPT_VITAL,
             LOOP_COUNTER,
             SELECT_SRAM,
             SELECT_EDRAM,
             START_ABIST_MATCH_VALUE));

#endif

#ifndef P9_HCD_STOP_SKIP_FLUSH

    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    FAPI_DBG("Scan0 region:all_but_pll type:all_but_gptr_repr_time rings");

    for(uint32_t l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                              p9hcd::SCAN0_REGION_ALL_BUT_PLL,
                                              p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

#endif

    FAPI_DBG("Drop sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY(putScom(i_target, C_CPLT_CONF0_CLEAR, MASK_SET(34)));

//#if not defined(P9_HCD_STOP_SKIP_FLUSH) || not defined(P9_HCD_STOP_SKIP_ARRAYINIT)
fapi_try_exit:
//#endif

    FAPI_INF("<<p9_hcd_core_arrayinit");
    return fapi2::current_err;
}

