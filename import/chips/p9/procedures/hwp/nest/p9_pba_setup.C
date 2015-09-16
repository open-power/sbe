/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/ipl/hwp/tests/p9_pba_setup.C $            */
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
//--------------------------------------------------------------------------
//
//
/// @file p9_pba_setup.C
/// @brief Setup the registers for a read/write to the PBA
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_pba_setup.H>
#include "p9_pba_coherent_utils.H"

extern "C"
{

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_pba_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>
                                   & i_target,
                                   const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ex_target,
                                   const uint64_t i_address,
                                   const bool i_rnw,
                                   const uint32_t i_flags,
                                   uint32_t& o_numGranules)
    {
        // mark HWP entry
        FAPI_INF("Entering ...\n");

        //check arguments
        FAPI_TRY(p9_pba_coherent_utils_check_args(i_target, i_address),
                 "Error from p9_pba_coherent_utils_check_args");

        //ensure fabric is running
        FAPI_TRY(p9_pba_coherent_utils_check_fbc_state(i_target),
                 "Error from p9_pba_coherent_utils_check_fbc_state");

        //reset the ADU - cleanup just calls reset
        //TODO Joe had made a comment on this about resetting the PBA as part of the setup process - I need to test
        //this more before I'm willing to actually put this in - I got some errors when I had it in earlier.
        //FAPI_TRY(p9_pba_coherent_cleanup_pba(i_target), "p9_pba_setup: Error from p9_pba_cleanup_pba");

        //The PBA Bar and PBA Bar Mask need to be setup before getting the number of granules because how they get setup affects the number of granules that can be read/written
        //setup the PBA Bar
        FAPI_TRY(p9_pba_coherent_setup_pba_bar(i_target, i_address),
                 "Error from p9_pba_coherent_setup_pba_bar");

        //setup the PBA for reading/writing
        FAPI_TRY(p9_pba_coherent_setup_pba(i_target, i_ex_target, i_address, i_rnw, i_flags),
                 "Error from p9_pba_coherent_setup_pba");

        //figure out the number of 128B granules that can be read/written
        FAPI_TRY(p9_pba_coherent_utils_get_num_granules(i_target, i_address,
                 o_numGranules),
                 "Error from p9_pba_coherent_utils_get_num_granules");

    fapi_try_exit:
        FAPI_INF("Exiting...");
        return fapi2::current_err;
    }
} // extern "C"

