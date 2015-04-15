/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/core/p9_hcd_core_startclocks.C $      */
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
/// @file  p9_hcd_core_startclocks.C
/// @brief Core Clock Start
///
/// *HWP HWP Owner   : David Du      <daviddu@us.ibm.com>
/// *HWP FW Owner    : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:CME
/// *HWP Level       : 1
///
/// Procedure Summary:
///   Drop pervasive thold
///   Drop pervasive fence
///   Reset abst clock muxsel, sync muxsel
///   Clear clock controller scan register before start
///   Start arrays + nsl regions
///   Start sl + refresh clock regions
///   Check for clocks started
///     If not, error
///   Check for core xstop (TODO: need for this (?) and then FIR structure in
///   for P9.  Note: CME can NOT read Cache FIR)
///     If so, error
///   Clear force align
///   Clear flush mode
///   Check security switch and set trusted boot en bit
///   (TODO: still needed for P9?)
///   Drop the core to cache logical fence
///

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_core_startclocks.H"

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Procedure: Core Clock Start
//-----------------------------------------------------------------------------

extern "C"
{

    fapi2::ReturnCode
    p9_hcd_core_startclocks(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
    {

#if 0

        fapi2::buffer<uint64_t> data;

        return fapi2::FAPI2_RC_SUCCESS;

        FAPI_CLEANUP();
        return fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

#endif

        return fapi2::FAPI2_RC_SUCCESS;

    } // Procedure


} // extern C



