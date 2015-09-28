/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_scomcust.C $       */
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
/// @file  p9_hcd_cache_scomcust.C
/// @brief Core Chiplet PCB Arbitration
///
/// *HWP HWP Owner   : David Du       <daviddu@us.ibm.com>
/// *HWP FW Owner    : Sangeetha T S  <sangeet2@in.ibm.com>
/// *HWP Team        : PM
/// *HWP Consumed by : SBE:SGPE
/// *HWP Level       : 1
///
/// Procedure Summary:
///   If CME, request PCB Mux.
///     Poll for PCB Mux grant
///   Else (SBE)
///     Nop (as the CME is not running in bringing up the first Core)
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>
//#include <common_scom_addresses.H>
//will be replaced with real scom address header file
#include "p9_hcd_cache_scomcust.H"

//------------------------------------------------------------------------------
// Constant Definitions: Core Chiplet PCB Arbitration
//------------------------------------------------------------------------------

extern "C"
{

    fapi2::ReturnCode
    p9_hcd_cache_scomcust(
        const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
    {

#if 0

        fapi2::buffer<uint64_t> data;

        //Dynamically built (and installed) routine that is inserted by the .XIP
        //Customization. process. (New for P9)
        //(TODO:  this part of the process is a placeholder at this point)
        //Dynamically built pointer where a NULL is checked before execution
        //If NULL (a potential early value); return
        //Else call the function at the pointer;
        //pointer is filled in by XIP Customization
        //Customization items:
        //Epsilon settings scan flush to super safe
        //Customize Epsilon settings for system config
        //LCO setup (chiplet specific)
        //FW setups up based victim caches

        return fapi2::FAPI2_RC_SUCCESS;

        FAPI_CLEANUP();
        return fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

#endif

        return fapi2::FAPI2_RC_SUCCESS;

    } // Procedure


} // extern C


