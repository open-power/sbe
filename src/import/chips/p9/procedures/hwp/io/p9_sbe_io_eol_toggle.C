/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/io/p9_sbe_io_eol_toggle.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2019                        */
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
/// @file  p9_sbe_io_eol_toggle.H
///
/// @brief Toggles a pervasive bit on a coarse basis to preserve I/O end of life
///------------------------------------------------------------------------------
/// *HWP HW Owner        : Chris Steffen <cwsteffen@us.ibm.com>
/// *HWP HW Backup Owner : Gary Peterson <garyp@us.ibm.com>
/// *HWP FW Owner        : Jamie Knight <rjknight@us.ibm.com>
/// *HWP Team            : IO
/// *HWP Level           : 3
/// *HWP Consumed by     : SBE
///------------------------------------------------------------------------------

#include "p9_sbe_io_eol_toggle.H"
#include "p9_perv_scom_addresses.H"
#include <target_filters.H>

fapi2::ReturnCode run_p9_cplt_conf0_eol_toggle(const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_cplt)
{
    FAPI_INF("run_p9_sbe_io_eol_toggle: Entering ...");
    fapi2::buffer<uint64_t> l_data64;

    FAPI_TRY(fapi2::getScom(i_target_cplt, PERV_CPLT_CONF0, l_data64));

    if(l_data64.getBit<45>())
    {
        l_data64.clearBit<45>();
    }
    else
    {
        l_data64.setBit<45>();
    }

    FAPI_TRY(fapi2::putScom(i_target_cplt, PERV_CPLT_CONF0, l_data64));

fapi_try_exit:
    FAPI_INF("run_p9_sbe_io_eol_toggle: Exiting ...");
    return fapi2::current_err;
}

fapi2::ReturnCode p9_sbe_io_eol_toggle(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_io_eol_toggle: Entering ...");

    // Xbus Targets
    for(auto& l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_FILTER_XBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_INF("Calling run_p9_cplt_conf0_eol_toggle on xbus unit.");
        FAPI_TRY(run_p9_cplt_conf0_eol_toggle(l_target_cplt));
    }

    // Obus Targets
    for(auto& l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_FILTER_ALL_OBUS, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_INF("Calling run_p9_cplt_conf0_eol_toggle on obus unit.");
        FAPI_TRY(run_p9_cplt_conf0_eol_toggle(l_target_cplt));
    }

    // MC Targets
    for(auto& l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
        (fapi2::TARGET_FILTER_ALL_MC, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_INF("Calling run_p9_cplt_conf0_eol_toggle on mc unit.");
        FAPI_TRY(run_p9_cplt_conf0_eol_toggle(l_target_cplt));
    }

fapi_try_exit:
    FAPI_INF("p9_sbe_io_eol_toggle: Exiting ...");
    return fapi2::current_err;
}
