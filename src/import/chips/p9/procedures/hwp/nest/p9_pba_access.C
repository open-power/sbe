/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_pba_access.C $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file p9_pba_access.C
/// @brief Read coherent state of memory via the PBA (FAPI)
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_pba_setup.H>
#include <p9_pba_coherent_utils.H>

extern "C" {

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_pba_access(const
                                    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
                                    const uint64_t i_address,
                                    const bool i_rnw,
                                    const uint32_t i_flags,
                                    const bool i_firstGranule,
                                    const bool i_lastGranule,
                                    uint8_t io_data[])
    {
        //return codes
        fapi2::ReturnCode rc;
        fapi2::ReturnCode rc1;

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        // Process input flag
        p9_PBA_oper_flag l_myPbaFlag;
        l_myPbaFlag.getFlag(i_flags);

        //if read
        if (i_rnw)
        {
            rc1 = p9_pba_coherent_pba_read(i_target, i_address, io_data);
        }
        //else if write
        else
        {
            rc1 = p9_pba_coherent_pba_write(i_target, i_address, io_data);
        }

        //If we are not in fastmode or this is the last granule, we want to check the status
        if (!rc1)
        {
            if ( i_lastGranule || (l_myPbaFlag.getFastMode() == false) )
            {
                rc1 = p9_pba_coherent_status_check(i_target);

                if (i_lastGranule)
                {
                    //Clean up the PBA since it's the last read/write and it has been finished
                    FAPI_TRY(p9_pba_coherent_cleanup_pba(i_target),
                             "Error doing p9_pba_coherent_cleanup_pba");
                }
            }
        }

        // mark HWP exit
    fapi_try_exit:

        //Handling error. PBA access is the main error if there's one.
        if (rc1)
        {
            //Commit error from clean up (secondary)
            if (rc)
            {
                //fapi2::fapiLogError(rc, FAPI_ERRL_SEV_RECOVERED);
            }

            //Set return error to pba access error
            fapi2::current_err = rc1;
        }

        FAPI_DBG("Exit ...\n");
        return fapi2::current_err;
    }

} // extern "C"

