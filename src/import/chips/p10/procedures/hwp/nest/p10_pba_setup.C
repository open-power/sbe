/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_pba_setup.C $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
//--------------------------------------------------------------------------
//
//
/// @file p10_pba_setup.C
/// @brief Setup the registers for a read/write to the PBA
///
// *HWP HW Maintainer : Chris Riedl (cmr@ibm.com)
// *HWP FW Maintainer : Raja Das    (rajadas2@in.ibm.com)
// *HWP Consumed by   : SBE
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p10_pba_setup.H>
#include <p10_pba_coherent_utils.H>

extern "C"
{

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p10_pba_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>
                                    & i_target,
                                    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
                                    const uint64_t i_address,
                                    const bool i_rnw,
                                    const uint32_t i_flags,
                                    uint32_t& o_numGranules)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        //check arguments
        FAPI_TRY(p10_pba_coherent_utils_check_args(i_target, i_address, i_flags),
                 "Error from p10_pba_coherent_utils_check_args");

        //ensure fabric is running
        FAPI_TRY(p10_pba_coherent_utils_check_fbc_state(i_target),
                 "Error from p10_pba_coherent_utils_check_fbc_state");

        //reset the PBA - cleanup just calls reset
        FAPI_TRY(p10_pba_coherent_cleanup_pba(i_target), "Error from p10_pba_cleanup_pba");

        //The PBA Bar and PBA Bar Mask need to be setup before getting the number of granules because how they get setup affects the number of granules that can be read/written
        //setup the PBA Bar
        FAPI_TRY(p10_pba_coherent_setup_pba_bar(i_target, i_address),
                 "Error from p10_pba_coherent_setup_pba_bar");

        //setup the PBA for reading/writing
        FAPI_TRY(p10_pba_coherent_setup_pba(i_target, i_core_target, i_address, i_rnw, i_flags),
                 "Error from p10_pba_coherent_setup_pba");

        //figure out the number of 128B granules that can be read/written
        FAPI_TRY(p10_pba_coherent_utils_get_num_granules(i_target, i_address,
                 o_numGranules),
                 "Error from p10_pba_coherent_utils_get_num_granules");

    fapi_try_exit:
        //Append the input data to an error if we got an error back
        fapi2::ReturnCode o_rc = fapi2::current_err;
#ifndef __PPE__

        if (fapi2::current_err)
        {
            p10_pba_coherent_append_input_data(i_address, i_rnw, i_flags,
                                               fapi2::current_err);
        }

#endif

        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }
} // extern "C"
