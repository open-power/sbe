/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_fabricinit.C $ */
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
///
/// @file p10_sbe_fabricinit.C
/// @brief Initialize island-mode fabric configuration
///
/// *HWP HW Maintainer: Jenny Huynh <jhuynh@us.ibm.com>
/// *HWP FW Maintainer: Ilya Smirnov <ismirno@us.ibm.com>
/// *HWP Consumed by: SBE
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_fabricinit.H>
#include <p10_adu_setup.H>
#include <p10_adu_access.H>
#include <p10_adu_utils.H>
#include <p9_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// adu should never be locked prior to calling sbe fabricinit
const uint8_t ALTD_MAX_LOCK_ATTEMPTS = 1;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_sbe_fabricinit(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_INF("Start p10_sbe_fabricinit...");

    fapi2::buffer<uint64_t> l_data;
    adu_operationFlag l_aduFlags;
    uint32_t l_fabricinit_flags;
    uint32_t l_numGranules;     // dummy variable
    uint8_t l_rwData[8] = {0};  // dummy variable

    // check state of pb_stop; it will prohibit all fabric commands from being broadcast if asserted
    FAPI_DBG("Check that fabric is in a healthy state");
    FAPI_TRY(getScom(i_target, PU_SND_MODE_REG, l_data), "Error reading pb_stop from pMisc Mode Register");

    FAPI_ASSERT(l_data.getBit<PU_SND_MODE_REG_PB_STOP>() == 0,
                fapi2::P10_SBE_FABRICINIT_FBC_STOPPED()
                .set_TARGET(i_target),
                "Pervasive stop control (pb_stop) is asserted, so fabric init will not run!");

    // setup adu utils parms for fabric init operation
    FAPI_DBG("Determine adu_utils parms for a fabric init operation");
    l_aduFlags.setOperationType(adu_operationFlag::PB_INIT_OPER);
    l_aduFlags.setLockControl(true);
    l_aduFlags.setNumLockAttempts(ALTD_MAX_LOCK_ATTEMPTS);

    l_fabricinit_flags = l_aduFlags.setFlag();

    // obtain adu lock, clear/reset state machines, setup and launch command
    FAPI_DBG("Setup and launch fabric init command via alter/display command register");
    FAPI_TRY(p10_adu_setup(i_target, 0, true, l_fabricinit_flags, l_numGranules),
             "Error setting up fabric init command via p10_adu_setup");

    // check adu command status
    FAPI_DBG("Check adu status register for command completion");
    FAPI_TRY(p10_adu_access(i_target, 0, true, l_fabricinit_flags, true, true, l_rwData),
             "Error checking the adu status via p10_adu_access");

    FAPI_DBG("Fabric init command from adu successfully completed!");

    // check state of fabric
    FAPI_TRY(p10_adu_utils_check_fbc_state(i_target),
             "Error checking state of fabric via p10_adu_utils_check_fbc_state");

    FAPI_DBG("Fabric is successfully initialized and running!");

fapi_try_exit:
    FAPI_INF("End p10_sbe_fabricinit...");
    return fapi2::current_err;
}
