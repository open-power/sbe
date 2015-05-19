/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: hwp/nest/p9_sbe_scominit.C $                                  */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* PPE Project                                                            */
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
//------------------------------------------------------------------------------
/// @file  p9_sbe_scominit.C
///
/// @brief This procedure contains SCOM based initialization required for
///  fabric configuration & HBI operation
/// *!
/// *!   o Set fabric node/chip ID configuration for all configured
/// *!     chiplets to chip specific values
/// *!   o Establish ADU XSCOM BAR for HBI operation
//------------------------------------------------------------------------------
// *HWP HW Owner         : Girisankar Paulraj <gpaulraj@in.ibm.com>
// *HWP HW Backup Owner  : Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner         : Thi N. Tran <thi@us.ibm.com>
// *HWP Team             : Nest
// *HWP Level            : 1
// *HWP Consumed by      : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_scominit.H"



fapi2::ReturnCode p9_sbe_scominit(const
                  fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> & i_target)
{
    FAPI_DBG("Entering ...");

    FAPI_DBG("Exiting ...");

    return fapi2::FAPI2_RC_SUCCESS;

}
