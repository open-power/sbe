/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_pba_access.C $   */
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
/// @file p10_pba_access.C
/// @brief Read coherent state of memory via the PBA (FAPI)
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

extern "C" {

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p10_pba_access(const
                                     fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
                                     const uint64_t i_address,
                                     const bool i_rnw,
                                     const uint32_t i_flags,
                                     const bool i_firstGranule,
                                     const bool i_lastGranule,
                                     uint8_t io_data[])
    {
        //return codes
        fapi2::ReturnCode l_statusRc = fapi2::FAPI2_RC_SUCCESS;

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        // Process input flag
        p10_PBA_oper_flag l_myPbaFlag;
        l_myPbaFlag.getFlag(i_flags);

        //if read
        if (i_rnw)
        {
            FAPI_TRY(p10_pba_coherent_pba_read(i_target, i_address,
                                               l_myPbaFlag.getOperationType(), io_data),
                     "p10_pba_coherent_pba_read() returns error l_rc 0x%.8X",
                     (uint64_t)fapi2::current_err);
        }
        //else if write
        else
        {
            FAPI_TRY(p10_pba_coherent_pba_write(i_target, i_address, io_data,
                                                l_myPbaFlag.getOperationType()),
                     "p10_pba_coherent_pba_write() returns error l_rc 0x%.8X",
                     (uint64_t)fapi2::current_err);

        }

        //If we are not in fastmode or this is the last granule, we want to check the status
        if ( i_lastGranule || (l_myPbaFlag.getFastMode() == false) )
        {
            l_statusRc = p10_pba_coherent_check_status(i_target);

            // Clean up PBA regardless of status check returned value.
            if (i_lastGranule)
            {
                //Clean up the PBA since it's the last read/write and it has been finished
                FAPI_TRY(p10_pba_coherent_cleanup_pba(i_target),
                         "Error doing p10_pba_coherent_cleanup_pba");
            }

        }

        // mark HWP exit
    fapi_try_exit:

        // If error from status check, return it for FFDC
        if (l_statusRc)
        {
            fapi2::current_err = l_statusRc;
        }

        //Handling error. PBA access is the main error if there's one.
        //Append the input data to an error if we got an error back for non-PPE env
#ifndef __PPE__

        if (fapi2::current_err)
        {
            p10_pba_coherent_append_input_data(i_address, i_rnw, i_flags, fapi2::current_err);
        }

#endif

        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

} // extern "C"
