/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_fastarray_cleanup.C $ */
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
/// @file  p9_sbe_fastarray_cleanup.C
///
/// @brief Cleanup sub-procedure for p9_fastarray procedure
//------------------------------------------------------------------------------
// *HWP HW Owner        : Joachim Fenkes <fenkes@de.ibm.com>
// *HWP HW Backup Owner : Nick Klazynski <jklazyns@us.ibm.com>
// *HWP FW Owner        : Nagendra Gurram <nagendra.g@in.ibm.com>
// *HWP Team            : Chip
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include "p9_sbe_fastarray_cleanup.H"
#include "p9_sbe_fastarray_abist_catchup.H"
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>

/**
 * @brief Cleanup sub-procedure for p9_fastarray procedure
 * @param[in] i_target_chiplet The chiplet to clean up
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
fapi2::ReturnCode p9_sbe_fastarray_cleanup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    fapi2::buffer<uint64_t> l_buf;

    /* Let ABIST engines run to completion */
    do
    {
        FAPI_TRY(p9_sbe_fastarray_abist_catchup(i_target_chiplet, 0xFFF), "Failed to clock ABIST to completion");
        FAPI_TRY(fapi2::getScom(i_target_chiplet, PERV_CPLT_STAT0, l_buf), "Failed to read Chiplet Status 0 Register");
    }
    while (!l_buf.getBit<PERV_1_CPLT_STAT0_SRAM_ABIST_DONE_DC>());

    /* Disable ABIST and clock engines so they can cleanly reset */
    l_buf = 0;
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_BIST,       l_buf), "Failed to clear BIST reg");
    FAPI_TRY(p9_sbe_fastarray_abist_catchup(i_target_chiplet, 0xFF), "Failed to clock ABIST engine reset");

    /* Clean up the clock controller */
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CLK_REGION, l_buf), "Failed to clear clock regions");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_CAPT1, l_buf), "Failed to clear OPCG_CAPT1");
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_OPCG_CAPT2, l_buf), "Failed to clear OPCG_CAPT2");

    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;
}


