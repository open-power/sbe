/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_fastarray_abist_catchup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
//------------------------------------------------------------------------------
/// @file  p9_sbe_fastarray_abist_catchup.C
///
/// @brief Abist catchup sub-procedure for p9_fastarray procedure
//         Used to catch up Abist cycles for the skipped array rows
//------------------------------------------------------------------------------
// *HWP HW Owner        : Joachim Fenkes <fenkes@de.ibm.com>
// *HWP HW Backup Owner : Nick Klazynski <jklazyns@us.ibm.com>
// *HWP FW Owner        : Nagendra Gurram <nagendra.g@in.ibm.com>
// *HWP Team            : Chip
// *HWP Level           : 3
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p9_sbe_fastarray_abist_catchup.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>


fapi2::ReturnCode p9_sbe_fastarray_abist_catchup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint32_t i_clockCyclesMinusOne)
{
    fapi2::buffer<uint64_t> l_cc_buf = i_clockCyclesMinusOne;

    l_cc_buf.setBit<PERV_1_OPCG_REG0_GO>();
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_REG0, l_cc_buf), "Failed to clock %d array cycle(s)",
             i_clockCyclesMinusOne + 1);

    /* If we clocked more than a single cycle, do due diligence and wait for OPCG_DONE */
    if( i_clockCyclesMinusOne )
    {
        uint32_t l_timeout = 100;

        while (--l_timeout)
        {
            FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_CPLT_STAT0, l_cc_buf), "Failed to read Chiplet Status 0 Register");

            if (l_cc_buf.getBit<PERV_1_CPLT_STAT0_CC_CTRL_OPCG_DONE_DC>())
            {
                break;
            }

            fapi2::delay(1000000, 100000);
        }

        FAPI_ASSERT(l_timeout, fapi2::FASTARRAY_CLOCK_TIMEOUT().set_TARGET(i_target_chiplet),
                    "Clocking ABIST cycles timed out");
    }

    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;

}
