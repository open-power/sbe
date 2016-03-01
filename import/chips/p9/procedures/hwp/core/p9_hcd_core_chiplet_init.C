/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_chiplet_init.C $     */
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
/// @file  p9_hcd_core_chiplet_init.C
/// @brief Core Flush/Initialize
///
/// Procedure Summary:
///   Switch the core glitchless mux to allow DPLL clocks on the clock grid
///   Clocking:
///    - setup controls based on DPLL frequency
///    - assert PM sync_enable (4x core, 2 x L2),
///      DCCs and SkewAdjust starts aligning clocks
///   Scan0 flush all chiplet rings except VITAL, GPTR and TIME

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_core_chiplet_init.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Core Flush/Initialize
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_core_chiplet_init(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    FAPI_INF(">>p9_hcd_core_chiplet_init");
    /*
    #ifndef P9_HCD_STOP_SKIP_FLUSH
        //--------------------------------------------
        // perform scan0 module for pervasive chiplet
        //--------------------------------------------
        // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
        // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
        // all stumps less than 8191, the loop can be removed.

        fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
            i_target.getParent<fapi2::TARGET_TYPE_PERV>();

        FAPI_DBG("Scan0 region:all_but_vital type:all_but_gptr_repr_time rings");

        for(uint32_t l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  p9hcd::SCAN0_REGION_PERV_CORE,
                                                  p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

    fapi_try_exit:

    #endif
    */
    FAPI_INF("<<p9_hcd_core_chiplet_init");
    return fapi2::current_err;
}



