/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/lib/p9_common_pro_epi_log.C $         */
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
/// @file  p9_common_pro_epi_log.C
/// @brief common procedure prologue/epilogue routines
///
/// *HWP HWP Owner          : David Du      <daviddu@us.ibm.com>
/// *HWP Backup HWP Owner   : Greg Still    <stillgs@us.ibm.com>
/// *HWP FW Owner           : Reshmi Nair   <resnair5@in.ibm.com>
/// *HWP Team               : PM
/// *HWP Consumed by        : SBE:SGPE
/// *HWP Level              : 1
///
/// Procedure Summary:
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>
#include "p9_common_pro_epi_log.H"

//------------------------------------------------------------------------------
// Constant Definitions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure:
//------------------------------------------------------------------------------

extern "C"
{

    fapi2::ReturnCode
    p9_common_pro_epi_log(
        const fapi2::Target < fapi2::TARGET_TYPE_EQ |
        fapi2::TARGET_TYPE_CORE > & i_target,
        int i_operation)
    {
        return fapi2::FAPI2_RC_SUCCESS;

    } // Procedure


} // extern C


