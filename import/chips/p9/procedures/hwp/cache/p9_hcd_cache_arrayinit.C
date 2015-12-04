/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_arrayinit.C $      */
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
/// @file  p9_hcd_cache_arrayinit.C
/// @brief EX Initialize arrays
///
/// Procedure Summary:
///   Use ABIST engine to zero out all arrays
///   Upon completion, scan0 flush all rings
///     except Vital, Repair, GPTR, TIME and DPLL
///

// *HWP HWP Owner          : David Du      <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still    <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_arrayinit.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
enum P9_HCD_CACHE_ARRAYINIT_Private_Constants
{
    LOOP_COUNTER = 0x0000000000042FFF,
    REGIONS_EXCEPT_VITAL = 0x7FF,
    REGIONS_FOR_PERV = 0x400,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    SELECT_EDRAM = 0x0,
    SELECT_SRAM = 0x1,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//-----------------------------------------------------------------------------
// Procedure: Initialize Cache Arrays
//-----------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_arrayinit(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{

    FAPI_INF(">>p9_hcd_cache_arrayinit");

#ifndef P9_HCD_STOP_SKIP_ARRAYINIT

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();

    FAPI_DBG("Arrayinit all regions except Vital");
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
    FAPI_DBG("Scan0 all except Vital/ANEP/DPLL, GPTR/REPR/TIME scan chains");

    for(uint32_t l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
        FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                              p9hcd::SCAN0_REGION_ALL_BUT_ANEP_PLL,
                                              p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

#endif

#if not defined(P9_HCD_STOP_SKIP_FLUSH) || not defined(P9_HCD_STOP_SKIP_ARRAYINIT)
fapi_try_exit:
#endif

    FAPI_INF("<<p9_hcd_cache_arrayinit");

    return fapi2::current_err;

}





